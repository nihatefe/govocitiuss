/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * govocitos
 * Copyright (C) Manuel Fernández Delgado 2012 <manuel.fernandez.delgado@usc.es>
 * 
 * govocitos is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * govocitos is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "VentanaSeleccionEspecie.h"


void listaEspeciesSelected(GtkWidget *widget, gint *selected)
{ 
	(*selected) = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
//	cout << "selected=" << (*selected) << endl;
}

/**
 Gestor asociado al botón de seleccionar especie
 */
void gestorAceptarEspecies(GtkWidget *b, GtkWidget *ventanaEspecies)
{
	 gtk_widget_destroy(ventanaEspecies);
}

/**
 Gestor asociado al botón de cancelar especie
 */
//void gestor_cancelar_especies(GtkWidget *b, GtkWidget *ventanaEspecies)
void gestorCancelarEspecies(GtkWidget *b, struct datosGestorCancelarEspecies *datos)
{
	datos->cancelar = true;
	gtk_widget_destroy(datos->ventanaEspecies);
}


/**
 Muestra la ventana para la selección de especies
 @param numEspecies, nº de especies
 @param especies, vector con los nombres de las especies
 @param especieSelec, nº de la especie seleccionada (argumento de salida)
 */
bool ventanaSeleccionEspecie(const int numEspecies, const vector<string> &especies, gint &especieSelec) {
	GtkWidget  *ventanaEspecies = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget  *listaEspecies = gtk_vbox_new(TRUE, 0);
	GtkWidget  *fixed  = gtk_fixed_new();
	struct datosGestorCancelarEspecies  datos;

	
	gtk_window_set_title(GTK_WINDOW(ventanaEspecies), "Species selection");
	gtk_window_set_position(GTK_WINDOW(ventanaEspecies), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(ventanaEspecies), 150, 150);	
	
	listaEspecies = gtk_combo_box_new_text();
	for(int i = 0; i < numEspecies; i++) {
		gtk_combo_box_append_text(GTK_COMBO_BOX(listaEspecies), especies[i].c_str());
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(listaEspecies), 0); especieSelec = 0;

	GtkWidget  *label = gtk_label_new("Select an species:");
	gtk_fixed_put(GTK_FIXED(fixed), label, 10, 10);

	gtk_fixed_put(GTK_FIXED(fixed), listaEspecies, 40, 50);
	gtk_container_add(GTK_CONTAINER(ventanaEspecies), fixed);

	GtkWidget *botonAceptar = gtk_button_new_with_label( "Accept" );
	gtk_fixed_put(GTK_FIXED(fixed), botonAceptar, 10, 110);

	GtkWidget *botonCancelar = gtk_button_new_with_label( "Cancel" );
	gtk_fixed_put(GTK_FIXED(fixed), botonCancelar, 80, 110);
	
	datos.ventanaEspecies = ventanaEspecies; datos.cancelar = false;
		
	g_signal_connect_swapped(G_OBJECT(ventanaEspecies), "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(ventanaEspecies));
	g_signal_connect(G_OBJECT(listaEspecies), "changed", G_CALLBACK(listaEspeciesSelected), (gpointer) &especieSelec);
	g_signal_connect( G_OBJECT( botonAceptar ), "clicked", G_CALLBACK( gestorAceptarEspecies ), ( gpointer ) ventanaEspecies );
	g_signal_connect( G_OBJECT( botonCancelar ), "clicked", G_CALLBACK( gestorCancelarEspecies ), ( gpointer ) &datos );

	gtk_widget_show_all(ventanaEspecies);
	gtk_main();

	if(true == datos.cancelar) {
		return(false);
	}

	return(true);
}

