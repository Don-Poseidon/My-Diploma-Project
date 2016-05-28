/*Implements flow network for Credits taking.
Remember that Credits are being added to flow network
accordingly to their order in My_Classes::Input_Data::Credits
*/
#ifndef MFMC_H
#define MFMC_H

#include <vector>

typedef long long int64;
typedef long double lf;
typedef unsigned int uint;

using namespace std;

namespace My_Classes {
	class mfmc {
		public:
		class flow_edge {
		public:
			uint From;
			uint To;
			int64 Flow;
			int64 Capacity;
			int64 Cost;
			flow_edge();
			flow_edge(uint From, uint To, int64 Flow, int64 Cost, int64 Capacity);
		};
		static vector<flow_edge> edges;
		static vector<vector<uint> > g;
		static void add_flow_edge(uint From, uint To, int64 Cost, int64 Capacity);
		//Builds and counts mfmc according to X, DAYS and C
		//Affects My_Classes::Core::Unpaid_Debts via deleting payed (with credit money) debts
		//Returns if we can get enough credits to pay all unpayed debts
		static bool Count_Flow(const vector<char> &X, const int DAYS, const vector<lf> &C);
	};
}
#endif