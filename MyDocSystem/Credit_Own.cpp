#include "Credit_Own.h"
#include "Input_Data.h"
#include "Core.h"

My_Classes::Credit_Own::Credit_Own() {
	this->Sum_Base = this->Sum_Percent = this->Next_Day_Percent = this->Today = this->Ind = -1;
}
My_Classes::Credit_Own::Credit_Own(uint Ind, int64 Sum_Base, time_t Today) {
	this->Ind = Ind;
	this->Sum_Base = Sum_Base;
	this->Sum_Percent = 0;
	this->Today = Today;
	this->Update_Next_Day_Percent();

}
int64 My_Classes::Credit_Own::Remains() {
	return this->Sum_Base + this->Sum_Percent;
}
bool My_Classes::Credit_Own::is_Completed() {
	return !(this->Sum_Base + this->Sum_Percent);
}
void My_Classes::Credit_Own::add_nDays(uint n) {
	this->Sum_Percent += this->Next_Day_Percent * n;
	this->Today += My_Classes::Core::SEC_IN_DAY * n;
}
void My_Classes::Credit_Own::Update_Next_Day_Percent() {
	this->Next_Day_Percent = My_Classes::Input_Data::Credits[this->Ind].Percentage / 100
		/ (My_Classes::Core::is_Leap_Year(gmtime(&this->Today)->tm_yday + 1900) ? 366 : 365)
		* this->Sum_Base;
}
int64 My_Classes::Credit_Own::Pay(int64 Sum) {
	if(this->Sum_Base > Sum) {
		this->Sum_Base -= Sum;
		this->Update_Next_Day_Percent();
		return 0;
	}
	Sum -= this->Sum_Base;
	this->Sum_Base = 0;
	if(this->Sum_Percent > Sum) {
		this->Sum_Percent -= Sum;
		this->Update_Next_Day_Percent();
		return 0;
	}
	Sum -= this->Sum_Percent;
	this->Sum_Percent = 0;
	this->Update_Next_Day_Percent();
	return Sum;
}
uint My_Classes::Credit_Own::Get_Ind() const {
	return this->Ind;
}
int64 My_Classes::Credit_Own::Get_Sum_Base() const {
	return this->Sum_Base;
}
int64 My_Classes::Credit_Own::Get_Sum_Percent() const {
	return this->Sum_Percent;
}
int64 My_Classes::Credit_Own::Get_Next_Day_Percent() const {
	return this->Next_Day_Percent;
}
time_t My_Classes::Credit_Own::Get_Today() const {
	return this->Today;
}
