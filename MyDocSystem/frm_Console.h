#pragma once

namespace MyDocSystem {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ frm_Console
	/// </summary>
	public ref class frm_Console : public System::Windows::Forms::Form {
		public:
		//My additional Data
		bool is_Closed;
	
		frm_Console(void) {
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
			this->is_Closed = true;
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~frm_Console()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::RichTextBox^  rtxt_Console;
	protected: 

	protected: 

	private:
		/// <summary>
		/// “ребуетс€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ќб€зательный метод дл€ поддержки конструктора - не измен€йте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frm_Console::typeid));
			this->rtxt_Console = (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// rtxt_Console
			// 
			this->rtxt_Console->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->rtxt_Console->Font = (gcnew System::Drawing::Font(L"Arial", 14));
			this->rtxt_Console->Location = System::Drawing::Point(12, 12);
			this->rtxt_Console->Name = L"rtxt_Console";
			this->rtxt_Console->ReadOnly = true;
			this->rtxt_Console->Size = System::Drawing::Size(432, 345);
			this->rtxt_Console->TabIndex = 0;
			this->rtxt_Console->Text = L"";
			// 
			// frm_Console
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(456, 369);
			this->Controls->Add(this->rtxt_Console);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"frm_Console";
			this->Text = L"Console";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &frm_Console::frm_Console_FormClosed);
			this->Shown += gcnew System::EventHandler(this, &frm_Console::frm_Console_Shown);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void frm_Console_Shown(System::Object^  sender, System::EventArgs^  e) {
				 this->is_Closed = false;
			 }
	private: System::Void frm_Console_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
				 this->is_Closed = true;
			 }
	};
}
