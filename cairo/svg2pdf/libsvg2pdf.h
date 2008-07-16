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
#include <string.h>
#include <gio/gio.h>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>

typedef struct _svgdata {
  RsvgHandle *handle;
  cairo_t *cairo;
  cairo_surface_t *out;
  cairo_status_t status;
  GError *error;
} SvgData;

void svg_file_to_pdf_file (const gchar *source_filename, const gchar *destination_filename, gdouble width, gdouble height);
guint8 *svg_data_to_pdf_data_with_destination_size (const guint8 *source, gsize source_len, gdouble width, gdouble height, gsize *destination_len);
void svg2pdf (const gchar *source_filename, const gchar *destination_filename);
void svg2pdf2 (const gchar *source_filename, const gchar *destination_filename);
void svg_file_to_pdf_file2 (const gchar *source_filename, const gchar *destination_filename, gdouble width, gdouble height);
guint8 * svg_data_to_pdf_data_with_destination_size (const guint8 *source, gsize source_len, gdouble width, gdouble height, gsize *destination_len);
