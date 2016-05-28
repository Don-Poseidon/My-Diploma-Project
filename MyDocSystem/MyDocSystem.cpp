// MyDocSystem.cpp: главный файл проекта.
#include "frm_Main.h"

using namespace System;

namespace MyDocSystem {
    using namespace System;
    using namespace System::Windows::Forms;

    [STAThread]
    int main(array<System::String ^> ^args)
    {
        Application::EnableVisualStyles();
        Application::SetCompatibleTextRenderingDefault(false);
		//setlocale(LC_ALL, "Russian");
        Application::Run(gcnew frm_Main());
        return 0;
    }
}
