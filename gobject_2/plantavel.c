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
#include "plantavel.h"

static guint plantavel_base_init_count = 0;

static void plantavel_base_init (PlantavelClass * klass);
static void plantavel_base_finalize (PlantavelClass * klass);

GType
plantavel_get_type (void)
{
  static GType plantavel_type = 0;

  if (plantavel_type == 0)
    {
      static const GTypeInfo plantavel_info = {
        sizeof (PlantavelClass),
        (GBaseInitFunc) plantavel_base_init,
        (GBaseFinalizeFunc) plantavel_base_finalize
      };
      plantavel_type = g_type_register_static (G_TYPE_INTERFACE,
                                               "Plantavel",
                                               &plantavel_info, 0);
    }
  return plantavel_type;
}


static void plantavel_base_init (PlantavelClass * klass)
{
  plantavel_base_init_count++;
  if (plantavel_base_init_count == 1)
  {
    /*register signals*/
  }
}

static void plantavel_base_finalize (PlantavelClass * klass)
{
  plantavel_base_init_count--;
  if (plantavel_base_init_count == 0)
  {
    /*unregister signals*/
  }
}
