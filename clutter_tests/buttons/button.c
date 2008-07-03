/*
 * Copyright (C) 2008 Gabriel Falcão Gonçalves de Moura
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.  You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <gst/gst.h>
#include <clutter/clutter.h>
#include <clutter-gst/clutter-gst.h>
#include <clutter-gtk/gtk-clutter-embed.h>

#define STAGE_WIDTH 310
#define STAGE_HEIGHT 400

typedef struct _HandCar {
  ClutterActor *stage_actor;
  ClutterActor *label_actor;
  ClutterActor *label_time;
  ClutterActor *entry_actor;
  ClutterActor *btn_actor_play;
  ClutterActor *btn_actor_next;
  ClutterActor *btn_actor_previous;
  ClutterActor *container1;
  GstElement *player;
  GstFormat format;
  gboolean playing;
  GdkPixbuf *play;
  GdkPixbuf *previous;
  GdkPixbuf *next;
  GdkPixbuf *stop;
  GstElement *videosink;
  ClutterActor *video;
  ClutterActor *rect1;
  ClutterActor *rect2;
  ClutterActor *rect3;
  gint minutes;
  gint seconds;
  gint time;
} HandCar;
void play_video (HandCar *all);
void stop_video (HandCar *all);

gboolean
coord_within_actor (ClutterActor *actor, gint x, gint y)
{
  gint x1, x2, y1, y2;

  clutter_actor_get_coords (actor, &x1, &y1, &x2, &y2);

  return ((x >= x1 && x < x2 || x == x2) && (y >= y1 && y < y2 || y == y2));
}

void
on_click (ClutterActor *actor,
          ClutterEvent *event,
          gpointer data)
{
  HandCar *all = (HandCar *)data;
  gint x, y;
  gint64 pos = -1;

  clutter_event_get_coords (event, &x, &y);
  if (coord_within_actor (all->btn_actor_play, x, y))
    {
      if (!all->playing)
        {
          play_video (all);
          clutter_texture_set_pixbuf (CLUTTER_TEXTURE(all->btn_actor_play), all->stop, NULL);
          all->playing = TRUE;
        }
      else
        {
          stop_video (all);
          clutter_texture_set_pixbuf (CLUTTER_TEXTURE(all->btn_actor_play), all->play, NULL);
          all->playing = FALSE;
        }
    }
  else if ((coord_within_actor (all->btn_actor_next, x, y) && (all->playing)))
    {
      if (!gst_element_query_position (all->player, &all->format, &pos))
        pos = 0;

      gst_element_seek_simple (all->player, all->format, GST_SEEK_FLAG_FLUSH, pos + 5 * GST_SECOND);
    }
  else if ((coord_within_actor (all->btn_actor_previous, x, y) && (all->playing)))
    {
      if (!gst_element_query_position (all->player, &all->format, &pos))
        pos = 5 * GST_SECOND;

      gst_element_seek_simple (all->player, all->format, GST_SEEK_FLAG_FLUSH, pos - 5 * GST_SECOND);
    }

}

void
mouse_over_lighten (ClutterActor *actor,
                    ClutterEvent *event,
                    gpointer data)
{
  HandCar *all = (HandCar *)data;
  gint x, y;
  
  clutter_event_get_coords (event, &x, &y);
  if (coord_within_actor (all->btn_actor_previous, x, y))
    {
      clutter_actor_set_opacity (all->btn_actor_previous, 0x45);
      clutter_actor_set_opacity (all->btn_actor_play, 0xff);
      clutter_actor_set_opacity (all->btn_actor_next, 0xff);
    }
  else if (coord_within_actor (all->btn_actor_play, x, y))
    {
      clutter_actor_set_opacity (all->btn_actor_previous, 0xff);
      clutter_actor_set_opacity (all->btn_actor_play, 0x45);
      clutter_actor_set_opacity (all->btn_actor_next, 0xff);

    }
  else if (coord_within_actor (all->btn_actor_next, x, y))
    {
      clutter_actor_set_opacity (all->btn_actor_previous, 0xff);
      clutter_actor_set_opacity (all->btn_actor_play, 0xff);
      clutter_actor_set_opacity (all->btn_actor_next, 0x45);

    }
  else
    {
      clutter_actor_set_opacity (all->btn_actor_previous, 0xff);
      clutter_actor_set_opacity (all->btn_actor_play, 0xff);
      clutter_actor_set_opacity (all->btn_actor_next, 0xff);
    }


}
void
on_update_clock(ClutterTimeline *timeline,
                      gint frame_num,
                      gpointer data)
{
  HandCar *all = (HandCar *)data;

  gchar *txt;
  gint64 time = -1;
  gint hours, minutes, seconds;
  

  if(!gst_element_query_position (all->player, &all->format, &time))
      clutter_label_set_text (CLUTTER_LABEL(all->label_time), "<span size='6000'>UNKNOWN</span>");
  else if (time != -1)
    {
      time /= GST_SECOND;
      hours = time / (60 * 60);
      minutes = (time - (hours *60 * 60)) / 60;
      seconds = time % 60;

      txt = g_strdup_printf ("%02d:%02d:%02d", 
                             hours, 
                             minutes,
                             seconds);

      clutter_label_set_text(CLUTTER_LABEL(all->label_time), txt);
      g_free (txt);
    }

}

void
on_timeline_new_frame(ClutterTimeline *timeline,
                      gint frame_num,
                      gpointer data)
{
  HandCar *all = (HandCar *)data;
  gint x, y, actor_width, actor_height;

  
  clutter_actor_get_position(all->label_actor, &x, &y);
  clutter_actor_get_size(all->label_actor, &actor_width, &actor_height);

  if (x < (actor_width * -1))
    x = STAGE_WIDTH;
  else
    x -= 10;
  
  clutter_actor_set_position(all->label_actor, x, y);
}


HandCar *
handcar_new()
{
  HandCar *all = g_new0(HandCar, 1);
  ClutterColor rect_color = { 0xdd, 0xdd, 0xdd, 0xee };
  ClutterColor rect_border_color = { 0xee, 0xee, 0xee, 0xdd };
  ClutterColor rect_entry_color = { 0xef, 0xef, 0xef, 0xee };
  ClutterColor rect_entry_border_color = { 0x96, 0x96, 0x96, 0xaa };

  ClutterColor label_color = { 0x55, 0x00, 0x00, 0xff };
  ClutterColor time_color = { 0x00, 0x33, 0x55, 0xbb };

  GError *error = NULL;

  all->play = gdk_pixbuf_new_from_file("imgs/start.svg", NULL);
  all->previous = gdk_pixbuf_new_from_file("imgs/backward.svg", NULL);
  all->next = gdk_pixbuf_new_from_file("imgs/forward.svg", NULL);
  all->stop = gdk_pixbuf_new_from_file("imgs/stop.svg", NULL);

  all->time = 0;
  all->playing = FALSE;

  all->player = gst_element_factory_make ("playbin", "player");

  if (error)
      g_error ("FUDEU: %s", error->message);

  all->label_actor = clutter_label_new_full("DejaVu Sans",
                                       "Blind Guardian - <b>Mirror Mirror</b>",
                                       &label_color);
  clutter_label_set_use_markup (CLUTTER_LABEL(all->label_actor), TRUE);
  all->label_time = clutter_label_new_full("DejaVu Sans",
                                       "00:00",
                                       &time_color);
  clutter_label_set_use_markup (CLUTTER_LABEL(all->label_time), TRUE);

  clutter_actor_set_position (all->label_actor, 10, 15);
  clutter_actor_set_position (all->label_time, STAGE_WIDTH - 80, STAGE_HEIGHT - 20);

  all->btn_actor_play = clutter_texture_new_from_pixbuf (all->play);
  all->btn_actor_next = clutter_texture_new_from_pixbuf (all->next);
  all->btn_actor_previous = clutter_texture_new_from_pixbuf (all->previous);

  clutter_actor_set_position (all->btn_actor_previous, 60, 50);
  clutter_actor_set_position (all->btn_actor_play, 140, 50);
  clutter_actor_set_position (all->btn_actor_next, 200, 50);
  all->rect1 = clutter_rectangle_new_with_color (&rect_color);
  clutter_actor_set_size (all->rect1, 320, 30);
  clutter_actor_set_position (all->rect1, -5, 10);
  clutter_rectangle_set_border_width (CLUTTER_RECTANGLE(all->rect1), 4);
  clutter_rectangle_set_border_color (CLUTTER_RECTANGLE(all->rect1), &rect_border_color);

  all->rect2 = clutter_rectangle_new_with_color (&rect_entry_color);
  clutter_actor_set_size (all->rect2, 270, 20);
  clutter_actor_set_position (all->rect2, 20, STAGE_HEIGHT - 50);
  clutter_rectangle_set_border_width (CLUTTER_RECTANGLE(all->rect2), 1);
  clutter_rectangle_set_border_color (CLUTTER_RECTANGLE(all->rect2), &rect_entry_border_color);

  all->rect3 = clutter_rectangle_new_with_color (&rect_entry_color);
  clutter_actor_set_size (all->rect3, 270, 203);
  clutter_actor_set_position (all->rect3, 20, 120);
  clutter_rectangle_set_border_width (CLUTTER_RECTANGLE(all->rect3), 1);
  clutter_rectangle_set_border_color (CLUTTER_RECTANGLE(all->rect3), &rect_entry_border_color);


  all->entry_actor = clutter_entry_new_full ("Monospace",
                                             "Teste",
                                             &label_color);
  clutter_actor_set_size (all->entry_actor, 270, 20);
  clutter_actor_set_position (all->entry_actor, 20, STAGE_HEIGHT - 50);

  all->video = clutter_gst_video_texture_new ();
  clutter_actor_set_size (all->video, 270, 203);
  clutter_actor_set_position (all->video, 20, 120);
  clutter_actor_set_opacity (all->video, 0x50);

  //  all->clutter_group_add_many (all->container1, all->rect1);
  all->videosink = clutter_gst_video_sink_new (CLUTTER_TEXTURE(all->video));
  g_object_set (all->player, "video-sink", all->videosink, NULL);

  all->format = GST_FORMAT_TIME;
  clutter_actor_show(all->btn_actor_play);
  clutter_actor_show(all->btn_actor_next);
  clutter_actor_show(all->btn_actor_previous);
  clutter_actor_show(all->label_actor);
  clutter_actor_show(all->label_time);
  clutter_actor_show(all->rect1);
  clutter_actor_show(all->rect2);
  clutter_actor_show(all->rect3);
  clutter_actor_show(all->video);
  clutter_actor_show(all->entry_actor);
  return all;
}

void
play_video (HandCar *all)
{
  g_object_set (all->player, "uri", "file:///home/gabriel/Desktop/Downloads/Brasileirinhas.200806.Tomando.no.Cu.Cena1.Babalu.XXX.DVDRip.wmv", NULL);
  //  g_object_set (all->player, "uri", "file:///home/gabriel/Desktop/Downloads/xoxotao.wmv", NULL);
  gst_element_set_state (all->player, GST_STATE_PLAYING);

}

void
stop_video (HandCar *all)
{
  gst_element_set_state (all->player, GST_STATE_NULL);
}

int main(int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *stage;
  ClutterColor stage_color = { 0xff, 0xff, 0xff, 0xff };
  ClutterTimeline *timeline, *clock;
  GError *error = NULL;
  HandCar *all;

  clutter_init(&argc, &argv);
  clutter_gst_init(&argc, &argv);
  gst_init(&argc, &argv);
  gtk_init(&argc, &argv);

  all = handcar_new ();
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW(window), "Clutter Player");
  g_signal_connect(G_OBJECT(window), "delete-event",
                   G_CALLBACK(gtk_main_quit), NULL);

  stage = gtk_clutter_embed_new();
  gtk_widget_set_size_request(stage, STAGE_WIDTH, STAGE_HEIGHT);
  gtk_container_add(GTK_CONTAINER(window), stage);

  all->stage_actor = gtk_clutter_embed_get_stage(GTK_CLUTTER_EMBED(stage));

  clutter_container_add_actor(CLUTTER_CONTAINER(all->stage_actor), all->rect1);
  clutter_container_add_actor(CLUTTER_CONTAINER(all->stage_actor), all->rect2);
  clutter_container_add_actor(CLUTTER_CONTAINER(all->stage_actor), all->rect3);
  clutter_container_add_actor(CLUTTER_CONTAINER(all->stage_actor), all->btn_actor_play);
  clutter_container_add_actor(CLUTTER_CONTAINER(all->stage_actor), all->btn_actor_previous);
  clutter_container_add_actor(CLUTTER_CONTAINER(all->stage_actor), all->btn_actor_next);
  clutter_container_add_actor(CLUTTER_CONTAINER(all->stage_actor), all->label_actor);
  clutter_container_add_actor(CLUTTER_CONTAINER(all->stage_actor), all->label_time);
  clutter_container_add_actor(CLUTTER_CONTAINER(all->stage_actor), all->video);
  clutter_container_add_actor(CLUTTER_CONTAINER(all->stage_actor), all->entry_actor);

  clutter_stage_set_color(CLUTTER_STAGE(all->stage_actor), &stage_color);

  timeline = clutter_timeline_new (100, 5);
  g_signal_connect(G_OBJECT(timeline), "new-frame",
                   G_CALLBACK(on_timeline_new_frame),
                   (gpointer)all);
  clutter_timeline_set_loop(timeline, TRUE);
  clutter_timeline_start(timeline);

  clock = clutter_timeline_new (60, 2);
  g_signal_connect(G_OBJECT(clock), "new-frame",
                   G_CALLBACK(on_update_clock),
                   (gpointer)all);
  clutter_timeline_set_loop(clock, TRUE);
  clutter_timeline_start(clock);

  g_signal_connect (all->stage_actor, "motion-event", G_CALLBACK(mouse_over_lighten), (gpointer) all);
  g_signal_connect (all->stage_actor, "button-press-event", G_CALLBACK(on_click), (gpointer) all);
  clutter_actor_raise_top (all->entry_actor);
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
