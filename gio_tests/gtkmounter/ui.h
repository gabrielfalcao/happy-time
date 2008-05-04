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
#include <gtk/gtk.h>
#include <gio/gio.h>

typedef struct {
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *label;
  GList *volumes;
  GHashTable *buttons;
} UserInterface;

typedef struct {
  GVolume *volume;
  UserInterface *ui;
} VolumeUIPair;

UserInterface *get_ui (void);
void on_click_to_unmount (GtkButton *widget,
                          gpointer user_data);

void finish_eject_cb (GObject *source_object,
                        GAsyncResult *res,
                        gpointer user_data);

void on_click_to_mount (GtkButton *widget,
                          gpointer user_data);
