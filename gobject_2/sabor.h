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

#ifndef _SABOR_H
#define _SABOR_H

#define _GNU_SOURCE
#include <string.h>
#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS
#define SABOR_TYPE             (sabor_get_type ())
#define SABOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SABOR_TYPE, Sabor))
#define SABOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SABOR_TYPE, SaborClass))
#define IS_SABOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SABOR_TYPE))
#define IS_SABOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SABOR_TYPE))
#define SABOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), SABOR_TYPE, SaborClass))
typedef struct _SaborClass SaborClass;
typedef struct _Sabor Sabor;

struct _SaborClass
{
  GObjectClass parent;
  /* class members */
};

struct _Sabor
{
  GObject parent;
  /* instance members */
  gboolean amargo;
  gboolean doce;
  gboolean acido;
  gboolean salgado;
  gchar *sabor;
};

enum {
  PROP_SABOR_0,
  PROP_SABOR_AMARGO,
  PROP_SABOR_DOCE ,
  PROP_SABOR_ACIDO ,
  PROP_SABOR_SALGADO
};

GType sabor_get_type (void);

Sabor *sabor_new (void);

G_END_DECLS
#endif
