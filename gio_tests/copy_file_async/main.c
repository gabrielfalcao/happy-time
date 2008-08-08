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
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gio/gio.h>

#define COPY_ASYNC_DATA(x) ((CopyAsyncData *) x)

typedef struct _copy_async_data {
  GFile *source;
  GFile *destination;
  GError *error;
  GIOSchedulerJob *job;  
  GCancellable *cancel;
  gboolean done;
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *progress;
  GtkWidget *btn;
  GtkWidget *btncancel;
  GtkWidget *text;
} CopyAsyncData;

CopyAsyncData *
copy_async_data_new (GError **err)
{
  CopyAsyncData *data;

  data = g_new0(CopyAsyncData, 1);
  data->source = NULL;
  data->destination = NULL;
  data->error = *err;
  data->job = NULL;
  data->done = FALSE;
  data->cancel = g_cancellable_new ();
  data->text = gtk_entry_new ();

  data->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  data->vbox = gtk_vbox_new (TRUE, 2);
  data->progress = gtk_progress_bar_new ();
  data->btn = gtk_button_new_with_label ("Copy file (download)");
  data->btncancel = gtk_button_new_with_label ("Cancel");
  gtk_widget_set_sensitive (data->btncancel, FALSE);

  return data;
}

gboolean
copy_async_done (gpointer data)
{
  return FALSE;
}

void
copy_async_data_free (gpointer data)
{
  CopyAsyncData *cad = COPY_ASYNC_DATA(data);
  g_object_unref (cad->source);
  g_object_unref (cad->destination);
  g_error_free (cad->error);
}
void
copy_progress_callback (goffset current_num_bytes, goffset total_num_bytes,
                        gpointer user_data)
{
  CopyAsyncData *data = user_data;
  gdouble percent;
  gchar *text;
  
  GDK_THREADS_ENTER ();
  if (g_cancellable_is_cancelled(data->cancel))
      return;
  
  percent = current_num_bytes / (total_num_bytes / 100);
  text = g_strdup_printf ("%li%% (%li kb of %li kb)", (long int)percent, (long int)current_num_bytes / 1024, (long int)total_num_bytes / 1024);
  g_printerr ("%s\n", text);
  gtk_progress_bar_set_text (GTK_PROGRESS_BAR(data->progress), text);
  gtk_progress_bar_set_fraction  (GTK_PROGRESS_BAR(data->progress), percent / 100);
  g_free (text);
  GDK_THREADS_LEAVE ();
}

gboolean
do_copy_async (GIOSchedulerJob *job,
               GCancellable *cancellable,
               gpointer user_data)
{
  CopyAsyncData *data = user_data;
  gboolean result;
  GError *err = NULL;
  result = FALSE;
  data->job = job;
  g_io_scheduler_job_send_to_mainloop (job, copy_async_done, NULL, NULL);
  if ((cancellable) && (g_cancellable_is_cancelled (cancellable)))
    {
      data->done = FALSE;
      gtk_widget_set_sensitive (data->btn, TRUE);
      gtk_widget_set_sensitive (data->btncancel, FALSE);
      gtk_progress_bar_set_text (GTK_PROGRESS_BAR(data->progress), "Cancelled!");
      g_cancellable_reset (data->cancel);
      return FALSE;
    }

  result = g_file_copy (data->source,
			data->destination,
			G_FILE_COPY_OVERWRITE,
			data->cancel,
			copy_progress_callback,
			data,
			&err);

  if (result)
    {
      gtk_progress_bar_set_text (GTK_PROGRESS_BAR(data->progress), "Copied successfully (It's in your ~/Desktop)");
      data->done = TRUE;
    }


  if (err)
    {
      g_warning ("Error: %s", err->message);
      g_error_free (err);
    }

  data->done = TRUE;


  return FALSE;
}

void
cancel_copy (GtkButton *button, gpointer user_data)
{
  CopyAsyncData *data = user_data;

  g_cancellable_cancel (data->cancel);
  gtk_progress_bar_set_text (GTK_PROGRESS_BAR(data->progress), "Cancelled!");
  gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(data->progress), 0);

  gtk_widget_set_sensitive (data->btncancel, FALSE);
  gtk_widget_set_sensitive (data->btn, TRUE);
  g_io_scheduler_cancel_all_jobs ();
}

void
copy_file (GtkButton *button, gpointer user_data)
{

  CopyAsyncData *data = user_data;
  gchar *destpath, *filename;
  if (G_IS_FILE(data->source))
    {
      g_object_unref (data->source);
      data->source = NULL;
    }
  if (G_IS_FILE(data->destination))
    {
      g_object_unref (data->destination);
      data->destination = NULL;
    }

  data->source = g_file_new_for_commandline_arg (gtk_entry_get_text (GTK_ENTRY(data->text)));
  filename = g_file_get_basename (data->source);
  destpath = g_build_filename ("file:///home/", g_get_user_name (), "Desktop",  filename, NULL);
  data->destination = g_file_new_for_uri (destpath);

  gtk_widget_set_sensitive (data->btncancel, TRUE);
  gtk_widget_set_sensitive (data->btn, FALSE);

  g_io_scheduler_push_job (do_copy_async, data,
                           NULL, 0, data->cancel);  
  g_free (destpath);
  g_free (filename);
}

int
main (int argc, char **argv)
{
  GError *err = NULL;

  CopyAsyncData *data;
  g_thread_init (NULL);
  gdk_threads_init ();
  gtk_init (&argc, &argv);
  data = copy_async_data_new (&err);

  gtk_widget_set_size_request (data->window, 500, 150);
  gtk_box_pack_start (GTK_BOX(data->vbox), data->text, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(data->vbox), data->progress, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(data->vbox), data->btn, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(data->vbox), data->btncancel, TRUE, FALSE, 2);
  gtk_container_add (GTK_CONTAINER(data->window), data->vbox);
  gtk_entry_set_text (GTK_ENTRY(data->text), "http://gabrielfalcao.com/ritmosdomundo_cuba.rar");
  gtk_widget_show_all (data->window);

  g_signal_connect (data->btn, "clicked", (GCallback)copy_file, data);
  g_signal_connect (data->btncancel, "clicked", (GCallback)cancel_copy, data);
  g_signal_connect (data->window, "destroy", gtk_main_quit, NULL);

  gtk_main ();
  return 0;
}
