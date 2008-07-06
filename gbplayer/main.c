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

int
main (int argc, char **argv)
{
  clutter_init(&argc, &argv);
  clutter_gst_init(&argc, &argv);
  gst_init(&argc, &argv);
  gtk_init(&argc, &argv);

  GbPlayer *player = g_object_new(GB_TYPE_PLAYER, NULL);
  gtk_main ();
  return 0;
}
