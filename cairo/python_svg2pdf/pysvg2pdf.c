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
file_to_file (PyObject *self, PyObject *args)
{
  const gchar *source_filename, *destination_filename;
  gboolean retval;
  GError *error = NULL;

  if (!PyArg_ParseTuple(args, "s|s:", &source_filename, &destination_filename))
    return NULL;

  retval = svg_file_to_pdf_file (source_filename,
                                 destination_filename, -1, -1, &error);
  if (error){
    PyErr_SetString(PyExc_RuntimeError, error->message);
    return NULL;
  }

  return retval ? Py_True : Py_False;
}

PyObject *
string_to_string (PyObject *self, PyObject *args)
{
  const gchar *svg_data;
  gchar *pdfdata;
  ClosureData *cdata;
  GError *error = NULL;
  FILE *file;

  if (!PyArg_ParseTuple(args, "s:", &svg_data))
    return NULL;

  cdata = svg_data_to_pdf_data_with_destination_size (svg_data, 
                                                      strlen(svg_data),
                                                      -1, -1, &error);
  
  if (error){
    PyErr_SetString(PyExc_RuntimeError, error->message);
    return NULL;
  }

  return PyString_FromStringAndSize(cdata->data, cdata->length);
}

static PyMethodDef GhMethods[] = { 
  {"file_to_file", (PyCFunction) file_to_file, METH_VARARGS, "Gets a svg filename as source and a "
                                                             "pdf filename as destination, and "
                                                             "then generates the pdf file"},
  {"string_to_string", (PyCFunction) string_to_string, METH_VARARGS, "Gets a string containing a svg"
                                                             "and returns a string containing the pdf"},

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
