#include "Core.h"
#include "frm_SelectSheet.h"
#include "Input_Data.h"
#include "Balas.h"
#include "mfmc.h"
#include "Output_Data.h"
#include <ctime>
#include <algorithm>
#include <msclr\marshal_cppstd.h>
#include "Forms_Instances.h"

#define mp make_pair
typedef unsigned int uint;

//const int My_Classes::Core::SUPER_ERROR_CODE = 777;
int My_Classes::Core::LastErr_Num = 0;
const char* const My_Classes::Core::DATE_ONLY = "%d.%m.%Y";
const char* const My_Classes::Core::DATE_AND_TIME = "%d.%m.%Y %H:%M:%S";
set<int> My_Classes::Core::Unpaid_Debts = set<int>();
set<int> My_Classes::Core::Unpaid_CDebts = set<int>();
vector<char> My_Classes::Core::X = vector<char>();
vector<char> My_Classes::Core::X_Credit = vector<char>();
vector<lf> My_Classes::Core::C = vector<lf>();
int My_Classes::Core::Days = 0;
vector<My_Classes::Credit_Own> My_Classes::Core::Credits_Taken = vector<My_Classes::Credit_Own>();

string My_Classes::Core::LastErr_Str = "";
ExcelFormat::BasicExcel My_Classes::Core::Xls = ExcelFormat::BasicExcel();
ExcelFormat::BasicExcelWorksheet* My_Classes::Core::Sheet = nullptr;
bool My_Classes::Core::XlsFile_Opened = false;

//Closes opened Excel file
//Requires initialized My_Classes::Core::Xls
//Affects My_Classes::Core::Xls and My_Classes::Core::XlsFile_Opened
void My_Classes::Core::Close_Excel_File() {
	My_Classes::Core::Xls.Close();
	My_Classes::Core::XlsFile_Opened = false;
}

//Opens desired Excel file
//Affects My_Classes::Core::Xls and My_Classes::Core::Sheet
//Affetcs Cancel_Pressed referenced flag
//Returns if file is opened successfully AND if frm_SelectSheet::btn_Cancel is pressed
bool My_Classes::Core::Open_Excel_File(System::String ^Path, bool &Cancel_Pressed) {
	Cancel_Pressed = false; //Cancel was not pressed yet
	try {
		const wchar_t *Buf = My_Classes::Core::Convert_netString_to_wchar(Path);
		if(!Core::Xls.Load(Buf)) //Can not open file
			return false;
	}
	catch(System::Exception ^err) {
		Core::LastErr_Num = err->HResult;
		Core::Set_LastErr_Str(err->Message);
		return false;
	}
	int TotalWorkSheets = Core::Xls.GetTotalWorkSheets();
	if(TotalWorkSheets == 0) {
		Core::LastErr_Num = Core::SUPER_ERROR_CODE;
		Core::LastErr_Str = "Выбранная Excel книга не содержит ни одного листа.";
		Core::Close_Excel_File();
		return false;
	}
	else if(TotalWorkSheets > 1) {
		MyDocSystem::frm_SelectSheet ^frm_SelectSheet_Instance = gcnew MyDocSystem::frm_SelectSheet();
		frm_SelectSheet_Instance->Show_Advanced(Core::Xls);
		if(frm_SelectSheet_Instance->Selected_Sheet_Ind == -1) { //sheet was not selected
			Cancel_Pressed = true;
			Core::Close_Excel_File();
			return false;
		}
		Core::Sheet = Core::Xls.GetWorksheet(frm_SelectSheet_Instance->Selected_Sheet_Ind);
	}
	else { //TotalWorkSheets == 1
		Core::Sheet = Core::Xls.GetWorksheet(0);
	}
	return Core::XlsFile_Opened = true;
}

System::String^ My_Classes::Core::Get_LastErr_Str() {
	return Core::Convert_String_to_netString(My_Classes::Core::LastErr_Str);
}
void My_Classes::Core::Set_LastErr_Str(System::String ^Str) {
	My_Classes::Core::LastErr_Str = msclr::interop::marshal_as<string>(Str);
}
System::String^ My_Classes::Core::Convert_Chars_to_netString(char *Buf) {
	return msclr::interop::marshal_as<System::String^>(Buf);
}
System::String^ My_Classes::Core::Convert_String_to_netString(const string &Str) {
	return msclr::interop::marshal_as<System::String^>((char *)Str.data());
}
const wchar_t* My_Classes::Core::Convert_netString_to_wchar(System::String^ &Str) {
	msclr::interop::marshal_context^ context = gcnew msclr::interop::marshal_context();
	return context->marshal_as<const wchar_t*>(Str);
}
string  My_Classes::Core::Convert_WString_to_String(wstring &Str) {
	msclr::interop::marshal_context^ context = gcnew msclr::interop::marshal_context();
	System::String^ Buf = context->marshal_as<System::String^>(Str.data());
	return (string(context->marshal_as<const char*>(Buf)));
}

System::String^ My_Classes::Core::Convert_wchars_to_netString(wchar_t *Str) {
	msclr::interop::marshal_context^ context = gcnew msclr::interop::marshal_context();
	return context->marshal_as<System::String^>(Str);
}

//Opens desired file and fulfills all necessary variables
//If smth happened during file opening (NOT during reading data), file is already closed.
//If everything went smoothy, file remains opened => do not forget to close in the end (I haven't forgot -> see in the end ;))
//Returns if file opening proceeded successfully
bool My_Classes::Core::Read_Data(System::String^ Xls_Path) {
	My_Classes::Core::LastErr_Num = 0; //Clear Error state
	bool Cancel_Pressed = false;
	if(!My_Classes::Core::Open_Excel_File(Xls_Path, Cancel_Pressed)) { //File was not opened successfully
		if(Cancel_Pressed) //frm_SelectSheet::btn_Cancel was pressed => no any errors occured
			return false;
		System::String^ Message = System::String::Format("Не удалось открыть указанный файл\n{0}.\n"
			+ "Возможно, файл не существует или доступ к файлу ограничен.\n", Xls_Path);
		if(My_Classes::Core::LastErr_Num != 0)
			Message += System::String::Format("Номер ошибки: {0}\nОписание ошибки: {1}",
			My_Classes::Core::LastErr_Num, My_Classes::Core::Get_LastErr_Str());
		System::Windows::Forms::MessageBox::Show(Message, "Ошибка чтения файла",
			System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Exclamation);
		return false; //No need to read data or close file => already closed
	}
	if(!My_Classes::Input_Data::Read_Data(Core::Sheet)) { //File opened, but data was not read successfully
		System::String^ Message = System::String::Format("Не удалось прочитать входные данные.\nНомер ошибки: {0}\n"
			+ "Описание ошибки: {1}", My_Classes::Core::LastErr_Num, My_Classes::Core::Get_LastErr_Str());
		System::Windows::Forms::MessageBox::Show(Message, "Ошибка чтения входных данных",
			System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Exclamation);
		return false;
	}
	Core::Close_Excel_File(); //May comment this line to check all read data
	return true;
}

//tm comparator
bool mycmp(tm &a, tm &b) {
	time_t a_Buf = mktime(&a);
	time_t b_Buf = mktime(&b);
	return a_Buf < b_Buf;
}
//Test binary search implementation
/*
bool Pseudo_Balas(int Val) {//Returns false if Val > 4
	return Val <= 4;
}
int Binary_Search(int From, int To) {//Should find 4 in Pseudo_Balas (= the greatest Val that returns true)
	while(To - From > 1) {
		int Mid = (From + To) >> 1;
		if(Pseudo_Balas(Mid))
			From = Mid;
		else
			To = Mid;
	}
	if(Pseudo_Balas(To))
		return To;
	else
		return From;
}*/
//Real binary search implementation
//Affects vector<char> X and vector<lf> B
//Returns maximum number of debts which we still can pay
//If nothing can pay, returns 0
//Assign returned number to B[My_Classes::Input_Data::Debts.size()]
//and to B[My_Classes::Input_Data::Debts.size() + 1] before
//pure calling My_Classes::Balas::Count_Balas(...)
int Bin_Search_Payables(const vector<lf> &C, const vector<vector<lf> > &A, vector<lf> &B, vector<char> &X) {
	int From = 0;
	int To = My_Classes::Input_Data::Debts.size();

	while(To - From > 1) {
		int Mid = (From + To) >> 1;
		B[My_Classes::Input_Data::Debts.size()] = Mid;
		B[My_Classes::Input_Data::Debts.size() + 1] = -Mid;
		if(My_Classes::Balas::Count_Balas(C, A, B, X))
			From = Mid;
		else
			To = Mid;
	}
	B[My_Classes::Input_Data::Debts.size()] = To;
	B[My_Classes::Input_Data::Debts.size() + 1] = -To;
	if(My_Classes::Balas::Count_Balas(C, A, B, X))
		return To;
	else {
		B[My_Classes::Input_Data::Debts.size()] = From;
		B[My_Classes::Input_Data::Debts.size() + 1] = -From;
		My_Classes::Balas::Count_Balas(C, A, B, X);
		return From;
	}
}

//static bool Count_Balas(const vector<lf> &C, const vector<vector<lf> > &A, const vector<lf> &B, vector<char> &X);
//Implements Balas part
//Affects My_Classes::Core::Payed_Debts and My_Classes::Core::Unpaid_Debts
void Calc_Balas() {
	//Total DAYS in T
	const int DAYS = (mktime(&My_Classes::Input_Data::T.To) - mktime(&My_Classes::Input_Data::T.From))
		/ My_Classes::Core::SEC_IN_DAY + 1; //Amount of Days
	const int X_AMOUNT = DAYS * My_Classes::Input_Data::Debts.size(); //Amount of X
	vector<char> X = vector<char>(X_AMOUNT);
	vector<lf> C = vector<lf>(X_AMOUNT); //Initially we have not any penies, so C[i] == 1
	const int CONSTRAINTS = My_Classes::Input_Data::Debts.size() + 2
		+ My_Classes::Input_Data::Money_Incomes.size() + 1; //Amount of Constraints 
	vector<vector<lf> > A = vector<vector<lf> >(CONSTRAINTS, vector<lf>(X_AMOUNT));
	vector<lf> B = vector<lf>(CONSTRAINTS);

	//Initalize C vector
	for(uint i = 0; i < My_Classes::Input_Data::Debts.size(); i++)
		C[i * DAYS] = My_Classes::Input_Data::Debts[i].Sum;
	for(uint i = 0; i < My_Classes::Input_Data::Debts.size(); i++) {
		time_t Buf_Time = mktime(&My_Classes::Input_Data::T.From) + My_Classes::Core::SEC_IN_DAY;
		for(int j = 1; j < DAYS; j++) {
			Buf_Time += My_Classes::Core::SEC_IN_DAY;
			//Debt in penalty range and today is new month first day
			tm* Buf = gmtime(&Buf_Time);
			if(Buf_Time > mktime(&My_Classes::Input_Data::Debts[i].DeadLine) && gmtime(&Buf_Time)->tm_mday == 1)
				C[i * DAYS + j] = C[i * DAYS + j - 1] +
					My_Classes::Input_Data::Debts[i].Percentage / 100 * C[i * DAYS];
			else
				C[i * DAYS + j] = C[i * DAYS + j - 1];
		}
	}

	//Initialize A vector and B vectors
	//First contradiction
	for(uint i = 0; i < My_Classes::Input_Data::Debts.size(); i++) {
		for(uint j = i * DAYS; j < (i + 1) * DAYS; j++) {
			A[i][j] = -1;
		}
		B[i] = -1;
	}
	//Second contradiction
	int Row = My_Classes::Input_Data::Debts.size(); //Current Row in A vector
	for(int i = 0; i < X_AMOUNT; i++) {
		A[Row][i] = 1;
		A[Row + 1][i] = -1;
	}
	B[Row] = My_Classes::Input_Data::Debts.size();
	B[Row + 1] = -(int)My_Classes::Input_Data::Debts.size();
	Row += 2;
	//Third (and the last) contradiction
	vector<pair<time_t, int> > Money_Incomes_Sorted(My_Classes::Input_Data::Money_Incomes.size());
	for(uint i = 0; i < My_Classes::Input_Data::Debts.size(); i++) {
		Money_Incomes_Sorted[i] = mp(mktime(&My_Classes::Input_Data::Money_Incomes[i].Date)
			- mktime(&My_Classes::Input_Data::T.From), i);
	}
	sort(Money_Incomes_Sorted.begin(), Money_Incomes_Sorted.end());
	int64 Sum_Buf = 0;
	//int Cur_Day = 0; //Next Day to be considered
	for(uint it = 0; it < Money_Incomes_Sorted.size(); it++) {
		for(int Cur_Day = 0; Cur_Day < Money_Incomes_Sorted[it].first / My_Classes::Core::SEC_IN_DAY; Cur_Day++) {
			for(uint i = 0; i < My_Classes::Input_Data::Debts.size(); i++) {		
				A[Row][i * DAYS + Cur_Day] = -C[i * DAYS + Cur_Day];
			}
		}
		B[Row] = -Sum_Buf;
		Sum_Buf += My_Classes::Input_Data::Money_Incomes[Money_Incomes_Sorted[it].second].Sum;
		Row++;
	}
	
	for(int Cur_Day = 0; Cur_Day < DAYS; Cur_Day++) {
		for(uint i = 0; i < My_Classes::Input_Data::Debts.size(); i++) {
			A[Row][i * DAYS + Cur_Day] = -C[i * DAYS + Cur_Day];
		}
	}
	B[Row] = -Sum_Buf;

	//All vectors completed
	//Now it's time to find max Payables value and call My_Classes::Balas::Count_Balas(...)
	lf Start_Time = clock() / (lf)(CLOCKS_PER_SEC);
	//int Payables = 3;
#ifndef _DEBUG //Do not forget to delete this!!!!!!!!!!!!!!!!
	int Payables = Bin_Search_Payables(C, A, B, X);

#else
	int Payables = 2;
	X[0] = X[182] = 1;
#endif
	B[My_Classes::Input_Data::Debts.size()] = Payables;
	B[My_Classes::Input_Data::Debts.size() + 1] = -Payables;
	//Uncomment following for debugging Balas algo results
	//----------------------------------------------------
	//
	//bool b = Payables ? true : false;
	//string Debug_String = "";
	//Debug_String += "RunningTIME : =" + to_string(clock() / (lf)(CLOCKS_PER_SEC) - Start_Time) + " sec.\n";
	//Debug_String += "Payables = " + to_string(Payables) + "\n";
	//for(int i = 0; i < X_AMOUNT; i++) {
	//	if(X[i] == 1)
	//		Debug_String += "X[" + to_string(i) + "] = 1\n";
	//}
	//Debug_String += "Solution found = " + to_string(b);
	//System::Windows::Forms::MessageBox::Show(My_Classes::Core::Convert_String_to_netString(Debug_String));
	//
	//----------------------------------------------------

	My_Classes::Core::Unpaid_Debts.clear(); //------!!!Do not forget!!!------

	for(uint i = 0; i < My_Classes::Input_Data::Debts.size(); i++) {
		time_t Buf = mktime(&My_Classes::Input_Data::T.From) - timezone - My_Classes::Core::SEC_IN_DAY;
		bool Broken = false;
		for(int j = 0; j < DAYS; j++) {
			Buf += My_Classes::Core::SEC_IN_DAY;
			if(X[i * DAYS + j] == 1) {
				Broken = true;
				break;
			}		
		}
		if(!Broken)
			My_Classes::Core::Unpaid_Debts.insert(i);
	}
	My_Classes::Core::X = X;
	My_Classes::Core::C = C;
	My_Classes::Core::Days = DAYS;
}
//Calculates all necessary data completely
void My_Classes::Core::Calc_Data() {
	Calc_Balas();
	My_Classes::mfmc::Count_Flow(X, My_Classes::Core::Days, My_Classes::Core::C);
}
//Outputs all data completely 
void My_Classes::Core::Output_Data() {
	My_Classes::Output_Data::Output();
}


//Start and end point for reading, calculating and output all data.
//Runs when btn_Start in MainForm is clicked
void My_Classes::Core::Do_Stuff(System::String^ Xls_Path) {
	if(!My_Classes::Core::Read_Data(Xls_Path))
		return;
	My_Classes::Core::Calc_Data();
	My_Classes::Core::Output_Data();
}
tm My_Classes::Core::Convert_XlsInt_to_Tm(int Time) {
	time_t rawtime = ((int64)Time - 25569) * My_Classes::Core::SEC_IN_DAY;
	tm* Ret = gmtime(&rawtime);
	mktime(Ret);
	return *Ret;
}
System::String^ My_Classes::Core::Convert_tm_to_netString(tm* const Val, const char* const Format) {
	char buffer [Core::BUF_SIZE];
	//strftime (buffer, Core::BUF_SIZE,"%d.%m.%Y %H:%M:%S", Val);
	strftime (buffer, Core::BUF_SIZE, Format, Val);
	return Core::Convert_Chars_to_netString(buffer);
}
string My_Classes::Core::Convert_tm_to_String(tm* const Val, const char* const Format) {
	char buffer [Core::BUF_SIZE];
	//strftime (buffer, Core::BUF_SIZE,"%d.%m.%Y %H:%M:%S", Val);
	strftime (buffer, Core::BUF_SIZE, Format, Val);
	return string(buffer);
}
string My_Classes::Core::Convert_time_t_to_String(time_t Val, const char* const Format) {
	tm* Buf = gmtime(&Val);
	return My_Classes::Core::Convert_tm_to_String(Buf, Format);
}
bool My_Classes::Core::is_Leap_Year(int Year) {
	return (Year % 4 == 0 && Year % 100 != 0) || (Year % 400 == 0);
}
int My_Classes::Core::Days_in_Month(int Month, int Year) { 
	if(Month == 4 || Month == 6 || Month == 9 || Month == 11)  
	  return 30;  
	else if(Month == 2)
		if(My_Classes::Core::is_Leap_Year(Year))  
			return 29;  
		else  
			return 28;    
	else  
	  return 31;  
}
void My_Classes::Core::AppendText(bool is_Bold, bool is_Italic, const System::Drawing::Color^ Color, const string &Text) {
	System::String^ net_Text = My_Classes::Core::Convert_String_to_netString(Text);
	//My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console
	int start = My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->TextLength;
	My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->AppendText(net_Text);
	int end = My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->TextLength;
	// Textbox may transform chars, so (end-start) != text.Length
	My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->Select(start, end - start);
	System::Drawing::FontStyle Font_Buf = System::Drawing::FontStyle::Regular;
	if(is_Bold)
		Font_Buf = Font_Buf + System::Drawing::FontStyle::Bold;
	if(is_Italic)
		Font_Buf = Font_Buf + System::Drawing::FontStyle::Italic;
	My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->SelectionColor = *Color;
	My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->SelectionFont = gcnew System::Drawing::Font(My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->SelectionFont, Font_Buf);

	My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->SelectionLength = 0; // clear
}
void My_Classes::Core::AppendText(bool is_Bold, bool is_Italic, const System::Drawing::Color^ Color, System::String^ Text) {
	int start = My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->TextLength;
	My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->AppendText(Text);
	int end = My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->TextLength;
	// Textbox may transform chars, so (end-start) != text.Length
	My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->Select(start, end - start);
	System::Drawing::FontStyle Font_Buf = System::Drawing::FontStyle::Regular;
	if(is_Bold)
		Font_Buf = Font_Buf + System::Drawing::FontStyle::Bold;
	if(is_Italic)
		Font_Buf = Font_Buf + System::Drawing::FontStyle::Italic;
	My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->SelectionColor = *Color;
	My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->SelectionFont = gcnew System::Drawing::Font(My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->SelectionFont, Font_Buf);

	My_Classes::Forms_Instances::frm_Console_hwnd->rtxt_Console->SelectionLength = 0; // clear
}
void My_Classes::Core::AppendText_Message(const string &Text) {
	My_Classes::Core::AppendText(false, false, My_Classes::Output_Data_Ref::MESSAGE_COLOR, Text);
}
void My_Classes::Core::AppendText_Header(const string &Text) {
	My_Classes::Core::AppendText(true, true, My_Classes::Output_Data_Ref::HEADER_COLOR, Text);
}
void My_Classes::Core::AppendText_SubHeader(const string &Text) {
	My_Classes::Core::AppendText(false, true, My_Classes::Output_Data_Ref::SUBHEADER_COLOR, Text);
}
void My_Classes::Core::AppendText_Computed(const string &Text) {
	My_Classes::Core::AppendText(false, false, My_Classes::Output_Data_Ref::COMPUTED_COLOR, Text);
}
void My_Classes::Core::AppendText_Warning(const string &Text) {
	My_Classes::Core::AppendText(false, true, My_Classes::Output_Data_Ref::WARNING_COLOR, Text);
}