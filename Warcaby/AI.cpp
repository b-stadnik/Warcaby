#include "Plansza.h"
#include <chrono>

/*********************************************************
**Funkcja wykorzystywana do tworzenia historii ruchow ****
**rozpoznaje pole na ktorym znajduje sie pionek***********
**********************************************************/
System::String^ Warcaby::Plansza::tab_index(int id)
{
	String^ tekst;
	switch (_i(id))
	{
		case 0: tekst = "A"; break;
		case 1: tekst = "B"; break;
		case 2: tekst = "C"; break;
		case 3: tekst = "D"; break;
		case 4: tekst = "E"; break;
		case 5: tekst = "F"; break;
		case 6: tekst = "G"; break;
		case 7: tekst = "H"; break;
	}
	tekst += System::Convert::ToString(_j(id)+1);
	return tekst;
}
/*************************************************************************
**Funkcja oszacowuje w formie punkow ktory kolor dominuje na planszy******
*************************************************************************/
int Warcaby::Plansza::evaluate(int czy_bialy)
{
	int j, i, kolor, score=0;
	for (int id = 0; id < 64; id++)  //sprawdzenie kazdego pola
	{
		j = _j(id); i = _i(id);
		if (pionki[j][i] != 0)
		{
			//zidentyfikowanie koloru oraz nadanie bonusowych punktow 
			//jesli pionek znajduje sie glebiej na terytorium przeciwnika
			if (pionki[j][i] > 0)
			{
				kolor = bialy;
				if (j == 0)
					score += 1;
				if (j == 1)
					score += 1;
				if (j == 2)
					score += 1;
			}
			else
			{
				kolor = czarny;
				if (j == 7)
					score -= 1;
				if (j == 6)
					score -= 1;
				if (j == 4)
					score -= 1;
			}

			//pionek istnieje na planszy
			score += kolor * 15;

			//pionek jest damka
			if (pionki[j][i] == kolor*2)
				score += kolor * 15;

			//pionek jest przy bocznej scianie
			if (i==0 || i==7)
				score += kolor * 1;

			//pionek znajduje sie w formacji 
			for (int k = 0; k < 2; k++) 
			{
				//sprawdzenie czy za pionkiem cos stoi
				if (exist(j + kolor, i + move[k]))
					if (pionki[j + kolor][i + move[k]] == kolor * 1 || pionki[j + kolor][i + move[k]] == kolor * 2) {
						score += kolor * 2;
						//sprawdzenie czy przed pionkiem cos stoi, jesli tak to punkty nie zostana przyznane
						if (exist(j - kolor, i + move[k])) {
							if (pionki[j - kolor][i + move[k]] == kolor * 1 || pionki[j - kolor][i + move[k]] == kolor * 2)
								score -= kolor * 2;
							//przed pionkiem stoi przeciwnik a za pionkiem sojusznik, oznacza to pozycje obronna
							if (pionki[j - kolor][i + move[k]] == kolor * -1 || pionki[j - kolor][i + move[k]] == kolor * -2)
								score += kolor * 2;
						}
					}
			}

			//figura ma mozliwosc bicia
			if(czy_bialy==kolor)
				if (czy_jest_bicie(kolor, id, id + 1)) {
					score += kolor * 2;
					
					for (int k = 0; k < 2; k++) {
						if (exist(j + kolor, i + move[k]))
							if (pionki[j + kolor][i + move[k]] == kolor * 1 || pionki[j + kolor][i + move[k]] == kolor * 2)
								if (exist(j - kolor, i - move[k]))
									if (pionki[j - kolor][i - move[k]] == kolor * -1 || pionki[j - kolor][i - move[k]] == kolor * -2)
										score += kolor * 2;
					}
					
				
			}
		}
	}
	return score;
}

/****************************************************************************
**Obsluga glownego algortymu, to tutaj wykonywane sa symulacje oraz**********
**identyfikacja najlepszego mozliwego ruchu wraz z jego potencjalna wartosia*
**Funkcja obsluguje wszystkie mozliwe warunki: kolor, rodzaj pionka, bicie***
**w tym do przodu i do tylu, bicie sekwencyjne, zwykly ruch, przeszkoda******
*****************************************************************************/
int Warcaby::Plansza::minmax_move(int id, int deapth, int kolor, int alpha, int beta, bool czy_sekwencja)
{
	int j = _j(id), i = _i(id), loops = 0;

	if ((kolor == czarny && pionki[j][i] >= 0) || (kolor == bialy && pionki[j][i] <= 0))  //jesli obecny pionek jest przeciwnego koloru niz obecnie jest obslugiwany
		return 1000 * kolor;
	if (deapth == deapth_max)	//algortym zostanie wywolany 4 razy przed podaniem ostatecznego wyniku
		return evaluate(kolor);
	bool _bicie = czy_jest_bicie(kolor, 0, 64), obstacle = false;
	if (pionki[j][i] == 2 || pionki[j][i] == -2)
		loops = size - 1;

	int best = kolor*-1000, val, temporary_current, temporary_opponent;;
	for (int k = 0; k < 2; k++)
	{
		obstacle = false;
		for (int l = 0; l <= loops; l++)
		{
			//warunek umozliwiajacy bicie do przodu
			if (exist(j - kolor * (1 + l), i + (1 + l)*move[k]))
			{
				if (pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] == kolor * 1 || pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] == kolor * 2)
					obstacle = true;
				if ((pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] == -kolor * 1 || pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] == -kolor * 2) && obstacle == false) {
					obstacle = true;
					if (exist(j - kolor * (2 + l), i + (2 + l)*move[k]) && pionki[j - kolor * (2 + l)][i + (2 + l)*move[k]] == 0)
					{
						//wynonanie ruchu
						temporary_opponent = pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]];
						temporary_current = pionki[j][i];
						pionki[j - kolor * (2 + l)][i + (2 + l)*move[k]] = pionki[j][i];	
						pionki[j][i] = 0;	
						pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] = 0;

						//obsluga sekwencyjnego bicia
						if (czy_jest_bicie(kolor, _id(j - kolor * (2 + l), i + (2 + l)*move[k]), _id(j - kolor * (2 + l), i + (2 + l)*move[k]) +1)) 
						{
							if (temporary_current == kolor * 2)		//jesli pionek jest damka to nadajemy mu wlasciowosci zwyklego pionka
								pionki[j - kolor * (2 + l)][i + (2 + l)*move[k]] = kolor * 1;
							val = minmax(kolor, best, minmax_move(_id(j - kolor * (2 + l), i + (2 + l)*move[k]), deapth, kolor, alpha, beta, true)); //nie zwiekszamy glebi, nadal jest ruch tego samego koloru
							best = minmax(kolor, best, val);
							if (kolor == czarny)
								beta = minmax(-kolor, alpha, best);
							else
								alpha = minmax(-kolor, beta, best);
							pionki[j - kolor * (2 + l)][i + (2 + l)*move[k]] = pionki[j][i];
						}
						else {							//jesli sekwencja bicia sie skonczy lub nigdy nie nastapila
							if (czy_damka(kolor, j))	//jesli pionek byl damka to przywracamy jego wlasicwosci
								pionki[j - kolor * (2 + l)][i + (2 + l)*move[k]] = kolor * 2;
							for (int id = 0; id < 64; id++)
							{
								val = minmax(kolor, best, minmax_move(id, deapth + 1, -kolor, alpha, beta, false));  //ponowne wywolanie algorytmu, ruch przeciwnego koloru i zwiekszamy glebie o 1
								best = minmax(kolor, best, val);
								if (kolor == czarny)
									beta = minmax(-kolor, alpha, best);
								else
									alpha = minmax(-kolor, beta, best);

								if ((beta <= alpha) && alpha_beta == true)
									break;
							}
						}
						//przywrocenie stanu planszy przed wykonaniem ruchu
						pionki[j][i] = temporary_current;
						pionki[j - kolor * (2 + l)][i + (2 + l)*move[k]] = 0;	
						pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] = temporary_opponent;

						//znalezienie najlepszego ruchu - wywolania w tych miejscach pozwoli zapisac ktorym pionkiem ruszamy, dokad nim ruszamy i wartosc jaka oszacowal algortym
						if (deapth == 0) {
							bestmove->global_best = minmax(kolor, bestmove->global_best, best);
							if (bestmove->global_best == best) {
								bestmove->start_id = id; bestmove->end_id = _id(j - kolor * (2 + l), i + (2 + l) * move[k]); bestmove->czy_bicie = true;
							}
						}
					}
				}
			}
		}
		obstacle = false;
		for (int l = 0; l <= loops; l++)
		{
			//warunek umozliwiajacy bicie do tylu
			if (exist(j + kolor * (1 + l), i + (1 + l)*move[k])) {
				if ((pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] == kolor * 1 || pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] == kolor * 2))
					obstacle = true;
				if ((pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] == -kolor * 1 || pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] == -kolor * 2) && obstacle == false) {
					obstacle = true;
					if (exist(j + kolor * (2 + l), i + (2 + l) * move[k]) && pionki[j + kolor * (2 + l)][i + (2 + l)*move[k]] == 0)
					{
						temporary_opponent = pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]];
						temporary_current = pionki[j][i];
						pionki[j + kolor * (2 + l)][i + (2 + l)*move[k]] = pionki[j][i];
						pionki[j][i] = 0;
						pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] = 0;

						if (czy_jest_bicie(kolor, _id(j + kolor * (2 + l), i + (2 + l)*move[k]), _id(j + kolor * (2 + l), i + (2 + l)*move[k]) + 1)) 
						{
							if (temporary_current == kolor * 2)
								pionki[j + kolor * (2 + l)][i + (2 + l)*move[k]] = kolor * 1;
							val = minmax(kolor, best, minmax_move(_id(j + kolor * (2 + l), i + (2 + l)*move[k]), deapth, kolor, alpha, beta, true));
							best = minmax(kolor, best, val);
							if (kolor == czarny)
								beta = minmax(kolor, alpha, best);
							else
								alpha = minmax(kolor, beta, best);
							pionki[j + kolor * (2 + l)][i + (2 + l)*move[k]] = pionki[j][i];
						}
						else {
							if (czy_damka(kolor, j))
								pionki[j + kolor * (2 + l)][i + (2 + l)*move[k]] = kolor * 2;
							for (int id = 0; id < 64; id++) {
								val = minmax(kolor, best, minmax_move(id, deapth + 1, -kolor, alpha, beta, false));
								best = minmax(kolor, best, val);
								if (kolor == czarny)
									beta = minmax(-kolor, alpha, best);
								else
									alpha = minmax(-kolor, beta, best);

								if ((beta <= alpha) && alpha_beta == true)
									break;
							}
						}
						pionki[j][i] = temporary_current;
						pionki[j + kolor * (2 + l)][i + (2 + l)*move[k]] = 0;
						pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] = temporary_opponent;

						if (deapth == 0) {
							bestmove->global_best = minmax(kolor, bestmove->global_best, best);
							if (bestmove->global_best == best) {
								bestmove->start_id = id; bestmove->end_id = _id(j + kolor * (2 + l), i + (2 + l) * move[k]); bestmove->czy_bicie = true;
							}
						}
						obstacle = true;
					}
				}
			}
		}
		//warunek na zwykle przemieszczenie pionka, wystepuje tylko gdy nie ma mozliwosci bicia
		if (!_bicie) {
			obstacle = false;
			//ruch do przodu
			for (int l = 0; l <= loops; l++)
			{
				if (exist(j - kolor * (1 + l), i + (1 + l)*move[k])) {
					if (pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] != 0)
						obstacle = true;
					if (pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] == 0 && obstacle == false)
					{
						temporary_current = pionki[j][i];
						pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] = pionki[j][i];	
						pionki[j][i] = 0;
						if (czy_damka(kolor, j))
							pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] = kolor * 2;
						for (int id = 0; id < 64; id++)
						{
							val = minmax(kolor, best, minmax_move(id, deapth + 1, -kolor, alpha, beta, false));
							best = minmax(kolor, best, val);
							if (kolor == czarny)
								beta = minmax(-kolor, alpha, best);
							else
								alpha = minmax(-kolor, beta, best);

							if ((beta <= alpha) && alpha_beta == true)
								break;
						}
						pionki[j][i] = temporary_current;
						pionki[j - kolor * (1 + l)][i + (1 + l)*move[k]] = 0;

						if (deapth == 0) {
							bestmove->global_best = minmax(kolor, bestmove->global_best, best);
							if (bestmove->global_best == best) {
								bestmove->start_id = id; bestmove->end_id = _id(j - kolor * (1 + l), i + (1 + l)*move[k]); bestmove->czy_bicie = false;
							}
						}
					}
				}
			}
			//ruch do tylu
			obstacle = false;
			for (int l = 0; l <= loops; l++)
			{
				if (loops > 0)
					if (exist(j + kolor * (1 + l), i + (1 + l)*move[k])) {
						if (pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] != 0)
							obstacle = true;
						if (pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] == 0 && obstacle == false)
						{
							temporary_current = pionki[j][i];
							pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] = pionki[j][i];	
							pionki[j][i] = 0;
							if (czy_damka(kolor, j))
								pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] = kolor * 2;

							for (int id = 0; id < 64; id++) {
								val = minmax(kolor, best, minmax_move(id, deapth + 1, -kolor, alpha, beta, false));
								best = minmax(kolor, best, val);
								if (kolor == czarny)
									beta = minmax(kolor, alpha, best);
								else
									alpha = minmax(kolor, beta, best);

								if ((beta <= alpha) && alpha_beta == true)
									break;
							}
							pionki[j][i] = temporary_current;
							pionki[j + kolor * (1 + l)][i + (1 + l)*move[k]] = 0;

							if (deapth == 0) {
								bestmove->global_best = minmax(kolor, bestmove->global_best, best);
								if (bestmove->global_best == best) {
									bestmove->start_id = id; bestmove->end_id = _id(j + kolor * (1 + l), i + (1 + l)*move[k]); bestmove->czy_bicie = false;
								}
							}
						}
					}
			}
		}
	}
	if(czy_sekwencja==false)
		if ((best == 1000 || best == -1000) && deapth!=0)
			return -best;
	return best;
}

int Warcaby::Plansza::minmax(int kolor, int _minmax, int liczba)
{
	if (kolor == bialy)
		if (liczba > _minmax)
			return liczba;
	if (kolor == czarny)
		if (liczba < _minmax)
			return liczba;
	return _minmax;
}

bool Warcaby::Plansza::czy_damka(int kolor, int j)
{
	if (kolor == bialy && j == 0)
		return true;
	if (kolor == czarny && j == 7)
		return true;
	return false;
}

void Warcaby::Plansza::AI()
{
	int score = 0, best = 1000;
	bestmove->global_best = 1000;
	for (int id = 0; id < 64; id++)
	{
		score = minmax_move(id, 0, czarny, -1000, 1000, false);
	}
	int end = bestmove->end_id;
	int start = bestmove->start_id;

	pionki[_j(end)][_i(end)] = pionki[_j(start)][_i(start)];
	pionki[_j(start)][_i(start)] = 0;
	b[_j(end), _i(end)]->Image = b[_j(start), _i(start)]->Image;
	b[_j(start), _i(start)]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Czarne pole.png")));

	String^ tekst;
	tekst = tab_index(start);
	tekst += "->";
	tekst += tab_index(end);

	//ponizsze warunki zapewniaja mechanike zbicia (usuniecia) pionka
	
		if (bestmove->czy_bicie)
		{
			int road = abs((_j(start) - _j(end))), road2 = abs((_i(start) - _i(end)));
			pionki[_j(end) + (_j(start) - _j(end)) / road][_i(end) + (_i(start) - _i(end)) / road2] = 0;
			b[_j(end) + (_j(start) - _j(end)) / road, _i(end) + (_i(start) - _i(end)) / road2]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Czarne pole.png")));

			int temporary = pionki[_j(start)][_i(start)];

			//bicie sekwencyjne
			while(czy_jest_bicie(czarny, end, end + 1))
			{
				if (czy_jest_bicie(czarny, end, end + 1)) {
					bestmove->global_best = 1000;
					minmax_move(end, 0, czarny, -1000, 1000, true);
					end = bestmove->end_id;
					start = bestmove->start_id;
					
					pionki[_j(end)][_i(end)] = -1;
					pionki[_j(start)][_i(start)] = 0;
					b[_j(end), _i(end)]->Image = b[_j(start), _i(start)]->Image;
					b[_j(start), _i(start)]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Czarne pole.png")));
					if (bestmove->czy_bicie)
					{
						int road = abs((_j(start) - _j(end))), road2 = abs((_i(start) - _i(end)));
						pionki[_j(end) + (_j(start) - _j(end)) / road][_i(end) + (_i(start) - _i(end)) / road2] = 0;
						b[_j(end) + (_j(start) - _j(end)) / road, _i(end) + (_i(start) - _i(end)) / road2]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\Czarne pole.png")));
					}
					tekst += "->";
					tekst += tab_index(end);
				}
			} 

			pionki[_j(end)][_i(end)];
		}
	//tworzenie damki
	if (czy_damka(czarny, _j(end)))
	{
		pionki[_j(end)][_i(end)] = -2;
		b[_j(end), _i(end)]->Image = (cli::safe_cast<System::Drawing::Image^>(Image::FromFile("..\\Pictures\\damka czarna.png")));
	}

	this->textBox2->Text = System::Convert::ToString(bestmove->global_best); //wartosc minimalizatora oszacowana w symulacji
	this->listBox1->Items->Insert(0, tekst);								 //tworzenie historii ruchow minimalizatora
}