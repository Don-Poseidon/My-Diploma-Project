#ifndef OUTPUT_DATA_H
#define OUTPUT_DATA_H

#include <vector>
#include <set>

typedef long long int64;
typedef unsigned int uint;

using namespace std;

namespace My_Classes{
	class Output_Data {
	private:
		//Vectors holds data for every single day within T
		static vector<int64> Money; //Overall money
		static vector<int64> Debts_Pays; //Debts payments with our own money
		static vector<int64> CDebts_Pays; //Debts payments with credit money
		static vector<int64> Money_Income; //Our own money income
		static vector<int64> Credits_Income; //Credit money income
		//for i-th Date and i-th Credit (My_Classes::Input_Data::Credits) holds paid sum;
		static vector<vector<int64> > Credits_Pays;
		static vector<int64> Credits_Complete_Dates; //Holds Date of last pay for i-th credit (in My_Classes::Input_Data::Credits)
		static set<int> Unpaid_Credits; //Holds indexes (in My_Classes::Input_Data::Credits) of all unpaid credits
		static void Init(); //Initializes above vectors
	public:
		static const int DESCRETE_PAY = 40000; //Pay credits with descrete amount of money
		//Outputs all necessary data
		//Affects My_Classes::Core::Credits_Taken <-- maybe not?
		static void Output();
	};

	value class Output_Data_Ref {
	public:
		static const System::Drawing::Color^ MESSAGE_COLOR = System::Drawing::Color::Green; //Different messages
		static const System::Drawing::Color^ HEADER_COLOR = System::Drawing::Color::Violet; //MainPart (1 of 5)
		static const System::Drawing::Color^ SUBHEADER_COLOR = System::Drawing::Color::DarkGoldenrod; //SubParts
		static const System::Drawing::Color^ COMPUTED_COLOR = System::Drawing::Color::Blue; //Computed info
		static const System::Drawing::Color^ WARNING_COLOR = System::Drawing::Color::DarkCyan; //Different warnings
	};
}

#endif