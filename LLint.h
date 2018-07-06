#ifndef LLint_H_INCLUDED
#define LLint_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define BYTES 260
#define BITS 2080

class LLint
{
	public:
	unsigned char n[BYTES];

	LLint();
	
	void set1At(unsigned int);
	void set0At(unsigned int);
	void setInf();
	void set0();
	unsigned int fistBit();
	bool equal0();
	bool equalInf();

	void setRandom();
	void setRandom(unsigned int);


	void operator = (char *);
	void operator = (char);
	void operator = (LLint);

	char * getString();
	int * toInt();
	unsigned int toInt32();

	LLint operator + (LLint);
	void operator += (LLint);

	LLint operator - (LLint);
	void operator -= (LLint);

	bool operator == (LLint);
	bool operator != (LLint);
	bool operator <= (LLint);
	bool operator < (LLint);
	bool operator >= (LLint);
	bool operator > (LLint);

	void _1 ();
	void _2 ();

	LLint operator * (LLint);
	void operator *= (LLint);
	LLint operator / (LLint);
	void operator /= (LLint);

	LLint operator % (LLint);
	void operator %= (LLint);
	
	LLint * divmod (LLint); 
	LLint * divmod (unsigned int); 
	
	LLint pow (LLint);
	LLint pow (unsigned int);
	LLint sqrt (LLint);
	LLint sqrt (unsigned int);

	
	LLint powMod (LLint,LLint);
	LLint powMod (unsigned int,unsigned int);

	LLint allPadding(unsigned int *);

	char * toString(LLint);

	
	void readFromFile(FILE *);
	void readFromFile(FILE *, int);
	void saveToFile(FILE *);
	void saveToFile(FILE *, int);
};


LLint int2LLint(unsigned int);
LLint readLLint (char *, LLint);
LLint * EuclidesExt(LLint, LLint); // as+bt=mcd(a,b) => {mcd(a,b), s, t}

#endif