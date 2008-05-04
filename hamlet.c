/*
 * Copyright (C) 2007 Gabriel Falcão <gabriel@guake-terminal.org>
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

void print_in_middle (WINDOW * win, int starty, int startx, int width,
                      char *string);
int
how_to_use (char *prog)
{

  printf ("\nModo de usar: %s <numero>\n", prog);
  return 1;
}

int
main (int argc, char **argv)
{
  char *be, *st;
  unsigned int howmuch;
  if (argc > 1)
    {
      howmuch = atoi (argv[1]);
      if (howmuch == 0)
        {
          return how_to_use (argv[0]);
        }
    }
  else
    {
      return how_to_use (argv[0]);
    }
  if (howmuch > 10000)
    {
      howmuch = 10000;
    }
  initscr ();                   /* Start curses mode        */
  if (has_colors () == FALSE)
    {
      endwin ();
      printf ("Your terminal does not support color\n");
      exit (1);
    }
  start_color ();               /* Start color          */
  init_pair (1, COLOR_WHITE, COLOR_BLUE);

  attron (COLOR_PAIR (1));
  print_in_middle (stdscr, LINES / 2, 0, 0, "Alocating memory...");
  be = malloc (sizeof (char));
  st = malloc (sizeof (char)*200);

  while (strlen (be) < howmuch)
    {
      sprintf (st,"\nConcatening %d of %d ...\n", strlen (be), howmuch);
      print_in_middle (stdscr, LINES / 2, 0, 0, st);
      be = strcat (be, "b");
      refresh();
    }

  endwin ();
  printf
    ("\n%s or not %s, that is the question...\n         William Shakespeare\n",
     be, be);

  free(st);
  free(be); 
  return 0;
}

void
print_in_middle (WINDOW * win, int starty, int startx, int width,
                 char *string)
{
  int length, x, y;
  float temp;

  if (win == NULL)
    win = stdscr;
  getyx (win, y, x);
  if (startx != 0)
    x = startx;
  if (starty != 0)
    y = starty;
  if (width == 0)
    width = 80;

  length = strlen (string);
  temp = (width - length) / 2;
  x = startx + (int) temp;
  mvwprintw (win, y, x, "%s", string);
  refresh ();
}
