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

#ifndef _CLASIFICACION_H_
#define _CLASIFICACION_H_

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include "Clasificador/svm.h"
#include "Utiles.h"
#include "Interfaz/DialogoBarraProgres.h"
#include <string>
#include <vector>
#include "entrenamiento_defs.h"


vector<double *>  patron;
vector<string>  clase;
vector<string>  estado;

struct svm_problem  prob;
struct svm_parameter  param;
struct svm_model  *svm;
struct svm_model  *svmClases;
struct svm_model  *svmEstados;
struct svm_node  x[1 + N_ENTRADAS];

string  fich[N_FICH_CLASIF];
string  nombreEspecie;
double  media[N_ENTRADAS];
double  desv[N_ENTRADAS];
float  valor_C[N_VALORES_C] = { 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1, 0.5,0.25, 0.125, 0.0625, 0.0312 };
float  valor_gamma[N_VALORES_GAMMA] = { 1, 0.5, 0.25, 0.125, 0.0625, 0.03125, 0.01562, 0.00781, 0.00391, 0.00195,  0.00098, 0.00049, 0.00024, 0.00012, 0.00006, 0.00003 };
float  acierto;
float  aciertoMejor;
float  C_mejorClases;
float  C_mejorEstados;
float  gammaMejorClases;
float  gammaMejorEstados;
int  i_val_C;
int  i_val_gamma;
int  numPatrones;
int  numPatronesEntrena;
int  numPatronesValida;
int  indicePatronEntrena[N_PATRONES_MAX];
int  indicePatronValida[N_PATRONES_MAX];

/*void calculaMatrizConfusionEstados();
void almacenaFicheros();
void finaliza();
void entrenamiento();*/

#endif // _CLASIFICACION_H_
