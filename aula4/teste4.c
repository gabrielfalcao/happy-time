/*
 * Copyright (C) 2007 Gabriel Falcão <root@gabrielfalcao.com>
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
#include <gtk/gtk.h>
#include <glib.h>
#include "teste4.h"

typedef struct _janela Janela;
struct _janela
{
  GtkWidget *main_window;
  GtkWidget *vbox;
  GtkWidget *entry, *button;
};

void
definir_widgets (void *data)
{
  Janela *prog = (Janela *) data;


  //criando a janela
  prog->main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  //definindo o titulo da janela
  gtk_window_set_title (GTK_WINDOW (prog->main_window),
                        "Janelinha dos Miguxos!");
  //desabilitando o redimensionamento da janela
  gtk_window_set_resizable (GTK_WINDOW (prog->main_window), FALSE);
  //tamanho da janela
  gtk_widget_set_size_request ((GtkWidget *) prog->main_window, 400, 200);

  //um container para os meus widgets GtkEntry e GtkButton
  prog->vbox = gtk_vbox_new (FALSE, 0);

  //criando uma caixa de texto:
  prog->entry = gtk_entry_new_with_max_length (100);
  /*gtk_button_new_from_stock("gtk-home"); */
  prog->button = gtk_button_new_with_label ("Botãozim miguxo!");

  //empacotando os widgets no vbox:
  gtk_box_pack_start ((GtkBox *) prog->vbox,
                      (GtkWidget *) prog->entry, TRUE, TRUE, 1);
  gtk_box_pack_start ((GtkBox *) prog->vbox,
                      (GtkWidget *) prog->button, FALSE, FALSE, 1);
  //adicionando o vbox à janela
  gtk_container_add (GTK_CONTAINER (prog->main_window), prog->vbox);

  //mostrando todo mundo:
  /*gtk_widget_show (prog->); */
  gtk_widget_show_all (prog->main_window);

  conectar_callbacks (prog);
}

void
conectar_callbacks (void *data)
{
  Janela *prog = (Janela *) data;
  /*g_signal_connect (G_OBJECT (prog->main_window), "destroy", G_CALLBACK (gtk_main_quit), NULL); */
  g_signal_connect_swapped (G_OBJECT (prog->main_window), "destroy",
                            G_CALLBACK (sair), G_OBJECT (prog));

}

void
sair (void *data)
{
  g_free ((Janela *) data);
  gtk_main_quit();
}

int
main (int argc, char **argv)
{
  /*int x; */
  gtk_init (&argc, &argv);
  Janela *prog = g_malloc (sizeof (Janela));
  definir_widgets (prog);
  gtk_main ();
  /*printf ("%d parametros\n\n", argc - 1);
     for (x = 0; x < argc; x++)
     {
     printf ("%s\n", argv[x]);
     } */
  return 0;
}
