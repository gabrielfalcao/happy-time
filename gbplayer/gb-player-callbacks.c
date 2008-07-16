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

#include "gb-player-callbacks.h"
#include "gb-player.h"

static gboolean
coord_within_actor (ClutterActor *actor, gint x, gint y)
{
  gint x1, x2, y1, y2;

  clutter_actor_get_coords (actor, &x1, &y1, &x2, &y2);

  return ((x >= x1 && x < x2 || x == x2) && (y >= y1 && y < y2 || y == y2));
}

void
on_actor_mouse_move (ClutterActor *actor,
                     ClutterEvent *event,
                     gpointer user_data)
{
	clutter_actor_set_opacity (actor, 0x95);
}

void
on_actor_mouse_leave (ClutterActor *actor,
                      ClutterEvent *event,
                      gpointer user_data)
{
	clutter_actor_set_opacity (actor, 0xff);
}

void
on_click_play (ClutterActor *actor,
               ClutterEvent *event,
               gpointer user_data)
{
	GbPlayer *player = GB_PLAYER (user_data);
	
	if (player->priv->playing)
		gb_player_stop (player);
	else
		gb_player_play (player);
}

void
playback_cb (ClutterTimeline *timeline,
             gint frame_num,
             gpointer data)
{

}

void
show_hide_effect_cb (ClutterTimeline *timeline,
					 gint frame_num,
					 gpointer data)
{
	gchar *txt;
	gint64 time = -1;
	gint hours, minutes, seconds;
	GbPlayer *player = GB_PLAYER(data);
	GstFormat format = GST_FORMAT_TIME;

	if (!GST_IS_ELEMENT(player->priv->player))
		return;

	if(gst_element_query_position (player->priv->player, &format, &time) && time != -1)
		{
			time /= GST_SECOND;
			hours = time / (60 * 60);
			minutes = (time - (hours *60 * 60)) / 60;
			seconds = time % 60;

			txt = g_strdup_printf ("%02d:%02d:%02d", 
								   hours, 
								   minutes,
								   seconds);
			g_print ("%s\n", txt);
		}

}
static void
finish_file_open (GtkDialog *dialog,
                  gint arg1,
                  gpointer user_data)
{
	GbPlayer *player = GB_PLAYER(user_data);	
	gchar *fname;
	fname = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER(player->priv->filechooser));
	g_object_set (player, "file", fname, NULL);
	g_free (fname);
	gtk_widget_destroy (GTK_WIDGET(dialog));
}

void
on_click_open_file (ClutterActor *actor,
					ClutterEvent *event,
					gpointer user_data)
{
	GbPlayer *player = GB_PLAYER(user_data);

	player->priv->filechooser = gtk_file_chooser_dialog_new ("Choose the media file", 
												  GTK_WINDOW(player->priv->window),
												  GTK_FILE_CHOOSER_ACTION_OPEN,
												  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
												  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
												  NULL);

	g_signal_connect (player->priv->filechooser, "response",
					  G_CALLBACK(finish_file_open), user_data);

	gtk_widget_show (GTK_WIDGET(player->priv->filechooser));
}

guint32
on_alpha (ClutterAlpha *alpha, gpointer data)
{
	ClutterTimeline *timeline = clutter_alpha_get_timeline (alpha);
	const int current_frame_num = clutter_timeline_get_current_frame (timeline);
	const int n_frames = clutter_timeline_get_n_frames (timeline);

	return (CLUTTER_ALPHA_MAX_ALPHA * current_frame_num / n_frames);
}
void
on_timeline_completed (ClutterTimeline *timeline,
                       gpointer user_data)
{
	clutter_timeline_stop (timeline);
}

void
on_mouse_move (ClutterActor *actor,
               ClutterEvent *event,
               gpointer user_data)
{
	gint x, y;
	GbPlayer *player = GB_PLAYER(user_data);

	clutter_event_get_coords (event, &x, &y);
	if ((coord_within_actor(player->priv->controls_group, x, y)) ||
		(coord_within_actor(player->priv->window_buttons_group, x, y)) ||
		(coord_within_actor(player->priv->title_group, x, y)) && 
		(!player->priv->controls_visible))
		{
			clutter_timeline_stop (player->priv->show_hide_timeline);
			gb_player_show_controls (player);
			player->priv->controls_visible = TRUE;
		}
	else
		{
			clutter_timeline_stop (player->priv->show_hide_timeline);
			gb_player_hide_controls (player);
			player->priv->controls_visible = FALSE;
		}
}
