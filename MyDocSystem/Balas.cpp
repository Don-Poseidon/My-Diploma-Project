#include "Balas.h"
#include <algorithm>

#define mp make_pair

typedef unsigned int uint;

static const int NOT_FOUND = -1;
static vector<int> Infeasible_Arr;
static int it; //iterator to Infeasible_Arr; Changes only in Check_Infeasible(...)

//Returns F value for given X and C vectors
lf Get_F(const vector<char> &X, const vector<lf> &C) {
	lf Ret = 0;
	for(uint i = 0; i < C.size(); i++) {
			Ret += X[i] * C[i];
	}
	return Ret;
}

//Checks current solution to feasibility
//Affects Infeasible_Arr with indexes of infeasible constraints (numbering from zero)
//If infeasibles were not found, it == 0
void Check_Infeasible(const vector<char> &X, const int Level, const vector<vector<lf> > &A, const vector<lf> &B) {
	//memset(Infeasible_Arr, -1, sizeof(Infeasible_Arr));
	it = 0;
	for(uint i = 0; i < A.size(); i++) {
		lf Sum = 0;
		for(uint j = 0; j < min((uint)Level + 1, X.size()); j++) {
			Sum += A[i][j] * X[j];
		}
		if(Sum < B[i])
			Infeasible_Arr[it++] = i;
	}
}

//Level is a number of last fixed X (numbering from zero)
//Infeasible is a vector of indexes of infeasible constraints
//Returns false if at least one constraint can never be satisfied
bool Check_Infeasible_to_Feasible(const vector<char> &X, const int Level, const vector<vector<lf> > &A, const vector<lf> &B) {
	for(int i = 0; i < it ; i++) {
		int Row = Infeasible_Arr[i];
		lf B_Buf = B[Row];
		for(int j = 0; j <= Level; j++) {
			B_Buf -= A[Row][j] * X[j];
		}
		lf Sum = 0;
		for(uint j = Level + 1; j < X.size(); j++) {
			if(A[Row][j] > 0)
				Sum += A[Row][j];
		}
		if(Sum < B_Buf)
			return false;
	}
	return true;
}

//Returns if current vertex can be expanded (== we are not in dead-end)
//Also may affect F_Best_X (if F_X is better) => in that case also returns false => no need to expand
//Also uses some of above implemented checks
//If Best_Vertex is not found yet, F_Best_X should equal to NOT_FOUND
bool is_Expandable(const vector<char> &X, const lf F_X, const int Level, const vector<vector<lf> > &A,
				   const vector<lf> &B, vector<char> &Best_X, lf &F_Best_X) {	
	if(F_Best_X != NOT_FOUND && F_X >= F_Best_X)
		return false;
	Check_Infeasible(X, Level, A, B);
	if(F_Best_X != NOT_FOUND && !it && F_X < F_Best_X
		|| F_Best_X == NOT_FOUND && !it) { //found solution!
		F_Best_X = F_X;
		Best_X = X;
		return false;
	}
	if(it && !Check_Infeasible_to_Feasible(X, Level, A, B))
		return false;
	if(Level == A[0].size() - 1)
		return false;
	return true;
}

//Implements depth-first-search iteration over a tree
//Executes recursively until dead-end is reached
//Affects X, Best_X and F_Best_X
//When called from My_Classes::Balas::Count_Balas, initialize F_Best_X = NOT_FOUND
void Dfs(vector<char> &X, const int Level, const vector<lf> &C, const vector<vector<lf> > &A, const vector<lf> &B,
		 vector<char> &Best_X, lf &F_Best_X) {
	lf F_X = Get_F(X, C);
	if(!is_Expandable(X, F_X, Level, A, B, Best_X, F_Best_X)) {
		//if(V.Get_F() == 12) //For checking iterators
		//	System::Windows::Forms::MessageBox::Show(System::String::Format("Best_Vertex F: {0}\nBest_Vertex Level: {1}", Best_Vertex->Get_F(), Best_Vertex->Get_Level()));
		return;
	}

	X[Level + 1] = 1;
	if(Level + 1 == C.size() - 1) {
		if(X[Level] == 0)
			return;
		Dfs(X, Level + 1, C, A, B, Best_X, F_Best_X);
	}
	else {
		vector<char> Buf_X = X;
		Dfs(Buf_X, Level + 1, C, A, B, Best_X, F_Best_X);
		X[Level + 1] = 0;
		X[Level + 2] = 1;
		Dfs(X, Level + 1, C, A, B, Best_X, F_Best_X);
	}
}

//Counts vector<char> X with Balas' algo
//Affects vector<char> X
bool My_Classes::Balas::Count_Balas(const vector<lf> &C, const vector<vector<lf> > &A, const vector<lf> &B, vector<char> &X) {
	vector<pair<lf, int> > C_Sorted_Adv(C.size());
	Infeasible_Arr.resize(A.size()); //<- necessary to initialize
	for(uint i = 0; i < C.size(); i++) {
		C_Sorted_Adv[i] = mp(C[i], i);
	}
	sort(C_Sorted_Adv.begin(), C_Sorted_Adv.end());
	vector<lf> C_Sorted(C.size());
	for(uint i = 0; i < C.size(); i++) {
		C_Sorted[i] = C_Sorted_Adv[i].first;
	}
	vector<int> Forward_fitness(C.size()); //= index in sotred vector via index in pure vector
	vector<int> Backward_fitness(C.size());//= index in pure vector via index in sotred vector
	for(uint i = 0; i < C.size(); i++) {
		Forward_fitness[C_Sorted_Adv[i].second] = i;
		Backward_fitness[i] = C_Sorted_Adv[i].second;
	}

	vector<vector<lf> > A_Sorted(A.size(), vector<lf>(C.size()));
	for(uint i = 0; i < A.size(); i++) {
		for(uint j = 0; j < C.size(); j++) {
			A_Sorted[i][j] = A[i][Backward_fitness[j]];
		}
	}

	X = vector<char>(X.size());
	vector<char> Best_X = X;
	lf F_Best_X = NOT_FOUND;
	Dfs(X, -1, C_Sorted, A_Sorted, B, Best_X, F_Best_X);

	if(F_Best_X == NOT_FOUND)
		return false;
	else {
		vector<char> X_Sorted = Best_X;
		for(uint i = 0; i < X.size(); i++) {
			X[i] = X_Sorted[Forward_fitness[i]];
		}
		return true;
	}
}
