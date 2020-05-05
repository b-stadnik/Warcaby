#pragma once
#include <cstdlib>
namespace Warcaby {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	static int size = 8;
	int move[2] = { -1,1 };
	bool bicie = false;

	#define bialy 1
	#define czarny -1

	/// <summary>
	/// Podsumowanie informacji o Plansza
	/// </summary>
	public ref class Plansza : public System::Windows::Forms::Form
	{
	private: int **pionki = new int*[size];
			 int historia = -1;
	public:
		Plansza(void)
		{
			InitializeComponent();
			InitializeComponent_2();

			for (int j = 0; j < size; j++) {
				pionki[j] = new int[size];
				for (int i = 0; i < size; i++) {
					pionki[j][i] = 0;
					if ((j == 0 || j == 1 || j == 2) && ((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0))) {
						//pionki[j][i] = -1;
						//b[j, i]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\czarny.png")));
					}
					if ((j == 5 || j == 5 || j == 7) && ((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0))) {
						pionki[j][i] = 1;
						b[j, i]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\bialy.png")));
					}
				}
			}
		}
		int _j(int id)
		{
			return id / 8;
		}

		int _i(int id)
		{
			return id % 8;
		}

		int _id(int j, int i)
		{
			return 8 * j + i;
		}

		bool exist(int j, int i)
		{
			if ((j >= 0 && j < 8) && (i >= 0 && i < 8))
				return true;
			else
				return false;
		}

		void czy_jest_bicie()
		{
			int j, i, loops;
			for (int id = 0; id < 64; id++)
			{
				j = _j(id); i = _i(id);
				if (pionki[j][i] == 2)
					loops = size - 1;
				else
					loops = 0;
				if (pionki[j][i] == 1 || pionki[j][i]==2)
					for (int k = 0; k < 2; k++) {
						for (int l = 0; l <= loops; l++) {
							if (exist(j - (1 + l), i + (1 + l)*move[k]))
								if (pionki[j - (1 + l)][i + (1 + l)*move[k]] != 0)
									if (exist(j - (2 + l), i + (2 + l) * move[k]) && pionki[j - 2 - l][i + (2 + l) * move[k]] == 0)
										bicie = true;
							if (exist(j + 1 + l, i + (1 + l)*move[k]) && pionki[j + (1 + l)][i + (1 + l)*move[k]] != 0)
								if (exist(j + (2 + l), i + (2 + l) * move[k]) && pionki[j + (2 + l)][i + (2 + l) * move[k]] == 0)
									bicie = true;
							if (bicie)
								break;
						}
					}
				if (bicie)
					break;
			}
			
		}

		/***********************************************************************************************
		**Czyszczenie wyboru w przypadku wcisniecia jednego pionka i pozniej zdecydowaniu sie na inny***
		************************************************************************************************/
		void czysc_wybor()
		{
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
						if (pionki[j - (1 + l)][i + (1 + l)*move[k]] != 0) {
							if (exist(j - (2 + l), i + (2 + l) * move[k]) && pionki[j - 2 - l][i + (2 + l) * move[k]] == 0) {
								this->b[j - (2 + l), i + (2 + l) * move[k]]->Click -= gcnew System::EventHandler(this, &Plansza::button2_Click);
								this->b[j - (2 + l), i + (2 + l) * move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\Czarne pole.png")));
							}
						}
					}

					//warunek umozliwiajacy bicie do tylu
					if (exist(j + 1 + l, i + (1 + l)*move[k]) && pionki[j + (1 + l)][i + (1 + l)*move[k]] != 0)
						if (exist(j + (2 + l), i + (2 + l) * move[k]) && pionki[j + (2 + l)][i + (2 + l) * move[k]] == 0) {
							this->b[j + (2 + l), i + (2 + l) * move[k]]->Click -= gcnew System::EventHandler(this, &Plansza::button2_Click);
							this->b[j + (2 + l), i + (2 + l) * move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\Czarne pole.png")));
						}
				}
				//warunek na zwykle przemieszczenie pionka, wystepuje tylko gdy nie ma mozliwosci bicia
				for (int l = 0; l <= loops; l++)
				{
					if (exist(j - (1 + l), i + (1 + l)*move[k]) && !bicie)
						if (pionki[j - (1 + l)][i + (1 + l)*move[k]] == 0) {
							this->b[j - (1 + l), i + (1 + l)*move[k]]->Click -= gcnew System::EventHandler(this, &Plansza::button2_Click);
							this->b[j - (1 + l), i + (1 + l)*move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\Czarne pole.png")));
						}
					if (loops > 0)
						if (exist(j + (1 + l), i + (1 + l)*move[k]) && !bicie)
							if (pionki[j + (1 + l)][i + (1 + l)*move[k]] == 0) {
								this->b[j + (1 + l), i + (1 + l)*move[k]]->Click -= gcnew System::EventHandler(this, &Plansza::button2_Click);
								this->b[j + (1 + l), i + (1 + l)*move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\Czarne pole.png")));
							}
				}
			}
		}

		void stworz_wybor(int id)
		{
			historia = id;
			int j = _j(id), i = _i(id), loops = 0;
			if (pionki[j][i] == 2)
				loops = size - 1;

			for (int k = 0; k < 2; k++)
			{
				for (int l = 0; l <= loops; l++)
				{
					//warunek umozliwiajacy bicie do przodu
					if (exist(j - (1 + l), i + (1 + l)*move[k]))
					{
						if (pionki[j - (1 + l)][i + (1 + l)*move[k]] != 0) {
							if (exist(j - (2 + l), i + (2 + l) * move[k]) && pionki[j - 2 - l][i + (2+l) * move[k]] == 0) {
								this->b[j - (2 + l), i + (2 + l) * move[k]]->Click += gcnew System::EventHandler(this, &Plansza::button2_Click);
								this->b[j - (2 + l), i + (2 + l) * move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\wybor.png")));
							}
						}
					}

					//warunek umozliwiajacy bicie do tylu
					if (exist(j + 1 + l, i + (1+l)*move[k]) && pionki[j + (1 + l)][i + (1 + l)*move[k]] != 0)
						if (exist(j + (2 + l), i + (2 + l) * move[k]) && pionki[j + (2 + l)][i + (2 + l) * move[k]] == 0) {
							this->b[j + (2 + l), i + (2 + l) * move[k]]->Click += gcnew System::EventHandler(this, &Plansza::button2_Click);
							this->b[j + (2 + l), i + (2 + l) * move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\wybor.png")));
						}
				}
				//warunek na zwykle przemieszczenie pionka, wystepuje tylko gdy nie ma mozliwosci bicia
				for (int l = 0; l <= loops; l++)
				{
					if (exist(j - (1 + l), i + (1 + l)*move[k]) && !bicie)
						if (pionki[j - (1 + l)][i + (1 + l)*move[k]] == 0) {
							this->b[j - (1 + l), i + (1 + l)*move[k]]->Click += gcnew System::EventHandler(this, &Plansza::button2_Click);
							this->b[j - (1 + l), i + (1 + l)*move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\wybor.png")));
						}
					if (loops>0)
						if (exist(j + (1 + l), i + (1 + l)*move[k]) && !bicie)
							if (pionki[j + (1 + l)][i + (1 + l)*move[k]] == 0) {
								this->b[j + (1 + l), i + (1 + l)*move[k]]->Click += gcnew System::EventHandler(this, &Plansza::button2_Click);
								this->b[j + (1 + l), i + (1 + l)*move[k]]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\wybor.png")));
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
	private: System::Windows::Forms::Button^  button1;
	private: cli::array<Button^, 2>^ b;

	internal:
		System::Windows::Forms::ImageList^ ImageList1;
	private: System::Windows::Forms::Button^  button2;
	internal:

	protected:

	protected:

	protected:

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(10);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(480, 480);
			this->flowLayoutPanel1->TabIndex = 0;
			this->flowLayoutPanel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Plansza::flowLayoutPanel1_Paint);
			// 
			// button1
			// 
			this->button1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button1.Image")));
			this->button1->Location = System::Drawing::Point(662, 136);
			this->button1->Margin = System::Windows::Forms::Padding(0);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(57, 59);
			this->button1->TabIndex = 100;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(653, 288);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(65, 67);
			this->button2->TabIndex = 101;
			this->button2->Text = L"button2";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// Plansza
			// 
			this->ClientSize = System::Drawing::Size(800, 800);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->flowLayoutPanel1);
			this->Name = L"Plansza";
			this->Load += gcnew System::EventHandler(this, &Plansza::Plansza_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	void InitializeComponent_2(void)
	{
		this->b = gcnew cli::array<Button^, 2>(8, 8);
		//
		// b
		//
		int k = 0;
		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				this->b[j, i] = gcnew Button();
				if ((i % 2 != 0 && j % 2 != 0) || (i % 2 == 0 && j % 2 == 0)) {
					//this->b[j, i]->Enabled = false;
					this->b[j, i]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\Biale pole.png")));
				}
				else
				{
					this->b[j, i]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\Czarne pole.png")));
					this->b[j, i]->Click += gcnew System::EventHandler(this, &Plansza::button1_Click);
				}
				this->b[j, i]->Margin = System::Windows::Forms::Padding(0);
				this->b[j, i]->Name = k.ToString();
				this->b[j, i]->Size = System::Drawing::Size(60, 60);
				this->b[j, i]->TabIndex = k;
				//this->b[j, i]->Text = k.ToString();
				this->b[j, i]->UseVisualStyleBackColor = true;
				flowLayoutPanel1->Controls->Add(this->b[j, i]);
				k++;
			}
		}
	}
	private: System::Void Plansza_Load(System::Object^  sender, System::EventArgs^  e)
	{
	}

	private: System::Void Plansza_Load_1(System::Object^  sender, System::EventArgs^  e)
	{
	}

	private: System::Void button1_Click(System::Object^  sender,  System::EventArgs^  e)
	{
		Button^ clickedButton = (Button^)sender;
		int id = clickedButton->TabIndex, i, j;
		czy_jest_bicie();
		if (historia != -1)
			czysc_wybor();
		if (pionki[_j(id)][_i(id)] == 1 || pionki[_j(id)][_i(id)] == 2)
			stworz_wybor(id);
	}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Button^ clickedButton = (Button^)sender;
		int id = clickedButton->TabIndex;

		pionki[_j(id)][_i(id)] = pionki[_j(historia)][_i(historia)];
		clickedButton->Image = b[_j(historia), _i(historia)]->Image;
		pionki[_j(historia)][_i(historia)] = 0;
		b[_j(historia),_i(historia)]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\Czarne pole.png")));

		//ponizsze warunki zapewniaja mechanike zbicia (usuniecia) pionka
		if (bicie)
		{
			int road = abs((_j(historia) - _j(id))), road2 = abs((_i(historia) - _i(id)));
			pionki[_j(id) + (_j(historia) - _j(id)) / road][_i(id) + (_i(historia) - _i(id))/road2] = 0;
			b[_j(id) + (_j(historia) - _j(id)) / road,_i(id) + (_i(historia) - _i(id)) / road2]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\Czarne pole.png")));
		}

		//gdy bialy pionek znajdzie sie na polu j z indeksem 0 - staje sie damka
		if (pionki[_j(id)][_i(id)] == 1 && _j(id) == 0) 
		{
			pionki[_j(id)][_i(id)] = 2;
			clickedButton->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("C:\\Users\\ikill\\Pictures\\warcaby\\damka biala.png")));
		}
		clickedButton->Click -= gcnew System::EventHandler(this, &Plansza::button2_Click);
		bicie = false;
		historia = -1;
	}

	private: System::Void flowLayoutPanel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
	}
	};
	
}

	
