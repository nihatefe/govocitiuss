/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Clasificador.cpp
 * Copyright (C) José Manuel Pintor Freire 2008-2011 <jpfreire@correo.ei.uvigo.es>
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

#include "Clasificador.h"

/**
 Constructor
 @param clEst, string que contiene la ruta del fichero con datos para la clasificacion
 de estados.
 @param clCL, string que contiene la ruta del fichero con datos para la clasificacion
 de clases.
 @param md, string que contiene la ruta del fichero con las medias para la clasificacion.
 */
Clasificador::Clasificador(string clEst, string clCl, string md)
{
	fichClasificEst = clEst;
	fichClasificCl = clCl;
	fichMediaDesvia = md;
}

/**
 Destructor.
 */
Clasificador::~Clasificador()
{

}

bool Clasificador::le_media_desv(const char *fich, float *media, float *desv)
{
	FILE  *pf;
	unsigned int  i;


	pf = fopen(fich, "r");
	if(! pf) {
		fprintf(stderr, "Clasificador::le_media_desv: erro en fopen abrindo %s: %s\n", fich, strerror(errno));
		return(false);
	}
	for(i = 0; i < N_ENTRADAS; i++)
	{
		(void)fscanf(pf, "%f %f\n", &media[i], &desv[i]);
		//printf("media= %f desviación= %f\n", media[i], desv[i]);
	}
	fclose(pf);
	return(true);
}


struct svm_model *Clasificador::crea_svm(const char *f_svm)
{
	svm_model  *svm;
	FILE  *f = fopen(f_svm, "r");
	if(! f) {
		fprintf(stderr, "Clasificador::crea_svm: error abriendo el fichero %s: %s\n", f_svm, strerror(errno));
		return(NULL);
	}
	fclose(f);
	printf("f_svm= %s\n", f_svm);
	svm = svm_load_model(f_svm);
	return(svm);
}



int Clasificador::saida_svm(struct svm_model *svm, float *x, float *media, float *desv)
{
	struct svm_node  t[1 + N_ENTRADAS];
	unsigned int  i;
	int  y;

	for(i = 0; i < N_ENTRADAS; i++) {
		t[i].index = i;
		if(desv[i]) {
			t[i].value = (x[i] - media[i])/desv[i];
			//printf("%f ", t[i].value);
		} else {
			t[i].value = x[i];
		}
	}
	t[N_ENTRADAS].index = -1;
	y = svm_predict(svm, t);
	//printf("y= %i\n", y);

	return(y);
}

void Clasificador::destrue_svm(struct svm_model *svm)
{
	svm_destroy_model(svm);
}

/**
 Carga los ficheros de clasificacion automatica de estados y clases y clasifica las
 celulas de la imagen cuya ruta corresponde con la pasada por parametro.
 @param ruta, string de la ruta de la imagen a analizar.
 @param listCell, objeto ListadoCeulas que contiene todas las celulas de la imagen
 a analizar.
 */
void Clasificador::clasificarCelulas(string ruta, ListadoCelulas &listCell)
{
	struct svm_model  *svmEst, *svmCl;
	float  media[N_ENTRADAS], desv[N_ENTRADAS];
	int  y1, y2;
	int numCelulas, numPuntos, anchoImg, altoImg, min_x, max_x, min_y, max_y;
	vector <int> *cx, *cy;
	Image *img;


	printf("ruta=%s\n", ruta.c_str());
	img = read_img ( ruta.c_str() );

	numCelulas = listCell.getNumCelulas();
	printf("numCelulas= %i\n", numCelulas);

	//Obtenemos path de los ficheros de clasificacion
	const char  *f_svm_Est = fichClasificEst.c_str();
	const char  *f_svm_Cl = fichClasificCl.c_str();
	const char  *f_med_desv = fichMediaDesvia.c_str();

	if(! le_media_desv(f_med_desv, media, desv)) {
        Dialogos::dialogoError("Error reading classifier", "Classifier not available");
	    return;
	}

	//Creamos los svm
	svmEst = crea_svm(f_svm_Est);
	if(! svmEst) {
		fprintf(stderr, "error leyendo fichero SVM estados\n");
        Dialogos::dialogoError("Error reading classifier", "Classifier not available");
		return;
	}

	svmCl = crea_svm(f_svm_Cl);
	if(! svmCl) {
		fprintf(stderr, "error leyendo fichero SVM núcleos\n");
        Dialogos::dialogoError("Error reading classifier", "Classifier not available");
		return;
	}

	int  numPatronesClase[N_CLASES], numPatronesEstado[N_ESTADOS];

	memset(numPatronesClase, 0, N_CLASES*sizeof(int));
	memset(numPatronesEstado, 0, N_ESTADOS*sizeof(int));

	DialogoBarraProgres dProgres("Classifying");
	double  porcProgreso = 0;

	for(int i = 0; i < numCelulas; i++)
	{
		PointList *puntos;

		if(listCell.getCelula(i)->getEstadoCelula() != "outimage" &&
		   listCell.getCelula(i)->getClaseCelula() != "outimage")
		{
			cx = listCell.getCelula(i)->getBordeCellX();
			cy = listCell.getCelula(i)->getBordeCellY();

			numPuntos = cx->size();
			puntos = alloc_point_list(numPuntos);

			//Convertimos puntos
			for(int j = 0; j < numPuntos; j++)
			{
				puntos->point[j] = alloc_point((*cy)[j], (*cx)[j]);
			}
			puntos->type = GEN_VALID;

            double *resul = textureLBPColor ( img, puntos);
			float resul2[N_ENTRADAS], resul3[N_ENTRADAS];

			for(unsigned int z = 0; z < N_ENTRADAS; z++)
			{
				resul2[z] = (float)resul[z];
				resul3[z] = (float)resul[z];
			}

			// si la célula está en el borde, no se clasifica en cn/sn
			anchoImg = get_num_cols(img); altoImg = get_num_rows(img);
			min_x = *min_element(cx->begin(), cx->end()); max_x = *max_element(cx->begin(), cx->end());
			min_y = *min_element(cy->begin(), cy->end()); max_y = *max_element(cy->begin(), cy->end());
			if(min_x > MARXE_BORDE_CLASIF && max_x < anchoImg - MARXE_BORDE_CLASIF && min_y > MARXE_BORDE_CLASIF && max_y < altoImg - MARXE_BORDE_CLASIF) {
				//Salida clasificador clases
				y1 = saida_svm(svmCl, resul2, media, desv);
				numPatronesClase[y1]++;
				switch(y1)
				{
					case 0:
						listCell.getCelula(i)->setClaseCelula("cn");
						break;
					case 1:
						listCell.getCelula(i)->setClaseCelula("sn");
						break;
				}
			} else {
				listCell.getCelula(i)->setClaseCelula("unknown");
				//printf("célula borde (%i-%i|%i-%i): clasificada como sn\n", min_x, max_x, min_y, max_y);
			}

			//Salida clasificador estados
			y2 = saida_svm(svmEst, resul3, media, desv);
			numPatronesEstado[y2]++;
			switch (y2)
			{
				case 0:
					listCell.getCelula(i)->setEstadoCelula("ac");
					break;
				case 1:
					listCell.getCelula(i)->setEstadoCelula("hid");
					break;
				case 2:
					listCell.getCelula(i)->setEstadoCelula("vit");// vitelinas o atresicas
					break;
			}
		    free_point_list ( puntos );
		    free ( puntos->point );
		    free ( puntos );
			free ( resul );
		}
		porcProgreso = (double) i/numCelulas;
		dProgres.setEstadoBarraProgreso(porcProgreso);
		dProgres.setPercentText(porcProgreso);
		dProgres.ejectuaDialogoProgreso();
		Utiles::actualizarInterfaz();
	}

	printf("patrones asignados a clases: ");
	const char  *nombreClase[N_CLASES] = {"cn", "sn"};
	for(int i = 0; i < N_CLASES; i++) {
		printf("%s: %i  ", nombreClase[i], numPatronesClase[i]);
	}
	printf("\npatrones asignados a estados: ");
	const char  *nombreEstado[N_ESTADOS] = {"ac", "hid", "vit/atre"};
	for(int i = 0; i < N_ESTADOS; i++) {
		printf("%s: %i  ", nombreEstado[i], numPatronesEstado[i]);
	}
	printf("\n");

	destrue_svm(svmCl);
	destrue_svm(svmEst);

	free_img ( img );
	free ( img );
}
