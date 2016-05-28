#ifndef BALAS_H
#define BALAS_H

#include <vector>

typedef long long int64;
typedef long double lf;

using namespace std;

namespace My_Classes {
	class Balas {
	public:
		//Counts vector<char>X according to minimization of Sum(CiXi) function
		//If some solutions are possible, vector<char>X will represent only one of them
		//Affects vector<char>X
		//Returns if count was successful
		//Aware that C vector must be totally nonnegative
		//Also be aware that Heights of A and B vectors MUST be the same as soon as C, A, and X vectors widths.
		//Also width of B vector MUST equal to 1
		static bool Count_Balas(const vector<lf> &C, const vector<vector<lf> > &A, const vector<lf> &B, vector<char> &X);
	};
}
#endif