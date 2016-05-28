#include "Input_Data.h"
#include "Core.h"

const char * const My_Classes::Input_Data::T_LIT = "%tin";
const char * const My_Classes::Input_Data::MONEY_INCOME_LIT = "%fin";
const char * const My_Classes::Input_Data::DEBT_LIT = "des";
const char * const My_Classes::Input_Data::CREDIT_LIT = "%crs";

const wchar_t * const My_Classes::Input_Data::T_LIT_L = L"%tin";
const wchar_t * const My_Classes::Input_Data::MONEY_INCOME_LIT_L = L"%fin";
const wchar_t * const My_Classes::Input_Data::DEBT_LIT_L = L"des";
const wchar_t * const My_Classes::Input_Data::CREDIT_LIT_L = L"%crs";

My_Classes::Input_Data::Time_Interval::Time_Interval() {
	this->From = tm();
	this->To = tm();
}
My_Classes::Input_Data::Time_Interval::Time_Interval(int From, int To) {
	this->From = My_Classes::Core::Convert_XlsInt_to_Tm(From);
	this->To = My_Classes::Core::Convert_XlsInt_to_Tm(From);
}
My_Classes::Input_Data::Time_Interval::Time_Interval(tm From, tm To) {
	this->From = From;
	this->To = To;
}

bool My_Classes::Input_Data::Time_Interval::Read_From_Xls(ExcelFormat::BasicExcelWorksheet *Sheet, Time_Interval &Instance) {
	for(int i = 0; i < Sheet->GetTotalRows(); i++) {
		for(int j = 0; j < Sheet->GetTotalCols(); j++) {
			wchar_t Buf[My_Classes::Core::BUF_SIZE];
			if(!Sheet->Cell(i, j)->Get(Buf))
				continue;
			if(wcsstr(Buf, Input_Data::T_LIT_L) != nullptr) {
				try {
					Instance.From = My_Classes::Core::Convert_XlsInt_to_Tm(Sheet->Cell(i + 2, j)->GetInteger());
					Instance.To = My_Classes::Core::Convert_XlsInt_to_Tm(Sheet->Cell(i + 2, j + 1)->GetInteger());
					return true;
				}
				catch(System::Exception ^err) { //Can not read desired two cells properly => return false
					Core::LastErr_Num = err->HResult;
					Core::Set_LastErr_Str(err->Message);
					return false;
				}
			}
		}
	}
	Core::LastErr_Num = Core::SUPER_ERROR_CODE; //The sheet's end was reached and T was not found => return false
	Core::Set_LastErr_Str("Ќе удалось прочитать временной интервал.\n¬озможно, он не содержитс€ в указанном Excel листе.");
	return false;
}

My_Classes::Input_Data::Money_Income::Money_Income() {
	this->Date = tm();
	this->Sum = -1;
}
My_Classes::Input_Data::Money_Income::Money_Income(tm Date, int64 Sum) {
	this->Date = Date;
	this->Sum = Sum;
}
void My_Classes::Input_Data::Money_Income::Read_Downward(ExcelFormat::BasicExcelWorksheet *Sheet,
	int Row, int Col, vector<Money_Income> &Vector) {
	tm Time_Buf;
	int Sum_Buf;
	Vector.clear();
	for(int i = Row; i < Sheet->GetTotalRows(); i++) {
		try {//Check that we can read current row and than read it
			if(Sheet->Cell(i, Col)->Get(Sum_Buf) && Sheet->Cell(i, Col + 1)->Get(Sum_Buf)) {
				Time_Buf = My_Classes::Core::Convert_XlsInt_to_Tm(Sheet->Cell(i, Col)->GetInteger());
				Sum_Buf = Sheet->Cell(i, Col + 1)->GetInteger();
				Vector.push_back(Money_Income(Time_Buf, Sum_Buf)); //Sum_Buf is int32 => for int64 constructor doesn't matter
			}
			else {
				return; //Time to stop reading
			}
		}
		catch(System::Exception ^err) {
			My_Classes::Core::LastErr_Num = err->HResult;
			My_Classes::Core::Set_LastErr_Str(err->Message);
		}
	}
}
bool My_Classes::Input_Data::Money_Income::Read_From_Xls(ExcelFormat::BasicExcelWorksheet *Sheet, vector<Money_Income> &Vector) {
	for(int i = 0; i < Sheet->GetTotalRows(); i++) {
		for(int j = 0; j < Sheet->GetTotalCols(); j++) {
			try {
				wchar_t Buf[My_Classes::Core::BUF_SIZE];
				if(!Sheet->Cell(i, j)->Get(Buf))
					continue;
				if(wcsstr(Buf, Input_Data::MONEY_INCOME_LIT_L) != nullptr) {
					Money_Income::Read_Downward(Sheet, i + 2, j, Vector);
					return true;
				}
			}
			catch(System::Exception ^err) {
				Core::LastErr_Num = err->HResult;
				Core::Set_LastErr_Str(err->Message);
				return false;
			}
		}
	}
	Core::LastErr_Num = Core::SUPER_ERROR_CODE;
	Core::Set_LastErr_Str("Ќе удалось прочитать данные о поступлении денежных средств.\n"
		+ "¬озможно, они не содержатс€ в указанном Excel листе.");
	return false;
}

My_Classes::Input_Data::Debt::Debt() {
	this->Name = "";
	this->DeadLine = tm();
	this->Sum = -1;
	this->Percentage = -1.0;
}
My_Classes::Input_Data::Debt::Debt(const string &Name, tm DeadLine, lf Percentage, int64 Sum) {
	this->Name = Name;
	this->DeadLine = DeadLine;
	this->Percentage = Percentage;
	this->Sum = Sum;
}
void My_Classes::Input_Data::Debt::Read_Downward(ExcelFormat::BasicExcelWorksheet *Sheet, int Row, int Col, vector<Debt> &Vector) {
	wchar_t Name_Buf_Temp_L[Core::BUF_SIZE];
	char Name_Buf_Temp[Core::BUF_SIZE];
	string Name_Buf;
	tm DeadLine_Buf;
	double Percentage_Buf;
	int Sum_Buf;
	int Counter = 0;
	Vector.clear();
	for(int i = Row; i < Sheet->GetTotalRows(); i++) {
		try { 
			//Check that we can read current row and than read it
			if(Sheet->Cell(i, Col + 1)->Get(Sum_Buf) && Sheet->Cell(i, Col + 2)->Get(Percentage_Buf)
				&& Sheet->Cell(i, Col + 3)->Get(Sum_Buf)) {
				if(Sheet->Cell(i, Col)->Get(Name_Buf_Temp_L)) {//Cell contains russian letters (But behaviour may vary sometimes (What???);D)
					Name_Buf = My_Classes::Core::Convert_WString_to_String(wstring(Name_Buf_Temp_L));
				}
				else {
					if(Sheet->Cell(i, Col)->Get(Name_Buf_Temp)) //Cell is not empty (Empty cell ExcelFormat won't read at all)
						Name_Buf = string(Name_Buf_Temp);
					else
						Name_Buf = "";

				}
				//Check if Cell(i, Col) consists only from spaces OR is empty
				if(Name_Buf.find_first_not_of(' ') == string::npos || Name_Buf.empty()) {
					Name_Buf = "ƒолговое об€зательство #" + to_string(++Counter); //Changes the string
				}
				DeadLine_Buf = My_Classes::Core::Convert_XlsInt_to_Tm(Sheet->Cell(i, Col + 1)->GetInteger());
				Percentage_Buf = Sheet->Cell(i, Col + 2)->GetDouble();
				Sum_Buf = Sheet->Cell(i, Col + 3)->GetInteger();
				Vector.push_back(My_Classes::Input_Data::Debt(Name_Buf, DeadLine_Buf, Percentage_Buf, Sum_Buf));
			}
			else {
				return; //Time to stop reading
			}
		}
		catch(System::Exception ^err) {
			My_Classes::Core::LastErr_Num = err->HResult;
			My_Classes::Core::Set_LastErr_Str(err->Message);
		}
	}
}

bool My_Classes::Input_Data::Debt::Read_From_Xls(ExcelFormat::BasicExcelWorksheet *Sheet, vector<Debt> &Vector) {
	for(int i = 0; i < Sheet->GetTotalRows(); i++) {
		for(int j = 0; j < Sheet->GetTotalCols(); j++) {
			try {
				wchar_t Buf[My_Classes::Core::BUF_SIZE];
				if(!Sheet->Cell(i, j)->Get(Buf))
					continue;
				if(wcsstr(Buf, Input_Data::DEBT_LIT_L) != nullptr) {
					Debt::Read_Downward(Sheet, i + 2, j, Vector);
					return true;
				}
			}
			catch(System::Exception ^err) {
				Core::LastErr_Num = err->HResult;
				Core::Set_LastErr_Str(err->Message);
				return false;
			}
		}
	}
	Core::LastErr_Num = Core::SUPER_ERROR_CODE;
	Core::Set_LastErr_Str("Ќе удалось прочитать данные о долговых об€зательствах предпри€ти€.\n"
		+ "¬озможно, они не содержатс€ в указанном Excel листе.");
	return false;
}

My_Classes::Input_Data::Credit::Credit() {
	this->Name = "";
	this->Max_Sum = -1;
	this->Percentage = -1.0;
}
My_Classes::Input_Data::Credit::Credit(const string &Name, lf Percentage, int64 Max_Sum) {
	this->Name = Name;
	this->Percentage = Percentage;
	this->Max_Sum = Max_Sum;
}
void My_Classes::Input_Data::Credit::Read_Downward(ExcelFormat::BasicExcelWorksheet *Sheet, int Row, int Col, vector<Credit> &Vector) {
	wchar_t Name_Buf_Temp_L[Core::BUF_SIZE];
	char Name_Buf_Temp[Core::BUF_SIZE];
	string Name_Buf;
	double Percentage_Buf;
	int Sum_Buf;
	int Counter = 0;
	Vector.clear();
	for(int i = Row; i < Sheet->GetTotalRows(); i++) {
		try { 
			//Check that we can read current row and than read it
			if(Sheet->Cell(i, Col + 1)->Get(Sum_Buf) && Sheet->Cell(i, Col + 2)->Get(Percentage_Buf)) {
				if(Sheet->Cell(i, Col)->Get(Name_Buf_Temp_L)) {//Cell contains russian letters (But behaviour may vary sometimes (What???);D)
					Name_Buf = My_Classes::Core::Convert_WString_to_String(wstring(Name_Buf_Temp_L));
				}
				else {
					if(Sheet->Cell(i, Col)->Get(Name_Buf_Temp)) //Cell is not empty (Empty cell ExcelFormat won't read at all)
						Name_Buf = string(Name_Buf_Temp);
					else
						Name_Buf = "";

				}
				//Check if Cell(i, Col) consists only from spaces OR is empty
				if(Name_Buf.find_first_not_of(' ') == string::npos || Name_Buf.empty()) {
					Name_Buf = " редитное предложение #" + to_string(++Counter); //Changes the string
				}
				Percentage_Buf = Sheet->Cell(i, Col + 2)->GetDouble();
				Sum_Buf = Sheet->Cell(i, Col + 1)->GetInteger();
				Vector.push_back(My_Classes::Input_Data::Credit(Name_Buf, Percentage_Buf, Sum_Buf));
			}
			else {
				return; //Time to stop reading
			}
		}
		catch(System::Exception ^err) {
			My_Classes::Core::LastErr_Num = err->HResult;
			My_Classes::Core::Set_LastErr_Str(err->Message);
		}
	}
}
bool My_Classes::Input_Data::Credit::Read_From_Xls(ExcelFormat::BasicExcelWorksheet *Sheet, vector<My_Classes::Input_Data::Credit> &Vector) {
	for(int i = 0; i < Sheet->GetTotalRows(); i++) {
		for(int j = 0; j < Sheet->GetTotalCols(); j++) {
			try {
				wchar_t Buf[My_Classes::Core::BUF_SIZE];
				if(!Sheet->Cell(i, j)->Get(Buf))
					continue;
				if(wcsstr(Buf, Input_Data::CREDIT_LIT_L) != nullptr) {
					Credit::Read_Downward(Sheet, i + 2, j, Vector);
					return true;
				}
			}
			catch(System::Exception ^err) {
				Core::LastErr_Num = err->HResult;
				Core::Set_LastErr_Str(err->Message);
				return false;
			}
		}
	}
	My_Classes::Core::LastErr_Num = My_Classes::Core::SUPER_ERROR_CODE;
	Core::Set_LastErr_Str("Ќе удалось прочитать данные о кредитных предложени€х.\n"
		+ "¬озможно, они не содержатс€ в указанном Excel листе.");
	return false;
}

My_Classes::Input_Data::Time_Interval My_Classes::Input_Data::T = My_Classes::Input_Data::Time_Interval();
vector<My_Classes::Input_Data::Money_Income> My_Classes::Input_Data::Money_Incomes = vector<My_Classes::Input_Data::Money_Income>();
vector<My_Classes::Input_Data::Debt> My_Classes::Input_Data::Debts = vector<My_Classes::Input_Data::Debt>();
vector<My_Classes::Input_Data::Credit> My_Classes::Input_Data::Credits = vector<My_Classes::Input_Data::Credit>();

//Reads all necessary data from Excel sheet
//Affects T, Money_Incomes, Debts and Credits instances
//Returns if reading proceeded successfully
bool My_Classes::Input_Data::Read_Data(ExcelFormat::BasicExcelWorksheet *Sheet) {
	My_Classes::Input_Data::T = My_Classes::Input_Data::Time_Interval();
	My_Classes::Input_Data::Money_Incomes.clear();
	My_Classes::Input_Data::Debts.clear();
	My_Classes::Input_Data::Credits.clear();

	if(My_Classes::Input_Data::Time_Interval::Read_From_Xls(Sheet, My_Classes::Input_Data::T)
		&& My_Classes::Input_Data::Money_Income::Read_From_Xls(Sheet, My_Classes::Input_Data::Money_Incomes)
		&& My_Classes::Input_Data::Debt::Read_From_Xls(Sheet, My_Classes::Input_Data::Debts)
		&& My_Classes::Input_Data::Credit::Read_From_Xls(Sheet, My_Classes::Input_Data::Credits)) {
			//Debugger shows completely incorrect My_Classes::Input_Data::T
			//Uncomment following for pure checking My_Classes::Input_Data::T

			//System::Windows::Forms::MessageBox::Show("My_Classes::Input_Data::T.From\n"
			//	+ Core::Convert_tm_to_netString(&My_Classes::Input_Data::T.From, My_Classes::Core::DATE_AND_TIME), "Check");
			//System::Windows::Forms::MessageBox::Show("My_Classes::Input_Data::T.To\n"
			//	+ Core::Convert_tm_to_netString(&My_Classes::Input_Data::T.To, My_Classes::Core::DATE_AND_TIME), "Check");

			return true;
	}
	else
		return false;
}
