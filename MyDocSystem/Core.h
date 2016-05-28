#ifndef CORE_H
#define CORE_H

#include <string>
#include "ExcelFormat Files\BasicExcel.hpp"
#include "ExcelFormat Files\ExcelFormat.h"
#include <ctime>
#include <set>
#include "Credit_Own.h"
//#include "Forms_Instances.h"

typedef long long int64;
typedef long double lf;

using namespace std;

namespace My_Classes {
	class Core {
	public:
		static const int SUPER_ERROR_CODE = 777; //For my own user specified errors
		static int LastErr_Num;
		static const int BUF_SIZE = 1000 * 1000; //For char* and wchar_t buffers
		static const char* const DATE_ONLY;
		static const char* const DATE_AND_TIME;
		static const int SEC_IN_DAY = 24 * 60 * 60;
		//(first == index in My_Classes::Input_Data::Debts; second == date)
		//Next are initialized during Balasian part
		static set<int> Unpaid_Debts; //Debts which are not paid with own money
		static set<int> Unpaid_CDebts; //Debts which are not paid with Credit money
		static vector<char> X; //Affected by Balas algo
		static vector<char> X_Credit; //Affected by mfmc algo
		static vector<lf> C;
		static int Days;
		static vector<My_Classes::Credit_Own> Credits_Taken; //Holds all taken credits
	private:
		static string LastErr_Str;
		static ExcelFormat::BasicExcel Xls;
		static ExcelFormat::BasicExcelWorksheet *Sheet;
		static bool XlsFile_Opened;
		
		static void Close_Excel_File();
		//returns if file opening proceeded successfully
		//affects XlsFile_Opened flag and ExcelFormat::BasicExcel::Xls and ExcelFormat::BasicExcelWorksheet *Sheet
		//affetcs Cancel_Pressed referenced flag
		static bool Open_Excel_File(System::String ^Path, bool &Cancel_Pressed);
		static bool Read_Data(System::String^ Xls_Path);
		static void Calc_Data();
		static void Output_Data();
	public:
		static System::String^ Get_LastErr_Str();
		static void Set_LastErr_Str(System::String ^Str);
		static System::String^ Convert_Chars_to_netString(char *Buf);
		static System::String^ Convert_String_to_netString(const string &Str);
		static const wchar_t* Convert_netString_to_wchar(System::String^ &Str);
		static string Convert_WString_to_String(wstring &Str);
		static System::String^ Convert_wchars_to_netString(wchar_t *Str);
		static void Do_Stuff(System::String^ Xls_Path);
		static tm Convert_XlsInt_to_Tm(int Time);
		static System::String^ Convert_tm_to_netString(tm* const Val, const char* const Format);
		static string Convert_tm_to_String(tm* const Val, const char* const Format);
		static string Convert_time_t_to_String(time_t Val, const char* const Format);
		static bool is_Leap_Year(int Year); //Year in YYYY format (not tm.tm_year)
		//Returns number of days in Month of certain Year (in 1..12 and YYYY format)
		static int Days_in_Month(int Month, int Year);
		//Below methods add formatted text to RichTextBox (default one is located in My_Classes::Forms_Instances::frm_Console_hwnd)
		static void AppendText(bool is_Bold, bool is_Italic, const System::Drawing::Color^ Color, const string &Text);
		static void AppendText(bool is_Bold, bool is_Italic, const System::Drawing::Color^ Color, System::String^ Text);
		static void AppendText_Message(const string &Text);
		static void AppendText_Header(const string &Text);
		static void AppendText_SubHeader(const string &Text);
		static void AppendText_Computed(const string &Text);
		static void AppendText_Warning(const string &Text);
	};
}

#endif