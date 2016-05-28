#ifndef CREDIT_OWN_H
#define CREDIT_OWN_H

#include <ctime>

typedef long long int64;
typedef long double lf;
typedef unsigned int uint;

using namespace std;

namespace My_Classes {
	class Credit_Own {
	private:
		//Percantage rate is taken from My_Classes::Input_Data::Credits[this->Ind]
		int Ind; //Index in My_Classes::Input_Data::Credits
		int64 Sum_Base;
		int64 Sum_Percent;
		/*Absolute value of money which will be counted to 
		this->Sum_Percent on next day*/
		int64 Next_Day_Percent; 
		time_t Today;
		void Update_Next_Day_Percent(); //Updates this->Next_Day_Percent
	public:
		Credit_Own();
		Credit_Own(uint Ind, int64 Sum_Base, time_t Today); //this->Sum_Percent will be intialized to 0
		int64 Remains(); //== Sum_Base + Sum_Percent
		bool is_Completed(); //returns Remains == 0
		void add_nDays(uint n); //Adds n days and affects Sum_Percent
		int64 Pay(int64 Sum); //Pays credit (affects Sum_Base and Sum_Percent) and returns extra money (if appropriate)
		//Accessors
		uint Get_Ind() const;
		int64 Get_Sum_Base() const;
		int64 Get_Sum_Percent() const;
		int64 Get_Next_Day_Percent() const;
		time_t Get_Today() const;
	};
}

#endif