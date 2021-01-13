#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <windows.h>
#include <string>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
#include <conio.h>

char choice;
char liczby[8];
char znakiMat[8];
char wybraneKarty[8];
bool again = true; //to bêdzie zmienna do wykonywania siê naszej pêtli 'g³ównej' while do gry
char znaki[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' }; //na pocz¹tek jeden zestaw znaków
char zestawKart[4][4]; //wylosowany zestaw kart
char realTimePlansza[4][4];
float pozostalyCzas = 60;
clock_t t1;
clock_t t2;
time_t s1;
time_t s2;

using namespace std;


void losowanie(char znaki[8], char zestawKart[4][4])
{
	int numerZnaku; //numer znaku wed³ug tablicy znaki[8]
	int uzycieZnakow[8];
	int liczbaPozZnak = 7; //mamy 8 liczb, bo jak by³o 8,
					   //to wyrzuca³o tam dalej b³¹d przy rand % (liczbaPozZnak-1)
					   //¿e dzielenie przez zero czy coœ, idk, dziwne

//tak¿e liczba znaków (liczbaPozZnak i wielkoœæ tablicy znaki) = N*M/2 - 1
//gdzie NxM, to wymiar tablicy. 
//Zauwa¿my ¿e N*M to musi byæ parzysta liczba

	for (int i = 0; i < 8; i++) { //u¿ycie liczymy od zera, czyli jak ==2, to ju¿ tego znaku nie u¿ywamy

		uzycieZnakow[i] = 0;
	}
	cout << endl;

	//LOSOWANIE ROZ£O¯ENIA KART

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			numerZnaku = rand() % (liczbaPozZnak + 1);
			zestawKart[i][j] = znaki[numerZnaku];
			uzycieZnakow[numerZnaku]++;
			// cout << zestawKart[j][i] << " u¿ycie " << uzycieZnakow[numerZnaku] << endl

			if (uzycieZnakow[numerZnaku] == 2) {
				if (numerZnaku < liczbaPozZnak) { //< bo indeksy i liczba znakow sie nie pokrywaj¹
					for (int k = numerZnaku; k < liczbaPozZnak; k++) {

						znaki[k] = znaki[k + 1]; //pomijamy ju¿ u¿yty dwa razy znak i przesuwamy resztê w lewo o jeden
						uzycieZnakow[k] = uzycieZnakow[k + 1];
					}
				}
				//jeœli numerZnaku == liczbaPozZnak, czyli numer znaku jest ostatnim dostêpnym numerem, to nic nie robimy
				//bo nie ma potrzeby
				liczbaPozZnak--;
			}
			/*
			for (int i = 0; i < 8; i++)
			{
				cout << uzycieZnakow[i];
			}
			cout << endl;
			for (int i = 0; i < 8; i++)
			{
				cout << znaki[i];
			}
			cout << endl;
			cout << liczbaPozZnak << endl;
			//kontrolne ^
			*/
			if (liczbaPozZnak == -1) { //-1, bo nasza liczbaPozZnak wynosi 7 zamiast 8

				zestawKart[i][j] = znaki[0];

			}
		}
	}
	/*
	//kontrolny podgl¹d wylosowanego uk³adu kart
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << zestawKart[i][j] << " ";

		}
		cout << endl;
	}
	*/
}
void plansza(char tablica[4][4], int I, int licznik)
{
	cout << "                       Liczba krokow: " << licznik << endl;
	cout << "                       Trafienia: " << I << endl;
	cout << "--------------------------------------------------------------" << endl << endl;
	cout << "   | 0 | 1 | 2 | 3 |\n";
	cout << "  ------------------\n";
	int i;
	for (i = 0; i < 4; i++) { //przechodzimy do kolejnego wiersza

		cout << " " << i << " | ";
		for (int j = 0; j < 4; j++) { // przechodzimy po kolejnych elementach w wierszu

			cout << tablica[i][j] << " | ";

		}
		cout << "\n  -------------------\n";
	}
}

void easy(char realTimePlansza[4][4], char zestawKart[4][4])
{
	int I = 0; //wielki i potê¿ny licznik naszej pêli. Pêtla siê koñczy, kiedy ju¿ wszystkie karty odkryjemy
			   //czyli kiedy bêdziemy mieæ 8 par
	int licznikKrokow = 0;
	do {
		int x1;
		int y1;
		int x2;
		int y2;

		plansza(realTimePlansza, I, licznikKrokow);
		cout << "I karta: " << endl;
		cout << "x: ";
		cin >> x1;
		cout << "y: ";
		cin >> y1;
		realTimePlansza[y1][x1] = zestawKart[y1][x1];

		system("cls");

		plansza(realTimePlansza, I, licznikKrokow);

		cout << "II karta: " << endl;
		cout << "x: ";
		cin >> x2;
		cout << "y: ";
		cin >> y2;
		realTimePlansza[y2][x2] = zestawKart[y2][x2];
		licznikKrokow++;

		system("cls");
		if (zestawKart[y1][x1] != zestawKart[y2][x2]) { //jeœli nietrafione, to znowu zakrywamy

			plansza(realTimePlansza, I, licznikKrokow);
			cout << endl << "Pudlo! Wpisz dowolny znak by kontynuowaæ" << endl;
			cin >> choice;
			system("cls");
			realTimePlansza[y1][x1] = '0';
			realTimePlansza[y2][x2] = '0';
		}
		else { //jeœli trafione, to powiêkszamy licznik pêtli i pomijamy ponowne drukowanie planszy

			I++;
			realTimePlansza[y1][x1] = ' ';
			realTimePlansza[y2][x2] = ' ';
			cout << "--------------------------------------------------------------" << endl;
			cout << "                       TRAFIONY ZATOPIONY!                    " << endl;
			cout << "--------------------------------------------------------------" << endl;
			this_thread::sleep_for(chrono::seconds(2));
			system("cls");
			continue;
		}

	} while (I < 8);

	cout << "--------------------------------------------------------------" << endl;
	cout << "                             WYGRA£EŒ!                       " << endl;
	cout << "                          w" << licznikKrokow << "krokach" << endl;
	cout << "--------------------------------------------------------------" << endl;
	plansza(realTimePlansza, I, licznikKrokow);
	cout << endl << "Pudlo! Wpisz dowolny znak by kontynuowaæ" << endl;
	cin >> choice;

}

int uplywCzasu(clock_t t1, clock_t t2) {
	float diff = 0;
	cout << t1;
	cout << t2;

	diff = t2 - t1;
	cout << diff << endl;
	float seconds = diff / CLOCKS_PER_SEC;
	cout << seconds << endl;
	pozostalyCzas -= seconds;
	cout << pozostalyCzas << endl;

	return pozostalyCzas;
}

void hard(char realTimePlansza[4][4], char zestawKart[4][4]) {
	int I = 0; //wielki i potê¿ny licznik naszej pêli. Pêtla siê koñczy, kiedy ju¿ wszystkie karty odkryjemy
		   //czyli kiedy bêdziemy mieæ 8 par
	int licznikKrokow = 0;

	do {
		int x1;
		int y1;
		int x2;
		int y2;

		t1 = clock();
		cout << "                       Pozostaly czas: " << pozostalyCzas << endl;
		plansza(realTimePlansza, I, licznikKrokow);
		cout << "I karta: " << endl;
		cout << "x: ";
		cin >> x1;
		cout << "y: ";
		cin >> y1;
		realTimePlansza[y1][x1] = zestawKart[y1][x1];
		system("cls");

		t2 = clock();
		pozostalyCzas = uplywCzasu(t1, t2);
		t1 = t2;


		cout << "                       Pozostaly czas: " << pozostalyCzas << endl;
		plansza(realTimePlansza, I, licznikKrokow);
		cout << "II karta: " << endl;
		cout << "x: ";
		cin >> x2;
		cout << "y: ";
		cin >> y2;
		realTimePlansza[y2][x2] = zestawKart[y2][x2];
		licznikKrokow++;
		system("cls");

		t2 = clock();
		pozostalyCzas = uplywCzasu(t1, t2);
		t1 = t2;

		if (zestawKart[y1][x1] != zestawKart[y2][x2]) { //jeœli nietrafione, to znowu zakrywamy
			cout << "                       Pozostaly czas: " << pozostalyCzas << endl;
			plansza(realTimePlansza, I, licznikKrokow);

			cout << endl << "Pudlo! Wpisz dowolny znak by kontynuowaæ" << endl;
			cin >> choice;
			system("cls");
			realTimePlansza[y1][x1] = '0';
			realTimePlansza[y2][x2] = '0';
		}
		else { //jeœli trafione, to powiêkszamy licznik pêtli i pomijamy ponowne drukowanie planszy

			I++;
			realTimePlansza[y1][x1] = ' ';
			realTimePlansza[y2][x2] = ' ';
			cout << "--------------------------------------------------------------" << endl;
			cout << "                       TRAFIONY ZATOPIONY!                    " << endl;
			cout << "--------------------------------------------------------------" << endl;
			this_thread::sleep_for(chrono::seconds(2));
			system("cls");
			continue;
		}
		t2 = clock();
		pozostalyCzas = uplywCzasu(t1, t2);
		t1 = t2;
	} while (I < 8 & pozostalyCzas>0);
	if (I == 8) {
		cout << "--------------------------------------------------------------" << endl;
		cout << "                             WYGRALES!                       " << endl;
		cout << "                          w" << licznikKrokow << "krokach" << endl;
		cout << "--------------------------------------------------------------" << endl;
		plansza(realTimePlansza, I, licznikKrokow);
		cout << endl << "Wpisz dowolny znak by kontynuowaæ" << endl;
		cin >> choice;
	}
	else {
		cout << "--------------------------------------------------------------" << endl;
		cout << "                           PRZEGRALES! :(                      " << endl;
		cout << "                           Czas min¹³!" << endl;
		cout << "--------------------------------------------------------------" << endl;
		cout << endl << "Wpisz dowolny znak by przejœc do menu" << endl;
		cin >> choice;
	}

}

void logo() {
	cout << "Oo      oO o.OOoOoo Oo      oO  .oOOOo.  `OooOOo.  o       O " << endl;
	cout << "O O    o o  O       O O    o o .O     o.  o     `o O       o " << endl;
	cout << "o  o  O  O  o       o  o  O  O O       o  O      O `o     O' " << endl;
	cout << "O   Oo   O  ooOO    O   Oo   O o       O  o     .O   O   o   " << endl;
	cout << "O        o  O       O        o O       o  OOooOO'     `O'    " << endl;
	cout << "o        O  o       o        O o       O  o    o       o     " << endl;
	cout << "o        O  O       o        O `o     O'  O     O      O     " << endl;
	cout << "O        o ooOooOoO O        o  `OoooO'   O      o     O     " << endl << endl;
}

void wybierzKarty() {
	system("cls");
	choice = 0;
	cout << "                       Wybierz zestaw kart" << endl;
	cout << "--------------------------------------------------------------" << endl;
	cout << "                       1. Literki" << endl;
	cout << "                       2. Cyferki" << endl;
	cout << "                       3. Matma style" << endl;
	cout << "                       Wybor: ";
	cin >> choice;

	switch (choice) {
	case '1':
		for (int j = 0; j < 8; j++)
		{
			wybraneKarty[j] = znaki[j];
		}
		break;

	case '2':
		for (int j = 0; j < 8; j++)
		{
			wybraneKarty[j] = liczby[j];

		}
		break;

	case '3':
		for (int j = 0; j < 8; j++)
		{
			wybraneKarty[j] = znakiMat[j];
		}
		break;

	default: break;
	}
}
void odliczanie() {
	system("cls");
	cout << "                             GRAMY!" << endl;
	cout << "                     " << " przygotuj sie" << endl;
	cout << "                               3" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	system("cls");
	cout << "                             GRAMY!" << endl;
	cout << "                     " << " przygotuj sie" << endl;
	cout << "                               2" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	system("cls");
	cout << "                             GRAMY!" << endl;
	cout << "                     " << " przygotuj sie" << endl;
	cout << "                               1" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	system("cls");
}


int main() {
	srand(time(NULL));
	string name;


	for (int i = 0; i < 4; i++) { //ogarniêcie real time planszy, rewers obecnie stanowi¹ "0"
		for (int j = 0; j < 4; j++) {
			realTimePlansza[i][j] = '0';
		}
	}

	ifstream wczytBiblLiczby;
	wczytBiblLiczby.open("bibliotekaLiczby.txt");
	if (wczytBiblLiczby.good())
	{
		for (int j = 0; j < 8; j++)
		{
			wczytBiblLiczby >> liczby[j];
			//cout << liczby[j] << " ";
		}
	}
	else
	{
		cout << "Nie udalo sie otworzyc" << endl;
	}
	wczytBiblLiczby.close();

	ifstream wczytBiblZnaki;
	wczytBiblZnaki.open("bibliotekaZnaki.txt");
	if (wczytBiblZnaki.good())
	{
		for (int j = 0; j < 8; j++)
		{
			wczytBiblZnaki >> znakiMat[j];
			//cout << znakiMat[j] << " ";
		}
	}
	else
	{
		cout << "Nie udalo sie otworzyc" << endl;
	}
	wczytBiblZnaki.close();


	logo();
	cout << "                             Witaj!" << endl;
	cout << "--------------------------------------------------------------" << endl;
	cout << "               Wpisz swoje imie: ";
	cin >> name;
	system("cls");

	while (again) {

		logo();
		cout << "                       Witaj " << name << "!" << endl;
		cout << "--------------------------------------------------------------" << endl;
		cout << "                       1. Rozpocznij gre" << endl;
		cout << "                       2. Wyjdz" << endl;
		cout << "                       Wybor: ";
		cin >> choice;
		system("cls");

		if (choice == '1') {

			choice = 0;
			cout << "                       Wybierz poziom" << endl;
			cout << "--------------------------------------------------------------" << endl;
			cout << "                       E. Easy" << endl;
			cout << "                       H. Hard" << endl;
			cout << "                       P. Punktowany" << endl;
			cout << "                       Wybor: ";
			cin >> choice;

			switch (choice) {
			case 'E':;
			case 'e':
				wybierzKarty();
				losowanie(wybraneKarty, zestawKart);
				odliczanie();
				easy(realTimePlansza, zestawKart);

				break;

			case 'H':;
			case 'h':
				wybierzKarty();
				losowanie(wybraneKarty, zestawKart);
				odliczanie();
				hard(realTimePlansza, zestawKart);
				break;
			case 'P':;
			case 'p':
				wybierzKarty();
				losowanie(wybraneKarty, zestawKart);
				odliczanie();
				easy(realTimePlansza, zestawKart);
				break;

			default: break;
			}

		}
		else if (choice == '2') {
			again = false;
		}

	}

	return 0;
}