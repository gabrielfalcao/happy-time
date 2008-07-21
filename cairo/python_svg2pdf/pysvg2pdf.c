/*
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
#include "libsvg2pdf.h"
#include "pysvg2pdf.h"

PyObject *
svg2pdf (PyObject *self, PyObject *args)
{
  const gchar *source_filename, *destination_filename;
  gboolean retval;

  if (!PyArg_ParseTuple(args, "s|s:", &source_filename, &destination_filename))
    return NULL;

  retval = svg_file_to_pdf_file (source_filename,
                                 destination_filename, -1, -1);
  return retval ? Py_BuildValue("s", "Done!") : Py_BuildValue("");
}

static PyMethodDef GhMethods[] = { 
  {"svg2pdf", (PyCFunction) svg2pdf, METH_VARARGS, "Gets a svg filename as source and a " \
   "pdf filename as destination, and then, generates the pdf file (wow! Are you serious ?)"},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};


PyMODINIT_FUNC
initsvg2pdf (void)
{
  PyObject *m; 
  m = Py_InitModule4("svg2pdf", GhMethods,
		     svg2pdf_module_documentation,
		     (PyObject*)NULL,PYTHON_API_VERSION);
  if (m == NULL) return;

}
