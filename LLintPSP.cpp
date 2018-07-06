#include "LLintPSP.h"

LLintPSP::LLintPSP() {}
LLintPSP::LLintPSP(unsigned int limit) {
	ini(limit);
}

void LLintPSP::ini(unsigned int limit) {
	nLP = limit;

	lPrimes = (LLint*) malloc(limit*sizeof(LLint));
	unsigned int * intlPrimes = (unsigned int*) malloc(limit*sizeof(unsigned int));
	unsigned int i = 0;
	unsigned int act = 2;
	while(i<limit) {
		bool divided = false;
		for(unsigned int j = 0; j<i; j++) {
			divided |= ((act%intlPrimes[j])==0);
		}
		if(!divided) {
			intlPrimes[i] = act;
			lPrimes[i] = int2LLint(act);
			i++;
		}
		act++;
	}
}

LLint LLintPSP::search(unsigned int b) {
	LLint a;
	a.setRandom(b);
	a.set1At(b);
	a.set1At(0);
	bool psp = false;

	while(!psp) {
		psp = true;
		printf("Probando :::\n%s\n", a.toString(int2LLint(10)));
		for(unsigned int i = 0; psp &&(i<nLP); i++) {
			if(!testEuler(a, lPrimes[i])) {
				printf("falla el test 1 para:\n%s\n", lPrimes[i].toString(int2LLint(10)));
				psp = false;
			}
		}
		for(unsigned int i = 1; psp &&(i<nLP); i++) {
			if(!testFermat(a, lPrimes[i])) {
				printf("falla el test 2 para:\n%s\n", lPrimes[i].toString(int2LLint(10)));
				psp = false;
			}
		}
		for(unsigned int i = 0; psp &&(i<nLP); i++) {
			LLint r;
			r.setRandom(b);
			r.set0At(b);
			if(!testSTR(a,r)) {
				printf("falla el test 3 para:\n%s\n", r.toString(int2LLint(10)));
				psp = false;
			}
		}

		if(!psp) {
			a +=int2LLint(2);
		}
	}
	
	printf("Resultado => %s\n", a.toString(int2LLint(10)));

	return a;
}

void LLintPSP::search(unsigned int b, char * file) {

}


bool testEuler(LLint a, LLint n) {
	LLint d = n - int2LLint(1);
	return (a.powMod(d,n)==int2LLint(1));
}

bool testFermat(LLint a, LLint n) {
	LLint d = n - int2LLint(1);
	LLint d2 = d/int2LLint(2);
	return ((a.powMod(d2,n)==int2LLint(1))||(a.powMod(d2,n)==d));
}

bool testSTR(LLint n, LLint a) {
	LLint d = n - int2LLint(1);
	unsigned int s=0;
	LLint t = d.allPadding(&s);
	LLint y = a.powMod(t,n);
	if((y==int2LLint(1))||(y==d))
		return true;
	
	for (unsigned int i = 1; (i <= s) && (y!=d); i++) {
		y = y.powMod(int2LLint(2), n);
		if(y==int2LLint(1))
			return false;
	}
	if(y!=d)
		return false;

	return true;
}
