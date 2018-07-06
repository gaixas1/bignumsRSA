#include "LLint.h"

LLint::LLint(){
	for(int i = 0; i<BYTES; i++) {
		n[i] = 0;
	}
}

void LLint::set1At(unsigned int x) {
	if(!(n[x/8] & 1<<(x%8)))
		n[x/8] += 1<<(x%8);
}
void LLint::set0At(unsigned int x) {
	if((n[x/8] & 1<<(x%8)))
		n[x/8] -= 1<<(x%8);
}

void LLint::setInf() {
	for(int i = 0; i<BYTES; i++) {
		n[i] = 255;
	}
}
void LLint::set0() {
	for(int i = 0; i<BYTES; i++) {
		n[i] = 0;
	}
}

bool LLint::equal0() {
	unsigned char r = 0;
	for(int i = 0; i<BYTES; i++) {
		r |= n[i];
	}
	return (r == 0);
}

bool LLint::equalInf() {
	unsigned char r = 255;
	for(int i = 0; i<BYTES; i++) {
		r &= n[i];
	}
	return (r == 255);
}


void LLint::setRandom() {
	for(int i = 0; i<BYTES; i++) {
		unsigned char bt = rand()*rand();
		n[i] =bt;
	}
}
void LLint::setRandom(unsigned int last) {
	setRandom();
	for(unsigned int i = last; i<BITS; i++) {
		set0At(i);
	}
}

void LLint::operator =(char * o) {
	for(int i = 0; i<BYTES; i++) {
		n[i] = o[i];
	}
}

void LLint::operator =(LLint o) {
	for(int i = 0; i<BYTES; i++) {
		n[i] = o.n[i];
	}
}

void LLint::operator =(char o) {
	n[0] = o;
	for(int i = 1; i<BYTES; i++) {
		n[i] = 0;
	}
}

LLint LLint::operator +(LLint o) {
	LLint r;
	unsigned char s = 0, t;

	for(int i = 0; i<BYTES; i++) {
		t = n[i] + s;
		if(t < n[i]) {
			r.n[i] = o.n[i];
			s = 1;
		} else {
			r.n[i] = t + o.n[i];
			if(r.n[i] < t) {
				s = 1;
			} else {
				s = 0;
			}
		}
	}

	return r;
}

void LLint::operator +=(LLint o) {
	unsigned char s = 0, t;

	for(int i = 0; i<BYTES; i++) {
		t = n[i] + s;
		if(t < n[i]) {
			n[i] = o.n[i];
			s = 1;
		} else {
			n[i] = t + o.n[i];
			if(n[i] < t) {
				s = 1;
			} else {
				s = 0;
			}
		}
	}
}


void LLint::_1() {
	unsigned char s = 0;

	for(int i = 0; i<BYTES; i++) {
		if(n[i]&128) {
			n[i]<<=1;
			n[i]+=s;
			s = 1;
		} else {
			n[i]<<=1;
			n[i]+=s;
			s = 0;
		}
	}
}


void LLint::_2() {
	unsigned char s = 0;

	for(int i = BYTES-1; i>=0; i--) {
		if(n[i]&1) {
			n[i]>>=1;
			n[i]+=s;
			s = 128;
		} else {
			n[i]>>=1;
			n[i]+=s;
			s = 0;
		}
	}
}

LLint LLint::operator -(LLint o) {
	LLint r;
	r.set0();
	unsigned char s = 0, t;
	
	for(int i = 0; i<BYTES; i++) {
		t = n[i] - s;
		if(t > n[i]) {
			r.n[i] = t - o.n[i];
			s = 1;
		} else {
			r.n[i] = t - o.n[i];
			if(r.n[i] > t) {
				s = 1;
			} else {
				s = 0;
			}
		}
	}

	return r;
}

void LLint::operator -=(LLint o) {
	unsigned char s = 0, t;

	for(int i = 0; i<BYTES; i++) {
		t = n[i] - s;
		if(t > n[i]) {
			n[i] = t -o.n[i];
			s = 1;
		} else {
			n[i] = t - o.n[i];
			if(n[i] > t) {
				s = 1;
			} else {
				s = 0;
			}
		}
	}
}


LLint LLint::operator *(LLint o) {
	LLint r;
	for(int i = BYTES-1; i>=0; i--) {
		int p = 128;
		for(int j = 0; j<8; j++) {
			r._1();
			if(n[i]&p) {
				r+=o;
			}
			p>>=1;
		}
	}
	return r;
}

void LLint::operator *=(LLint o) {
	LLint r;
	r = (char)1;
	for(int i = BYTES-1; i>=0; i--) {
		int p = 128;
		for(int j = 0; j<8; j++) {
			r._1();
			if(n[i]&p) {
				r+=o;
			}
			p>>=1;
		}
	}
	for(int i = 0; i<BYTES; i++)
		n[i] = r.n[i];
}


LLint* LLint::divmod(LLint o) {
	LLint *ret = (LLint*)malloc(sizeof(LLint)*2);
	LLint *r, *d;
	d = &ret[0];
	r = &ret[1];
	LLint oo = o;

	d->set0();
	r->set0();

	if(o.equal0()) {
		d->setInf();
	} else if(o>*this) {
		*r = *this;
	} else {
		*r = *this; 
		d->set0();
		int x = 0;
		while(*r >= o && x<BITS) {
			o._1();
			x++;
		}
		for (; x>=0; x--) {
			d->_1();
			if(*r >= o) {
				*r -= o;
				d->n[0] += 1;
			}
			o._2();
		}
		
		*r = *this - oo*(*d);
	}
	return ret;
}

LLint* LLint::divmod(unsigned int o) {
	return divmod(int2LLint(o));
}

void LLint::operator /= (LLint o) {
	LLint * r = this->divmod(o);
	*this = r[0];
}
void LLint::operator %= (LLint o) {
	LLint * r = this->divmod(o);
	*this = r[1];
}

LLint LLint::operator / (LLint o) {
	LLint * r = this->divmod(o);
	LLint ret = r[0];
	return ret;
}
LLint LLint::operator % (LLint o) {
	LLint * r = this->divmod(o);
	LLint ret = r[1];
	return ret;
}


char * LLint::getString() {
	
	int a;
	a=3;
	a+=43;
	a*=3;
	char * r = (char*) malloc(sizeof(char)*(BYTES+1));
	for(int i = 0; i<BYTES; i++) {
		r[i] = n[i];
	}
	r[BYTES] = '\0';
	return r;
}

LLint LLint::pow(LLint o) {
	LLint r;
	r = (char)1;
	for(int i = BYTES-1; i>=0; i--) {
		unsigned char p = 128;
		for(int j = 0; j<8; j++) {
			r*=r;
			if(o.n[i]&p) {
				r*=*this;
			}
			p>>=1;
		}
	}
	return r;
}

LLint LLint::pow(unsigned int o) {
	return pow(int2LLint(o));
}

LLint LLint::powMod (LLint o, LLint base) {
	LLint r;
	r = (char)1;
	int i = (o.fistBit()+7)/8;
	if(i>=BYTES)
		i = BYTES-1;
	for(; i>=0; i--) {
		unsigned char p = 128;
		for(int j = 0; j<8; j++) {
			r*=r;
			if(r >= base) {
				r %= base;
			}
			if(o.n[i]&p) {
				r*=*this;
				if(r >= base) {
					r %= base;
				}
			}
			p>>=1;
		}
	}
	return r;
}

LLint LLint::powMod (unsigned int o, unsigned int base) {
	return powMod(int2LLint(o),int2LLint(base));
}

LLint LLint::sqrt(LLint o) {
	return sqrt(o.toInt32());
}

LLint LLint::sqrt(unsigned int o) {
	LLint r;
	if(o==0) {
		r.setInf();
		return r;
	}
	if(o==1) {
		r = *this;
		return r;
	}
	unsigned int i = 2+(fistBit()/o);
	for(; i>0; i--) {
		r.set1At(i-1);
		LLint t = r.pow(o);
		if(t > *this) {
			r.set0At(i-1);
		}
		if(t==*this)
			return r;
	}
	
	return r;
}

bool LLint::operator == (LLint o) {
	for(int i = BYTES-1; i>=0; i--) {
		if(n[i]!=o.n[i])
			return false;
	}
	return true;
}

bool LLint::operator != (LLint o) {
	for(int i = BYTES-1; i>=0; i--) {
		if(n[i]!=o.n[i])
			return true;
	}
	return false;
}

bool LLint::operator < (LLint o) {
	for(int i = BYTES-1; i>0; i--) {
		if(n[i]>o.n[i])
			return false;
		if(n[i]<o.n[i])
			return true;
	}
	if(n[0]>=o.n[0])
		return false;
	return true;
}

bool LLint::operator > (LLint o) {
	for(int i = BYTES-1; i>0; i--) {
		if(n[i]<o.n[i])
			return false;
		if(n[i]>o.n[i])
			return true;
	}
	if(n[0]<=o.n[0])
		return false;
	return true;
}

bool LLint::operator >= (LLint o) {
	for(int i = BYTES-1; i>=0; i--) {
		if(n[i]<o.n[i])
			return false;
		if(n[i]>o.n[i])
			return true;
	}
	return true;
}
bool LLint::operator <= (LLint o) {
	for(int i = BYTES-1; i>=0; i--) {
		if(n[i]>o.n[i])
			return false;
		if(n[i]<o.n[i])
			return true;
	}
	return true;
}

int * LLint::toInt() {
	return (int*) n;
}

unsigned int LLint::toInt32() {
	unsigned int ret = 0;
	for(int i = 3; i >= 0; i--) {
		ret *= 256;
		ret += n[i];
	}
	return ret;
}

unsigned int LLint::fistBit() {
	for(int i = BYTES-1; i >= 0; i--) {
		if(n[i]!=0) {
			for(int j = 7; j >= 0; j--) {
				if(n[i] & 1<<j)
					return (i*8)+j+1;
			}
		}
	}
	return 0;
}


LLint LLint::allPadding(unsigned int * s) {
	LLint ret = *this;
	while(ret.n[0]&1) {
		ret._2();
		*s = *s + 1;
	}
	return ret;
}

char * LLint::toString(LLint base) {
	if(equal0()) {
		char * c;
		c = (char *) calloc(2, sizeof(char));
		c[0] = '0';
		c[1] = '\0';
		return c;
	}

	unsigned char * c;
	LLint tem = *this;
	int max = 30;
	int act = 0;
	char caras[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	c = (unsigned char *) calloc(max, sizeof(unsigned char));
	while (tem != int2LLint(0)) {
		LLint * index = tem.divmod(base);
		c[act] = caras[index[1].toInt32()];
		tem = index[0];
		act++;
		if(act == max) {
			max+=10;
			c = (unsigned char*) realloc(c, max*sizeof(unsigned char));
		}
	}
	char * ret;
	ret =  (char *) calloc(act+1, sizeof(char));
	for (int i = 0; i<act; i++) {
		char aaaaa = c[act-i-1];
		ret[i] = c[act-i-1];
	}
	ret[act] = '\0';

	return ret;
}

LLint readLLint (char * c, LLint base) {
	LLint ret;
	int i = 0;
	while(c[i]!='\0') {
		ret *= base;
		switch(c[i]) {
			case '0' :	ret += int2LLint(0);	break;
			case '1' :	ret += int2LLint(1);	break;
			case '2' :	ret += int2LLint(2);	break;
			case '3' :	ret += int2LLint(3);	break;
			case '4' :	ret += int2LLint(4);	break;
			case '5' :	ret += int2LLint(5);	break;
			case '6' :	ret += int2LLint(6);	break;
			case '7' :	ret += int2LLint(7);	break;
			case '8' :	ret += int2LLint(8);	break;
			case '9' :	ret += int2LLint(9);	break;
			case 'a' :	
			case 'A' :	
				ret += int2LLint(10);	break;
			case 'b' :	
			case 'B' :	
				ret += int2LLint(11);	break;
			case 'c' :	
			case 'C' :	
				ret += int2LLint(12);	break;
			case 'd' :	
			case 'D' :	
				ret += int2LLint(13);	break;
			case 'e' :	
			case 'E' :	
				ret += int2LLint(14);	break;
			case 'f' :	
			case 'F' :	
				ret += int2LLint(15);	break;
			case 'g' :	
			case 'G' :	
				ret += int2LLint(16);	break;
			case 'h' :	
			case 'H' :	
				ret += int2LLint(17);	break;
			case 'i' :	
			case 'I' :	
				ret += int2LLint(18);	break;
			case 'j' :	
			case 'J' :	
				ret += int2LLint(19);	break;
			case 'k' :	
			case 'K' :	
				ret += int2LLint(20);	break;
			case 'l' :	
			case 'L' :	
				ret += int2LLint(21);	break;
			case 'm' :	
			case 'M' :	
				ret += int2LLint(22);	break;
			case 'n' :	
			case 'N' :	
				ret += int2LLint(23);	break;
			case 'o' :	
			case 'O' :	
				ret += int2LLint(24);	break;
			case 'p' :	
			case 'P' :	
				ret += int2LLint(25);	break;
			case 'q' :	
			case 'Q' :	
				ret += int2LLint(26);	break;
			case 'r' :	
			case 'R' :	
				ret += int2LLint(27);	break;
			case 's' :	
			case 'S' :	
				ret += int2LLint(28);	break;
			case 't' :	
			case 'T' :	
				ret += int2LLint(29);	break;
			case 'u' :	
			case 'U' :	
				ret += int2LLint(30);	break;
			case 'v' :	
			case 'V' :	
				ret += int2LLint(31);	break;
			case 'w' :	
			case 'W' :	
				ret += int2LLint(32);	break;
			case 'x' :	
			case 'X' :	
				ret += int2LLint(33);	break;
			case 'y' :	
			case 'Y' :	
				ret += int2LLint(34);	break;
			case 'z' :	
			case 'Z' :	
				ret += int2LLint(35);	break;
			default :	ret += int2LLint(0);	break;
		}
		i++;
	}
	return ret;
}

LLint int2LLint(unsigned int o) {
	LLint ret;
	for(int i = 0; i < 4; i++) {
		ret.n[i] += o%256;
		o /= 256;
	}
	return ret;
}


LLint * EuclidesExt(LLint a, LLint b) {
	LLint r0, r1, r2;
	LLint s0,s1,s2;
	LLint t0,t1,t2;
	LLint temp;
	LLint n;

	if(a>=b) { r0 = a;  r1 = b; n=a;} 
	else { r1 = a; r0 = b; n = b;}

	s0 = int2LLint(1); s1.set0();
	t0.set0(); t1 = int2LLint(1); 

	while(!r1.equal0()) {
		LLint * rq = r0.divmod(r1);

		r2 = rq[1];

		temp = rq[0]*s1;
		temp %= n;
		if(s0>=temp)
			s2 = s0-temp;
		else
			s2 = (s0+n)-temp;

		temp = rq[0]*t1;
		temp %= n;
		if(t0>=temp)
			t2 = t0-temp;
		else
			t2 = (t0+n)-temp;
		r0 = r1; 
		r1 = r2;
		s0 = s1; 
		s1 = s2;
		t0 = t1; 
		t1 = t2;
	}
	LLint * ret = (LLint*) malloc(sizeof(LLint)*3);
	ret[0] = r0;
	if(a>=b) { ret[1] = s0%n;  ret[2] = t0%n;} 
	else { ret[2] = s0%n; ret[1] = t0%n;}

	return ret;
}

void LLint::readFromFile(FILE * f) {
	for(int i = 0; (i<BYTES)&&(!feof(f)); i++) {
		fscanf(f, "%c", &(n[i]));
	}
}
void LLint::saveToFile(FILE * f) {
	for(int i = 0; (i<BYTES)&&(!feof(f)); i++) {
		fprintf(f, "%c", n[i]);
	}
}

void LLint::readFromFile(FILE * f, int limit) {
	for(int i = 0; (i<limit)&&(!feof(f)); i++) {
		fscanf(f, "%c", &(n[i]));
	}
}
void LLint::saveToFile(FILE * f, int limit) {
	LLint ret;
	for(int i = 0; (i<limit)&&(!feof(f)); i++) {
		fprintf(f, "%c", n[i]);
	}
}