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

#include "Entrenamiento.h"


void inicializa() {
	param.svm_type = C_SVC; param.kernel_type = RBF; param.cache_size = CACHE_SIZE; param.eps = EPSILON_LIBSVM; param.nr_weight = 0; param.shrinking = 1;
}

/** preprocesamiento de patrones
 */
void preprocesa() {
	double  t, u;
	int  i, j;


	for(i = 0; i < N_ENTRADAS; i++) {
		for(j = u = 0; j < numPatrones; j++) {
			u += patron[j][i];
		}
		for(media[i] = u/numPatrones, j = u = 0; j < numPatrones; j++) {
			t = patron[j][i] - media[i]; u += t*t;
		}
		if((desv[i] = sqrt(u/numPatrones))) {
			for(j = 0; j < numPatrones; j++) {
				patron[j][i] = (patron[j][i] - media[i])/desv[i];
			}
		}
 		//printf("entrada %i media= %g desv= %g\n", i, media[i], desv[i]);
	}
}

void cuentaPatronesClase() {
	int  i, j, n_cn = 0, n_sn = 0;

	for(i = 0; i < numPatronesEntrena; i++) {
		j = indicePatronEntrena[i];
		if(clase[j] == "cn") {
			n_cn++;
		} else {
			n_sn++;
		}
	}
	printf("Patrones de entrenamiento: n_cn= %i n_sn= %i\n", n_cn, n_sn);

	n_cn = 0; n_sn = 0;
	for(i = 0; i < numPatronesValida; i++) {
		j = indicePatronValida[i];
		if(clase[j] == "cn") {
			n_cn++;
		} else {
			n_sn++;
		}
	}
	printf("Patrones de validacion: n_cn= %i, n_sn= %i\n", n_cn, n_sn);
}


/**
 */
void repartePatronesClase() {
	int  i, numPatronesClase[N_CLASES], numPatronesEntrenaClase[N_CLASES], numPatronesValidaClase[N_CLASES];


	memset(numPatronesClase, 0, N_CLASES*sizeof(int));
	for(i = 0; i < numPatrones; i++) {
		if(clase[i] == "cn") {
			numPatronesClase[0]++;
		} else {  // clase[i]=="sn"
			numPatronesClase[1]++;
		}
	}

	for(i = 0; i < N_CLASES; i++) {
		numPatronesEntrenaClase[i] = PORC_PATRONES_ENTRENA*numPatronesClase[i];
		numPatronesValidaClase[i] = numPatronesClase[i] - numPatronesEntrenaClase[i];
		printf("clase %i: %i patrones: %i entrenamiento, %i validacion\n", i, numPatronesClase[i], numPatronesEntrenaClase[i], numPatronesValidaClase[i]);
	}

	int  numPatronesEntrenaTemp[N_CLASES];

	memset(numPatronesEntrenaTemp, 0, N_CLASES*sizeof(int));
	numPatronesEntrena = 0; numPatronesValida = 0;
	for(i = 0; i < numPatrones; i++) {
		if(clase[i] == "cn") {
			if(numPatronesEntrenaTemp[0] < numPatronesEntrenaClase[0]) {
				indicePatronEntrena[numPatronesEntrena++] = i;
				numPatronesEntrenaTemp[0]++;
			} else {
				indicePatronValida[numPatronesValida++] = i;
			}
		} else { // clase[i]="sn"
			if(numPatronesEntrenaTemp[1] < numPatronesEntrenaClase[1]) {
				indicePatronEntrena[numPatronesEntrena++] = i;
				numPatronesEntrenaTemp[1]++;
			} else {
				indicePatronValida[numPatronesValida++] = i;
			}
		}
	}
	printf("entrenamiento clases: numPatronesEntrena= %i numPatronesValida= %i\n", numPatronesEntrena, numPatronesValida);
	cuentaPatronesClase();
}

/**
 */
void inicializaProbClases() {
	int  i, j, k, numPatronesClase[N_CLASES];


	prob.l = numPatronesEntrena;
	prob.y = (double *) calloc(numPatronesEntrena, sizeof(double));
	prob.x = (struct svm_node **) calloc(numPatronesEntrena, sizeof(struct svm_node *));
	j = 1 + N_ENTRADAS;
	for(i = 0; i < numPatronesEntrena; i++) {
		prob.x[i] = (struct svm_node *) calloc(j, sizeof(struct svm_node));
	}

	memset(numPatronesClase, 0, N_CLASES*sizeof(int));

	for(i = 0; i < numPatronesEntrena; i++) {
		j = indicePatronEntrena[i];
		if(clase[j] == "cn") {
			prob.y[i] = 0; numPatronesClase[0]++;
		} else {  // clase[j] = sn
			prob.y[i] = 1; numPatronesClase[1]++;
		}
		for(k = 0, prob.x[i][N_ENTRADAS].index = -1; k < N_ENTRADAS; k++) {
			prob.x[i][k].index = k; prob.x[i][k].value = patron[j][k];
			//printf("%g ", prob.x[i][k].value);
		}
		//printf("%g\n", prob.y[i]);
	}
}

void validaClases() {
	int  i, k, l, y, d = 0;


	for(i = acierto = 0; i < numPatronesValida; i++) {
		l = indicePatronValida[i];
		for(k = 0, x[N_ENTRADAS].index = -1; k < N_ENTRADAS; k++) {
			x[k].index = k; x[k].value = patron[l][k];
			//printf("%g ", x[k].value);
		}
		//printf("\n");
		y = svm_predict(svm, x);
		if(clase[l] == "cn") {
			d = 0;
		} else {  // clase[l] = sn
			d = 1;
		}
		//printf("y= %g d= %i\n", y, d);
		if(y == d) {
			acierto++;
		}
	}
	acierto = 100*acierto/numPatronesValida;
	//printf("C= %g gamma= %g acierto= %.1f%%\n", paramClases.C, paramClases.gamma, acierto);
}


void cuentaPatronesEstado() {
	int  i, j, n_ac = 0, n_hid = 0, n_vat = 0;

	for(i = 0; i < numPatronesEntrena; i++) {
		j = indicePatronEntrena[i];
		if(estado[j] == "ac") {
			n_ac++;
		} else if(estado[j] == "hid") {
			n_hid++;
		} else {
			n_vat++;
		}
	}
	printf("Patrones de entrenamiento: n_ac= %i n_hid= %i n_vat= %i\n", n_ac, n_hid, n_vat);

	n_ac= 0; n_hid=0; n_vat= 0;
	for(i = 0; i < numPatronesValida; i++) {
		j = indicePatronValida[i];
		if(estado[j] == "ac") {
			n_ac++;
		} else if(estado[j] == "hid") {
			n_hid++;
		} else {
			n_vat++;
		}
	}
	printf("Patrones de validacion: n_ac= %i n_hid= %i n_vat= %i\n", n_ac, n_hid, n_vat);
}

/**
 */
void repartePatronesEstado() {
	int  i, numPatronesEstado[N_ESTADOS], numPatronesEntrenaEstado[N_ESTADOS], numPatronesValidaEstado[N_ESTADOS];


	memset(numPatronesEstado, 0, N_ESTADOS*sizeof(int));
	for(i = 0; i < numPatrones; i++) {
		if(estado[i] == "ac") {
			numPatronesEstado[0]++;
		} else if(estado[i] == "hid") {
			numPatronesEstado[1]++;
		} else {  // estado[i]=="vit" || "atre"
			numPatronesEstado[2]++;
		}
	}

	for(i = 0; i < N_ESTADOS; i++) {
		numPatronesEntrenaEstado[i] = PORC_PATRONES_ENTRENA*numPatronesEstado[i];
		numPatronesValidaEstado[i] = numPatronesEstado[i] - numPatronesEntrenaEstado[i];
		printf("estado %i: %i patrones: %i entrenamiento, %i validacion\n", i, numPatronesEstado[i], numPatronesEntrenaEstado[i], numPatronesValidaEstado[i]);
	}

	int  numPatronesEntrenaTemp[N_ESTADOS];

	memset(numPatronesEntrenaTemp, 0, N_ESTADOS*sizeof(int));
	numPatronesEntrena = 0; numPatronesValida = 0;
	for(i = 0; i < numPatrones; i++) {
		if(estado[i] == "ac") {
			if(numPatronesEntrenaTemp[0] < numPatronesEntrenaEstado[0]) {
				indicePatronEntrena[numPatronesEntrena++] = i;
				numPatronesEntrenaTemp[0]++;
			} else {
				indicePatronValida[numPatronesValida++] = i;
			}
		} else if(estado[i] == "hid") {
			if(numPatronesEntrenaTemp[1] < numPatronesEntrenaEstado[1]) {
				indicePatronEntrena[numPatronesEntrena++] = i;
				numPatronesEntrenaTemp[1]++;
			} else {
				indicePatronValida[numPatronesValida++] = i;
			}
		} else {
			if(numPatronesEntrenaTemp[2] < numPatronesEntrenaEstado[2]) {
				indicePatronEntrena[numPatronesEntrena++] = i;
				numPatronesEntrenaTemp[2]++;
			} else {
				indicePatronValida[numPatronesValida++] = i;
			}
		}
	}
	//printf("numPatronesEntrena= %i numPatronesValida= %i\n", numPatronesEntrena, numPatronesValida);

}

void inicializaProbEstados() {
	int  i, j, k, numPatronesEstado[N_ESTADOS];


	prob.l = numPatronesEntrena;
	prob.y = (double *) calloc(numPatronesEntrena, sizeof(double));
	prob.x = (struct svm_node **) calloc(numPatronesEntrena, sizeof(struct svm_node *));
	j = 1 + N_ENTRADAS;
	for(i = 0; i < numPatronesEntrena; i++) {
		prob.x[i] = (struct svm_node *) calloc(j, sizeof(struct svm_node));
	}

	memset(numPatronesEstado, 0, N_ESTADOS*sizeof(int));
	for(i = 0; i < numPatronesEntrena; i++) {
		j = indicePatronEntrena[i];
		if(estado[j] == "ac") {
			prob.y[i] = 0; numPatronesEstado[0]++;
		} else if(estado[j] == "hid") {
			prob.y[i] = 1; numPatronesEstado[1]++;
		} else {  // estado[j]=vit || atre
			prob.y[i] = 2; numPatronesEstado[2]++;
		}
		for(k = 0, prob.x[i][N_ENTRADAS].index = -1; k < N_ENTRADAS; k++) {
			prob.x[i][k].index = k; prob.x[i][k].value = patron[j][k];
			//printf("%g ", prob.x[i][k].value);
		}
		//printf("%g\n", prob.y[i]);
	}
}

void inicializaProbFinal() {
	int  i, k;


	prob.l = numPatrones;
	prob.y = (double *) calloc(numPatrones, sizeof(double));
	prob.x = (struct svm_node **) calloc(numPatrones, sizeof(struct svm_node *));
	for(i = 0, k = 1 + N_ENTRADAS; i < numPatrones; i++) {
		prob.x[i] = (struct svm_node *) calloc(k, sizeof(struct svm_node));
	}

	for(i = 0; i < numPatrones; i++) {
		for(k = 0, prob.x[i][N_ENTRADAS].index = -1; k < N_ENTRADAS; k++) {
			prob.x[i][k].index = k; prob.x[i][k].value = patron[i][k];
			//printf("%g ", prob.x[i][k].value);
		}
		//printf("%g\n", prob.y[i]);
	}
}

void inicializaProbClasesFinal() {
	int  i, numPatronesClase[N_CLASES];


	memset(numPatronesClase, 0, N_CLASES*sizeof(int));
	for(i = 0; i < numPatrones; i++) {
		if(clase[i] == "cn") {
			prob.y[i] = 0; numPatronesClase[0]++;
		} else {  // clase[j] = sn
			prob.y[i] = 1; numPatronesClase[1]++;
		}
	}
}


/**
 calcula la matriz de confusión para clases
 */
void calculaMatrizConfusionClases() {
	float  se[N_CLASES], total;
	int  mc[N_CLASES][N_CLASES], i, k, y, d = 0;


	memset(mc, 0, N_CLASES*N_CLASES*sizeof(int));
	for(i = acierto = 0; i < numPatrones; i++) {
		for(k = 0, x[N_ENTRADAS].index = -1; k < N_ENTRADAS; k++) {
			x[k].index = k; x[k].value = patron[i][k];
			//printf("%f ", x[k].value);
		}
		y = svm_predict(svmClases, x);
		if(clase[i] == "cn") {
			d = 0;
		} else {  // clase = sn
			d = 1;
		}
		//printf("y= %i d= %i\n", y, d);
		if(y == d) {
			acierto++;
		}
		mc[d][y]++;
	}
	acierto = 100*acierto/numPatrones;

	memset(se, 0, N_CLASES*sizeof(float));
	for(i = 0; i < N_CLASES; i++) {
		for(k = total = 0; k < N_CLASES; k++) {
			total += mc[i][k];
		}
		if(total) {
			se[i] = 100.*mc[i][i]/total;
		}
	}
	printf("matriz de confusion de clases:\n\tcn\tsn\n");
	printf("cn\t%i\t%i\n", mc[0][0], mc[0][1]);
	printf("sn\t%i\t%i\n", mc[1][0], mc[1][1]);
	printf("clases: acierto= %.1f%% se(cn)= %.1f%%, se(sn)= %.1f%%\n", acierto, se[0], se[1]);
}

void inicializaProbEstadosFinal() {
	int  i, numPatronesEstado[N_ESTADOS];


	memset(numPatronesEstado, 0, N_ESTADOS*sizeof(int));
	for(i = 0; i < numPatrones; i++) {
		if(estado[i] == "ac") {
			prob.y[i] = 0; numPatronesEstado[0]++;
		} else if(estado[i] == "hid") {
			prob.y[i] = 1; numPatronesEstado[1]++;
		} else {  // estado[j]=vit || atre
			prob.y[i] = 2; numPatronesEstado[2]++;
		}
	}
}

void validaEstados() {
	int  i, k, l, y, d = 0;


	for(i = acierto = 0; i < numPatronesValida; i++) {
		l = indicePatronValida[i];
		for(k = 0, x[N_ENTRADAS].index = -1; k < N_ENTRADAS; k++) {
			x[k].index = k; x[k].value = patron[l][k];
			//printf("%g ", x[k].value);
		}
		//printf("\n");
		y = svm_predict(svm, x);
		if(estado[l] == "ac") {
			d = 0;
		} else if(estado[l] == "hid") {
			d = 1;
		} else {  // estado[l] == "vit" o "atre"
			d = 2;
		}
		//printf("y= %g d= %i\n", y, d);
		if(y == d) {
			acierto++;
		}
	}
	acierto = 100*acierto/numPatronesValida;
	//printf("C= %g gamma= %g acierto= %.1f%%\n", param.C, param.gamma, acierto);
}

/**
 calcula la matriz de confusión para estados: valida sobre el conjunto completo de patrones
 */
void calculaMatrizConfusionEstados() {
	float  se[N_ESTADOS], total;
	int  i, k, mc[N_ESTADOS][N_ESTADOS], y, d = 0;


	memset(mc, 0, N_ESTADOS*N_ESTADOS*sizeof(int));
	for(i = acierto = 0; i < numPatrones; i++) {
		for(k = 0; k < N_ENTRADAS; k++) {
			x[k].index = k; x[k].value = patron[i][k];
			//printf("%g ", x[k].value);
		}
		x[N_ENTRADAS].index = -1;
		y = svm_predict(svmEstados, x);
		if(estado[i] == "ac") {
			d = 0;
		} else if(estado[i] == "hid") {
			d = 1;
		}  else {  // clase = vit o atre
			d = 2;
		}
		//printf("y= %i d= %i\n", y, d);
		if(y == d) {
			acierto++;
		}
		mc[d][y]++;
	}
	acierto=100*acierto/numPatrones;

	memset(se, 0, N_ESTADOS*sizeof(float));
	for(i = 0; i < N_ESTADOS; i++) {
		for(k = total = 0; k < N_ESTADOS; k++) {
			total += mc[i][k];
		}
		if(total) {
			se[i] = 100.*mc[i][i]/total;
		}
	}
	printf("matriz de confusion de estados:\n\tac\thid\tv/at\n");
	printf("ac\t%i\t%i\t%i\n", mc[0][0], mc[0][1], mc[0][2]);
	printf("hid\t%i\t%i\t%i\n", mc[1][0], mc[1][1], mc[1][2]);
	printf("v/at\t%i\t%i\t%i\n", mc[2][0], mc[2][1], mc[2][2]);
	printf("estados: acierto= %.1f%% se(ac)= %.1f%%, se(hid)= %.1f%%, se(vit/atre)= %.1f%%\n", acierto, se[0], se[1], se[2]);
}

void liberaProb() {
	int  i, n = prob.l;

	for(i = 0; i < n; i++) {
		free(prob.x[i]);
	}
	free(prob.x); free(prob.y);
}

/** liberación de memoria
 */
void finaliza() {
	int  i, n;

	for(i = 0, n = prob.l; i < n; i++) {
		free(prob.x[i]);
	}
	free(prob.x); free(prob.y);

	for(i = 0, n = patron.size(); i < n; i++) {
		delete patron[i];
	}
	patron.clear();

}


/**
 */
void entrenamiento() {
	inicializa();


	DialogoBarraProgres dProgres("Training");
	double  porcProgreso = 0;
	int  numPruebas = 2*N_VALORES_C*N_VALORES_GAMMA;

	preprocesa();

	// entrenamiento de clases
	repartePatronesClase();
	inicializaProbClases();
	printf("entrenando SVM clases ...\n");
	aciertoMejor = 0; C_mejorClases = 0; gammaMejorClases = 0;
	for(i_val_C = 0; i_val_C < N_VALORES_C; i_val_C++) {
		param.C = valor_C[i_val_C];
		for(i_val_gamma = 0; i_val_gamma < N_VALORES_GAMMA; i_val_gamma++) {
			param.gamma = valor_gamma[i_val_gamma];
			svm = svm_train(&prob, &param);
			validaClases();
			//printf("C= %g gamma= %g acierto= %.1f%%\n", param.C, param.gamma, acierto);
			if(acierto > aciertoMejor) {
				aciertoMejor = acierto; C_mejorClases= param.C; gammaMejorClases = param.gamma;
			}
			svm_destroy_model(svm);
			// actualiza barra de progreso
			porcProgreso = (double) (i_val_C*N_VALORES_GAMMA + i_val_gamma)/numPruebas;
			fprintf(stderr, "progreso %.1f%% aciertoMejor= %.1f%%\r", 100*porcProgreso, aciertoMejor); fflush(stderr);
			dProgres.setEstadoBarraProgreso(porcProgreso);
			dProgres.setPercentText(porcProgreso);
			dProgres.ejectuaDialogoProgreso();
			Utiles::actualizarInterfaz();
		}
	}
	printf("\nentrenamiento de clases: C_mejor= %g gammaMejor= %g aciertoMejor= %.1f%%\n", C_mejorClases, gammaMejorClases, aciertoMejor);
	liberaProb();

	// entrenamiento para estados
	repartePatronesEstado();
	inicializaProbEstados();
	printf("entrenando SVM estados ...\n");
	aciertoMejor = 0; C_mejorEstados = 0; gammaMejorEstados = 0;
	for(i_val_C = 0; i_val_C < N_VALORES_C; i_val_C++) {
		param.C = valor_C[i_val_C];
		for(i_val_gamma = 0; i_val_gamma < N_VALORES_GAMMA; i_val_gamma++) {
			param.gamma = valor_gamma[i_val_gamma];
			svm = svm_train(&prob, &param);
			validaEstados();
			if(acierto > aciertoMejor) {
				aciertoMejor = acierto; C_mejorEstados= param.C; gammaMejorEstados = param.gamma;
			}
			svm_destroy_model(svm);
			// actualiza barra de progreso
			porcProgreso = (double) (N_VALORES_C*N_VALORES_GAMMA + i_val_C*N_VALORES_GAMMA + i_val_gamma)/numPruebas;
			fprintf(stderr, "progreso %.1f%% aciertoMejor= %.1f%%\r", 100*porcProgreso, aciertoMejor); fflush(stderr);
			dProgres.setEstadoBarraProgreso(porcProgreso);
			dProgres.setPercentText(porcProgreso);
			dProgres.ejectuaDialogoProgreso();
			Utiles::actualizarInterfaz();
		}
	}
	printf("\nentrenamiento de estados: C_mejor= %g gammaMejor= %g acierto_mejor= %.1f%%\n", C_mejorEstados, gammaMejorEstados, aciertoMejor);
	liberaProb();

	string  fichClasificCl = nombreEspecie + "_mejor_svm_nucleos.dat";
	string  fichClasificEst = nombreEspecie + "_mejor_svm_estados.dat";
	string  fichMediaDesvia = nombreEspecie + "_med_desv.dat";

	fich[0] = Utiles::getClasificadorFile(fichClasificCl);
	fich[1] = Utiles::getClasificadorFile(fichClasificEst);
	fich[2] = Utiles::getClasificadorFile(fichMediaDesvia);

	inicializaProbFinal();
	// entrenamiento final con conjunto completo
	inicializaProbClasesFinal();
	param.C = C_mejorClases; param.gamma = gammaMejorClases;
	svmClases = svm_train(&prob, &param);
	calculaMatrizConfusionClases();
	svm_save_model (fich[0].c_str(), svmClases);
	svm_destroy_model(svmClases);

	// entrenamiento final con conjunto completo
	inicializaProbEstadosFinal();
	param.C = C_mejorEstados; param.gamma = gammaMejorEstados;
	svmEstados = svm_train(&prob, &param);
	calculaMatrizConfusionEstados();

	svm_save_model(fich[1].c_str(), svmEstados);
	svm_destroy_model(svmEstados);

	// almacenamento de media e desviacion tipica para cada entrada
	FILE  *f = fopen(fich[2].c_str(), "w");
	if(f) {
		for(int i = 0; i < N_ENTRADAS; i++) {
			fprintf(f, "%g\t%g\n", media[i], desv[i]);
		}
		fclose(f);
	} else {
		fprintf(stderr, "error abriendo %s: %s\n", fich[2].c_str(), strerror(errno));
	}
	printf("almacenados los ficheros del clasificador\n");


	dProgres.cierraVentanaProgreso();
	Utiles::actualizarInterfaz();

	finaliza();
}


