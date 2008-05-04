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

#ifndef _CENOURA_H
#define _CENOURA_H

#define _GNU_SOURCE
#include <string.h>
#include <glib.h>
#include <glib-object.h>
/* inheriting */
#include "vegetal.h"

G_BEGIN_DECLS
#define CENOURA_TYPE             (cenoura_get_type ())
#define CENOURA(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CENOURA_TYPE, Cenoura))
#define CENOURA_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CENOURA_TYPE, CenouraClass))
#define IS_CENOURA(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CENOURA_TYPE))
#define IS_CENOURA_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CENOURA_TYPE))
#define CENOURA_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CENOURA_TYPE, CenouraClass))
typedef struct _CenouraClass CenouraClass;
typedef struct _Cenoura Cenoura;

struct _CenouraClass
{
  VegetalClass *parent;
  /* class members */
};

struct _Cenoura
{
  Vegetal parent;
  /* instance members */
  gboolean branca;
};

enum {
  PROP_CENOURA_0,
  PROP_CENOURA_BRANCA,
};

GType cenoura_get_type (void);

Cenoura *cenoura_new (void);

G_END_DECLS
#endif
