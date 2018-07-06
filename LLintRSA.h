#ifndef LLintRSA_H_INCLUDED
#define LLintRSA_H_INCLUDED

#include "LLint.h"

class LLintRSA
{
private:
	LLint p, q;
	LLint n, e, d, eu;
	char * name;

public:
	LLintRSA();
	LLintRSA(LLint, LLint); //Los dos primos
	
	void setP(LLint);
	void setQ(LLint);
	void generateKeys();
	
	LLint encrypt(LLint);
	LLint decrypt(LLint);

	void showKeys();
	
	LLint getN();
	LLint getE();
	LLint getD();
	
	char * FileName();
	char * getFileName();
	void loadFilePr(char *);
	void loadFilePu(char *);
	void loadFile(char *,char *);
	void saveFile(char *,char *);
};

#endif
