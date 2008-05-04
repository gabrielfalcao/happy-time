#include <Python.h>

typedef struct
{
  PyObject_HEAD;
  PyObject *command;
  PyObject *system;
} Spam;

static void
Spam_dealloc (Spam * self)
{
  Py_XDECREF (self->command);
  Py_XDECREF (self->system);
  self->ob_type->tp_free ((PyObject *) self);
}

static PyObject *
Spam_new (PyTypeObject * type, PyObject * args, PyObject * kwargs)
{
  Spam *self;

  self = (Spam *) type->tp_alloc (type,0);
  if (self != NULL)
    {

      self->command = PyString_FromString ("");
      if (self->command == NULL)
	{
	  Py_DECREF (self);
	  return NULL;
	}
      self->system = PyString_FromString ("SISTEMA!");
      if (self->system == NULL)
	{
	  Py_DECREF (self);
	  return NULL;
	}
    }
    return (PyObject *) self;
}


static int 
Spam_init (Spam * self, PyObject * args, PyObject * kwargs)
{
  self->system = PyInt_FromLong((long) system("echo $PWD"));
  return 0;
}

static PyMemberDef spam_members[] = {
  {"system", T_OBJECT_EX, offsetof (Spam, system), 0, ""},
  {"command", T_OBJECT_EX, offsetof (Spam, command), 0, ""},
  {NULL}                        /* Sentinel */
};

static PyMethodDef spam_methods[] = {
  {NULL}
};

static PyTypeObject SpamType = {
  PyObject_HEAD_INIT (NULL) 0,  /*ob_size */
  "spam.Spam",            /*tp_name */
  sizeof (Spam),             /*tp_basicsize */
  0,                            /*tp_itemsize */
  (destructor) Spam_dealloc, /*tp_dealloc */
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
  "Strength test!",      /* tp_doc */
  0,                            /* tp_traverse */
  0,                            /* tp_clear */
  0,                            /* tp_richcompare */
  0,                            /* tp_weaklistoffset */
  0,                            /* tp_iter */
  0,                            /* tp_iternext */
  spam_methods,              /* tp_methods */
  spam_members,              /* tp_members */
  0,                            /* tp_getset */
  0,                            /* tp_base */
  0,                            /* tp_dict */
  0,                            /* tp_descr_get */
  0,                            /* tp_descr_set */
  0,                            /* tp_dictoffset */
  (initproc) Spam_init,      /* tp_init */
  0,                            /* tp_alloc */
  Spam_new,                  /* tp_new */
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

  m = Py_InitModule3 ("spam", spam_methods, "Testing");

  Py_INCREF (&BarcodeType);
  PyModule_AddObject (m, "Spam", (PyObject *) &SpamType);
  PyModule_AddStringConstant (m, "MODULE_VERSION", "0.0.0.1");
}
