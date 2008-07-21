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

PyDoc_STRVAR(svg2pdf_module_documentation,
"A simple svg to pdf conversion module.\n"
"\n"
"This module simplify the task of get a svg data \n"
"and transform in a pdf data, or get a filename path \n"
"as source, and a destination, to do all the dirty work. \n"
"\n"
"Usage:\n"
"\n"
"  from cStringIO import StringIO \n"
"  from svg2pdf import file_to_file, stream_to_stream\n"
"\n"
"  file_to_file('report.svg', '/home/pythonist/Report.pdf')\n"
"  'Done!'\n"
"\n"
"  output_buf = StringIO()\n"
"  input_buf = open('report.svg', 'r')\n"
"  stream_to_stream(input_buf, output_buf)\n"
"  print an_output_stream.getvalue()[:10]\n"
"   '%PDF-1.4\\r\\n'\n"
"\n"
"  \n"
"pysvg2pdf.c,v 0.1 2008/07/21 12:02:22 Gabriel Falcao <gabriel@nacaolivre.org>\n");
