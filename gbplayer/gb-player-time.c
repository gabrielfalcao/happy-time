/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gb-player-time.c
 * Copyright (C) Gabriel Falcão Gonçalves de Moura 2008 <gabriel@nacaolivre.org>
 * 
 * gb-player-time.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gb-player-time.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gb-player-time.h"

typedef struct _GbPlayerTimePrivate GbPlayerTimePrivate;
struct _GbPlayerTimePrivate
{
	gchar * formated_time;
	gint64 time;
	guint hours;
	guint minutes;
	guint seconds;
};

#define GB_PLAYER_TIME_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GB_TYPE_PLAYER_TIME, GbPlayerTimePrivate))

enum
{
	PROP_0,

	PROP_FORMATED_TIME,
	PROP_TIME,
	PROP_HOURS,
	PROP_MINUTES,
	PROP_SECONDS
};



G_DEFINE_TYPE (GbPlayerTime, gb_player_time, G_TYPE_OBJECT);

static void
gb_player_time_init (GbPlayerTime *object)
{
	/* TODO: Add initialization code here */
}

static void
gb_player_time_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (gb_player_time_parent_class)->finalize (object);
}

static void
gb_player_time_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GB_IS_PLAYER_TIME (object));

	switch (prop_id)
	{
	case PROP_FORMATED_TIME:
		/* TODO: Add setter for "formated_time" property here */
		break;
	case PROP_TIME:
		/* TODO: Add setter for "time" property here */
		break;
	case PROP_HOURS:
		/* TODO: Add setter for "hours" property here */
		break;
	case PROP_MINUTES:
		/* TODO: Add setter for "minutes" property here */
		break;
	case PROP_SECONDS:
		/* TODO: Add setter for "seconds" property here */
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gb_player_time_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GB_IS_PLAYER_TIME (object));

	switch (prop_id)
	{
	case PROP_FORMATED_TIME:
		/* TODO: Add getter for "formated_time" property here */
		break;
	case PROP_TIME:
		/* TODO: Add getter for "time" property here */
		break;
	case PROP_HOURS:
		/* TODO: Add getter for "hours" property here */
		break;
	case PROP_MINUTES:
		/* TODO: Add getter for "minutes" property here */
		break;
	case PROP_SECONDS:
		/* TODO: Add getter for "seconds" property here */
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gb_player_time_class_init (GbPlayerTimeClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GbPlayerTimePrivate));

	object_class->finalize = gb_player_time_finalize;
	object_class->set_property = gb_player_time_set_property;
	object_class->get_property = gb_player_time_get_property;

	g_object_class_install_property (object_class,
	                                 PROP_FORMATED_TIME,
	                                 g_param_spec_string ("formated_time",
	                                                      "FormatedTime",
	                                                      "The hh:mm:ss formated time",
	                                                      "",
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (object_class,
	                                 PROP_TIME,
	                                 g_param_spec_int64 ("time",
	                                                      "Time",
	                                                      "The gint64 raw time",
														  G_MAXINT64,
                                                          G_MININT64,
                                                          0,
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (object_class,
	                                 PROP_HOURS,
	                                 g_param_spec_int64 ("hours",
	                                                      "Hours",
	                                                      "The hours",
														  G_MAXINT64,
                                                          G_MININT64,
														  0,
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (object_class,
	                                 PROP_MINUTES,
	                                 g_param_spec_int64 ("minutes",
	                                                      "Minutes",
	                                                      "The minutes",
														  G_MAXINT64,
                                                          G_MININT64,
                                                          0,
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (object_class,
	                                 PROP_SECONDS,
	                                 g_param_spec_int64 ("seconds",
	                                                      "Seconds",
	                                                      "The seconds",
														  G_MAXINT64,
                                                          G_MININT64,
                                                          0,
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}


gchar *
gb_player_time_strftime (GbPlayerTime *self, const gchar * format)
{
	/* TODO: Add public function implementation here */
}
