/*
 * Copyright (C) 2007 Lincoln de Sousa <lincoln@archlinux-br.org>
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

#include <Python.h>
#include "structmember.h"

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include "barcode.h"

typedef struct
{
  PyObject_HEAD;
  PyObject *code;
  PyObject *ascii;
  PyObject *partial;
  PyObject *textinfo;
  PyObject *encoding;
  PyObject *width, *height;
  PyObject *xoff, *yoff;
  PyObject *margin;
  PyObject *scalef;
} Barcode;


static void
Barcode_dealloc (Barcode * self)
{
  Py_XDECREF (self->code);
  Py_XDECREF (self->partial);
  Py_XDECREF (self->textinfo);
  Py_XDECREF (self->encoding);
  Py_XDECREF (self->width);
  Py_XDECREF (self->height);
  Py_XDECREF (self->xoff);
  Py_XDECREF (self->yoff);
  Py_XDECREF (self->margin);
  Py_XDECREF (self->scalef);
  self->ob_type->tp_free ((PyObject *) self);
}


static PyObject *
Barcode_new (PyTypeObject * type, PyObject * args, PyObject * kwds)
{
  Barcode *self;

  self = (Barcode *) type->tp_alloc (type, 0);
  if (self != NULL)
    {
      self->code = PyString_FromString ("");
      if (self->code == NULL)
        {
          Py_DECREF (self);
          return NULL;
        }

      self->partial = PyString_FromString ("");
      if (self->partial == NULL)
        {
          Py_DECREF (self);
          return NULL;
        }

      self->textinfo = PyString_FromString ("");
      if (self->textinfo == NULL)
        {
          Py_DECREF (self);
          return NULL;
        }

      self->encoding = PyString_FromString ("");
      if (self->encoding == NULL)
        {
          Py_DECREF (self);
          return NULL;
        }
    }

  return (PyObject *) self;
}

static int
Barcode_init (Barcode * self, PyObject * args, PyObject * kwds)
{
  char *code = NULL;
  struct Barcode_Item *tmp_bar;

  if (!PyArg_ParseTuple (args, "s", &code))
    return -1;

  self->code = PyString_FromString (code);
  tmp_bar = Barcode_Create (code);
  Barcode_Encode (tmp_bar, BARCODE_EAN);

  if (tmp_bar->error != 0)
    {
      Py_DECREF (self);
      PyErr_SetString (PyExc_ValueError, "Wrong barcode number");
      return -1;
    }

  self->ascii = PyString_FromString (tmp_bar->ascii);
  self->partial = PyString_FromString (tmp_bar->partial);
  self->textinfo = PyString_FromString (tmp_bar->textinfo);
  self->encoding = PyString_FromString (tmp_bar->encoding);
  self->width = PyInt_FromSsize_t (tmp_bar->width);
  self->height = PyInt_FromSsize_t (tmp_bar->height);

  return 0;
}

static PyMemberDef barcode_members[] = {
  {"code", T_OBJECT_EX, offsetof (Barcode, code), 0, ""},
  {"ascii", T_OBJECT_EX, offsetof (Barcode, ascii), 0, ""},
  {"partial", T_OBJECT_EX, offsetof (Barcode, partial), 0, ""},
  {"textinfo", T_OBJECT_EX, offsetof (Barcode, textinfo), 0, ""},
  {"encoding", T_OBJECT_EX, offsetof (Barcode, encoding), 0, ""},
  {"width", T_OBJECT_EX, offsetof (Barcode, width), 0, ""},
  {"height", T_OBJECT_EX, offsetof (Barcode, height), 0, "height"},
  {NULL}                        /* Sentinel */
};

static PyMethodDef barcode_methods[] = {
  {NULL}                        /* Sentinel */
};

static PyTypeObject BarcodeType = {
  PyObject_HEAD_INIT (NULL) 0,  /*ob_size */
  "barcode.Barcode",            /*tp_name */
  sizeof (Barcode),             /*tp_basicsize */
  0,                            /*tp_itemsize */
  (destructor) Barcode_dealloc, /*tp_dealloc */
  0,                            /*tp_print */
  0,                            /*tp_getattr */
  0,                            /*tp_setattr */
  0,                            /*tp_compare */
  0,                            /*tp_repr */
  0,                            /*tp_as_number */
  0,                            /*tp_as_sequence */
  0,                            /*tp_as_mapping */
  0,                            /*tp_hash */
  0,                            /*tp_call */
  0,                            /*tp_str */
  0,                            /*tp_getattro */
  0,                            /*tp_setattro */
  0,                            /*tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,     /*tp_flags */
  "Class to abstract gnu barcode",      /* tp_doc */
  0,                            /* tp_traverse */
  0,                            /* tp_clear */
  0,                            /* tp_richcompare */
  0,                            /* tp_weaklistoffset */
  0,                            /* tp_iter */
  0,                            /* tp_iternext */
  barcode_methods,              /* tp_methods */
  barcode_members,              /* tp_members */
  0,                            /* tp_getset */
  0,                            /* tp_base */
  0,                            /* tp_dict */
  0,                            /* tp_descr_get */
  0,                            /* tp_descr_set */
  0,                            /* tp_dictoffset */
  (initproc) Barcode_init,      /* tp_init */
  0,                            /* tp_alloc */
  Barcode_new,                  /* tp_new */
};

#ifndef PyMODINIT_FUNC          /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC
initbarcode (void)
{
  PyObject *m;

  BarcodeType.tp_new = PyType_GenericNew;
  if (PyType_Ready (&BarcodeType) < 0)
    return;

  m = Py_InitModule3 ("barcode", barcode_methods, "Barcode extension module");

  Py_INCREF (&BarcodeType);
  PyModule_AddObject (m, "Barcode", (PyObject *) &BarcodeType);
  PyModule_AddStringConstant (m, "BARCODE_VERSION", BARCODE_VERSION);
}
