#pragma once
#include <cstdlib>
struct BestMove
{
	int start_id;
	int end_id;
	int global_best;
	bool czy_bicie;
};
namespace Warcaby {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	static int size = 8;
	static int move[2] = { -1,1 };

	#define bialy 1
	#define czarny -1
	
	/// <summary>
	/// Podsumowanie informacji o Plansza
	/// </summary>
	public ref class Plansza : public System::Windows::Forms::Form
	{
	private: int **pionki = new int*[size];
			 int historia = -1, deapth_max = 4;
	public:
			bool bicie = false, alpha_beta=false;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
			 System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
			 System::Windows::Forms::TextBox^  textBox1;
			 System::Windows::Forms::Label^  label1;
			 System::Windows::Forms::Label^  label2;
			 System::Windows::Forms::TextBox^  textBox2;
			 System::Windows::Forms::TrackBar^  trackBar1;
			 System::Windows::Forms::Label^  label3;
			 System::Windows::Forms::TextBox^  textBox3;
			 System::Windows::Forms::ListBox^  listBox1;
			 System::Windows::Forms::Label^  label4;
			 System::Windows::Forms::CheckBox^  checkBox1;


	public:
		BestMove *bestmove = new BestMove;
		Plansza(void)
		{
			InitializeComponent();
			InitializeComponent_2();

			for (int j = 0; j < size; j++) {
				pionki[j] = new int[size];
				for (int i = 0; i < size; i++) {
					pionki[j][i] = 0;
					if ((j == 0 || j == 1 || j == 2) && ((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0))) {
						pionki[j][i] = -1;
						b[j, i]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\czarny.png")));
					}
					if ((j == 5 || j == 6 || j == 7) && ((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0))) {
						pionki[j][i] = 1;
						b[j, i]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\bialy.png")));
					}
				}
			}
		}
		/*************************************************************************
		******Zapowiedzi funkcji wykorzystywanych glownie przez AI****************
		**************************************************************************/
		String^ tab_index(int id);
		int minmax(int kolor, int _minmax, int liczba);
		int minmax_move(int id, int deapth, int kolor, int best, int worst, bool czy_sekwencja);
		int evaluate(int czy_bialy);
		bool czy_damka(int kolor, int j);
		void AI();

		/*********************************************************************************************************
		**Ponizsze trzy funkcje sluza do prostej translacji z przycisk->TabIndeks na pionki[_j][_i] i na odwrot***
		*********************************************************************************************************/
		int _j(int id){
			return id / 8;
		}

		int _i(int id){
			return id % 8;
		}

		int _id(int j, int i){
			return 8 * j + i;
		}

		//Sprawdzenie czy element nie wykracza poza indeks tablicy 
		bool exist(int j, int i)
		{
			if ((j >= 0 && j < 8) && (i >= 0 && i < 8))
				return true;
			else
				return false;
		}

		/**Funkcje czy_jest_bicie(), czysc_wybor(), stworz_wybor(int id) bazuja na roznych 
		wersjach tych samych zestawow petli i obsluguja zarowno zwykle pionki jak i damki,
		w przypadku petlii ktore dzialaja zarowno dla gracza jak i komputera wystepuje 
		rowniez obsluga kolorow**/

		/**********************************************************************************************
		**Sprawdzenie czy ktorys z pionkow ma mozliwosc bicia, wystarczy jeden taki pionek/mozliwosc***
		**Najpierw szukane sa niezerowe elementy tablicy, pozniej typ pionka i dalej mozliwosci bicia**
		**Dodatkowo mozemy sprwadzic bicie tylko dla jednego pionka wpisujac w miejsca start i stop****
		**odpowiednio id szukanego pionka i id+1*******************************************************
		***********************************************************************************************/
		bool czy_jest_bicie(int kolor, int start, int stop)
		{
			int j, i, loops;
			bool obstacle;
			for (int id = start; id < stop; id++)				//od 0 do 64 dla calej planszy
			{
				j = _j(id); i = _i(id);
				if (pionki[j][i] == 2 || pionki[j][i] == -2)	//damka moze sie poruszyc dalej niz raz po przekatnej
					loops = size - 1;
				else
					loops = 0;
				if (pionki[j][i] == kolor*1 || pionki[j][i]==kolor*2)   //jesli id zawiera pionek o kolorze
					for (int k = 0; k < 2; k++) {						//move[0]==-1, sprawdzamy elementy po lewej stronie pionka; move[1]==1 elementy po prawej
						//bicie do przodu
						obstacle = false;
						for (int l = 0; l <= loops; l++) {				//wykona sie raz dla pionka; dla damki do konca planszy
							if (exist(j - kolor * (1 + l), i + (1 + l)*move[k])) {
								if (pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] == 1 * kolor || pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] == 2 * kolor)								//czy przed pionkiem stoi przeszkoda
									obstacle = true;
								if ((pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] == -1 * kolor || pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] == -2 * kolor) && obstacle == false) {	//czy przed pionkiem stoi przeciwnik
									obstacle = true;
									if (exist(j - kolor * (2 + l), i + (2 + l) * move[k]) && pionki[j - kolor * (2 + l)][i + (2 + l) * move[k]] == 0)											//czy za przeciwnikiem jest puste pole
										return true;
								}
							}
						}
						//bicie do tylu
						obstacle = false;
						for (int l = 0; l <= loops; l++) {
							if (exist(j + kolor * (1 + l), i + (1 + l)*move[k])) {
								if (pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] == 1 * kolor || pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] == 2 * kolor)
									obstacle = true;
								if ((pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] == -1 * kolor || pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] == -2 * kolor) && obstacle == false) {
									obstacle = true;
									if (exist(j + kolor * (2 + l), i + (2 + l) * move[k]) && pionki[j + kolor * (2 + l)][i + (2 + l) * move[k]] == 0)
										return true;
								}
							}
						}
					}
			}
			return false;
			
		}

		/***********************************************************************************************
		**Czyszczenie wyboru w przypadku wcisniecia jednego pionka i pozniej zdecydowaniu sie na inny***
		************************************************************************************************/
		void czysc_wybor()
		{
			int kolor = bialy;
			int j = _j(historia), i = _i(historia), loops = 0;

			if (pionki[j][i] == 2) 
				loops = size - 1;

			for (int k = 0; k < 2; k++) 
			{
				for (int l = 0; l <= loops; l++) 
				{
					//warunek umozliwiajacy bicie do przodu
					if (exist(j - (1 + l), i + (1 + l)*move[k]))
					{
						if (pionki[j - (1 + l)][i + (1 + l)*move[k]] < 0) {
							if (exist(j - (2 + l), i + (2 + l) * move[k]) && pionki[j - 2 - l][i + (2 + l) * move[k]] == 0) {
								this->b[j - (2 + l), i + (2 + l) * move[k]]->Click -= gcnew System::EventHandler(this, &Plansza::button2_Click);					//usuniecie mozliwosci przeniesienia pionka
								this->b[j - (2 + l), i + (2 + l) * move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Czarne pole.png"))); //pole wyboru jest teraz pustym polem
							}
						}
					}

					//warunek umozliwiajacy bicie do tylu
					if (exist(j + 1 + l, i + (1 + l)*move[k]) && pionki[j + (1 + l)][i + (1 + l)*move[k]] < 0)
						if (exist(j + (2 + l), i + (2 + l) * move[k]) && pionki[j + (2 + l)][i + (2 + l) * move[k]] == 0) {
							this->b[j + (2 + l), i + (2 + l) * move[k]]->Click -= gcnew System::EventHandler(this, &Plansza::button2_Click);
							this->b[j + (2 + l), i + (2 + l) * move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Czarne pole.png")));
						}
				}
				//warunek na zwykle przemieszczenie pionka, wystepuje tylko gdy nie ma mozliwosci bicia
				if (!bicie)
				for (int l = 0; l <= loops; l++)
				{
					if (exist(j - (1 + l), i + (1 + l)*move[k]))
						if (pionki[j - (1 + l)][i + (1 + l)*move[k]] == 0) {
							this->b[j - (1 + l), i + (1 + l)*move[k]]->Click -= gcnew System::EventHandler(this, &Plansza::button2_Click);
							this->b[j - (1 + l), i + (1 + l)*move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Czarne pole.png")));
						}
					if (loops > 0)
						if (exist(j + (1 + l), i + (1 + l)*move[k]))
							if (pionki[j + (1 + l)][i + (1 + l)*move[k]] == 0) {
								this->b[j + (1 + l), i + (1 + l)*move[k]]->Click -= gcnew System::EventHandler(this, &Plansza::button2_Click);
								this->b[j + (1 + l), i + (1 + l)*move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Czarne pole.png")));
							}
				}
			}
		}

		void stworz_wybor(int id)
		{
			historia = id;
			bool obstacle;
			int j = _j(id), i = _i(id), loops = 0;

			if (pionki[j][i] == 2)
				loops = size - 1;

			for (int k = 0; k < 2; k++)
			{
				//warunek umozliwiajacy bicie do przodu
				obstacle = false;
				for (int l = 0; l <= loops; l++)
				{
					if (exist(j - (1 + l), i + (1 + l)*move[k]))
					{
						if (pionki[j - (1 + l)][i + (1 + l)*move[k]] > 0)
							obstacle = true;
						if (pionki[j - (1 + l)][i + (1 + l)*move[k]] < 0 && obstacle == false) {
							obstacle = true;
							if (exist(j - (2 + l), i + (2 + l) * move[k]) && pionki[j - 2 - l][i + (2 + l) * move[k]] == 0) {
								this->b[j - (2 + l), i + (2 + l) * move[k]]->Click += gcnew System::EventHandler(this, &Plansza::button2_Click);		//po nadaniu klikniecie na ten przycisk przeniesie w to miejsce uprzednio wybrany pionek
								this->b[j - (2 + l), i + (2 + l) * move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\wybor.png"))); //puste pole jest teraz polem wyboru
							}
						}
					}
				}
				//warunek umozliwiajacy bicie do tylu
				obstacle = false;
				for (int l = 0; l <= loops; l++)
				{
					if (exist(j + 1 + l, i + (1 + l)*move[k]))
					{
						if (pionki[j + (1 + l)][i + (1 + l)*move[k]] > 0)
							obstacle = true;
						if (pionki[j + (1 + l)][i + (1 + l)*move[k]] < 0 && obstacle == false) {
							obstacle = true;
							if (exist(j + (2 + l), i + (2 + l) * move[k]) && pionki[j + (2 + l)][i + (2 + l) * move[k]] == 0) {
								this->b[j + (2 + l), i + (2 + l) * move[k]]->Click += gcnew System::EventHandler(this, &Plansza::button2_Click);
								this->b[j + (2 + l), i + (2 + l) * move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\wybor.png")));
							}
						}
					}
				}
				//warunek na zwykle przemieszczenie pionka, wystepuje tylko gdy nie ma mozliwosci bicia
				if (!bicie) {
					//ruch do przodu
					obstacle = false;
					for (int l = 0; l <= loops; l++)
					{
						if (exist(j - (1 + l), i + (1 + l)*move[k])) {				//czy pole istnieje
							if (pionki[j - (1 + l)][i + (1 + l)*move[k]] != 0)		//w przypadku damki jesli w pewnym momencie natrafimy na przeszkode (np swoj pionek) to nie ma mozliwosci przeniesienia sie za niego
								obstacle = true;
							if (pionki[j - (1 + l)][i + (1 + l)*move[k]] == 0 && obstacle==false) { //jesli pole jest puste a miedzy polem a pionkiem nie ma przeszkody
								this->b[j - (1 + l), i + (1 + l)*move[k]]->Click += gcnew System::EventHandler(this, &Plansza::button2_Click);
								this->b[j - (1 + l), i + (1 + l)*move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\wybor.png")));
							}
						}
					}
					//ruch do tylu (tylko dla damki)
					obstacle = false;
					for (int l = 0; l <= loops; l++)
					{
						if (loops > 0)
							if (exist(j + (1 + l), i + (1 + l)*move[k])) {
								if (pionki[j + (1 + l)][i + (1 + l)*move[k]] != 0)
									obstacle = true;
								if (pionki[j + (1 + l)][i + (1 + l)*move[k]] == 0 && obstacle==false) {
									this->b[j + (1 + l), i + (1 + l)*move[k]]->Click += gcnew System::EventHandler(this, &Plansza::button2_Click);
									this->b[j + (1 + l), i + (1 + l)*move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\wybor.png")));
								}
							}
					}
				}
			}
		}

	protected:
		/// <summary>
		/// Wyczyœæ wszystkie u¿ywane zasoby.
		/// </summary>
		~Plansza()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;

	private: cli::array<Button^, 2>^ b;

	internal:
		System::Windows::Forms::ImageList^ ImageList1;


	private:
		/// <summary>
		/// Wymagana zmienna projektanta.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda wymagana do obs³ugi projektanta — nie nale¿y modyfikowaæ
		/// jej zawartoœci w edytorze kodu.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Plansza::typeid));
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			this->SuspendLayout();
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Location = System::Drawing::Point(15, 15);
			this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(10);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(480, 480);
			this->flowLayoutPanel1->TabIndex = 0;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(0, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(510, 510);
			this->pictureBox1->TabIndex = 102;
			this->pictureBox1->TabStop = false;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				47.28261F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				52.71739F)));
			this->tableLayoutPanel1->Controls->Add(this->textBox2, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->label2, 1, 0);
			this->tableLayoutPanel1->Location = System::Drawing::Point(547, 15);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 36.84211F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 63.15789F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(201, 46);
			this->tableLayoutPanel1->TabIndex = 103;
			// 
			// textBox2
			// 
			this->textBox2->AccessibleRole = System::Windows::Forms::AccessibleRole::TitleBar;
			this->textBox2->Anchor = System::Windows::Forms::AnchorStyles::Top;
			this->textBox2->Location = System::Drawing::Point(109, 19);
			this->textBox2->Name = L"textBox2";
			this->textBox2->ReadOnly = true;
			this->textBox2->Size = System::Drawing::Size(77, 20);
			this->textBox2->TabIndex = 3;
			this->textBox2->Text = L"0";
			// 
			// textBox1
			// 
			this->textBox1->AccessibleRole = System::Windows::Forms::AccessibleRole::TitleBar;
			this->textBox1->Anchor = System::Windows::Forms::AnchorStyles::Top;
			this->textBox1->Location = System::Drawing::Point(9, 19);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(77, 20);
			this->textBox1->TabIndex = 0;
			this->textBox1->Text = L"0";
			// 
			// label1
			// 
			this->label1->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(10, 3);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(74, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Obecny wynik";
			// 
			// label2
			// 
			this->label2->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(99, 3);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(98, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Szacunkowy wynik";
			// 
			// trackBar1
			// 
			this->trackBar1->BackColor = System::Drawing::SystemColors::Control;
			this->trackBar1->Location = System::Drawing::Point(547, 94);
			this->trackBar1->Maximum = 8;
			this->trackBar1->Minimum = 1;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(201, 45);
			this->trackBar1->TabIndex = 104;
			this->trackBar1->Value = 4;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &Plansza::trackBar1_Scroll);
			// 
			// label3
			// 
			this->label3->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(559, 74);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(125, 13);
			this->label3->TabIndex = 105;
			this->label3->Text = L"G³êbia algorytmu minmax";
			// 
			// textBox3
			// 
			this->textBox3->AccessibleRole = System::Windows::Forms::AccessibleRole::TitleBar;
			this->textBox3->Anchor = System::Windows::Forms::AnchorStyles::Top;
			this->textBox3->Location = System::Drawing::Point(690, 71);
			this->textBox3->Name = L"textBox3";
			this->textBox3->ReadOnly = true;
			this->textBox3->Size = System::Drawing::Size(40, 20);
			this->textBox3->TabIndex = 106;
			this->textBox3->Text = L"4";
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(560, 188);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(170, 251);
			this->listBox1->TabIndex = 107;
			// 
			// label4
			// 
			this->label4->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(559, 172);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(139, 13);
			this->label4->TabIndex = 108;
			this->label4->Text = L"Historia ruchów przeciwnika";
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(602, 136);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(96, 17);
			this->checkBox1->TabIndex = 109;
			this->checkBox1->Text = L"tryb alpha-beta";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Plansza::checkBox1_CheckedChanged);
			// 
			// Plansza
			// 
			this->ClientSize = System::Drawing::Size(800, 550);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->listBox1);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->flowLayoutPanel1);
			this->Controls->Add(this->pictureBox1);
			this->MaximizeBox = false;
			this->Name = L"Plansza";
			this->Load += gcnew System::EventHandler(this, &Plansza::Plansza_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	void InitializeComponent_2(void)
	{
		this->b = gcnew cli::array<Button^, 2>(8, 8);
		//
		// tablica przyciskow
		//
		int k = 0;
		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				this->b[j, i] = gcnew Button();
				if ((i % 2 != 0 && j % 2 != 0) || (i % 2 == 0 && j % 2 == 0)) {
					this->b[j, i]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Biale pole.png")));
				}
				else
				{
					this->b[j, i]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Czarne pole.png")));
					this->b[j, i]->Click += gcnew System::EventHandler(this, &Plansza::button1_Click);
				}
				this->b[j, i]->Margin = System::Windows::Forms::Padding(0);
				this->b[j, i]->Size = System::Drawing::Size(60, 60);
				this->b[j, i]->TabIndex = k;
				this->b[j, i]->UseVisualStyleBackColor = true;
				flowLayoutPanel1->Controls->Add(this->b[j, i]);
				k++;
			}
		}
	}
	private: System::Void Plansza_Load(System::Object^  sender, System::EventArgs^  e)
	{
	}

	/*********************************************************************
	**Obsluga przycisku pierwszego typu - klikniecie na bialy pionek******
	**********************************************************************/
	private: System::Void button1_Click(System::Object^  sender,  System::EventArgs^  e)
	{
		Button^ clickedButton = (Button^)sender;
		int id = clickedButton->TabIndex;
		bicie=czy_jest_bicie(bialy, 0, 64);
		if (historia != -1)
			czysc_wybor();
		if (pionki[_j(id)][_i(id)] == 1 || pionki[_j(id)][_i(id)] == 2)
			stworz_wybor(id);
	}

	/*********************************************************************
	**Obsluga przycisku drugiego typu - klikniecie na pole wyboru*********
	**********************************************************************/
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Button^ clickedButton = (Button^)sender;
		int id = clickedButton->TabIndex;

		//podmiana pola wyboru na pionek i pionka na puste pole
		pionki[_j(id)][_i(id)] = pionki[_j(historia)][_i(historia)];
		clickedButton->Image = b[_j(historia), _i(historia)]->Image;
		pionki[_j(historia)][_i(historia)] = 0;
		b[_j(historia),_i(historia)]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Czarne pole.png")));

		//ponizsze warunki zapewniaja mechanike zbicia (usuniecia) pionka
		if (bicie)
		{
			int road = abs((_j(historia) - _j(id))), road2 = abs((_i(historia) - _i(id)));
			pionki[_j(id) + (_j(historia) - _j(id)) / road][_i(id) + (_i(historia) - _i(id))/road2] = 0;
			b[_j(id) + (_j(historia) - _j(id)) / road,_i(id) + (_i(historia) - _i(id)) / road2]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Czarne pole.png")));

			/********Bicie sekwencyjne********/
			int temporary = pionki[_j(id)][_i(id)];		
			if (temporary == 2)
				pionki[_j(id)][_i(id)] = 1;			//podmiana wlasciowosci damki na wlasciwosci zwyklego pionka

			if (czy_jest_bicie(bialy, id, id + 1))	//czy pionek po zmianie polozenia dalej posiada mozliwosc bicia
			{
				czysc_wybor();
				stworz_wybor(id);
				clickedButton->Click -= gcnew System::EventHandler(this, &Plansza::button2_Click);
				pionki[_j(id)][_i(id)] = temporary;
				return;	//AI() nie moze sie wywolac bo wciaz jest kolej gracza
			}
			pionki[_j(id)][_i(id)] = temporary;
		}

		//gdy bialy pionek znajdzie sie na polu j z indeksem 0 - staje sie damka
		if (pionki[_j(id)][_i(id)] == 1 && _j(id) == 0) 
		{
			pionki[_j(id)][_i(id)] = 2;
			clickedButton->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\damka biala.png")));
		}

		bicie = false;
		clickedButton->Click -= gcnew System::EventHandler(this, &Plansza::button2_Click);
		czysc_wybor();
		historia = -1;
		//wywolanie algorytmu sztucznej inteligencji do obslugi czarnych pionkow
		//wywolanie nastepuje po koncu tury gracza czyli natychmiast po wykonaniu ruchu przez gracza
		AI();

		this->textBox1->Text = System::Convert::ToString(evaluate(1));
	}

	private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
		deapth_max = this->trackBar1->Value;
		this->textBox3->Text = System::Convert::ToString(deapth_max);
	}
	private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		alpha_beta = this->checkBox1->Checked;
	}
};
	
}

	
