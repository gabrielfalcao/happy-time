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

#ifndef _PLANTAVEL_H
#define _PLANTAVEL_H

#define _GNU_SOURCE
#include <string.h>
#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS
#define PLANTAVEL_TYPE             (plantavel_get_type ())
#define IS_PLANTAVEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PLANTAVEL_TYPE))
#define PLANTAVEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), PLANTAVEL_TYPE, PlantavelClass))
typedef struct _Plantavel Plantavel;

typedef struct _PlantavelClass
{
  GTypeInterface base_interface;
  void (*plantar) (Plantavel *object);
  void (*desmatar) (Plantavel *object);
};


GType plantavel_get_type (void);

G_END_DECLS
#endif
