
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
#include <glib.h>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>

typedef struct _svgdata {
  RsvgHandle *handle;
  cairo_t *cairo;
  cairo_surface_t *out;
  gchar *filename;
  GError *error;
} SvgData;

int
main (int argc, char **argv)
{
  SvgData *svg;
  cairo_status_t tt;
  rsvg_init ();
  
  svg = g_new0(SvgData, 1);
  svg->error = NULL;
  svg->out = cairo_pdf_surface_create ("teste.pdf", 1024.0, 768.0); 
  tt = cairo_surface_status (svg->out);
  g_print ("%s\n", cairo_status_to_string (tt));

  svg->filename = g_strdup_printf ("/home/gabriel/Imagens/svg/wall_pitonisto_1024.svg");
  svg->handle = rsvg_handle_new_from_file (svg->filename,
                                           &svg->error);
  if (svg->error)
    g_printerr ("%s\n", svg->error->message);

  svg->cairo = cairo_create (svg->out);
  cairo_set_source_rgb (svg->cairo, 0xff, 0xff, 0xff);
  cairo_paint (svg->cairo);
  cairo_move_to (svg->cairo, 0, 0);
  cairo_save (svg->cairo);
  rsvg_handle_render_cairo (svg->handle,
                            svg->cairo);
  cairo_surface_flush (svg->out);  
  cairo_restore (svg->cairo);
  cairo_show_page (svg->cairo);
  cairo_copy_page (svg->cairo);
  cairo_destroy (svg->cairo);
  cairo_surface_destroy (svg->out);
  return 0;
}
