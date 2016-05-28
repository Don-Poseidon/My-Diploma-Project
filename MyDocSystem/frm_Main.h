#ifndef FRM_MAIN_H
#define FRM_MAIN_H

#include "Core.h"

#pragma once

namespace MyDocSystem {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для frm_Main
	/// </summary>
	public ref class frm_Main : public System::Windows::Forms::Form
	{
	public:
		frm_Main(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~frm_Main()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	public: System::Windows::Forms::TextBox^  txt_FilePath;
	private: 

	private: System::Windows::Forms::Button^  btn_ChoosePath;
	private: System::Windows::Forms::OpenFileDialog^  dlg_OpenFile;
	private: System::Windows::Forms::Button^  btn_Start;
	private: System::Windows::Forms::Button^  btn_Recover;

	private: System::Windows::Forms::SaveFileDialog^  dlg_SaveFile;
	private: System::Windows::Forms::ToolTip^  tt_Help;
	private: System::ComponentModel::IContainer^  components;







	protected: 



	protected: 

	protected: 

	private:
		/// <summary>
		/// Требуется переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Обязательный метод для поддержки конструктора - не изменяйте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frm_Main::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txt_FilePath = (gcnew System::Windows::Forms::TextBox());
			this->btn_ChoosePath = (gcnew System::Windows::Forms::Button());
			this->dlg_OpenFile = (gcnew System::Windows::Forms::OpenFileDialog());
			this->btn_Start = (gcnew System::Windows::Forms::Button());
			this->btn_Recover = (gcnew System::Windows::Forms::Button());
			this->dlg_SaveFile = (gcnew System::Windows::Forms::SaveFileDialog());
			this->tt_Help = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(162, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Выберите xls файл с данными:";
			// 
			// txt_FilePath
			// 
			this->txt_FilePath->Location = System::Drawing::Point(15, 36);
			this->txt_FilePath->Name = L"txt_FilePath";
			this->txt_FilePath->Size = System::Drawing::Size(308, 20);
			this->txt_FilePath->TabIndex = 1;
			// 
			// btn_ChoosePath
			// 
			this->btn_ChoosePath->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btn_ChoosePath.BackgroundImage")));
			this->btn_ChoosePath->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->btn_ChoosePath->Location = System::Drawing::Point(339, 30);
			this->btn_ChoosePath->Name = L"btn_ChoosePath";
			this->btn_ChoosePath->Size = System::Drawing::Size(30, 30);
			this->btn_ChoosePath->TabIndex = 2;
			this->btn_ChoosePath->UseVisualStyleBackColor = true;
			this->btn_ChoosePath->Click += gcnew System::EventHandler(this, &frm_Main::btn_ChoosePath_Click);
			// 
			// dlg_OpenFile
			// 
			this->dlg_OpenFile->Filter = L"Файл Excel 2003|*.xls|Все файлы|*.*";
			// 
			// btn_Start
			// 
			this->btn_Start->Location = System::Drawing::Point(160, 115);
			this->btn_Start->Name = L"btn_Start";
			this->btn_Start->Size = System::Drawing::Size(99, 42);
			this->btn_Start->TabIndex = 3;
			this->btn_Start->Text = L"Старт!";
			this->btn_Start->UseVisualStyleBackColor = true;
			this->btn_Start->Click += gcnew System::EventHandler(this, &frm_Main::btn_Start_Click);
			// 
			// btn_Recover
			// 
			this->btn_Recover->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btn_Recover.BackgroundImage")));
			this->btn_Recover->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->btn_Recover->Location = System::Drawing::Point(377, 30);
			this->btn_Recover->Name = L"btn_Recover";
			this->btn_Recover->Size = System::Drawing::Size(30, 30);
			this->btn_Recover->TabIndex = 2;
			this->btn_Recover->UseVisualStyleBackColor = true;
			this->btn_Recover->Click += gcnew System::EventHandler(this, &frm_Main::btn_Recover_Click);
			// 
			// dlg_SaveFile
			// 
			this->dlg_SaveFile->Filter = L"Файл Excel 2003|*.xls";
			// 
			// frm_Main
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(419, 192);
			this->Controls->Add(this->btn_Start);
			this->Controls->Add(this->btn_Recover);
			this->Controls->Add(this->btn_ChoosePath);
			this->Controls->Add(this->txt_FilePath);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"frm_Main";
			this->Text = L"Распределение долговых обязательств";
			this->Load += gcnew System::EventHandler(this, &frm_Main::frm_Main_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		//System::String^ Assembly_Namespace; //Holds assembly name for accessing resources
	private: System::Void frm_Main_Load(System::Object^ sender, System::EventArgs^  e) {
			String^ Path = System::IO::Path::GetDirectoryName(
			System::Reflection::Assembly::GetExecutingAssembly()->GetName()->CodeBase);
			this->txt_FilePath->Text = Path->Substring(6) + "\\Input_Data.xls"; //6 to cut "file:/"
			this->dlg_OpenFile->InitialDirectory = Path;
			this->tt_Help->SetToolTip(this->btn_ChoosePath, "Выберите файл");
			this->tt_Help->SetToolTip(this->btn_Recover, "Создать шаблонный файл");
			//this->Assembly_Namespace = this->GetType()->Assembly->GetEntryAssembly()->GetName()->Name->ToString();
		}
	private: System::Void btn_ChoosePath_Click(System::Object^ sender, System::EventArgs^  e) {
		if(this->dlg_OpenFile->ShowDialog() != System::Windows::Forms::DialogResult::OK)
			return;
		this->txt_FilePath->Text = this->dlg_OpenFile->FileName;
	}
	private: System::Void btn_Start_Click(System::Object^  sender, System::EventArgs^ e) {
				 My_Classes::Core::Do_Stuff(this->txt_FilePath->Text);
			 }
private: System::Void btn_Recover_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(this->dlg_SaveFile->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				 System::IO::Stream^ input = System::Reflection::Assembly::GetExecutingAssembly()->GetManifestResourceStream("Init.xls");
				 try {
					 System::IO::Stream^ output = System::IO::File::Create(this->dlg_SaveFile->FileName);
					 array<unsigned char, 1> ^Buf = gcnew array<unsigned char, 1>(My_Classes::Core::BUF_SIZE);
					 int Len;
					 while ((Len = input->Read(Buf, 0, My_Classes::Core::BUF_SIZE)) > 0)
						 output->Write(Buf, 0, Len);
					 output->Close();
					 System::Windows::Forms::MessageBox::Show("Шаблонный файл создан успешно!", "Сообщение",
						 System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
				 }
				 catch(System::Exception^ err) {
					System::String^ Message = System::String::Format("Номер ошибки: {0}\nОписание ошибки: {1}",
					err->HResult, err->Message);
					System::Windows::Forms::MessageBox::Show(Message, "Ошибка записи шаблонного файла",
					System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }

		 }
};
}

#endif