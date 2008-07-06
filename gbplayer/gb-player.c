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

#include "gb-player.h"

#define GB_PLAYER_GET_PRIVATE(o) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), GB_TYPE_PLAYER, GbPlayerPrivate))
#define CLUTTER_GROUP_AND_SHOW(x,y) \
	clutter_container_add_actor (CLUTTER_CONTAINER(x), y); \
	clutter_actor_show (y)

enum
{
	PROP_0,

	PROP_FILE
};

enum
{
	ELAPSED_CHANGED,

	LAST_SIGNAL
};


static guint player_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (GbPlayer, gb_player, G_TYPE_OBJECT);

static void
gb_player_init (GbPlayer *self)
{
	self->priv = GB_PLAYER_GET_PRIVATE(self);
}

static void
gb_player_dispose (GObject *self)
{
}

static GObject *
gb_player_constructor (GType type, guint n_construct_properties,
                       GObjectConstructParam *construct_properties)
{
	GObject * obj;
	GbPlayerClass * klass;
	GObjectClass * parent_class;
	GbPlayer * self;
	ClutterColor cc_white = {0xff, 0xff, 0xff, 0x60};
	ClutterColor cc_black = {0x00, 0x00, 0x00, 0x90};

	klass = GB_PLAYER_CLASS (g_type_class_peek (GB_TYPE_PLAYER));
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = GB_PLAYER (obj);

	GbPlayerPrivate *p = GB_PLAYER_GET_PRIVATE(self);

	p->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	p->stage_widget = gtk_clutter_embed_new ();
	gtk_widget_set_size_request (p->stage_widget, 640, 480);
	g_signal_connect (p->window, "delete-event",
					  G_CALLBACK(gtk_main_quit), NULL);

	gtk_container_add (GTK_CONTAINER(p->window), p->stage_widget);

	p->play = gdk_pixbuf_new_from_file(IMG_PATH "/start.svg", NULL);
	p->backward = gdk_pixbuf_new_from_file(IMG_PATH "/backward.svg", NULL);
	p->forward = gdk_pixbuf_new_from_file(IMG_PATH "/forward.svg", NULL);
	p->stop = gdk_pixbuf_new_from_file(IMG_PATH "/stop.svg", NULL);
	p->pause = gdk_pixbuf_new_from_file(IMG_PATH "/pause.svg", NULL);
	p->fullscreen = gdk_pixbuf_new_from_file(IMG_PATH "/fullscreen.svg", NULL);
	p->unfullscreen = gdk_pixbuf_new_from_file(IMG_PATH "/unfullscreen.svg", NULL);
	p->open_file = gdk_pixbuf_new_from_file(IMG_PATH "/openfile.svg", NULL);
	p->time = 0;
	p->playing = FALSE;
	p->player = gst_element_factory_make ("playbin", "player");

	/* title group */
	p->title_group = clutter_group_new ();
	p->title_rect = clutter_rectangle_new_with_color (&cc_white);
	p->title_label = clutter_label_new_full ("DejaVu Sans",
											 "<span size='10000'><b>Metallica - Sad But True - Load(1991)</b></span>",
											 &cc_black);
	clutter_label_set_use_markup (CLUTTER_LABEL(p->title_label), TRUE);
	CLUTTER_GROUP_AND_SHOW(p->title_group, p->title_rect);
	CLUTTER_GROUP_AND_SHOW(p->title_group, p->title_label);
	clutter_actor_set_size (p->title_rect, 400, 25);
	/* window buttons group */
	p->window_buttons_group = clutter_group_new ();
	p->window_buttons_rect = clutter_rectangle_new_with_color (&cc_white);
	p->window_aspect_button = clutter_texture_new_from_pixbuf (p->fullscreen);
	p->window_open_file_button = clutter_texture_new_from_pixbuf (p->open_file);
	clutter_label_set_use_markup (CLUTTER_LABEL(p->title_label), TRUE);
	CLUTTER_GROUP_AND_SHOW(p->window_buttons_group, p->window_buttons_rect);
	CLUTTER_GROUP_AND_SHOW(p->window_buttons_group, p->window_aspect_button);
	CLUTTER_GROUP_AND_SHOW(p->window_buttons_group, p->window_open_file_button);
	p->stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED(p->stage_widget));

	/* background group*/
	p->bg_group = clutter_group_new ();
	p->bg_video = clutter_gst_video_texture_new ();
	CLUTTER_GROUP_AND_SHOW(p->bg_group, p->bg_video);
	clutter_actor_set_size (p->bg_video, 640, 480);

	/* controls group */
	p->controls_group = clutter_group_new ();
	p->controls_rect = clutter_rectangle_new_with_color (&cc_white);
	p->controls_backward = clutter_texture_new_from_pixbuf (p->backward);
	p->controls_forward = clutter_texture_new_from_pixbuf (p->forward);
	p->controls_play_pause = clutter_texture_new_from_pixbuf (p->play);
	p->controls_stop = clutter_texture_new_from_pixbuf (p->stop);

	CLUTTER_GROUP_AND_SHOW(p->controls_group, p->controls_rect);
	CLUTTER_GROUP_AND_SHOW(p->controls_group, p->controls_backward);
	CLUTTER_GROUP_AND_SHOW(p->controls_group, p->controls_forward);
	CLUTTER_GROUP_AND_SHOW(p->controls_group, p->controls_play_pause);
	CLUTTER_GROUP_AND_SHOW(p->controls_group, p->controls_stop);

	CLUTTER_GROUP_AND_SHOW(p->stage, p->bg_group);
	CLUTTER_GROUP_AND_SHOW(p->stage, p->title_group);
	CLUTTER_GROUP_AND_SHOW(p->stage, p->controls_group);
	CLUTTER_GROUP_AND_SHOW(p->stage, p->window_buttons_group);

	gtk_widget_show_all (p->window);

	p->videosink = clutter_gst_video_sink_new (CLUTTER_TEXTURE(p->bg_video));
	g_object_set (p->player, "video-sink", p->videosink, NULL);
	g_object_set (p->player, "uri", "file:///home/gabriel/Filmes/Jeff.Dunham.Spark.Of.Insanity.2007.DVDRip.XviD.AC3-STS-cd1.avi", NULL);
	gst_element_set_state (p->player, GST_STATE_PLAYING);

}

static void
gb_player_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (gb_player_parent_class)->finalize (object);
}

static void
gb_player_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GB_IS_PLAYER (object));

	switch (prop_id)
	{
	case PROP_FILE:
		/* TODO: Add setter for "file" property here */
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gb_player_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GB_IS_PLAYER (object));

	switch (prop_id)
	{
	case PROP_FILE:
		/* TODO: Add getter for "file" property here */
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gb_player_elapsed_changed (GbPlayer *self, GbPlayerTime *time, gpointer user_data)
{
	/* TODO: Add default signal handler implementation here */
}

static void
gb_player_class_init (GbPlayerClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = gb_player_finalize;
    object_class->dispose = gb_player_dispose;
    object_class->constructor = gb_player_constructor;

	object_class->set_property = gb_player_set_property;
	object_class->get_property = gb_player_get_property;

	klass->elapsed_changed = gb_player_elapsed_changed;

	g_object_class_install_property (object_class,
	                                 PROP_FILE,
	                                 g_param_spec_string ("file",
	                                                      "FileName",
	                                                      "The name of currently file name",
	                                                      "",
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

	player_signals[ELAPSED_CHANGED] =
		g_signal_new ("elapsed-changed",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GbPlayerClass, elapsed_changed),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__OBJECT,
		              G_TYPE_NONE, 2,
		              GB_TYPE_PLAYER_TIME, G_TYPE_POINTER);
	g_type_class_add_private (klass, sizeof (GbPlayerPrivate));
}


void
gb_player_play (GbPlayer *self)
{
	/* TODO: Add public function implementation here */
}
