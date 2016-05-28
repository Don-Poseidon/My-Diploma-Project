#include "Output_Data.h"
#include "Core.h"
#include "Input_Data.h"
#include "Forms_Instances.h"
#include <algorithm>

vector<int64> My_Classes::Output_Data::Money = vector<int64>();
vector<int64> My_Classes::Output_Data::Debts_Pays = vector<int64>();
vector<int64> My_Classes::Output_Data::CDebts_Pays = vector<int64>();
vector<int64> My_Classes::Output_Data::Money_Income = vector<int64>();
vector<int64> My_Classes::Output_Data::Credits_Income = vector<int64>();
vector<vector<int64> > My_Classes::Output_Data::Credits_Pays = vector<vector<int64> >();
vector<int64> My_Classes::Output_Data::Credits_Complete_Dates = vector<int64>();
set<int> My_Classes::Output_Data::Unpaid_Credits = set<int>();
//Decending of Date
bool mycmp_date(const My_Classes::Credit_Own &a, const My_Classes::Credit_Own &b) {
	return a.Get_Today() < b.Get_Today();
}
//Ascending of Next_Day_Percent or Sum_Percent if equal
bool mycmp_percent(const My_Classes::Credit_Own &a, const My_Classes::Credit_Own &b) {
	int64 ap = a.Get_Next_Day_Percent();
	int64 bp = b.Get_Next_Day_Percent();
	if(ap == bp)
		return a.Get_Sum_Percent() > b.Get_Sum_Percent();
	return ap > bp;
}

void My_Classes::Output_Data::Init() {
	My_Classes::Output_Data::Money = vector<int64>(My_Classes::Core::Days, 0);
	My_Classes::Output_Data::Debts_Pays = vector<int64>(My_Classes::Core::Days, 0);
	My_Classes::Output_Data::CDebts_Pays = vector<int64>(My_Classes::Core::Days, 0);
	My_Classes::Output_Data::Money_Income = vector<int64>(My_Classes::Core::Days, 0);
	My_Classes::Output_Data::Credits_Income = vector<int64>(My_Classes::Core::Days, 0);
	My_Classes::Output_Data::Credits_Pays = vector<vector<int64> >
	//	(My_Classes::Input_Data::Credits.size(), vector<int64>(My_Classes::Core::Days, 0));
		(My_Classes::Core::Days, vector<int64>(My_Classes::Input_Data::Credits.size(), 0));
	vector<My_Classes::Credit_Own> Credits_Taken_Copy = My_Classes::Core::Credits_Taken;

	int Debt_Pay_Last_Day = 0;
	time_t Cur_Day = mktime(&My_Classes::Input_Data::T.From) - timezone;
	for(int i = 0; i < My_Classes::Core::Days; i++) {
		//Debts
		for(uint j = 0; j < My_Classes::Input_Data::Debts.size(); j++) {
			int Ind = j * My_Classes::Core::Days + i;
			My_Classes::Output_Data::Debts_Pays[i] += My_Classes::Core::X[Ind] * My_Classes::Core::C[Ind];
			My_Classes::Output_Data::CDebts_Pays[i] += My_Classes::Core::X_Credit[Ind] * My_Classes::Core::C[Ind];
			if(My_Classes::Core::X[Ind] || My_Classes::Core::X_Credit[Ind]) //Paid something
				Debt_Pay_Last_Day = i;
		}
		//Money income
		for(uint j = 0; j < My_Classes::Input_Data::Money_Incomes.size(); j++)
			if(mktime(&My_Classes::Input_Data::Money_Incomes[j].Date) - timezone == Cur_Day)
				My_Classes::Output_Data::Money_Income[i] += My_Classes::Input_Data::Money_Incomes[j].Sum;
		//Credit money income
		for(uint j = 0; j < Credits_Taken_Copy.size(); j++)
			if(Credits_Taken_Copy[j].Get_Today() == Cur_Day)
				My_Classes::Output_Data::Credits_Income[i] += Credits_Taken_Copy[j].Get_Sum_Base();
		//Money[i] count
		My_Classes::Output_Data::Money[i] = -My_Classes::Output_Data::Debts_Pays[i] - My_Classes::Output_Data::CDebts_Pays[i]
			+ My_Classes::Output_Data::Money_Income[i] + My_Classes::Output_Data::Credits_Income[i];
		if(i) //Not the first day
			My_Classes::Output_Data::Money[i] += My_Classes::Output_Data::Money[i - 1];
		Cur_Day += My_Classes::Core::SEC_IN_DAY;
	}
	//Time to take credits and count percents every single day
	sort(Credits_Taken_Copy.begin(), Credits_Taken_Copy.end(), mycmp_date);
	Cur_Day = mktime(&My_Classes::Input_Data::T.From) - timezone;
	uint Credits_Amount = 0; //Amount of credits taken (up to Cur_Day)
	for(int i = 0; i < Debt_Pay_Last_Day; i++) {
		while(Credits_Amount < Credits_Taken_Copy.size())
			if(Credits_Taken_Copy[Credits_Amount].Get_Today() == Cur_Day) //Time to take one more credit
				Credits_Amount++;
			else
				break;
		for(uint j = 0; j < Credits_Amount; j++)
			Credits_Taken_Copy[j].add_nDays(1);
	}
	Credits_Complete_Dates = vector<int64>(My_Classes::Input_Data::Credits.size());
	for(int i = Debt_Pay_Last_Day; i < My_Classes::Core::Days; i++) {
		//Debugger misinterprets My_Classes::Output_Data::Money; Uncomment following for pure check
		//System::Windows::Forms::MessageBox::Show(My_Classes::Core::Convert_String_to_netString(to_string(My_Classes::Output_Data::Money[i])));
		sort(Credits_Taken_Copy.begin(), Credits_Taken_Copy.end(), mycmp_percent);
		while(My_Classes::Output_Data::Money[i] != 0 && !Credits_Taken_Copy[0].is_Completed()) {
			int64 To_pay = min(My_Classes::Output_Data::DESCRETE_PAY, My_Classes::Output_Data::Money[i]);
			int64 Paid = To_pay - Credits_Taken_Copy[0].Pay(To_pay);
			if(Paid != To_pay) //Credit completed
				My_Classes::Output_Data::Credits_Complete_Dates[Credits_Taken_Copy[0].Get_Ind()] = 
					mktime(&My_Classes::Input_Data::T.From) - timezone + i * My_Classes::Core::SEC_IN_DAY;
			for(int j = i; j < My_Classes::Core::Days; j++)
				My_Classes::Output_Data::Money[j] -= Paid;
			//System::Windows::Forms::MessageBox::Show(My_Classes::Core::Convert_String_to_netString(to_string(My_Classes::Output_Data::Money[i])));
			//My_Classes::Output_Data::Credits_Pays[Credits_Taken_Copy[0].Get_Ind()][i] += Paid;
			My_Classes::Output_Data::Credits_Pays[i][Credits_Taken_Copy[0].Get_Ind()] += Paid;
			sort(Credits_Taken_Copy.begin(), Credits_Taken_Copy.end(), mycmp_percent);
		}
		for(uint j = 0; j < Credits_Taken_Copy.size(); j++)
				Credits_Taken_Copy[j].add_nDays(1);
	}
	My_Classes::Output_Data::Unpaid_Credits.clear(); //-----!!!Do not forget!!!------
	for(uint i = 0; i < Credits_Taken_Copy.size(); i++) {
		if(!Credits_Taken_Copy[i].is_Completed())
			My_Classes::Output_Data::Unpaid_Credits.insert(Credits_Taken_Copy[i].Get_Ind());
	}

}
void My_Classes::Output_Data::Output() {
	My_Classes::Output_Data::Init();
	
	//All output stuff goes right below
	My_Classes::Forms_Instances::Reinit();
	//----------------1st Part -> Debts payed with own money--------------------
	My_Classes::Core::AppendText_Header("Выплата долговых обязательств за счет собственных средств:\n");
	for(uint i = 0; i < My_Classes::Input_Data::Debts.size(); i++) {
		for(uint j = i * My_Classes::Core::Days; j < (i + 1) * My_Classes::Core::Days; j++) {
			if(My_Classes::Core::X[j]) { //Have payed -> need message!
				My_Classes::Core::AppendText_SubHeader("Долговое обязательство \"");
				My_Classes::Core::AppendText_Computed(My_Classes::Input_Data::Debts[i].Name);
				My_Classes::Core::AppendText_SubHeader("\":\n");
				My_Classes::Core::AppendText_Message("Выплачено ");
				time_t Time_Buf = mktime(&My_Classes::Input_Data::T.From) - timezone + j % My_Classes::Core::Days * My_Classes::Core::SEC_IN_DAY;
				My_Classes::Core::AppendText_Computed(
					My_Classes::Core::Convert_tm_to_String(gmtime(&Time_Buf), My_Classes::Core::DATE_ONLY));
				My_Classes::Core::AppendText_Message(" на сумму ");
				My_Classes::Core::AppendText_Computed(to_string((int64)My_Classes::Core::C[j]));
				My_Classes::Core::AppendText_Message(" руб.\n(В т.ч. Штраф = ");
				My_Classes::Core::AppendText_Computed(to_string((int64)My_Classes::Core::C[j] - My_Classes::Input_Data::Debts[i].Sum) + ") ");
				My_Classes::Core::AppendText_Message(" руб.\n");
			}
		}
	}
	if(My_Classes::Core::Unpaid_Debts.empty()) { //We have enough own money to pay all Debts
		My_Classes::Core::AppendText_Warning("Собственных средств хватило для уплаты всех без исключения долговых обязательств.\n");
		My_Classes::Core::AppendText_Warning("Взятие кредитов не требуется...\n");
		return; //No more output is neccessary
	}
	//----------------2nd Part -> Debts payed with Credit money--------------------
	My_Classes::Core::AppendText_Header("\nВыплата долговых обязательств за счет заемных средств:\n");
	for(uint i = 0; i < My_Classes::Input_Data::Debts.size(); i++) {
		for(uint j = i * My_Classes::Core::Days; j < (i + 1) * My_Classes::Core::Days; j++) {
			if(My_Classes::Core::X_Credit[j]) { //Have payed -> need message!
				My_Classes::Core::AppendText_SubHeader("Долговое обязательство \"");
				My_Classes::Core::AppendText_Computed(My_Classes::Input_Data::Debts[i].Name);
				My_Classes::Core::AppendText_SubHeader("\":\n");
				My_Classes::Core::AppendText_Message("Выплачено ");
				time_t Time_Buf = mktime(&My_Classes::Input_Data::T.From) - timezone + j % My_Classes::Core::Days * My_Classes::Core::SEC_IN_DAY;
				My_Classes::Core::AppendText_Computed(
					My_Classes::Core::Convert_tm_to_String(gmtime(&Time_Buf), My_Classes::Core::DATE_ONLY));
				My_Classes::Core::AppendText_Message(" на сумму ");
				My_Classes::Core::AppendText_Computed(to_string((int64)My_Classes::Core::C[j]));
				My_Classes::Core::AppendText_Message(" руб.\n(В т.ч. Штраф = ");
				My_Classes::Core::AppendText_Computed(to_string((int64)My_Classes::Core::C[j] - My_Classes::Input_Data::Debts[i].Sum) + ")\n");
			}
		}
	}
	//Check if we have some unpayed Debts
	if(!My_Classes::Core::Unpaid_CDebts.empty()) {
		My_Classes::Core::AppendText_Warning("Имеются не оплаченные долговые обязательства (для них кредиты взяты не были):");
		for(auto it = My_Classes::Core::Unpaid_CDebts.begin(); it != My_Classes::Core::Unpaid_CDebts.end(); it++) {
			My_Classes::Core::AppendText_SubHeader("\nДолговое обязательство \"");
			My_Classes::Core::AppendText_Computed(My_Classes::Input_Data::Debts[*it].Name);
			My_Classes::Core::AppendText_Message("\" на сумму ");
			My_Classes::Core::AppendText_Computed(to_string(My_Classes::Input_Data::Debts[*it].Sum));
		}

	}
	//----------------3rd Part -> Credit taking--------------------
	My_Classes::Core::AppendText_Header("\nВзятие кредитов:");
	for(uint i = 0; i < My_Classes::Core::Credits_Taken.size(); i++) {
		My_Classes::Core::AppendText_SubHeader("\nКредитное предложение \"");
		My_Classes::Core::AppendText_Computed(My_Classes::Input_Data::Credits
			[My_Classes::Core::Credits_Taken[i].Get_Ind()].Name);
		My_Classes::Core::AppendText_SubHeader("\":\n");
		My_Classes::Core::AppendText_Message("Сумма заема составляет ");
		My_Classes::Core::AppendText_Computed(to_string(My_Classes::Core::Credits_Taken[i].Get_Sum_Base()));
		My_Classes::Core::AppendText_Message(" руб.\nДата заема: ");
		My_Classes::Core::AppendText_Computed(My_Classes::Core::Convert_time_t_to_String
			(My_Classes::Core::Credits_Taken[i].Get_Today(), My_Classes::Core::DATE_ONLY));
	}
	//----------------4th Part -> Credit payments------------------------
	My_Classes::Core::AppendText_Header("\n\nПогашение кредитов:");
	for(int i = 0; i < My_Classes::Core::Days; i++) {
		for(uint j = 0; j < My_Classes::Input_Data::Credits.size(); j++) {
			if(My_Classes::Output_Data::Credits_Pays[i][j] != 0) { //Paid some credit today
				My_Classes::Core::AppendText_SubHeader("\nКредитное предложение \"");
				My_Classes::Core::AppendText_Computed(My_Classes::Input_Data::Credits[j].Name);
				My_Classes::Core::AppendText_SubHeader("\":\n");
				My_Classes::Core::AppendText_Message("Сумма к погашению: ");
				My_Classes::Core::AppendText_Computed(to_string(My_Classes::Output_Data::Credits_Pays[i][j]) + "\n");
				My_Classes::Core::AppendText_Message("Дата погашения: ");
				time_t Buf = mktime(&My_Classes::Input_Data::T.From) - timezone	+ i * My_Classes::Core::SEC_IN_DAY;
				My_Classes::Core::AppendText_Computed(My_Classes::Core::Convert_time_t_to_String(Buf, My_Classes::Core::DATE_ONLY));
				//If today is Credit completion day
				if(Buf == My_Classes::Output_Data::Credits_Complete_Dates[j]) {
					My_Classes::Core::AppendText_Warning("\nКредитное предложение \"");
					My_Classes::Core::AppendText_Computed(My_Classes::Input_Data::Credits[j].Name);
					My_Classes::Core::AppendText_Warning("\": погашено полностью!");
				}
			}
		}
	}
	//Now it's time to output unpayed Credits (if we have some)
	if(!My_Classes::Output_Data::Unpaid_Credits.empty()) {
		My_Classes::Core::AppendText_Warning("\nИмеются не оплаченные кредиты:\n");
		for(auto it = My_Classes::Output_Data::Unpaid_Credits.begin(); it != My_Classes::Output_Data::Unpaid_Credits.end(); it++) {
			My_Classes::Core::AppendText_SubHeader("Кредитное предложение \"");
			My_Classes::Core::AppendText_Computed(My_Classes::Input_Data::Credits
				[My_Classes::Core::Credits_Taken[*it].Get_Ind()].Name);
			My_Classes::Core::AppendText_SubHeader(" \":\n");
			My_Classes::Core::AppendText_Message("Сумма заема составляет ");
			My_Classes::Core::AppendText_Computed(to_string(My_Classes::Core::Credits_Taken[*it].Get_Sum_Base()));
		}
	}
	//----------------5th (and the last) Part -> Summary------------------------
	My_Classes::Core::AppendText_Warning("\n\nМоделирование завершено. Ниже приведены обобщенные результаты:\n");
	My_Classes::Core::AppendText_SubHeader("Остаток собственных средств по окончании рассматриваемого периода");
	My_Classes::Core::AppendText_Computed
		(" (на " + My_Classes::Core::Convert_tm_to_String(&My_Classes::Input_Data::T.To, My_Classes::Core::DATE_ONLY) + ") ");
	My_Classes::Core::AppendText_SubHeader(" составляет ");
	My_Classes::Core::AppendText_Computed(to_string(My_Classes::Output_Data::Money[My_Classes::Core::Days - 1]) + " руб.");
	My_Classes::Core::AppendText_SubHeader("\nВсего потрачено средств на погашение долговых обязательств: ");
	int64 Sum_Buf = 0;
	for(uint i = 0; i < My_Classes::Output_Data::Debts_Pays.size(); i++)
		Sum_Buf += My_Classes::Output_Data::Debts_Pays[i] + My_Classes::Output_Data::CDebts_Pays[i];
	My_Classes::Core::AppendText_Computed(to_string(Sum_Buf) + " руб.");
	My_Classes::Core::AppendText_SubHeader("\nВсего потрачено средств на погашение кредитов: ");
	Sum_Buf = 0;
	for(int i = 0; i < My_Classes::Core::Days; i++)
		for(uint j = 0; j < My_Classes::Input_Data::Credits.size(); j++)
			Sum_Buf += My_Classes::Output_Data::Credits_Pays[i][j];
	My_Classes::Core::AppendText_Computed(to_string(Sum_Buf) + " руб.");
	My_Classes::Core::AppendText_SubHeader("\nВсего получено собственных средств: ");
	Sum_Buf = 0;
	for(uint i = 0; i < My_Classes::Input_Data::Money_Incomes.size(); i++)
		Sum_Buf += My_Classes::Input_Data::Money_Incomes[i].Sum;
	My_Classes::Core::AppendText_Computed(to_string(Sum_Buf) + " руб.");
	My_Classes::Core::AppendText_SubHeader("\nВсего получено заемных средств: ");
	Sum_Buf = 0;
	for(uint i = 0; i < My_Classes::Core::Credits_Taken.size(); i++)
		Sum_Buf += My_Classes::Core::Credits_Taken[i].Get_Sum_Base();
	My_Classes::Core::AppendText_Computed(to_string(Sum_Buf) + " руб.");
	My_Classes::Forms_Instances::frm_Console_hwnd->Show();
	//----------That's all!----------------
	//----------Thank you for attention!-----------
}
