/**
 * Copyright (C) 2008 Gabriel Falcão <gabriel@nacaolivre.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#include "ui.h"

void volume_added_cb (GVolumeMonitor *volume_monitor,
                       GVolume *volume,
                       gpointer user_data)
{
  GtkWidget *button;

  g_warning ("Volume added !");
  UserInterface *ui = (UserInterface *) user_data;
  VolumeUIPair *vuipair = g_new0(VolumeUIPair, 1);
  vuipair->volume = volume;
  vuipair->ui = ui;
  GMount *mount = g_volume_get_mount(volume);
  gchar *uuid, *name, *btn_label;
  uuid = g_volume_get_identifier (volume, "uuid");
  name = g_volume_get_name (volume);
  if (mount)
  
    btn_label = g_strdup_printf ("%s:%s (Montado)", name, uuid);
  else
    btn_label = g_strdup_printf ("%s:%s (Desmontado)", name, uuid);
  

  button = gtk_button_new_with_label (btn_label);
  gtk_box_pack_start (GTK_BOX(ui->vbox), button, FALSE, TRUE, 1);
  gtk_widget_show (button);
  if (mount)
    g_signal_connect (button, "clicked", G_CALLBACK(on_click_to_unmount), vuipair);
  else
    g_signal_connect (button, "clicked", G_CALLBACK(on_click_to_mount), vuipair);

  g_hash_table_replace (ui->buttons, uuid, button);

  g_free (name);
  g_free (uuid);
  g_free (btn_label);
}
void volume_changed_cb (GVolumeMonitor *volume_monitor,
                       GVolume *volume,
                       gpointer user_data)
{
  GtkWidget *button;
  g_warning ("Volume changed !");
  UserInterface *ui = (UserInterface *) user_data;
  VolumeUIPair *vuipair = g_new0(VolumeUIPair, 1);
  vuipair->volume = volume;
  vuipair->ui = ui;
  GMount *mount = g_volume_get_mount (volume);
  gchar *uuid, *name, *btn_label;
  uuid = g_volume_get_identifier (volume, "uuid");
  name = g_volume_get_name (volume);
  if (mount)
    btn_label = g_strdup_printf ("%s:%s (Montado)", name, uuid);
  else
    btn_label = g_strdup_printf ("%s:%s (Desmontado)", name, uuid);
  

  button = g_hash_table_lookup(ui->buttons, uuid);
  if (button)
    gtk_button_set_label (GTK_BUTTON(button), btn_label);
  else {
    button = gtk_button_new_with_label (btn_label);
    gtk_box_pack_start (GTK_BOX(ui->vbox), button, FALSE, TRUE, 1);
    gtk_widget_show (button);
  }
  if (mount)
    g_signal_connect (button, "clicked", G_CALLBACK(on_click_to_unmount), vuipair);
  else
    g_signal_connect (button, "clicked", G_CALLBACK(on_click_to_mount), vuipair);

  g_hash_table_replace (ui->buttons, uuid, button);

  g_free (name);
  g_free (uuid);
  g_free (btn_label);
}

void volume_removed_cb (GVolumeMonitor *volume_monitor,
                       GVolume *volume,
                       gpointer user_data)
{
  GtkWidget *button;
  g_warning ("Volume removed !");
  UserInterface *ui = (UserInterface *) user_data;
  VolumeUIPair *vuipair = g_new0(VolumeUIPair, 1);
  vuipair->volume = volume;
  vuipair->ui = ui;
  gchar *uuid;
  uuid = g_volume_get_identifier (volume, "uuid");
  button = g_hash_table_lookup (ui->buttons, uuid);
  if (button)
  {
    gtk_widget_hide (button);
    g_message ("Volume: %s removido!", uuid);
    g_object_unref (button);
  }
  g_free (uuid);
}
void finish_eject_cb (GObject *source_object,
                        GAsyncResult *res,
                        gpointer user_data)
{
  gchar *uuid, *name, *btn_label;
  g_warning ("Finalizando a desmontagem");
  VolumeUIPair *vuipair = (VolumeUIPair *) user_data;
  uuid = g_volume_get_identifier (vuipair->volume, "uuid");
  name = g_volume_get_name (vuipair->volume);
  GtkButton *btn = GTK_BUTTON(g_hash_table_lookup(vuipair->ui->buttons, uuid));
  if (btn)
  {
    btn_label = g_strdup_printf ("%s:%s (Desmontado)", name, uuid);
    gtk_button_set_label (btn, btn_label);
    g_free (btn_label);
    g_object_disconnect (btn, "clicked", on_click_to_unmount, user_data, NULL);
    g_signal_connect (btn, "clicked", G_CALLBACK(on_click_to_mount), user_data);
  }
  g_free (uuid);
  g_free (name);
}

void on_click_to_unmount (GtkButton *button,
                          gpointer user_data)
{
  VolumeUIPair *vuipair = (VolumeUIPair *)user_data;
  GMount *mount = g_volume_get_mount (vuipair->volume);
  g_mount_eject (mount,
                 G_MOUNT_UNMOUNT_NONE,
                 NULL,
                 finish_eject_cb,
                 vuipair);

}
void on_click_to_mount (GtkButton *button,
                          gpointer user_data)
{}

UserInterface * get_ui ()
{
  static gboolean created = FALSE;
  static UserInterface *ui;
  static GVolumeMonitor *monitor;

  VolumeUIPair *vuipair;
  GVolume *volume;
  GMount *mount;
  GtkWidget *button;
  gchar *uuid, *name, *btn_label;
  if (!created)
  {
    monitor = g_volume_monitor_get ();
    ui = g_new0 (UserInterface, 1);
    ui->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    ui->label = gtk_label_new ("");
    ui->vbox = gtk_vbox_new (FALSE, 2);
    ui->buttons = g_hash_table_new (g_str_hash, g_str_equal);
    gtk_box_pack_start (GTK_BOX(ui->vbox), ui->label, FALSE, TRUE, 1);

    gtk_label_set_use_markup (GTK_LABEL(ui->label),
                              TRUE);
    ui->volumes = g_volume_monitor_get_volumes (monitor);
    gtk_label_set_markup (GTK_LABEL(ui->label), "<span size=\"16500\" foreground=\"red\">IHU!</span>");
    while (ui->volumes)
    {
      volume = G_VOLUME(ui->volumes->data);
      uuid = g_volume_get_identifier (volume, "uuid");
      name = g_volume_get_name (volume);
      mount = g_volume_get_mount (volume);
      vuipair = g_new0 (VolumeUIPair, 1);
      vuipair->ui = ui;
      vuipair->volume = volume;
      if (mount)
      {
        btn_label = g_strdup_printf ("%s:%s (Montado)",
                                     name,
                                     uuid);

        button = gtk_button_new_with_label (btn_label);
        g_signal_connect (button, "clicked", G_CALLBACK(on_click_to_unmount), vuipair);
        g_free (btn_label);
      }else{
        btn_label = g_strdup_printf ("%s:%s (Desmontado)",
                                     name,
                                     uuid);

        button = gtk_button_new_with_label (btn_label);
        g_signal_connect (button, "clicked", G_CALLBACK(on_click_to_mount), vuipair);
        g_free (btn_label);
      }

      g_hash_table_replace (ui->buttons, uuid, button);
      gtk_box_pack_start (GTK_BOX(ui->vbox), button, FALSE, TRUE, 1);
      g_free (uuid);
      g_free (name);
      gtk_container_add (GTK_CONTAINER (ui->window), ui->vbox);
      gtk_widget_set_size_request (ui->window, 400, 200);

      g_message ("IHU\n");
      ui->volumes = g_list_next (ui->volumes);
    }
    ui->volumes = g_list_first (ui->volumes);
    g_signal_connect (monitor, "volume-changed", G_CALLBACK(volume_changed_cb), ui);
    g_signal_connect (monitor, "volume-added", G_CALLBACK(volume_added_cb), ui);
    g_signal_connect (monitor, "volume-removed", G_CALLBACK(volume_changed_cb), ui);
    g_signal_connect_swapped (ui->window, "destroy",
                              G_CALLBACK (gtk_main_quit),
                              NULL);

    created = TRUE;
  }
  return ui;
}
