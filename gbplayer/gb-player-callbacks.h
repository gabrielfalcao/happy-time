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
#include <gst/gst.h>
#include <clutter/clutter.h>
#include <clutter-gst/clutter-gst.h>

void
on_actor_mouse_move (ClutterActor *actor,
                     ClutterEvent *event,
                     gpointer user_data);
void
on_actor_mouse_leave (ClutterActor *actor,
                      ClutterEvent *event,
                      gpointer user_data);
void
on_click_play (ClutterActor *actor,
               ClutterEvent *event,
               gpointer user_data);
void
playback_cb (ClutterTimeline *timeline,
             gint frame_num,
             gpointer data);

void
show_hide_effect_cb (ClutterTimeline *timeline,
                     gint frame_num,
                     gpointer data);

void
on_click_open_file (ClutterActor *actor,
                    ClutterEvent *event,
                    gpointer user_data);
guint32
on_alpha (ClutterAlpha *alpha, gpointer data);

void
on_timeline_completed (ClutterTimeline *timeline,
                       gpointer user_data);
void
on_mouse_move (ClutterActor *actor,
               ClutterEvent *event,
               gpointer user_data);
