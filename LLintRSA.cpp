
#include "LLintRSA.h"

LLintRSA::LLintRSA() { }

LLintRSA::LLintRSA(LLint _p, LLint _q) {
	p = _p;
	q = _q;
}

void LLintRSA::setP(LLint _p) {
	p = _p;
}

void LLintRSA::setQ(LLint _q) {
	q = _q;
}

void LLintRSA::generateKeys() {
	n = p*q;
	p-int2LLint(1);
	eu = (p-int2LLint(1))*(q-int2LLint(1));
	LLint * eX;
	bool eCoPu = false, MCDeq1 = false;
	unsigned int maxBits = eu.fistBit()-1;

	e = int2LLint(67);
	eX = EuclidesExt(e, eu);
		d = eX[1];
	
	while(!MCDeq1) {
		eCoPu = false;
		while (!eCoPu) {
			e.setRandom(maxBits);
			eCoPu = !(eu%e).equal0() ;
		}
		eX = EuclidesExt(e, eu);
		d = eX[1];
		MCDeq1 = (eX[0] == int2LLint(1));
	}

	name = this->FileName
		();
}

LLint LLintRSA::encrypt(LLint data) {
	return data.powMod(e, n);
}
LLint LLintRSA::decrypt(LLint data) {
	return data.powMod(d, n);
}

void LLintRSA::showKeys() {
	printf("name:\n %s\n\n", name);
	printf("n:\n %s\n\n", n.toString(int2LLint(10)));
	printf("eu:\n %s\n\n", eu.toString(int2LLint(10)));
	printf("e :\n %s\n\n", e.toString(int2LLint(10)));
	printf("d :\n %s\n\n", d.toString(int2LLint(10)));
}
char * LLintRSA::FileName() {
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

	LLint * t = (e+d).divmod(a);
	
	char * ret = (char*) malloc(sizeof(char)*200);
	sprintf(ret, "%u_%s", n.fistBit(), t[1].toString(int2LLint(32)));
	return ret;
}
char * LLintRSA::getFileName() {
	return name;
}

LLint LLintRSA::getN() {return n;}
LLint LLintRSA::getE() {return e;}
LLint LLintRSA::getD() {return d;}


void LLintRSA::loadFilePr(char * fname){
	name = (char*) malloc(sizeof(char)*200);

	FILE *fPr;
	fPr = fopen(fname, "rb");
	if(fPr) {
		fscanf(fPr, "%s ", name);
		
		n.readFromFile(fPr);
		d.readFromFile(fPr);
		fclose(fPr);
	}
}

void LLintRSA::loadFilePu(char * fname){
	name = (char*) malloc(sizeof(char)*200);

	FILE * fPu;
	fPu = fopen(fname, "rb");
	if(fPu) {
		fscanf(fPu, "%s ", name);
		
		n.readFromFile(fPu);
		e.readFromFile(fPu);
		fclose(fPu);
	}
}


void LLintRSA::loadFile(char * fnamePr, char * fnamePu){
	name = (char*) malloc(sizeof(char)*200);
	char * name2 = (char*) malloc(sizeof(char)*200);

	FILE * fPu, *fPr;
	fPr = fopen(fnamePr, "rb");
	fPu = fopen(fnamePu, "rb");
	if(fPr && fPu) {
		fscanf(fPr, "%s ", name);
		fscanf(fPu, "%s ", name2);
		
		eu.readFromFile(fPr);
		n.readFromFile(fPu);

		e.readFromFile(fPu);
		d.readFromFile(fPr);
		fclose(fPr);
		fclose(fPu);
	}else{
		printf("cagada");
	}
}
void LLintRSA::saveFile(char * fnamePr, char * fnamePu){
	FILE * fPu, *fPr;
	fPr = fopen(fnamePr, "wb");
	fPu = fopen(fnamePu, "wb");
	if(fPr && fPu) {
		fprintf(fPr, "%s ", name);
		n.saveToFile(fPr);
		d.saveToFile(fPr);
		
		fprintf(fPu, "%s ", name);
		n.saveToFile(fPu);
		e.saveToFile(fPu);

		fclose(fPr);
		fclose(fPu);
	}
}