//â ýòî êëàññå îïèñàíà ðàáîòà ãåíåðàòîðà. Ãåíåðàòîð íå âûäàåò âñþ ïîñëåäîâàòåëüíîñòü, òîëüêî íîâûé ýëåìåíò ïîñëåäîâàòåëüíîñòè

#include "pch.h"
#include <iostream>
#include <Vector>

using namespace std; 

class Generator {
public:
	Generator() {};
	Generator(long long m, long long a, long long b, uint32_t X_0) {
		this->m = m;
		this->a = a;
		this->b = b;
		this->X_0 = X_0;
	}
	~Generator() {};
	long double next() {
		X_0 = ((a*X_0 + b) % m);
		long double eps = (long double)X_0 / m;
		return eps;
	}

private:
	long long m, a, b;
	uint32_t X_0;
};
