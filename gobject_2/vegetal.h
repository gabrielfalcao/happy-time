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

#ifndef _VEGETAL_H
#define _VEGETAL_H

#define _GNU_SOURCE
#include <string.h>
#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS
#define VEGETAL_TYPE             (vegetal_get_type ())
#define VEGETAL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), VEGETAL_TYPE, Vegetal))
#define VEGETAL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), VEGETAL_TYPE, VegetalClass))
#define IS_VEGETAL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VEGETAL_TYPE))
#define IS_VEGETAL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), VEGETAL_TYPE))
#define VEGETAL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), VEGETAL_TYPE, VegetalClass))
typedef struct _VegetalClass VegetalClass;
typedef struct _Vegetal Vegetal;

struct _VegetalClass
{
  GObjectClass parent;
  /* class members */
};

struct _Vegetal
{
  GObject parent;
  /* instance members */
  gchar *cor;
  gboolean comestivel;
  gpointer sabor;
  gdouble tamanho;
  gchar *tipo;
};

enum {
  PROP_VEGETAL_0,
  PROP_VEGETAL_COR,
  PROP_VEGETAL_COMESTIVEL,
  PROP_VEGETAL_SABOR,
  PROP_VEGETAL_TAMANHO,
  PROP_VEGETAL_TIPO
};

GType vegetal_get_type (void);

Vegetal *vegetal_new (void);

G_END_DECLS
#endif
