#ifndef INPUT_DATA_H
#define INPUT_DATA_H

//#include <atltime.h>
#include <ctime>
#include <vector>
#include "ExcelFormat Files\BasicExcel.hpp"
#include "ExcelFormat Files\ExcelFormat.h"

typedef long long int64;
typedef long double lf;

namespace My_Classes {
	class Input_Data {
	public:
		static const char * const T_LIT;
		static const char * const MONEY_INCOME_LIT;
		static const char * const DEBT_LIT;
		static const char * const CREDIT_LIT;

		static const wchar_t * const T_LIT_L;
		static const wchar_t * const MONEY_INCOME_LIT_L;
		static const wchar_t * const DEBT_LIT_L;
		static const wchar_t * const CREDIT_LIT_L;

		class Time_Interval {
		public:
			tm From, To;
			Time_Interval();
			Time_Interval(int From, int To);
			Time_Interval(tm From, tm To);

			//Reads Time interval in specified Time_Interval variable
			//returns if Reading was succeded
			static bool Read_From_Xls(ExcelFormat::BasicExcelWorksheet *Sheet, Time_Interval &Instance); //Not reference & to work properly
		};

		class Money_Income {
		public:
			tm Date;
			int64 Sum;
			Money_Income();
			Money_Income(tm Date, int64 Sum);

			//Attempts to Read data to <Money_Incomes> vector starting from position Sheet->Cell(Row, Col)
			//Always succeeds
			//Initial Row must be next after the SubHeader Row (not after the Header itself!!!) (so Row + 2 is needed)
			//Initial Column is the same
			//Finish work when empty cell or error data is reached
			static void Read_Downward(ExcelFormat::BasicExcelWorksheet *Sheet,
				int Row, int Col, vector<Money_Income> &Vector);
				//Reads an <Money_Incomes> vector. Returns if reading succeeded
			static bool Read_From_Xls(ExcelFormat::BasicExcelWorksheet *Sheet, vector<Money_Income> &Vector);
		};

		class Debt {
		public:
			string Name;
			tm DeadLine;
			lf Percentage;
			int64 Sum;
			Debt();
			Debt(const string &Name, tm DeadLine, lf Percentage, int64 Sum);
			static void Read_Downward(ExcelFormat::BasicExcelWorksheet *Sheet, int Row, int Col, vector<Debt> &Vector);
			static bool Read_From_Xls(ExcelFormat::BasicExcelWorksheet *Sheet, vector<Debt> &Vector);
		};

		class Credit {
		public:
			string Name;
			lf Percentage;
			int64 Max_Sum;
			Credit();
			Credit(const string &Name, lf Percentage, int64 Max_Sum);
			static void Read_Downward(ExcelFormat::BasicExcelWorksheet *Sheet, int Row, int Col, vector<Credit> &Vector);
			static bool Read_From_Xls(ExcelFormat::BasicExcelWorksheet *Sheet, vector<Credit> &Vector);
		};
		
		//Standart vectors of Input_Data nested classes
		//These ones represent all input data, used in program
		static Time_Interval T;
		static vector<Money_Income> Money_Incomes;
		static vector<Debt> Debts;
		static vector<Credit> Credits;

		//Reads all input data and returns if reading succeeded
		static bool Read_Data(ExcelFormat::BasicExcelWorksheet *Sheet);
	};
}

#endif

