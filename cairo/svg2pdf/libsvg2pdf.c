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
#define _GNU_SOURCE
#include <stdio.h>
#include "libsvg2pdf.h"
#define return_and_tell_if_fail(condition, string, format) \
  if (!condition) g_printerr (string, format); return
#define return_if_no_file(filename) return_and_tell_if_fail \
  (g_file_test (filename, G_FILE_TEST_EXISTS), \
   "The file \"%s\" does not exist\n", filename)

static cairo_status_t
append_to_string (void *closure,
                unsigned char *data,
                unsigned int length)
{
  ClosureData *cdata = (ClosureData *) closure;
  cdata->length += length;
  cdata->data = (gchar*)realloc(cdata->data, sizeof(gchar)*cdata->length);
  memcpy(cdata->data+(cdata->length-length), data, length);
  return CAIRO_STATUS_SUCCESS;
}

ClosureData *
svg_data_to_pdf_data_with_destination_size (const gchar *source, 
                                            gsize source_len,
                                            gdouble width,
                                            gdouble height)
{
  RsvgHandle *svg = NULL;
  RsvgDimensionData dimensions;
  cairo_t *cairo;
  cairo_surface_t *surface;
  cairo_status_t status;
  gchar *pdfdata = NULL;
  GError *error = NULL;
  gsize len;
  ClosureData *cdata;
  rsvg_init ();
  
  if (source_len == 0)
    source_len = strlen (source) -1;

  svg = rsvg_handle_new_from_data ((guint8 *) source, source_len, &error);
  cdata = g_new0(ClosureData, 1);
  cdata->data = (gchar*)malloc(sizeof(gchar));
  cdata->length = 0;

  if (error)
    {
      g_printerr ("Error when mapping the SVG. Check the xml compliance!\n%s\n",
                  error->message);
      return;
    }
  if (!svg)
    {
      g_printerr ("Impossible to create the rsvg handler\n");
      return;
    }
    
  rsvg_handle_get_dimensions (svg, &dimensions);
  if ((width < 0) || (height <0))
    {
      width = dimensions.width;
      height = dimensions.height;
    }

  surface = cairo_pdf_surface_create_for_stream ((cairo_write_func_t) append_to_string, 
                                                 (void *) cdata, width, height);
  status = cairo_surface_status (surface);

  if (status)
    {
      g_printerr ("Error when creating the PDF data\n");
      return;
    }
  
  cairo = cairo_create (surface);
  cairo_set_source_rgb (cairo, 0xff, 0xff, 0xff);
  cairo_paint (cairo);
  cairo_move_to (cairo, 0, 0);
  cairo_save (cairo);
  rsvg_handle_render_cairo (svg, cairo);
  cairo_surface_flush (surface);  
  cairo_restore (cairo);
  cairo_show_page (cairo);
  cairo_destroy (cairo);
  cairo_surface_destroy (surface);
  rsvg_handle_close (svg, NULL);
  g_object_unref (svg);
  return cdata;
}

void
svg_file_to_pdf_file (const gchar *source_filename, 
                    const gchar *destination_filename,
                    gdouble width,
                    gdouble height)
{
  RsvgHandle *svg = NULL;
  RsvgDimensionData dimensions;
  cairo_t *cairo;
  cairo_surface_t *surface;
  cairo_status_t status;
  GError *error = NULL;

  rsvg_init ();

  svg = rsvg_handle_new_from_file (source_filename, &error);
  if (error)
    {
      g_printerr ("Error when loading the SVG file \"%s\":%s\n",
                  source_filename, error->message);
      return;
    }

  if (!svg)
    {
      g_printerr ("Impossible to create the rsvg handler\n");
      return;
    }
    
  rsvg_handle_get_dimensions (svg, &dimensions);
  if ((width < 0) || (height <0))
    {
      width = dimensions.width;
      height = dimensions.height;
    }
  surface = cairo_pdf_surface_create (destination_filename, width, height); 
  status = cairo_surface_status (surface);
  if (status)
    {
      g_printerr ("Error when creating the PDF file \"%s\":%s\n",
                  destination_filename, cairo_status_to_string (status));
      return;
    }
  
  cairo = cairo_create (surface);
  cairo_set_source_rgb (cairo, 0xff, 0xff, 0xff);
  cairo_paint (cairo);
  cairo_move_to (cairo, 0, 0);
  cairo_save (cairo);
  rsvg_handle_render_cairo (svg, cairo);
  cairo_surface_flush (surface);  
  cairo_restore (cairo);
  cairo_show_page (cairo);
  cairo_destroy (cairo);
  cairo_surface_destroy (surface);
  rsvg_handle_close (svg, NULL);
  g_object_unref (svg);
}
void
svg_file_to_pdf_file2 (const gchar *source_filename, 
                      const gchar *destination_filename,
                      gdouble width,
                      gdouble height)
{
  ClosureData *cdata;
  gsize srcsize;
  gchar *srcdata;
  GError *error = NULL;
  FILE *file;

  g_file_get_contents (source_filename, &srcdata, &srcsize, NULL);
  cdata = svg_data_to_pdf_data_with_destination_size (srcdata,
                                                      srcsize,
                                                      width,
                                                      height);

  file = fopen (destination_filename, "wb");
  fwrite (cdata->data, sizeof(gchar), cdata->length, file);
  fclose(file);
  if (error)
    {
      g_printerr ("%s\n", error->message);
      return;
    }
}

void
svg2pdf (const gchar *source_filename,
        const gchar *destination_filename)
{
  svg_file_to_pdf_file (source_filename,
                        destination_filename, -1, -1);
}

void
svg2pdf2 (const gchar *source_filename,
        const gchar *destination_filename)
{
  svg_file_to_pdf_file2 (source_filename,
                         destination_filename, -1, -1);
}
