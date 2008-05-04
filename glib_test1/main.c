/**
 * Copyright (C) 2007 Gabriel Falcão <gabriel@nacaolivre.org>
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
#include "main.h"
#define REDS(x)   g_strdup_printf ("\e[0;31m%s\e[0m", x)
#define GREENS(x) g_strdup_printf ("\e[0;32m%s\e[0m", x)
#define BLUES(x)  g_strdup_printf ("\e[0;34m%s\e[0m", x)
#define LGRAYS(x) g_strdup_printf ("\e[0;37m%s\e[0m", x)

TestErrors *
build_errors ()
{
  TestErrors *e;
  e = g_new (TestErrors, 1);
  e->domain = g_quark_from_string ("G_TEST_ERROR");
  e->error_foo = g_error_new (e->domain, G_TEST_ERROR_FOO, "FOO ERROR");
  e->error_bar = g_error_new (e->domain, G_TEST_ERROR_BAR, "BAR ERROR");
  return e;
}

void
destroy_errors (TestErrors * t)
{
  g_clear_error (&t->error_foo);
  g_clear_error (&t->error_bar);
  g_free (t);
}

void
foo_function (gchar * something, GError * err)
{
  if (something)
    {
      g_print (something);
    }
  else
    {
      g_print (GREENS("I am errando pra caralho"));
      g_critical (err->message);
    }
}

int
main (void)
{
  TestErrors *errors = build_errors ();
  foo_function ("\nprintando blablablabla sem erros\n", errors->error_foo);
  foo_function (NULL, errors->error_foo);
  if (errors)
    {
      destroy_errors (errors);
    }
  return 0;
}
