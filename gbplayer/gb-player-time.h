/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gb-player-time.c
 * Copyright (C) Gabriel Falcão Gonçalves de Moura 2008 <gabriel@nacaolivre.org>
 * 
 * gb-player-time.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gb-player-time.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GB_PLAYER_TIME_H_
#define _GB_PLAYER_TIME_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GB_TYPE_PLAYER_TIME             (gb_player_time_get_type ())
#define GB_PLAYER_TIME(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GB_TYPE_PLAYER_TIME, GbPlayerTime))
#define GB_PLAYER_TIME_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GB_TYPE_PLAYER_TIME, GbPlayerTimeClass))
#define GB_IS_PLAYER_TIME(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GB_TYPE_PLAYER_TIME))
#define GB_IS_PLAYER_TIME_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GB_TYPE_PLAYER_TIME))
#define GB_PLAYER_TIME_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GB_TYPE_PLAYER_TIME, GbPlayerTimeClass))

typedef struct _GbPlayerTimeClass GbPlayerTimeClass;
typedef struct _GbPlayerTime GbPlayerTime;

struct _GbPlayerTimeClass
{
	GObjectClass parent_class;
};

struct _GbPlayerTime
{
	GObject parent_instance;
};

GType gb_player_time_get_type (void) G_GNUC_CONST;
gchar * gb_player_time_strftime (GbPlayerTime *self, const gchar * format);

G_END_DECLS

#endif /* _GB_PLAYER_TIME_H_ */
