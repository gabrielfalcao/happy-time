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

#ifndef _GF_MONITOR_H
#define _GF_MONITOR_H

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define GF_TYPE_MONITOR                   (gf_monitor_get_type ())
#define GF_MONITOR(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), GF_TYPE_MONITOR, GfMonitor))
#define GF_MONITOR_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), GF_TYPE_MONITOR, GfMonitorClass))
#define GF_IS_MONITOR(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GF_TYPE_MONITOR))
#define GF_IS_MONITOR_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), GF_TYPE_MONITOR))
#define GF_MONITOR_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), GF_TYPE_MONITOR, GfMonitorClass))

typedef struct _GfMonitor GfMonitor;
typedef struct _GfMonitorClass GfMonitorClass;

struct _GfMonitor
{
  GObject parent;
};

struct _GfMonitorClass
{
  GObjectClass parent;
};

GType gf_monitor_get_type (void);
GfMonitor *gf_monitor_new (void);
void gf_monitor_run (GfMonitor *self);
G_END_DECLS

#endif /* _GF_MONITOR_H */

