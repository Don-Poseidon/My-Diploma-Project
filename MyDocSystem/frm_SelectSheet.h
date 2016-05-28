#ifndef FRM_SELECTSHEET_H
#define FRM_SELECTSHEET_H

#include "ExcelFormat Files\BasicExcel.hpp"
#include "ExcelFormat Files\ExcelFormat.h"
#include "Core.h"

using namespace std;

#pragma once


/*namespace My_Classes {
	class Core {
	public:
		static System::String^ Convert_Chars_to_netString(char *Buf);
	};
}*/

namespace MyDocSystem {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для frm_SelectSheet
	/// </summary>
	public ref class frm_SelectSheet : public System::Windows::Forms::Form
	{
	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~frm_SelectSheet()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ListBox^  lst_Sheets;
	private: System::Windows::Forms::Button^  btn_Ok;
	private: System::Windows::Forms::Button^  btn_Cancel;

	private:
		/// <summary>
		/// Требуется переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Обязательный метод для поддержки конструктора - не изменяйте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frm_SelectSheet::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->lst_Sheets = (gcnew System::Windows::Forms::ListBox());
			this->btn_Ok = (gcnew System::Windows::Forms::Button());
			this->btn_Cancel = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(21, 21);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(274, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Выбранный Excel файл содержит несколько листов.";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(21, 51);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(174, 13);
			this->label2->TabIndex = 0;
			this->label2->Text = L"Пожалуйта, выберите один лист:";
			// 
			// lst_Sheets
			// 
			this->lst_Sheets->FormattingEnabled = true;
			this->lst_Sheets->Location = System::Drawing::Point(24, 92);
			this->lst_Sheets->Name = L"lst_Sheets";
			this->lst_Sheets->Size = System::Drawing::Size(349, 199);
			this->lst_Sheets->TabIndex = 1;
			this->lst_Sheets->SelectedIndexChanged += gcnew System::EventHandler(this, &frm_SelectSheet::lst_Sheets_SelectedIndexChanged);
			this->lst_Sheets->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &frm_SelectSheet::lst_Sheets_MouseDoubleClick);
			// 
			// btn_Ok
			// 
			this->btn_Ok->Enabled = false;
			this->btn_Ok->Location = System::Drawing::Point(312, 21);
			this->btn_Ok->Name = L"btn_Ok";
			this->btn_Ok->Size = System::Drawing::Size(61, 21);
			this->btn_Ok->TabIndex = 2;
			this->btn_Ok->Text = L"OK";
			this->btn_Ok->UseVisualStyleBackColor = true;
			this->btn_Ok->Click += gcnew System::EventHandler(this, &frm_SelectSheet::btn_Ok_Click);
			// 
			// btn_Cancel
			// 
			this->btn_Cancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btn_Cancel->Location = System::Drawing::Point(312, 51);
			this->btn_Cancel->Name = L"btn_Cancel";
			this->btn_Cancel->Size = System::Drawing::Size(61, 21);
			this->btn_Cancel->TabIndex = 2;
			this->btn_Cancel->Text = L"Отмена";
			this->btn_Cancel->UseVisualStyleBackColor = true;
			this->btn_Cancel->Click += gcnew System::EventHandler(this, &frm_SelectSheet::btn_Cancel_Click);
			// 
			// frm_SelectSheet
			// 
			this->AcceptButton = this->btn_Ok;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->btn_Cancel;
			this->ClientSize = System::Drawing::Size(397, 300);
			this->Controls->Add(this->btn_Cancel);
			this->Controls->Add(this->btn_Ok);
			this->Controls->Add(this->lst_Sheets);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"frm_SelectSheet";
			this->ShowInTaskbar = false;
			this->Text = L"Выбор листа";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public:
		frm_SelectSheet(void) {
			InitializeComponent();
			this->Selected_Sheet_Ind = -1;
		}

		int Selected_Sheet_Ind; //equals -1 if btn_Ok was not pressed

		void Show_Advanced(ExcelFormat::BasicExcel &Xls) {
			this->lst_Sheets->Items->Clear();
			for(int i = 0; i < Xls.GetTotalWorkSheets(); i++) {
				ExcelFormat::BasicExcelWorksheet* sheet = Xls.GetWorksheet(i);
				//this->lst_Sheets->Items->Add(My_Classes::Core::Convert_Chars_to_netString(sheet->GetAnsiSheetName()));
				wchar_t Buf[My_Classes::Core::BUF_SIZE];
				if(sheet->GetSheetName(Buf) == false) { //sheet name consists only from english symbols
					char Buf[My_Classes::Core::BUF_SIZE];
					sheet->GetSheetName(Buf);
					this->lst_Sheets->Items->Add(My_Classes::Core::Convert_Chars_to_netString(Buf));
				}
				else {
					this->lst_Sheets->Items->Add(My_Classes::Core::Convert_wchars_to_netString(Buf));
				}

			}
			this->ShowDialog();
		}


	private:
		//Affects this->Selected_Sheet_Ind and closes the form
		void Save_and_Close() {
			this->Selected_Sheet_Ind = this->lst_Sheets->SelectedIndex;
			this->Close();
		}

		System::Void btn_Cancel_Click(System::Object^  sender, System::EventArgs^  e) {
			this->Close();
		}
		System::Void btn_Ok_Click(System::Object^  sender, System::EventArgs^  e) {
			this->Save_and_Close();
		}
		System::Void lst_Sheets_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			this->btn_Ok->Enabled = true;
		}
		System::Void lst_Sheets_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			this->Save_and_Close();
		}
};
}

#endif