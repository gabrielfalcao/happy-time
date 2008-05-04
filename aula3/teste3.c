/*
 * Copyright (C) 2007 Gabriel Falcão <root@gabrielfalcao.com>
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
#include <vte/vte.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "teste3.h"
#define SAUDACAO "Terminal recarregado com sucesso!"
GtkWidget *warning_label1;
GtkWidget *terminal;            //vte
int count = 0;

//gpointer e (void *) são a mesma coisa! Gpointer é só um açúcar sintático.
static void
initialize_vte (GtkWidget * vte)
{
  vte_terminal_fork_command (VTE_TERMINAL (vte), "/bin/bash", NULL, NULL,
                             NULL, FALSE, FALSE, FALSE);
  start_warning ();
}

static void
warning_show (gpointer id)
{
  gtk_label_set_markup (GTK_LABEL (warning_label1),
                        g_strdup_printf
                        ("<b><span foreground=\"#FF0000\">%s</span></b>",
                         SAUDACAO));
  gtk_widget_show ((GtkWidget *) warning_label1);
  printf ("%d", count);
  if (count > 0)
    {
      remove_event ((guint *) id);
    }
  count++;
}
static void
warning_hide ()
{
  gtk_widget_hide ((GtkWidget *) warning_label1);
  vte_terminal_reset (VTE_TERMINAL ((GtkWidget *) terminal), TRUE, FALSE);
}

static void
remove_event (gpointer event_id)
{
  warning_hide ();
  if (event_id > 0)
    {
      g_source_remove ((guint) event_id);
    }
  count = -1;
}

static void
start_warning ()
{
  guint event_id;
  event_id = g_timeout_add (500,
                            (GSourceFunc) warning_show, (void *) event_id);
}


static void
destroy (GtkWidget * widget, gpointer data)
{
  g_print ("\nSaindo!\n");
  gtk_main_quit ();
}

static void
on_button1_clicked (GList * data)
{
  gpointer ptr_entry, ptr_window;
  ptr_entry = (GtkWidget *) (GList *) g_list_first (data)->data;
  if (ptr_entry != NULL)
    {
      ptr_window = (GtkWidget *) (GList *) g_list_last (data)->data;
      if (ptr_window != NULL)
        {
          gtk_window_set_title (GTK_WINDOW (ptr_window),
                                gtk_entry_get_text (GTK_ENTRY (ptr_entry)));
        }
    }
}


int
main (int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *vbox;              //container
  GtkWidget *entry1, *button1, *label1; //widgets diversos
  GList *dados = NULL;
  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 1);
  gtk_widget_set_size_request (window, 640, 480);
  gtk_window_set_icon_name (GTK_WINDOW (window), "gtk-home");
  label1 = gtk_label_new ("Meu primeiro hello world em C");
  button1 = gtk_button_new_with_label ("Definir titulo");
  entry1 = gtk_entry_new_with_max_length (200);
  vbox = gtk_vbox_new (FALSE, 1);
  warning_label1 = gtk_label_new ("");

  //terminal hack
  terminal = vte_terminal_new ();
  initialize_vte (terminal);
  gtk_box_pack_start ((GtkBox *) vbox, label1, FALSE, FALSE, 5);
  gtk_box_pack_start ((GtkBox *) vbox, entry1, FALSE, FALSE, 2);
  gtk_box_pack_start ((GtkBox *) vbox, button1, FALSE, FALSE, 2);
  gtk_box_pack_start ((GtkBox *) vbox, terminal, TRUE, TRUE, 2);
  gtk_box_pack_start ((GtkBox *) vbox, warning_label1, FALSE, FALSE, 2);

  gtk_widget_hide ((GtkWidget *) warning_label1);
  gtk_window_set_title (GTK_WINDOW (window), "Meu Hello World com terminal");

  dados = g_list_append (dados, GTK_ENTRY (entry1));
  dados = g_list_append (dados, GTK_WINDOW (window));
  g_signal_connect (G_OBJECT (terminal), "child-exited",
                    G_CALLBACK (initialize_vte), NULL);
  g_signal_connect_swapped (G_OBJECT (button1), "clicked",
                            G_CALLBACK (on_button1_clicked),
                            G_OBJECT (dados));

  gtk_container_add (GTK_CONTAINER (window), vbox);

  gtk_widget_show (vbox);
  gtk_widget_show (entry1);
  gtk_widget_show (button1);

  gtk_widget_show (window);
  gtk_widget_show_all (window);

  gtk_main ();
  return 0;
}
