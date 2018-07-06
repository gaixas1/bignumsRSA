#ifndef LLintPSP_H_INCLUDED
#define LLintPSP_H_INCLUDED

#include "LLint.h"

class LLintPSP
{
private:
	LLint * lPrimes;
	unsigned int nLP;

public:
	LLintPSP();
	LLintPSP(unsigned int);
	void ini(unsigned int);
	
	LLint search(unsigned int);
	void search(unsigned int, char *);
};

bool testEuler(LLint, LLint);
bool testFermat(LLint, LLint);
bool testSTR(LLint, LLint);

#endif
