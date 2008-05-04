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
#include "sabor.h"

static void sabor_class_init (SaborClass * klass);
static void sabor_init (Sabor * self);
static void sabor_set_string_sabor (Sabor * self, const gchar * valor);
static void sabor_set_property (GObject * object, guint property_id,
                                const GValue * value, GParamSpec * pspec);
static void
sabor_get_property (GObject * object, guint property_id, GValue * value,
                    GParamSpec * pspec);


GType
sabor_get_type (void)
{
  static GType sabor_type = 0;

  if (sabor_type == 0)
    {
      sabor_type = g_type_register_static_simple (G_TYPE_OBJECT,
                                                  "Sabor",
                                                  sizeof (SaborClass),
                                                  (GClassInitFunc)
                                                  sabor_class_init,
                                                  sizeof (Sabor),
                                                  (GInstanceInitFunc)
                                                  sabor_init, 0);
    }
  return sabor_type;
}

static void
sabor_class_init (SaborClass * klass)
{
  GParamSpec *amargo_param;
  GParamSpec *doce_param;
  GParamSpec *acido_param;
  GParamSpec *salgado_param;
  GObjectClass *g_object_class;

  klass = SABOR_CLASS (klass);
  g_object_class = G_OBJECT_CLASS (klass);

  amargo_param = g_param_spec_boolean ("amargo",        /* identificação */
                                       "amargo ?",      /*nickname */
                                       "eh amargo?",    /*descrição */
                                       FALSE, G_PARAM_READWRITE /*Public */
    );

  doce_param = g_param_spec_boolean ("doce",    /* identificação */
                                     "doce ?",  /*nickname */
                                     "eh doce?",        /*descrição */
                                     FALSE, G_PARAM_READWRITE   /*Public */
    );

  acido_param = g_param_spec_boolean ("acido",  /* identificação */
                                      "acido ?",        /*nickname */
                                      "eh acido?",      /*descrição */
                                      FALSE, G_PARAM_READWRITE  /*Public */
    );

  salgado_param = g_param_spec_boolean ("salgado",      /* identificação */
                                        "salgado ?",    /*nickname */
                                        "eh salgado?",  /*descrição */
                                        FALSE, G_PARAM_READWRITE        /*Public */
    );

  g_object_class->set_property = sabor_set_property;
  g_object_class->get_property = sabor_get_property;
}

static void
sabor_init (Sabor * self)
{
  self = SABOR (self);
}

static void
sabor_set_string_sabor (Sabor * self, const gchar * valor)
{
  self = SABOR (self);

  if (valor)
    {
      if (valor != self->sabor)
        {
          g_free (self->sabor);
          if (strlen ((char *) valor) > 0)
            {
              self->sabor = g_strdup_printf (valor);
            }
          else
            {
              self->sabor = g_strdup_printf ("sem sabor");
            }
        }
    }
  else
    {
      self->sabor = g_strdup_printf ("sem sabor");
    }
}
static void
sabor_set_property (GObject * object, guint property_id, const GValue * value,
                    GParamSpec * pspec)
{
  Sabor *self = SABOR (object);

  switch (property_id)
    {
    case PROP_SABOR_AMARGO:
      {
        self->amargo = g_value_get_boolean (value);

        self->doce = FALSE;
        self->acido = FALSE;
        self->salgado = FALSE;
        if (self->amargo)
          {
            sabor_set_string_sabor (self, "amargo");
          }
        else
          {
            sabor_set_string_sabor (self, NULL);
          }
      }
      break;
    case PROP_SABOR_DOCE:
      {
        self->doce = g_value_get_boolean (value);

        self->amargo = FALSE;
        self->acido = FALSE;
        self->salgado = FALSE;
        if (self->doce)
          {
            sabor_set_string_sabor (self, "doce");
          }
        else
          {
            sabor_set_string_sabor (self, NULL);
          }
      }
      break;
    case PROP_SABOR_ACIDO:
      {
        self->acido = g_value_get_boolean (value);

        self->amargo = FALSE;
        self->doce = FALSE;
        self->salgado = FALSE;
        if (self->acido)
          {
            sabor_set_string_sabor (self, "acido");
          }
        else
          {
            sabor_set_string_sabor (self, NULL);
          }
      }
      break;
    case PROP_SABOR_SALGADO:
      {
        self->salgado = g_value_get_boolean (value);

        self->amargo = FALSE;
        self->doce = FALSE;
        self->acido = FALSE;
        if (self->salgado)
          {
            sabor_set_string_sabor (self, "salgado");
          }
        else
          {
            sabor_set_string_sabor (self, NULL);
          }
      }
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
sabor_get_property (GObject * object,
                    guint property_id, GValue * value, GParamSpec * pspec)
{
  Sabor *self = SABOR (object);

  switch (property_id)
    {
    case PROP_SABOR_AMARGO:
      {
        g_value_set_boolean (value, self->amargo);
      }
      break;
    case PROP_SABOR_DOCE:
      {
        g_value_set_boolean (value, self->doce);
      }
      break;
    case PROP_SABOR_ACIDO:
      {
        g_value_set_boolean (value, self->acido);
      }
      break;
    case PROP_SABOR_SALGADO:
      {
        g_value_set_boolean (value, self->salgado);
      }
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}
Sabor *
sabor_new (void)
{
  Sabor *obj;

  obj = g_object_new (SABOR_TYPE, NULL);
  return obj;
}
