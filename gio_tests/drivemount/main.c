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
#include <gio/gio.h>
void umounted_finish   (GObject *source_object,
                        GAsyncResult *res,
                        gpointer user_data)
{
  g_printerr ("O Volume: %s, foi desmontado!\n", (gchar *)user_data);
  g_free (user_data);
}

int
main (int argc, char **argv)
{
  GVolumeMonitor*  vmonitor;
  GVolume *vol;
  GDrive *drv;
  GMount *mnt;
  GList *volumes, *drives, *mounts;
  gchar *vname;

  g_type_init ();
  vmonitor = g_volume_monitor_get ();
  volumes = g_volume_monitor_get_volumes (vmonitor);
  while (volumes)
  {
    vol = G_VOLUME(volumes->data);
    vname = g_volume_get_identifier (vol, "uuid");
    if (g_volume_can_mount(vol))
      g_print ("Montavel: %s\n", vname);
    else
      g_print ("Nao montavel: %s\n", vname);
    g_free (vname);
    volumes = g_list_next(volumes);
  }
  drives = g_volume_monitor_get_connected_drives (vmonitor);
  while (drives)
  {
    drv = G_DRIVE(drives->data);
    vname = g_drive_get_name (drv);
    g_print ("Drive: %s\n", vname);
    g_free (vname);
    drives = g_list_next(drives);
  }
  mounts = g_volume_monitor_get_mounts (vmonitor);
  while (mounts)
  {
    mnt = G_MOUNT(mounts->data);
    vname = g_mount_get_uuid (mnt);
    g_print ("Mount: %s\n", vname);
    g_free (vname);
    g_mount_eject (mnt,
                   G_MOUNT_UNMOUNT_FORCE,
                   NULL,
                   umounted_finish,
                   g_mount_get_name (mnt));
    mounts = g_list_next(mounts);

  }
  g_free (volumes);
  g_free (drives);
  g_free (mounts);


  return 0;
}

