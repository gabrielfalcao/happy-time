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
#include "gb-player-callbacks.h"

#define GB_PLAYER_GET_PRIVATE(o) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), GB_TYPE_PLAYER, GbPlayerPrivate))
#define CLUTTER_GROUP_AND_SHOW(x,y) \
	clutter_container_add_actor (CLUTTER_CONTAINER(x), y); \
	clutter_actor_show (y)

enum
{
	PROP_0,

	PROP_FILE,
	PROP_PLAYING
};

enum
{
	ELAPSED_CHANGED,

	LAST_SIGNAL
};


static guint player_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (GbPlayer, gb_player, G_TYPE_OBJECT);
static ClutterActor *gb_player_rounded_rect (GbPlayer *self, 
											 gint width,
											 gint height,
											 ClutterColor *bgcolor, 
											 gint round_radius);
static void _gb_player_setup_widgets (GbPlayer *self);
static void _gb_player_connect_signals (GbPlayer *self);

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
	p->filechooser = gtk_file_chooser_dialog_new ("Choose the media file", 
												  GTK_WINDOW(p->window),
												  GTK_FILE_CHOOSER_ACTION_OPEN,
												  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
												  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
												  NULL);

	gtk_container_add (GTK_CONTAINER(p->window), p->stage_widget);
	p->stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED(p->stage_widget));
	p->play = gdk_pixbuf_new_from_file(IMG_PATH "/start.svg", NULL);
	p->backward = gdk_pixbuf_new_from_file(IMG_PATH "/backward.svg", NULL);
	p->forward = gdk_pixbuf_new_from_file(IMG_PATH "/forward.svg", NULL);
	p->stop = gdk_pixbuf_new_from_file(IMG_PATH "/stop.svg", NULL);
	p->pause = gdk_pixbuf_new_from_file(IMG_PATH "/pause.svg", NULL);
	p->fullscreen = gdk_pixbuf_new_from_file(IMG_PATH "/fullscreen.svg", NULL);
	p->unfullscreen = gdk_pixbuf_new_from_file(IMG_PATH "/unfullscreen.svg", NULL);
	p->open_file = gdk_pixbuf_new_from_file(IMG_PATH "/openfile.svg", NULL);
	p->bgpixbuf = gdk_pixbuf_new_from_file(IMG_PATH "/background.svg", NULL);
	p->bglogo = gdk_pixbuf_new_from_file(IMG_PATH "/logo.svg", NULL);
	p->time = 0;
	p->controls_visible = TRUE;
	p->last_second = -1;
	p->filename = NULL;
	p->playing = FALSE;
	p->player = gst_element_factory_make ("playbin", "player");



	p->show_hide_timeline = clutter_timeline_new_for_duration (1000);
	g_signal_connect(p->show_hide_timeline, "completed",
					 G_CALLBACK(on_timeline_completed),
					 (gpointer) self);

	/* p->playback_timeline = clutter_timeline_new_for_duration (1000); */
	/* g_signal_connect(p->playback_timeline, "new-frame", */
	/* 				 G_CALLBACK(playback_cb), */
	/* 				 (gpointer) self); */
	/* clutter_timeline_set_loop(p->playback_timeline, TRUE); */
	/* clutter_timeline_start(p->playback_timeline); */

	_gb_player_setup_widgets (self);
	_gb_player_connect_signals (self);
	// TODO: conectar o sinal de mouse move no stage nas areas dos controles pra poder mostrar ou esconder os controles
}

static void
gb_player_finalize (GObject *object)
{
	G_OBJECT_CLASS (gb_player_parent_class)->finalize (object);
}

static void
gb_player_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GB_IS_PLAYER (object));
	GbPlayer *self = GB_PLAYER(object);
	GbPlayerPrivate *priv = GB_PLAYER_GET_PRIVATE(self);

	switch (prop_id)
	{
	case PROP_FILE:
		if (priv->filename)
			g_free (priv->filename);
		self->priv->filename = g_strdup(g_value_get_string (value));
		if (self->priv->playing)
			{
				gb_player_stop (self);
				gb_player_play (self);
			}
		else
			gb_player_stop (self);

		break;
	case PROP_PLAYING:
		self->priv->playing = g_value_get_boolean (value);
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
	GbPlayer *self = GB_PLAYER(object);

	switch (prop_id)
	{
	case PROP_FILE:
		g_value_set_string(value, self->priv->filename);
		break;
	case PROP_PLAYING:
		g_value_set_boolean(value, self->priv->playing);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gb_player_elapsed_changed (GbPlayer *self, GbPlayerTime *time, gpointer user_data)
{
	
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

	g_object_class_install_property (object_class,
	                                 PROP_PLAYING,
	                                 g_param_spec_boolean ("playing",
														   "Playing",
														   "The state of the player",
														   FALSE,
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
	GtkWidget *dialog = gtk_message_dialog_new_with_markup  (GTK_WINDOW(self->priv->window),
															 GTK_DIALOG_MODAL,
															 GTK_MESSAGE_ERROR,
															 GTK_BUTTONS_OK,
															 "Choose a valid file to play before clicking the fucking play!", 
															 NULL);

	if (!self->priv->filename)
		{
			gtk_widget_show (GTK_WIDGET(dialog));
			//	gtk_widget_destroy (dialog);
			return;
		}
	if (self->priv->playing)
		return;

	g_object_set (self->priv->player, "uri", self->priv->filename, NULL);
	gst_element_set_state (self->priv->player, GST_STATE_PLAYING);
	clutter_texture_set_pixbuf (CLUTTER_TEXTURE(self->priv->controls_play_pause), self->priv->pause, NULL);
	g_object_set (self, "playing", TRUE, NULL);
	g_timeout_add (3, (GSourceFunc) gb_player_hide_controls, (gpointer) self);
}

void
gb_player_pause (GbPlayer *self)
{
	if (!self->priv->playing)
		return;

	gst_element_set_state (self->priv->player, GST_STATE_PAUSED);
	clutter_texture_set_pixbuf (CLUTTER_TEXTURE(self->priv->controls_play_pause), self->priv->play, NULL);
	g_object_set (self, "playing", FALSE, NULL);
	g_timeout_add (3, (GSourceFunc) gb_player_show_controls, (gpointer) self);
}
	
void

gb_player_stop (GbPlayer *self)
{
	if (!self->priv->playing)
		return;

	gst_element_set_state (self->priv->player, GST_STATE_NULL);
	clutter_texture_set_pixbuf (CLUTTER_TEXTURE(self->priv->controls_play_pause), self->priv->play, NULL);
	g_object_set (self, "playing", FALSE, NULL);
	g_timeout_add (3, (GSourceFunc) gb_player_show_controls, (gpointer) self);
}

gboolean
gb_player_hide_controls (GbPlayer *self)
{
	ClutterEffectTemplate *effect_template;
	gint x, y;
	ClutterKnot t_knot[2];
	ClutterKnot b_knot[2];
	ClutterKnot w_knot[2];
	ClutterKnot c_knot[2];
	if ((!self->priv->playing) || (clutter_timeline_is_playing (self->priv->show_hide_timeline)))
		return TRUE;

	clutter_timeline_start(self->priv->show_hide_timeline);

	effect_template = clutter_effect_template_new (self->priv->show_hide_timeline, &on_alpha);
	clutter_actor_get_position (self->priv->title_group, &x, &y);
	t_knot[0].x = x;
	t_knot[0].y = y;
	t_knot[1].x= -600;
	t_knot[1].y= -600;
	clutter_actor_get_position (self->priv->window_buttons_group, &x, &y);
	b_knot[0].x = x;
	b_knot[0].y = y;
	b_knot[1].x=  1000;
	b_knot[1].y= -600;
	clutter_actor_get_position (self->priv->controls_group, &x, &y);
	c_knot[0].x = x;
	c_knot[0].y = y;
	c_knot[1].x=  x;
	c_knot[1].y=  700;
 
	/* clutter_effect_path (effect_template, self->priv->title_group, t_knot, sizeof(t_knot) / sizeof(ClutterKnot), NULL, NULL); */
	/* clutter_effect_path (effect_template, self->priv->window_buttons_group, b_knot, sizeof(b_knot) / sizeof(ClutterKnot), NULL, NULL); */
	/* clutter_effect_path (effect_template, self->priv->controls_group, c_knot, sizeof(c_knot) / sizeof(ClutterKnot), NULL, NULL); */
	clutter_effect_fade (effect_template, self->priv->title_group, 0, NULL, NULL);
	clutter_effect_fade (effect_template, self->priv->window_buttons_group, 0, NULL, NULL);
	clutter_effect_fade (effect_template, self->priv->controls_group, 0, NULL, NULL);
	g_object_unref (effect_template);
	return TRUE;
}

gboolean
gb_player_show_controls (GbPlayer *self)
{
	ClutterEffectTemplate *effect_template;
	gint x, y;
	ClutterKnot t_knot[2];
	ClutterKnot b_knot[2];
	ClutterKnot c_knot[2];
	if ((!self->priv->playing) || (clutter_timeline_is_playing (self->priv->show_hide_timeline)))
		return TRUE;
	
	clutter_timeline_start(self->priv->show_hide_timeline);
	effect_template = clutter_effect_template_new (self->priv->show_hide_timeline, &on_alpha);
	clutter_actor_get_position (self->priv->title_group, &x, &y);
	t_knot[0].x = x;
	t_knot[0].y = y;
	t_knot[1].x= -20;
	t_knot[1].y= 20;
	clutter_actor_get_position (self->priv->window_buttons_group, &x, &y);
	b_knot[0].x = x;
	b_knot[0].y = y;
	b_knot[1].x=  640 - 200;
	b_knot[1].y=  20;
	clutter_actor_get_position (self->priv->window_buttons_group, &x, &y);
	c_knot[0].x = x;
	c_knot[0].y = y;
	c_knot[1].x=  ((640 / 2) - (400 / 2));
	c_knot[1].y=  480 - 70 - 30;
 
	/* clutter_effect_path (effect_template, self->priv->title_group, t_knot, sizeof(t_knot) / sizeof(ClutterKnot), NULL, NULL); */
	/* clutter_effect_path (effect_template, self->priv->window_buttons_group, b_knot, sizeof(b_knot) / sizeof(ClutterKnot), NULL, NULL); */
	/* clutter_effect_path (effect_template, self->priv->controls_group, c_knot, sizeof(c_knot) / sizeof(ClutterKnot), NULL, NULL); */
	clutter_effect_fade (effect_template, self->priv->title_group, 0x8f, NULL, NULL);
	clutter_effect_fade (effect_template, self->priv->window_buttons_group, 0x8f, NULL, NULL);
	clutter_effect_fade (effect_template, self->priv->controls_group, 0x8f, NULL, NULL);
	g_object_unref (effect_template);
	return TRUE;
}
	
ClutterActor *
gb_player_rounded_rect (GbPlayer *self,
						gint width,
						gint height,
						ClutterColor *bgcolor,
						gint round_radius)
{
	ClutterActor *actor;
	cairo_t *cr;
	
	actor = clutter_cairo_new (width, height);
	cr = clutter_cairo_create (CLUTTER_CAIRO (actor));
	cairo_move_to (cr, round_radius, 0);
	cairo_line_to (cr, width - round_radius, 0);
	cairo_curve_to (cr, width, 0, width, 0, width, round_radius);
	cairo_line_to (cr, width, height - round_radius);
	cairo_curve_to (cr, width, height, width, height, width - round_radius, height);
	cairo_line_to (cr, round_radius, height);
	cairo_curve_to (cr, 0, height, 0, height, 0, height - round_radius);
	cairo_line_to (cr, 0, round_radius);
	cairo_curve_to (cr, 0, 0, 0, 0, round_radius, 0);
	cairo_close_path (cr);
	
	clutter_cairo_set_source_color (cr, bgcolor);
	cairo_fill_preserve (cr);
	cairo_destroy (cr);
	return actor;
}

static void
_gb_player_setup_widgets (GbPlayer *self)
{
	GbPlayerPrivate *p = GB_PLAYER_GET_PRIVATE(self);
	ClutterColor cc_white = {0xff, 0xff, 0xff, 0x8f};
	ClutterColor cc_black = {0x00, 0x00, 0x00, 0xff};

	/* title group */
	p->title_group = clutter_group_new ();
	p->title_rect = gb_player_rounded_rect (self, 400, 36, &cc_white, 35);
	p->title_label = clutter_label_new_full ("DejaVu Sans",
											 "<span size='11500'><b>Metallica - Sad But True - Load(1991)</b></span>",
											 &cc_black);
	clutter_label_set_use_markup (CLUTTER_LABEL(p->title_label), TRUE);
	clutter_actor_set_position (p->title_group, -20, 20);
	clutter_actor_set_position (p->title_label, 50, 10);
	CLUTTER_GROUP_AND_SHOW(p->title_group, p->title_rect);
	CLUTTER_GROUP_AND_SHOW(p->title_group, p->title_label);

	/* window buttons group */
	p->window_buttons_group = clutter_group_new ();
	p->window_buttons_rect = gb_player_rounded_rect (self, 230, 90, &cc_white, 35);
	p->window_aspect_button = clutter_texture_new_from_pixbuf (p->fullscreen);
	p->window_open_file_button = clutter_texture_new_from_pixbuf (p->open_file);
	clutter_label_set_use_markup (CLUTTER_LABEL(p->title_label), TRUE);
	CLUTTER_GROUP_AND_SHOW(p->window_buttons_group, p->window_buttons_rect);
	CLUTTER_GROUP_AND_SHOW(p->window_buttons_group, p->window_aspect_button);
	CLUTTER_GROUP_AND_SHOW(p->window_buttons_group, p->window_open_file_button);
	clutter_actor_set_position (p->window_buttons_group, 640 - 200, 20);
	clutter_actor_set_position (p->window_open_file_button, 40, 20);
	clutter_actor_set_position (p->window_aspect_button, 130, 20);

	/* background group*/
	p->bg_group = clutter_group_new ();
	p->bg_video = clutter_gst_video_texture_new ();
	p->bg_image = clutter_texture_new_from_pixbuf (p->bgpixbuf);
	p->bg_logo = clutter_texture_new_from_pixbuf (p->bglogo);

	CLUTTER_GROUP_AND_SHOW(p->bg_group, p->bg_image);
	CLUTTER_GROUP_AND_SHOW(p->bg_group, p->bg_logo);
	CLUTTER_GROUP_AND_SHOW(p->bg_group, p->bg_video);
	clutter_actor_set_position (p->bg_logo, (640 / 2) - (467 / 2), (480 / 2) - (210 / 2));
	clutter_actor_set_size (p->bg_video, 640, 480);

	/* controls group */
	p->controls_group = clutter_group_new ();
	p->controls_rect = gb_player_rounded_rect (self, 400, 70, &cc_white, 45);
	p->controls_backward = clutter_texture_new_from_pixbuf (p->backward);
	p->controls_forward = clutter_texture_new_from_pixbuf (p->forward);
	p->controls_play_pause = clutter_texture_new_from_pixbuf (p->play);
	p->controls_stop = clutter_texture_new_from_pixbuf (p->stop);

	CLUTTER_GROUP_AND_SHOW(p->controls_group, p->controls_rect);
	CLUTTER_GROUP_AND_SHOW(p->controls_group, p->controls_backward);
	CLUTTER_GROUP_AND_SHOW(p->controls_group, p->controls_forward);
	CLUTTER_GROUP_AND_SHOW(p->controls_group, p->controls_play_pause);
	clutter_actor_set_position (p->controls_group, (640 / 2) - (400 / 2), 480 - 70 - 30);
	clutter_actor_set_position (p->controls_backward, 30, 15);
	clutter_actor_set_position (p->controls_play_pause, 170, 5);
	clutter_actor_set_position (p->controls_forward, 300, 15);

	CLUTTER_GROUP_AND_SHOW(p->stage, p->bg_group);
	CLUTTER_GROUP_AND_SHOW(p->stage, p->title_group);
	CLUTTER_GROUP_AND_SHOW(p->stage, p->controls_group);
	CLUTTER_GROUP_AND_SHOW(p->stage, p->window_buttons_group);

	gtk_widget_show_all (p->window);
	
	clutter_actor_set_reactive (p->controls_backward, TRUE);
	clutter_actor_set_reactive (p->controls_forward, TRUE);
	clutter_actor_set_reactive (p->controls_play_pause, TRUE);
	clutter_actor_set_reactive (p->window_aspect_button, TRUE);
	clutter_actor_set_reactive (p->window_open_file_button, TRUE);
	clutter_actor_set_reactive (p->stage, TRUE);

	clutter_actor_hide (p->bg_video);
	p->videosink = clutter_gst_video_sink_new (CLUTTER_TEXTURE(p->bg_video));
	g_object_set (p->player, "video-sink", p->videosink, NULL);
	


}
static void
_gb_player_connect_signals (GbPlayer *self)
{
	GbPlayerPrivate *p = GB_PLAYER_GET_PRIVATE(self);

	g_signal_connect(p->controls_backward, "enter-event", G_CALLBACK(on_actor_mouse_move), (gpointer) self);
	g_signal_connect(p->controls_play_pause, "enter-event", G_CALLBACK(on_actor_mouse_move), (gpointer) self);
	g_signal_connect(p->controls_forward, "enter-event", G_CALLBACK(on_actor_mouse_move), (gpointer) self);
	g_signal_connect(p->window_aspect_button, "enter-event", G_CALLBACK(on_actor_mouse_move), (gpointer) self);
	g_signal_connect(p->window_open_file_button, "enter-event", G_CALLBACK(on_actor_mouse_move), (gpointer) self);

	g_signal_connect(p->controls_backward, "leave-event", G_CALLBACK(on_actor_mouse_leave), (gpointer) self);
	g_signal_connect(p->controls_play_pause, "leave-event", G_CALLBACK(on_actor_mouse_leave), (gpointer) self);
	g_signal_connect(p->controls_forward, "leave-event", G_CALLBACK(on_actor_mouse_leave), (gpointer) self);
	g_signal_connect(p->window_aspect_button, "leave-event", G_CALLBACK(on_actor_mouse_leave), (gpointer) self);
	g_signal_connect(p->window_open_file_button, "leave-event", G_CALLBACK(on_actor_mouse_leave), (gpointer) self);

	g_signal_connect(p->controls_play_pause, "button-press-event", G_CALLBACK(on_click_play), (gpointer) self);
	g_signal_connect(p->window_open_file_button, "button-press-event", G_CALLBACK(on_click_open_file), (gpointer) self);

	g_signal_connect(p->stage, "motion-event", G_CALLBACK(on_mouse_move), (gpointer) self);
}
