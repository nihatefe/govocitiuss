/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) José Manuel Pintor Freire 2008 <jpfreire@correo.ei.uvigo.es>
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANPOILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _UTILES_H_
	#define _UTILES_H_

	#include <libglademm/xml.h>
	#include <gtkmm.h>
	#include <iostream>
	#include <glibmm.h>
	#include <glib.h>

	#include "Estructuras/Celula.h"

	using namespace std;

	class Utiles
	{
		public:
			static void getWidgetGlade(const string &filename,const char* root_node,const char *widget_name,...);
			static void getObjectBuilder(const string &filename, const Glib::StringArrayHandle root_objects, const char *object_name,...);
			static string obtieneTipoArch(const string &tipo);
			static string obtieneNombreArch(const string &nomArch);
			static string converString(int valor);
			static string converString(long int valor);
			static string converString(double valor);
			static string convertCharToString(char *cad);
			static string obtenerDireccionConfig();
			static string obtenerDireccionCache();
			static string obtenerDireccionCacheImgReduced();
			static string obtenerDireccionClasificador();
			static string getClasificadorFile(const string &file);
			static string getDataDir();
			static string getGladeFile(const string &file);
			static string getIconoFile(const string &file);
			static string direccionConfig(const string &fichero);
			static string direccionCache(const string &fichero);
			static string direccionCacheReduced(const string &fichero);
			static void creaDirectorio(const string &path);
			static void calculoCoordMaxMin(int cX, int cY, double zoom, Celula &cell);
			static void actualizarInterfaz();
			static double redondearNDecimales(double num, int dec);
			static int doubleToIntWhichAllCharacter(double num);
			static string timestamp();
			static void extrae_ruta_fichero(const string &rutaCompleta, string &ruta, string &fichero);
			static string extrae_fichero(const string &rutaCompleta);
		private:
			/**
			 * Devuelve el path de instalación de govocitos en windows
			 * @return string que contiene el path de instalación
			 */ 
			inline static std::string get_windos_installation_path()
			{
				std::string path_win32("");
	
				#ifdef G_OS_WIN32
				path_win32 = convertCharToString(g_win32_get_package_installation_directory_of_module(NULL));
				#endif

				return path_win32;
			}
	};
#endif // _UTILES_H_
