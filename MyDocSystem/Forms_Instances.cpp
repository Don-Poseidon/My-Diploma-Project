#include "Forms_Instances.h"

void My_Classes::Forms_Instances::Reinit() {
	if(My_Classes::Forms_Instances::frm_Console_hwnd == nullptr
		|| My_Classes::Forms_Instances::frm_Console_hwnd->is_Closed)
		My_Classes::Forms_Instances::frm_Console_hwnd = gcnew MyDocSystem::frm_Console();
	else {
		My_Classes::Forms_Instances::frm_Console_hwnd->Close();
		My_Classes::Forms_Instances::frm_Console_hwnd = gcnew MyDocSystem::frm_Console();
	}
}
