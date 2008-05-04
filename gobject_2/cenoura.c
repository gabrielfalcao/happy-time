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
#include "cenoura.h"
#include "sabor.h"
static void cenoura_class_init (CenouraClass * klass);
static void cenoura_init (Cenoura * self);
static void cenoura_set_property (GObject * object, guint property_id,
                                const GValue * value, GParamSpec * pspec);
static void
cenoura_get_property (GObject * object, guint property_id, GValue * value,
                    GParamSpec * pspec);

GType
cenoura_get_type (void)
{
  static GType cenoura_type = 0;

  if (cenoura_type == 0)
    {
      cenoura_type = g_type_register_static_simple (G_TYPE_OBJECT,
                                                    "Cenoura",
                                                    sizeof (CenouraClass),
                                                    (GClassInitFunc)
                                                    cenoura_class_init,
                                                    sizeof (Cenoura),
                                                    (GInstanceInitFunc)
                                                    cenoura_init, 0);
    }
  return cenoura_type;
}

static void
cenoura_class_init (CenouraClass * klass)
{
  GParamSpec *branca_param;
  GObjectClass *g_object_class;

  klass = CENOURA_CLASS (klass);
  g_object_class = G_OBJECT_CLASS (klass);

  branca_param = g_param_spec_boolean ("branca",        /* identificação */
                                           "branca ?",      /*nickname */
                                           "o cenoura eh branca ?",  /*descrição */
                                           FALSE, G_PARAM_READWRITE     /*Public */
    );


  g_object_class->set_property = cenoura_set_property;
  g_object_class->get_property = cenoura_get_property;
}

static void
cenoura_init (Cenoura * self)
{
  self = CENOURA (self);
}

static void
cenoura_set_property (GObject * object, guint property_id, const GValue * value,
                    GParamSpec * pspec)
{
  Cenoura *self = CENOURA (object);

  switch (property_id)
    {
    case PROP_CENOURA_BRANCA:
      {
        self->branca = g_value_get_boolean (value);
      }
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
cenoura_get_property (GObject * object,
                    guint property_id, GValue * value, GParamSpec * pspec)
{
  Cenoura *self = CENOURA (object);

  switch (property_id)
    {
    case PROP_CENOURA_BRANCA:
      {
        g_value_set_boolean (value, self->branca);
      }
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}
Cenoura *
cenoura_new (void)
{
  Cenoura *obj;

  obj = g_object_new (CENOURA_TYPE, NULL);
  return obj;
}
