#include "mfmc.h"
#include "Core.h"
#include "Input_Data.h"
#include <set>

#define mp make_pair

My_Classes::mfmc::flow_edge::flow_edge() {
	Flow = Capacity = Cost = From = To = 0;
}
My_Classes::mfmc::flow_edge::flow_edge(uint From, uint To, int64 Flow, int64 Cost, int64 Capacity) {
	this->From = From;
	this->To = To;
	this->Flow = Flow;
	this->Capacity = Capacity;
	this->Cost = Cost;
}

//--------------------------mfmc structure------------------------
//Vertexes are numbered from zero
static uint Source, Stock; //source and stock indexes
static vector<int64> distances; //holds layer network: distances from source to i-th vertex;
vector<My_Classes::mfmc::flow_edge> My_Classes::mfmc::edges = vector<My_Classes::mfmc::flow_edge>();
vector<vector<uint> > My_Classes::mfmc::g = vector<vector<uint> >(); //for i holds vector<int64> with edges numbers, prolonged from this point;
static vector<int64> phi; //vertexes potentials array
static vector<int> parents; //for i-th vertex holds last edge number, which has updated it;
static vector<char> used; //== if we already were at used[i] vertex
//-----------------------------------------------------------------

void My_Classes::mfmc::add_flow_edge(uint From, uint To, int64 Cost, int64 Capacity) {
	edges.push_back(flow_edge(From, To, 0, Cost, Capacity));
	g[From].push_back(edges.size() - 1);

	edges.push_back(flow_edge(To, From, 0, -Cost, 0));
	g[To].push_back(edges.size() - 1);
}

//--------------------------mfmc stuff----------------------------

static const int64 INF = 0x7F7F7F7F7F7F7F7F;

//counts distances from Source and returns min cost to Stock if found
//also pushes flow
//returns -1 otherwise
//initializes distances, used and phi;
int64 deikst() {
	set<pair<int64, int>> myset; //first -> distance; second -> vertex number
	set<pair<int64, int>> :: iterator got, it;

	distances.assign(distances.size(), INF);
	used.assign(used.size(), false);
	distances[Source] = 0;
	myset.insert(mp(0, Source));

	while(!myset.empty()) {
		int from = myset.begin()->second;
		int64 from_weight = myset.begin()->first, cur_weight;
		int to;
		My_Classes::mfmc::flow_edge cur_edge;

		myset.erase(myset.begin());
		if(used[from])
				continue;
		used[from] = true;

		for(uint i = 0; i < My_Classes::mfmc::g[from].size(); i++) {
			cur_edge = My_Classes::mfmc::edges[My_Classes::mfmc::g[from][i]];
			to = cur_edge.To;
			cur_weight = cur_edge.Cost + phi[from] - phi[to];

			if(distances[from] + cur_weight < distances[to] && cur_edge.Flow < cur_edge.Capacity) {
				distances[to] = distances[from] + cur_weight;
				myset.insert(mp(distances[to], to));
				parents[to] = My_Classes::mfmc::g[from][i];
			}
		}
	}
	
	if(!used[Stock])
		return -1;
	
	int cur_vertex = Stock;
	int64 cur_flow = INF;
	int64 cur_cost = 0;

	while(cur_vertex != Source) {
		int prev = parents[cur_vertex];
		cur_flow = min(cur_flow, My_Classes::mfmc::edges[prev].Capacity - My_Classes::mfmc::edges[prev].Flow);
		cur_vertex = My_Classes::mfmc::edges[prev].From;
	}

	cur_vertex = Stock;
	while(cur_vertex != Source) {
		int prev = parents[cur_vertex];
		My_Classes::mfmc::edges[prev].Flow += cur_flow;
		My_Classes::mfmc::edges[prev ^ 1].Flow -= cur_flow;
		cur_vertex = My_Classes::mfmc::edges[prev].From;
		cur_cost += My_Classes::mfmc::edges[prev].Cost;
	}

	for(uint i = 0; i <= My_Classes::mfmc::g.size(); i++) { //or for(int64 i = 0; i < n + m + 2; i++) -> depends on TLE;)
		if(used[i])
			phi[i] += distances[i];
	}

	return cur_cost * cur_flow;
}

//counts maxflow mincost
int64 Count_mfmc() {
	int64 ret = 0;
	while(true) {
		int64 d = deikst();
		if(d == -1)
			break;

		ret += d;
	}
	return ret;
}
//----------------------------------------------------------------

bool My_Classes::mfmc::Count_Flow(const vector<char> &X, const int DAYS, const vector<lf> &C) {
	//-------Initialization of mfmc structure---------------
	Source = 0;
	Stock = My_Classes::Input_Data::Credits.size() + My_Classes::Core::Unpaid_Debts.size() + 1;
	My_Classes::mfmc::g = vector<vector<uint> >(Stock + 1, vector<uint>(0));
	My_Classes::mfmc::edges.clear();
	used = vector<char>(Stock + 1, false);
	phi = vector<int64>(Stock + 1, 0);
	distances = vector<int64>(Stock + 1, INF);
	parents = vector<int>(Stock + 1, -1);
	//------------------------------------------------------
	//--------Add all credits to flow network-------
	const int64 SCALE_FACTOR = 1000 * 1000 * 1000;
	for(uint i = 0; i < My_Classes::Input_Data::Credits.size(); i++) {
		//Connect Source with Credit
		My_Classes::mfmc::add_flow_edge(Source, i + 1, My_Classes::Input_Data::Credits[i].Percentage * SCALE_FACTOR, 
			My_Classes::Input_Data::Credits[i].Max_Sum);
	}
	for(uint i = 0; i < My_Classes::Input_Data::Credits.size(); i++) {
		//Connect Credit with all Debts
		for(uint j = 0; j < My_Classes::Core::Unpaid_Debts.size(); j++)
			My_Classes::mfmc::add_flow_edge(i + 1, My_Classes::Input_Data::Credits.size() + j + 1, 0, INF);
	}

	//------------------------------------------------------
	//---------------------------------------Estimation of Debts capacities-----------------------------------
	vector<pair<time_t, int> > Not_Payed_Sorted; //Time of Deadline and index in My_Classes::Input_Data::Debts
	for(auto it = My_Classes::Core::Unpaid_Debts.begin(); it != My_Classes::Core::Unpaid_Debts.end(); it++)
		Not_Payed_Sorted.push_back(mp(mktime(&My_Classes::Input_Data::Debts[*it].DeadLine) - timezone, *it));
	sort(Not_Payed_Sorted.begin(), Not_Payed_Sorted.end());
	uint it = 0; //Index of next Not_Payed Debt to be processed by mfmc
	time_t Cur_Day = mktime(&My_Classes::Input_Data::T.From) - timezone;
	int64 Money = 0;
	//Holds times (.first) to take credits for Debt (with .second index in My_Classes::Input_Data::Debts)
	vector<pair<time_t, int> > Debts_Dates(Not_Payed_Sorted.size());

	for(int i = 0; i < DAYS && it < Not_Payed_Sorted.size(); i++) {
		for(uint j = 0; j < My_Classes::Input_Data::Money_Incomes.size(); j++) {
			if(mktime(&My_Classes::Input_Data::Money_Incomes[j].Date) - timezone == Cur_Day) //We have some income!
				Money += My_Classes::Input_Data::Money_Incomes[j].Sum;
		}
		for(uint j = 0; j < My_Classes::Input_Data::Debts.size(); j++) {
			if(X[j * DAYS + i] == 1) //We have Debt to pay!
				Money -= C[j * DAYS + i];
		}
		tm Today = *gmtime(&Cur_Day);
		time_t Buf = Cur_Day + My_Classes::Core::SEC_IN_DAY; //Only for init of tm Tomorrow
		tm Tomorrow = *gmtime(&Buf);
		//Today is the last day of month and tomorrow debt will be expired, so take a credit!
		while(it < Not_Payed_Sorted.size()) { //If more than one Debt expires in the same month
			if(Cur_Day > Not_Payed_Sorted[it].first && Today.tm_mon < Tomorrow.tm_mon)  {
				My_Classes::mfmc::add_flow_edge(My_Classes::Input_Data::Credits.size() + it + 1, Stock, 0, 
					My_Classes::Input_Data::Debts[Not_Payed_Sorted[it].second].Sum - Money);
				Debts_Dates[it] = mp(Cur_Day, Not_Payed_Sorted[it].second);
				it++;
			}
			else
				break;
		}
		Cur_Day += My_Classes::Core::SEC_IN_DAY;
	}
	Count_mfmc();
	//-----------------------Estimation of Credits dates and money-------------------------------------
	My_Classes::Core::Credits_Taken.clear(); //!!!Do not forget!!!
	My_Classes::Core::X_Credit = vector<char>(My_Classes::Core::X.size(), 0); //!!!Also do not forget!!!
	bool Ret = true; //value to return //!!!Also do not forget!!!
	set<int> Payed_With_Credits_Pays; //Needed to delete payed with credits Debts from My_Classes::Core::Unpaid_Debts
	//Iterating over Credits in flow network
	for(uint i = 1; i <= My_Classes::Input_Data::Credits.size(); i++) {
		time_t Early_Date = mktime(&My_Classes::Input_Data::T.To) - timezone;
		Money = 0; //How much money we need from i-th Credit
		//Iterating over Debts in flow network
		bool Debt_Payable = false;
		for(uint j = My_Classes::Input_Data::Credits.size() + 1;
			j <= My_Classes::Input_Data::Credits.size() + My_Classes::Core::Unpaid_Debts.size(); j++) {
				int Edge_Num;
				for(uint k = 0; k < My_Classes::mfmc::g[j].size(); k++)
					if(My_Classes::mfmc::edges[My_Classes::mfmc::g[j][k]].To == Stock) {
						Edge_Num = My_Classes::mfmc::g[j][k];
						break;
					}
				//If debt can be payed with credit (which is not still taken for this Debt)
				if(My_Classes::mfmc::edges[Edge_Num].Flow == My_Classes::mfmc::edges[Edge_Num].Capacity) {
					int Debt_Num = Debts_Dates[My_Classes::mfmc::edges[Edge_Num].From
						- My_Classes::Input_Data::Credits.size() - 1].second; //Because edges was added in order of Debts_Dates vector
					Payed_With_Credits_Pays.insert(Debt_Num);
					Debt_Payable = true;
					pair<time_t, int> Pair_Buf = Debts_Dates[j - My_Classes::Input_Data::Credits.size() - 1];
					int Day = (Pair_Buf.first - mktime(&My_Classes::Input_Data::T.From) + timezone)
						/ My_Classes::Core::SEC_IN_DAY;
					My_Classes::Core::X_Credit[Pair_Buf.second * My_Classes::Core::Days + Day] = 1;
					//Money += My_Classes::mfmc::edges[Edge_Num].Flow;
					for(uint k = 0; k < g[i].size(); k++)
						if(My_Classes::mfmc::edges[g[i][k]].To == j) {
							Money = My_Classes::mfmc::edges[g[i][k]].Flow;
							break;
						}
					Early_Date = min(Early_Date,
						Debts_Dates[j - My_Classes::Input_Data::Credits.size() - 1].first);
				}
		}
		if(!Debt_Payable) //if at least one Debt can not be payed even with credits
			Ret = false;
		if(Money) //should not add Credit which is not taken
			My_Classes::Core::Credits_Taken.push_back(My_Classes::Credit_Own(i - 1, Money, Early_Date));
	}
	My_Classes::Core::Unpaid_CDebts.clear(); //------!!!Do not forget!!!------
	for(auto it = My_Classes::Core::Unpaid_Debts.begin(); it != My_Classes::Core::Unpaid_Debts.end(); it++) {
		if(Payed_With_Credits_Pays.find(*it) == Payed_With_Credits_Pays.end()) //Add to My_Classes::Core::Unpaid_CDebts
			My_Classes::Core::Unpaid_CDebts.insert(*it);
	}
	return Ret;
}
