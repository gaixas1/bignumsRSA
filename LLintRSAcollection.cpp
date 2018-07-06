#include "LLintRSAcollection.h"
#include <math.h>

#include <conio.h>

LLintRSAcollection::LLintRSAcollection(){
	rsa = new LLintRSA();
}
LLintRSAcollection::LLintRSAcollection(LLintRSA * _rsa){
	rsa = _rsa;
}

void LLintRSAcollection::setRSA(LLintRSA * _rsa){
	rsa = _rsa;
}
LLintRSA * LLintRSAcollection::getRSA(){
	return rsa;
}

	
void LLintRSAcollection::encrypt(char * in, char * out){
	FILE * fin = fopen(in, "rb");
	FILE * fout = fopen(out, "wb");
	if(fin && fout) {
		fseek(fin, 0, SEEK_END);
		unsigned int tam = ftell(fin);
		fseek(fin, 0, SEEK_SET);

		printf("file size :: %d\n", tam);

		unsigned int fragBytes = (ceil(rsa->getN().fistBit()/8.0f))-1;

		float frags = tam/(float)fragBytes;
		int fragsNumber = ceil(frags);

		fprintf(fout, "%s\n", rsa->getFileName());
		fprintf(fout, "%d\n",tam);
		fprintf(fout, "%d\n",fragsNumber);
		fprintf(fout, "%d\n",fragBytes);
		
//timer	>>>>	
	int tin, tout;
	tin = clock();
//timer <<<<	
		for(int i = 0; i<fragsNumber-1; i++) {
			printf("Frag %d/%d\r", i+1, fragsNumber);
			LLint a;
			a.readFromFile(fin, fragBytes);
			LLint b = rsa->encrypt(a);
			b.saveToFile(fout,fragBytes+1);
		}
		
		printf("Frag %d/%d\n", fragsNumber, fragsNumber);
		LLint a;
		a.readFromFile(fin, tam-fragBytes*(fragsNumber-1));
		LLint b = rsa->encrypt(a);
		b.saveToFile(fout,fragBytes+1);
	
		fclose(fin);
		fclose(fout);
//timer	>>>>	
	tout = clock();
	printf("-----Tiempo de ejecución : %d------------\n",tout-tin);
//timer <<<<	
	}
}
void LLintRSAcollection::decrypt(char * in, char * out){
	FILE * fin = fopen(in, "rb");
	FILE * fout = fopen(out, "wb");


	if(fin && fout) {
		unsigned int tam;
		unsigned int fragBytes;
		int fragsNumber;

		char rsaFile[200];
		
		fscanf(fin, "%s\n", rsaFile);
		fscanf(fin, "%d\n", &tam);
		fscanf(fin, "%d\n", &fragsNumber);
		fscanf(fin, "%d\n", &fragBytes);


		char filenamePr[300];
		sprintf(filenamePr, "Claves/privadas/%s.keys.llint", rsaFile);
		printf("Leer claves %s\n", filenamePr);
		rsa->loadFilePr(filenamePr);

		printf("Claves cargadas.\n");
//timer	>>>>	
	int tin, tout;
	tin = clock();
//timer <<<<	
		for(int i = 0; i<fragsNumber-1; i++) {
			printf("Frag %d/%d\r", i+1, fragsNumber);
			LLint a;
			a.readFromFile(fin, fragBytes+1);
			LLint b = rsa->decrypt(a);
			b.saveToFile(fout, fragBytes);
		}
		printf("Frag %d/%d\n", fragsNumber, fragsNumber);
		LLint a;
		a.readFromFile(fin, fragBytes+1);
		LLint b = rsa->decrypt(a);
		b.saveToFile(fout, tam-fragBytes*(fragsNumber-1));
	
		fclose(fin);
		fclose(fout);

//timer	>>>>	
	tout = clock();
	printf("-----Tiempo de ejecución : %d------------\n",tout-tin);
//timer <<<<	
	}
}