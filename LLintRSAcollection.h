#ifndef LLintRSAcollection_H_INCLUDED
#define LLintRSAcollection_H_INCLUDED

#include "LLintRSA.h"
#include <time.h> 

class LLintRSAcollection
{
private:
	LLintRSA * rsa;

public:
	LLintRSAcollection();
	LLintRSAcollection(LLintRSA *);

	void setRSA(LLintRSA *);
	LLintRSA * getRSA();
	
	void encrypt(char *, char *);
	void decrypt(char *, char *);
};

#endif
