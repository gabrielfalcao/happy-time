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

typedef struct _closure_data {
  gchar *data;
  guint length;
} ClosureData;

static void *
get_file_contents (const char *filename)
{
  FILE *fh;
  char *contents;
  char *tmp;
  contents = g_malloc0 (256);
  tmp = g_malloc (256);
  fh = fopen (filename, "rb");
  if (!fh)
    return NULL;
  while (!feof (fh))
    {
      fread (tmp, 256, 1, fh);
      asprintf (&contents, "%s%s", contents, tmp);
      g_free (tmp);
      tmp = g_malloc0(256);
    }
  fclose (fh);
  g_free (tmp);
  return contents;
}

static cairo_status_t
append_to_string (void *closure,
                unsigned char *data,
                unsigned int length)
{
  ClosureData *cdata = (ClosureData *) closure;
  gchar *new = g_strdup(cdata->data);
  g_free (cdata->data);
  cdata->data = g_strconcat (new, (gchar *) data, NULL);
  /* g_strlcat (cdata->data, (gchar *)data, length + strlen(cdata->data)); */
  return CAIRO_STATUS_SUCCESS;
}

guint8 *
svg_data_to_pdf_data_with_destination_size (const guint8 *source, 
                                            gsize source_len,
                                            gdouble width,
                                            gdouble height,
                                            gsize *destination_len)
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
  
  if (source_len < 0)
    source_len = strlen (source);
  gchar *zaz = (gchar *) source;
  svg = rsvg_handle_new_from_data ((guint8 *)zaz, source_len, &error);
  cdata = g_new0(ClosureData, 1);
  cdata->data = g_strdup_printf ("");

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
  len = (gsize) strlen(cdata->data);
  destination_len = &len;
  return (guint8 *) cdata->data;
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
  
  gsize srcsize, dstsize;
  gchar *srcdata, *dstdata;
  GError *error = NULL;
  FILE *file;

  srcdata = (gchar *) get_file_contents (source_filename);
  dstdata = svg_data_to_pdf_data_with_destination_size ((guint8 *) srcdata,
                                                        srcsize,
                                                        width,
                                                        height,
                                                        &dstsize);

  file = fopen (destination_filename, "wb");
  fwrite (dstdata, sizeof(char), dstsize, file);
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
