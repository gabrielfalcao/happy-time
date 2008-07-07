/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gb-player.c
 * Copyright (C) Gabriel Falcão Gonçalves de Moura 2008 <gabriel@nacaolivre.org>
 * 
 * gb-player.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gb-player.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "gb-player-time.h"

#ifndef _GB_PLAYER_H_
#define _GB_PLAYER_H_

#include <glib.h>
#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <gst/gst.h>
#include <cairo/cairo.h>

#include <clutter/clutter.h>
#include <clutter-gst/clutter-gst.h>
#include <clutter-cairo/clutter-cairo.h>
#include <clutter-gtk/gtk-clutter-embed.h>

G_BEGIN_DECLS

#define GB_TYPE_PLAYER             (gb_player_get_type ())
#define GB_PLAYER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GB_TYPE_PLAYER, GbPlayer))
#define GB_PLAYER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GB_TYPE_PLAYER, GbPlayerClass))
#define GB_IS_PLAYER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GB_TYPE_PLAYER))
#define GB_IS_PLAYER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GB_TYPE_PLAYER))
#define GB_PLAYER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GB_TYPE_PLAYER, GbPlayerClass))

typedef struct _GbPlayerClass GbPlayerClass;
typedef struct _GbPlayer GbPlayer;
typedef struct _GbPlayerPrivate GbPlayerPrivate;

struct _GbPlayerPrivate
{
	/* <gstreamer-stuff> */
	GstElement *player;
	GstElement *videosink;
	GstFormat format;
	/* </gstreamer-stuff> */

	/* <clutter-stuff>*/
	ClutterActor *stage;
	ClutterTimeline *show_hide_timeline;
	ClutterTimeline *playback_timeline;
	ClutterEffectTemplate *effect_template;
	/* <groups> */
	ClutterActor *title_group;
	ClutterActor *window_buttons_group;
	ClutterActor *bg_group;
	ClutterActor *controls_group;
	/* </groups> */

	/* <title-group> */
	ClutterActor *title_rect;
	ClutterActor *title_label;
	/* </title-group> */

	/* <window-buttons-group> */
	ClutterActor *window_buttons_rect;
	ClutterActor *window_aspect_button;
	ClutterActor *window_open_file_button;
	/* </window-buttons-group> */

	/* <bg-group> */
	ClutterActor *bg_video;
	ClutterActor *bg_image;
	ClutterActor *bg_logo;
	/* </bg-group> */

	/* <controls-group> */
	ClutterActor *controls_rect;
	ClutterActor *controls_backward;
	ClutterActor *controls_forward;
	ClutterActor *controls_play_pause;
	ClutterActor *controls_stop;
	/* </controls-group> */

	/* </clutter-stuff>*/

	/* <pixbuf-stuff> */
	GdkPixbuf *play;
	GdkPixbuf *backward;
	GdkPixbuf *forward;
	GdkPixbuf *stop;
	GdkPixbuf *pause;
	GdkPixbuf *fullscreen;
	GdkPixbuf *unfullscreen;
	GdkPixbuf *open_file;
	GdkPixbuf *bgpixbuf;
	GdkPixbuf *bglogo;
	/* <pixbuf-stuff> */

	/* <gtk-stuff> */
	GtkWidget *window;
	GtkWidget *stage_widget;
	GtkWidget *filechooser;
	/* </gtk-stuff> */

	/* my stuff :) */
	gboolean playing;
	gint last_second; /* if compare the gint64 time, will be too precise.
						 I need only the "seconds" difference */
	gchar *filename;
	gint minutes;
	gint seconds;
	gint time;
};

struct _GbPlayerClass
{
	GObjectClass parent_class;

	/* Signals */
	void(* elapsed_changed) (GbPlayer *self, GbPlayerTime *time, gpointer user_data);
};

struct _GbPlayer
{
	GObject parent_instance;
	GbPlayerPrivate *priv;
};

GType gb_player_get_type (void) G_GNUC_CONST;
void gb_player_play (GbPlayer *self);
void gb_player_pause (GbPlayer *self);
void gb_player_stop (GbPlayer *self);
void gb_player_set_title (GbPlayer *self);
gboolean gb_player_hide_controls (GbPlayer *self);
gboolean gb_player_show_controls (GbPlayer *self);
G_END_DECLS

#endif /* _GB_PLAYER_H_ */
