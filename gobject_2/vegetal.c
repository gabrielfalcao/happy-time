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
#include "vegetal.h"
#include "sabor.h"
static void vegetal_class_init (VegetalClass * klass);
static void vegetal_init (Vegetal * self);
static void vegetal_set_property (GObject * object, guint property_id,
                                const GValue * value, GParamSpec * pspec);
static void
vegetal_get_property (GObject * object, guint property_id, GValue * value,
                    GParamSpec * pspec);

GType
vegetal_get_type (void)
{
  static GType vegetal_type = 0;

  if (vegetal_type == 0)
    {
      vegetal_type = g_type_register_static_simple (G_TYPE_OBJECT,
                                                    "Vegetal",
                                                    sizeof (VegetalClass),
                                                    (GClassInitFunc)
                                                    vegetal_class_init,
                                                    sizeof (Vegetal),
                                                    (GInstanceInitFunc)
                                                    vegetal_init, 0);
    }
  return vegetal_type;
}

static void
vegetal_class_init (VegetalClass * klass)
{
  GParamSpec *cor_param;
  GParamSpec *comestivel_param;
  GParamSpec *sabor_param;
  GParamSpec *tamanho_param;
  GParamSpec *tipo_param;
  GObjectClass *g_object_class;

  klass = VEGETAL_CLASS (klass);
  g_object_class = G_OBJECT_CLASS (klass);

  cor_param = g_param_spec_string ("cor",       /* identificação */
                                   "cor do vegetal",    /*nickname */
                                   "cor do vegetal",    /*descrição */
                                   "",
                                   G_PARAM_READWRITE    /*Public */
    );

  comestivel_param = g_param_spec_boolean ("comestivel",        /* identificação */
                                           "comestivel ?",      /*nickname */
                                           "o vegetal eh comestivel?",  /*descrição */
                                           FALSE, G_PARAM_READWRITE     /*Public */
    );

  sabor_param = g_param_spec_object ("sabor", /* identificação */
      "sabor do vegetal",    /*nickname */
      "sabor do vegetal",    /*descrição */
      SABOR_TYPE,
      G_PARAM_READWRITE    /*Public */
      );

  tamanho_param = g_param_spec_double ("tamanho", /* identificação */
      "tamanho do vegetal",    /*nickname */
      "tamanho do vegetal",    /*descrição */
      1.0, 
      100.0,
      20.0,
      G_PARAM_READWRITE    /*Public */
      );

  tipo_param = g_param_spec_string ("tipo",       /* identificação */
                                   "tipo do vegetal",    /*nickname */
                                   "tipo do vegetal",    /*descrição */
                                   "",
                                   G_PARAM_READWRITE    /*Public */
    );

  g_object_class->set_property = vegetal_set_property;
  g_object_class->get_property = vegetal_get_property;
}

static void
vegetal_init (Vegetal * self)
{
  self = VEGETAL (self);
}

static void
vegetal_set_property (GObject * object, guint property_id, const GValue * value,
                    GParamSpec * pspec)
{
  Vegetal *self = VEGETAL (object);

  switch (property_id)
    {
    case PROP_VEGETAL_COR:
      {
        self->cor = (gchar *) g_value_get_string (value);
      }
      break;
    case PROP_VEGETAL_COMESTIVEL:
      {
        self->comestivel = g_value_get_boolean (value);
      }
      break;
    case PROP_VEGETAL_SABOR:
      {
        self->sabor = SABOR(g_value_get_object (value));
      }
      break;
    case PROP_VEGETAL_TAMANHO:
      {
        self->tamanho = g_value_get_double (value);
      }
      break;
    case PROP_VEGETAL_TIPO:
      {
        self->tipo = (gchar *)g_value_get_string (value);
      }
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
vegetal_get_property (GObject * object,
                    guint property_id, GValue * value, GParamSpec * pspec)
{
  Vegetal *self = VEGETAL (object);

  switch (property_id)
    {
    case PROP_VEGETAL_COR:
      {
        g_value_set_string (value, self->cor);
      }
      break;
    case PROP_VEGETAL_COMESTIVEL:
      {
        g_value_set_boolean (value,self->comestivel);
      }
      break;
    case PROP_VEGETAL_SABOR:
      {
        g_value_set_object (value, (gpointer) SABOR(self->sabor));
      }
      break;
    case PROP_VEGETAL_TAMANHO:
      {
        self->tamanho = g_value_get_double (value);
      }
      break;
    case PROP_VEGETAL_TIPO:
      {
        self->tipo = (gchar *) g_value_get_string (value);
      }
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}
Vegetal *
vegetal_new (void)
{
  Vegetal *obj;

  obj = g_object_new (VEGETAL_TYPE, NULL);
  return obj;
}
