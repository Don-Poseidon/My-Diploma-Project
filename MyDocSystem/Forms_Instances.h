#ifndef FORMS_INSTANCES_H
#define FORMS_INSTANCES_H

#include "frm_Console.h"
#include "frm_Main.h"
#include "frm_SelectSheet.h"

using namespace std;

namespace My_Classes {
	value class Forms_Instances {
	public:
		static MyDocSystem::frm_Console^ frm_Console_hwnd = nullptr;
		static MyDocSystem::frm_Main^ frm_Main_hwnd = nullptr;
		static MyDocSystem::frm_SelectSheet^ frm_SelectSheet_hwnd = nullptr;

		//Closes and reinitializes frm_Console_hwnd (and smth more in the future...) only!
		static void Reinit(); 
	};
}

#endif