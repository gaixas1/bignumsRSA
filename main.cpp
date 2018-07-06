#include <stdio.h>
#include "LLint.h"
#include "LLintRSA.h"
#include "LLintRSAcollection.h"
#include "LLintPSP.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <conio.h>
#include <iostream>
#include <windows.h>

LLintPSP c;
LLintRSA rsa;

LLint generarPSP();
LLint leerPSP();
LLintRSA generarRSA();
LLintRSA leerRSA();

void encriptar();
void decriptar();

int main ()
{
	printf("MINI RSA :: \n");
	printf("Sergio Leon Gaixas - UPF 2011\n\n");
	printf("Iniciando:\n");

	srand ( time(NULL) );
	c.ini(100);
	char pulsado;
	system("cls");

	do {
		printf("MINI RSA :: \n");
		printf("Sergio Leon Gaixas - UPF 2011\n");
		printf("---------------------------\n\n");
		printf("Selecciona una opcion para continuar:\n");
		printf("\ta - Generar numero PseudoPrimo.\n");
		printf("\tb - Generar Clave RSA.\n");
		printf("\tc - Cifrar archivo.\n");
		printf("\td - DesCifrar archivo.\n");
		printf("\tESC - SALIR\n");
		while (!_kbhit());
		pulsado=_getch();


		switch(pulsado) {
			case 'a': 
				generarPSP();
				break;
			case 'b': 
				generarRSA();
				break;
			/*case 'c': 
				leerRSA();
				break;*/
			case 'd': 
				decriptar();
				break;
			case 'c': 
				encriptar();
				break;
		}

		system("cls");
	} while(pulsado != 27);

	return 1;
}

LLint generarPSP() {
	printf("\n---------------------------\n");
	printf("Generar numero PseudoPrimo.\n");
	printf("---------------------------\n\n");
	
	int nb;
	bool pcorr = false;
	while(!pcorr) {
		printf("Introduce el numero de bits (50/512)\n");
		scanf("%d", &nb);
		if(nb<50 || nb>512) {
			printf("Valor incorrecto\n");
			printf("---------------------------\n\n");
		} else {
			pcorr = true;
		}
	}
	
//timer	>>>>	
	int tin, tout;
	tin = clock();
//timer <<<<	
	LLint n = c.search(nb);
	
//timer	>>>>	
	tout = clock();
	printf("-----Tiempo de ejecución : %d------------\n",tout-tin);
//timer <<<<	


	LLint a;
	a.n[0] = 255;
	a.n[1] = 255;
	a.n[2] = 255;
	a.n[3] = 255;
	a.n[4] = 255;
	a.n[5] = 255;
	a.n[6] = 255;
	a.n[7] = 255;
	a.n[8] = 255;
	a.n[9] = 255;
	a.n[10] = 255;

	LLint * t = n.divmod(a);

	char file[200];
	sprintf(file, "Primos/%d_%s.prime.llint", nb,t[1].toString(int2LLint(32)));
	FILE * f = fopen(file, "wb");
	if(f) {
		printf("\nGuardando en %s\n", file);
		n.saveToFile(f);
		fclose(f);
	} else {
		printf("\nFallo al guardar en %s\n", file);
	}
	printf("Aprete cualquier tecla para continuar.\n");

	while (!_kbhit());
	_getch();

	return n;
}

LLintRSA generarRSA() {
	printf("\n---------------------------\n");
	printf("Generar clave RSA.\n");
	printf("---------------------------\n\n");
	
	printf("Pseudoprimo p\n");
	printf("\ta - Generar numero PseudoPrimo.\n");
	printf("\tb - Leer numero PseudoPrimo.\n");
	char pulsado = 0;
	bool pcorr = false;
	while(!pcorr) {
		while (!_kbhit());
			pulsado=_getch();

			switch(pulsado) {
				case 'a': 
					rsa.setP(generarPSP());
					pcorr = true;
					break;
				case 'b': 
					rsa.setP(leerPSP());
					pcorr = true;
					break;
		}
	}
	printf("Pseudoprimo q\n");
	printf("\ta - Generar numero PseudoPrimo.\n");
	printf("\tb - Leer numero PseudoPrimo.\n");
	pcorr = false;
	while(!pcorr) {
		while (!_kbhit());
			pulsado=_getch();

			switch(pulsado) {
				case 'a': 
					rsa.setQ(generarPSP());
					pcorr = true;
					break;
				case 'b': 
					rsa.setQ(leerPSP());
					pcorr = true;
					break;
		}
	}
	
	printf("Generando claves\n");
	rsa.generateKeys();
	
		rsa.showKeys();

	char filePr[200], filePu[200];
	
	sprintf(filePr, "Claves/privadas/%s.keys.llint", rsa.getFileName());
	sprintf(filePu, "Claves/publicas/%s.keys.llint", rsa.getFileName());
	printf("\nGuardando en:\n %s\n %s\n\n", filePr,filePu);
	rsa.saveFile(filePr, filePu);
	printf("Aprete cualquier tecla para continuar.\n");

	while (!_kbhit());
	_getch();

	return rsa;
}

LLint leerPSP() {
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
 
	hFind = FindFirstFile("Primos\\*", &findFileData);
 
	char files[50][260];
	int act = 0;
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if(strcmp(findFileData.cFileName, "..") && strcmp(findFileData.cFileName, ".") )
			{
				strcpy (files[act],findFileData.cFileName);
				act++;
			}
		} while (FindNextFile(hFind, &findFileData) != 0);
	}
	
	bool pcorr = false;
	int nfile;
	while(!pcorr) {
		printf("Selecciona un archivo:\n");
		for (int i = 0; i<act; i++) {
			printf("\t%d - %s\n", i, files[i]);
		}
		
		scanf("%d", &nfile);
		if(nfile<0 || nfile>=act) {
			printf("Valor incorrecto\n");
			printf("---------------------------\n\n");
		} else {
			pcorr = true;
		}
	}

	LLint a;
	FILE * file;
	char filename[300];
	sprintf(filename, "Primos/%s", files[nfile]);

	printf("\n\n----------------------------\n%s\n----------------------------\n", filename);
	file = fopen(filename, "rb");
	if(file) {
		a.readFromFile(file);
		fclose(file);
	}
	printf("\n\n----------------------------\n%s\n----------------------------\n", a.toString(int2LLint(10)));
	return a;
}

LLintRSA leerRSA() {
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
 
	hFind = FindFirstFile("Claves/publicas\\*", &findFileData);
 
	char files[50][260];
	int act = 0;
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if(strcmp(findFileData.cFileName, "..") && strcmp(findFileData.cFileName, ".") )
			{
				strcpy (files[act],findFileData.cFileName);
				act++;
			}
		} while (FindNextFile(hFind, &findFileData) != 0);
	}
	
	bool pcorr = false;
	int nfile;
	while(!pcorr) {
		printf("Selecciona una clave:\n");
		for (int i = 0; i<act; i++) {
			printf("\t%d - %s\n", i, files[i]);
		}
		
		scanf("%d", &nfile);
		if(nfile<0 || nfile>=act) {
			printf("Valor incorrecto\n");
			printf("---------------------------\n\n");
		} else {
			pcorr = true;
		}
	}

	LLint a;
	char filenamePu[300];
	sprintf(filenamePu, "Claves/publicas/%s", files[nfile]);

	printf("\n\n----------------------------\n%s\n----------------------------\n", filenamePu);
	rsa.loadFilePu(filenamePu);
	return rsa;
}

void encriptar() {
	printf("\n---------------------------\n");
	printf("Cifrar archivo.\n");
	printf("---------------------------\n\n");

	leerRSA();

	WIN32_FIND_DATA findFileData;
	HANDLE hFind;

 
	hFind = FindFirstFile("Files\\*", &findFileData);
 
	char files[50][260];
	int act = 0;
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if(strcmp(findFileData.cFileName, "..") && strcmp(findFileData.cFileName, ".") )
			{
				strcpy (files[act],findFileData.cFileName);
				act++;
			}
		} while (FindNextFile(hFind, &findFileData) != 0);
	}
	
	bool pcorr = false;
	int nfile;
	while(!pcorr) {
		printf("Selecciona un archivo:\n");
		for (int i = 0; i<act; i++) {
			printf("\t%d - %s\n", i, files[i]);
		}
		
		scanf("%d", &nfile);
		if(nfile<0 || nfile>=act) {
			printf("Valor incorrecto\n");
			printf("---------------------------\n\n");
		} else {
			pcorr = true;
		}
	}

	LLint a;
	FILE * file;
	char infilename[300];
	sprintf(infilename, "Files/%s", files[nfile]);
	char outfilename[300];
	sprintf(outfilename, "EnFiles/%s", files[nfile]);

	LLintRSAcollection cripter = LLintRSAcollection(&rsa);
	
	printf("Inicio\n");
	cripter.encrypt(infilename,outfilename);

	printf("Aprete cualquier tecla para continuar.\n");

	while (!_kbhit());
	_getch();
}
void decriptar() {
	printf("\n---------------------------\n");
	printf("Descifrar archivo.\n");
	printf("---------------------------\n\n");
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
 
	hFind = FindFirstFile("EnFiles\\*", &findFileData);
 
	char files[50][260];
	int act = 0;
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if(strcmp(findFileData.cFileName, "..") && strcmp(findFileData.cFileName, ".") )
			{
				strcpy (files[act],findFileData.cFileName);
				act++;
			}
		} while (FindNextFile(hFind, &findFileData) != 0);
	}
	
	bool pcorr = false;
	int nfile;
	while(!pcorr) {
		printf("Selecciona un archivo:\n");
		for (int i = 0; i<act; i++) {
			printf("\t%d - %s\n", i, files[i]);
		}
		
		scanf("%d", &nfile);
		if(nfile<0 || nfile>=act) {
			printf("Valor incorrecto\n");
			printf("---------------------------\n\n");
		} else {
			pcorr = true;
		}
	}

	LLint a;
	FILE * file;
	char infilename[300];
	sprintf(infilename, "EnFiles/%s", files[nfile]);
	char outfilename[300];
	sprintf(outfilename, "Files/dec_%s", files[nfile]);
	
	LLintRSAcollection cripter = LLintRSAcollection();
	cripter.decrypt(infilename,outfilename);

	printf("Aprete cualquier tecla para continuar.\n");

	while (!_kbhit());
	_getch();
}