/*
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

#include "gf-monitor.h"

static void gf_monitor_class_init (GfMonitorClass * klass);
static void gf_monitor_init (GfMonitor * self);

GType
gf_monitor_get_type (void)
{
  static GType type = 0;

  if (type == 0)
    {
      static const GTypeInfo info = {
        sizeof (GfMonitorClass),
        NULL,                   /* base_init */
        NULL,                   /* base_finalize */
        (GClassInitFunc) gf_monitor_class_init,  /* class_init */
        NULL,                   /* class_finalize */
        NULL,                   /* class_data */
        sizeof (GfMonitor),
        0,                      /* n_preallocs */
        (GInstanceInitFunc) gf_monitor_init,     /* instance_init */
        NULL
      };
      type =
        g_type_register_static (G_TYPE_OBJECT, "GfMonitorType", &info,
                                0);
    }
  return type;
}

static void
gf_monitor_class_init (GfMonitorClass * klass)
{
}

static void
gf_monitor_init (GfMonitor * self)
{
}

GfMonitor *
gf_monitor_new ()
{
  GfMonitor *obj;

  obj = g_object_new (GF_TYPE_MONITOR, NULL);
  return obj;
}

static void
file_changed (GFileMonitor      *file_monitor,
    GFile             *child,
    GFile             *other_file,
    GFileMonitorEvent  event_type,
    gpointer           user_data)
{
  switch (event_type)
  {
    case G_FILE_MONITOR_EVENT_DELETED:
      g_print ("Filho da puta! Removeu o '%s'...\n", g_file_get_basename (child));
      break;
    case G_FILE_MONITOR_EVENT_CREATED:
      g_print ("Filho da puta! Criou o '%s'\n", g_file_get_basename (child));
      break;
    case G_FILE_MONITOR_EVENT_CHANGED:
      g_print ("Filho da puta! Modificou o '%s'\n", g_file_get_basename (child));
      break;
    default: 
    break;
  }
}

void
gf_monitor_run(GfMonitor *self)
{
  GMainLoop *loop;
  GFile *file;
  GError *err;
  GFileMonitor *monitor;
  loop = g_main_loop_new(NULL, FALSE);
  file = g_file_new_for_uri ("file:///home/gabriel/Desktop");
  monitor = g_file_monitor_directory (file, 0, NULL, &err);
  g_signal_connect (monitor, "changed", G_CALLBACK (file_changed), NULL);
  g_main_loop_run(loop);

}
