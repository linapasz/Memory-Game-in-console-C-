#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <windows.h>
#include <string>
#include <thread>         // std::this_thread::sleep_for


char choice;
char liczby[8];
char znakiMat[8];
char wybraneKarty[8];
bool again = true; //to bêdzie zmienna do wykonywania siê naszej pêtli 'g³ównej' while do gry
char znaki[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' }; //na pocz¹tek jeden zestaw znaków
char zestawKart[4][4]; //wylosowany zestaw kart
char realTimePlansza[4][4];
float pozostalyCzas = 300;
bool blad;
clock_t t1;
clock_t t2;
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
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
	cout << "   |  0  |  1  |  2  |  3  |\n";
	cout << "  ---------------------------\n";
	int i;
	for (i = 0; i < 4; i++) { //przechodzimy do kolejnego wiersza

		cout << " " << i << " | ";
		for (int j = 0; j < 4; j++) { // przechodzimy po kolejnych elementach w wierszu

			if (tablica[i][j] == 'O') {//¿eby by³o niebieskie t³o z bia³¹ czcionk¹
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
				cout << " ";
				cout << tablica[i][j];
				cout << " ";
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				cout << " | ";
			}
			//BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED
			else if (tablica[i][j] == ' ') { //¿eby by³o czarne
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
				cout << " ";
				cout << tablica[i][j];
				cout << " ";
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				cout << " | ";
			}
			else {//¿eby by³o szare z niebiesk¹ czcionk¹
				SetConsoleTextAttribute(hOut, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
				cout << " ";
				cout << tablica[i][j];
				cout << " ";
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				cout << " | ";
			}


		}
		cout << endl << "  ---------------------------\n";
	}
}

void easy(char realTimePlansza[4][4], char zestawKart[4][4], string name)
{
	int I = 0; //wielki i potê¿ny licznik naszej pêli. Pêtla siê koñczy, kiedy ju¿ wszystkie karty odkryjemy
			   //czyli kiedy bêdziemy mieæ 8 par
	int licznikKrokow = 0;
	do {
		int x1;
		int y1;
		int x2;
		int y2;

		do {
			plansza(realTimePlansza, I, licznikKrokow);
			cout << "I karta: " << endl;
			cout << "x: ";
			cin >> x1;
			cout << "y: ";
			cin >> y1;

			if ((x1 > 3 || x1 < 0) || (y1 > 3 || y1 < 0)) {
				cout << "\nNie ma takiego pola!\n";
				this_thread::sleep_for(chrono::seconds(1));
				system("cls");
				blad = true;
			}
			else if (realTimePlansza[y1][x1] == ' ') {//kiedy by³a ju¿ odgadniêta
				cout << "\nJuz wybrales ta karte!\n";
				this_thread::sleep_for(chrono::seconds(1));
				system("cls");
				blad = true;
			}
			else {
				blad = false;
			}
		} while (blad == true);

		realTimePlansza[y1][x1] = zestawKart[y1][x1];

		system("cls");

		do {
			plansza(realTimePlansza, I, licznikKrokow);
			cout << "II karta: " << endl;
			cout << "x: ";
			cin >> x2;
			cout << "y: ";
			cin >> y2;

			if ((x2 > 3 || x2 < 0) || (y2 > 3 || y2 < 0)) {
				cout << "\nNie ma takiego pola!\n";
				this_thread::sleep_for(chrono::seconds(1));
				system("cls");
				blad = true;
			}
			else if (realTimePlansza[y2][x2] == ' ' || (x2 == x1 && y1 == y2)) { //kiedy by³a ju¿ odkryta lub I karta to by³a ta
				cout << "\nJuz wybrales ta karte!\n";
				this_thread::sleep_for(chrono::seconds(1));
				system("cls");
				blad = true;
			}
			else {
				blad = false;
			}
		} while (blad == true);

		realTimePlansza[y2][x2] = zestawKart[y2][x2];
		licznikKrokow++;

		system("cls");
		if (zestawKart[y1][x1] != zestawKart[y2][x2]) { //jeœli nietrafione, to znowu zakrywamy

			plansza(realTimePlansza, I, licznikKrokow);
			cout << endl << "Pudlo! Wpisz dowolny znak by kontynuowaæ" << endl;
			cin >> choice;
			system("cls");
			realTimePlansza[y1][x1] = 'O';
			realTimePlansza[y2][x2] = 'O';
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

	int punkty = licznikKrokow;
	cout << "--------------------------------------------------------------" << endl;
	cout << "                             WYGRA£EŒ!                       " << endl;
	cout << "                          w" << licznikKrokow << "krokach" << endl;
	cout << "                          punkty: " << punkty << endl;
	cout << "--------------------------------------------------------------" << endl;

	fstream newfile;
	newfile.open("RankingEasy.txt");
	if (newfile.is_open()) {
		newfile << name << " " << punkty << "pkt";
		newfile.close();
	}

	plansza(realTimePlansza, I, licznikKrokow);
	cout << endl << "Pudlo! Wpisz dowolny znak by kontynuowaæ" << endl;
	cin >> choice;
}

int uplywCzasu(clock_t t1, clock_t t2) { //obliczanie pozostalego czasu
	float diff = 0;
	//cout << t1;
	//cout << t2;

	diff = t2 - t1;
	//cout << diff << endl;
	float seconds = diff / CLOCKS_PER_SEC;
	//cout << seconds << endl;
	pozostalyCzas -= seconds;
	//cout << pozostalyCzas << endl;

	return pozostalyCzas;
}

void hard(char realTimePlansza[4][4], char zestawKart[4][4], string name) {
	int I = 0; //wielki i potê¿ny licznik naszej pêli. Pêtla siê koñczy, kiedy ju¿ wszystkie karty odkryjemy
		   //czyli kiedy bêdziemy mieæ 8 par
	int licznikKrokow = 0;

	do {
		int x1;
		int y1;
		int x2;
		int y2;

		t1 = clock();//pobieranie pierwszego czasu
		cout << "                       Pozostaly czas: " << pozostalyCzas << endl;
		do {
			plansza(realTimePlansza, I, licznikKrokow);
			cout << "I karta: " << endl;
			cout << "x: ";
			cin >> x1;
			cout << "y: ";
			cin >> y1;

			if ((x1 > 3 || x1 < 0) || (y1 > 3 || y1 < 0)) {
				cout << "\nNie ma takiego pola!\n";
				this_thread::sleep_for(chrono::seconds(1));
				system("cls");
				blad = true;
			}
			else if (realTimePlansza[y1][x1] == ' ') {
				cout << "\nJuz wybrales ta karte!\n";
				this_thread::sleep_for(chrono::seconds(1));
				system("cls");
				blad = true;
			}
			else {
				blad = false;
			}
		} while (blad == true);

		realTimePlansza[y1][x1] = zestawKart[y1][x1];
		system("cls");

		t2 = clock();//drugi czas
		pozostalyCzas = uplywCzasu(t1, t2);//obliczanie czasu pozosta³ego
		t1 = t2;//drugi czas staje siê pierwszym do kolejnego obliczenia czasu

		do {
			cout << "                       Pozostaly czas: " << pozostalyCzas << " s" << endl;
			plansza(realTimePlansza, I, licznikKrokow);
			cout << "II karta: " << endl;
			cout << "x: ";
			cin >> x2;
			cout << "y: ";
			cin >> y2;

			if ((x2 > 3 || x2 < 0) || (y2 > 3 || y2 < 0)) {
				cout << "\nNie ma takiego pola!\n";
				this_thread::sleep_for(chrono::seconds(1));
				system("cls");
				blad = true;
			}
			else if (realTimePlansza[y2][x2] == ' ' || (x2 == x1 && y1 == y2)) {
				cout << "\nJuz wybrales ta karte!\n";
				blad = true;
			}
			else {
				blad = false;
			}
		} while (blad == true);

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
			realTimePlansza[y1][x1] = 'O';
			realTimePlansza[y2][x2] = 'O';
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
		float kroki = licznikKrokow;
		cout << endl << kroki << endl;
		float punkty = kroki + 300 - pozostalyCzas;

		cout << "--------------------------------------------------------------" << endl;
		cout << "                             WYGRALES!                       " << endl;
		cout << "                          w" << licznikKrokow << "krokach" << endl;
		cout << "                          punkty: " << punkty << endl;
		cout << "--------------------------------------------------------------" << endl;
		plansza(realTimePlansza, I, licznikKrokow);
		cout << endl << "Wpisz dowolny znak by kontynuowaæ" << endl;
		cin >> choice;

		fstream newfile;
		newfile.open("RankingHard.txt");
		if (newfile.is_open()) {
			newfile << name << " " << punkty << "pkt.\n";
			newfile.close();
		}

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

void logo() {			//wczytywanie logo MEMORY z pliku tekstowego
	fstream newfile;
	newfile.open("MemoryLogo.txt");
	if (newfile.is_open()) {
		string Logo;
		while (getline(newfile, Logo)) {
			cout << Logo << endl;
		}
		newfile.close();
	}
}

void wybierzKarty() {
	system("cls");

	do {
		choice = 0;
		cout << "                       Wybierz zestaw kart" << endl;
		cout << "--------------------------------------------------------------" << endl << endl;
		cout << "                       1. Literki" << endl;
		cout << "                       2. Cyferki" << endl;
		cout << "                       3. Matma style" << endl << endl;
		cout << "                       Wybor: ";
		cin >> choice;

		if ((choice != '1') && (choice != '2') && (choice != '3')) {
			cout << "\nNie ma takiej opcji!" << endl;
			this_thread::sleep_for(chrono::seconds(1));
			system("cls");
			blad = true;
		}
		else {
			blad = false;
		}
	} while (blad == true);

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

void odliczanie() {//odliczanie do rozpoczecia gry
	for (int i = 3; i > 0; i--) {
		system("cls");
		cout << "\t   GRAMY!" << endl;
		cout << "\tprzygotuj sie" << endl;
		cout << "\t     " << i << endl;
		Sleep(1000);
	}
	system("cls");
}


int main() {
	srand(time(NULL));
	string name;


	for (int i = 0; i < 4; i++) { //ogarniêcie real time planszy, rewers obecnie stanowi¹ "0"
		for (int j = 0; j < 4; j++) {
			realTimePlansza[i][j] = 'O';
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
		cout << "Nie udalo sie wczytac znakow" << endl;
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
		cout << "Nie udalo sie wczytac znakow" << endl;
	}
	wczytBiblZnaki.close();


	logo();
	cout << endl << "                             Witaj!" << endl;
	cout << "--------------------------------------------------------------" << endl;
	cout << "               Wpisz swoje imie: ";
	cin >> name;
	system("cls");
	choice = 1;

	while (again) {
		switch (choice) {
		case 1: {
			system("cls");
			logo();
			cout << endl << "                       Witaj " << name << "!" << endl;
			cout << "--------------------------------------------------------------" << endl;
			cout << "                       1. Rozpocznij gre" << endl;
			cout << "                       2. Wyjdz" << endl;
			cout << "                       3. Ranking" << endl;
			cout << "                       Wybor: ";
			cin >> choice;
			break;
		}
		default: {
			cout << "                           SZYBKIE MENU" << endl;
			cout << "--------------------------------------------------------------" << endl;
			cout << "                       1. Rozpocznij gre" << endl;
			cout << "                       2. Wyjdz" << endl;
			cout << "                       Wybor: ";
			cin >> choice;
		}
		}


		if ((choice != '1') && (choice != '2') && (choice != '3')) {		//sprawdzam czy jest opcja pasujaca do podanego znaku
			cout << "\nNie ma takiej opcji!" << endl;
			return 0;
		}
		system("cls");

		if (choice == '1') {

			do {
				choice = 0;
				cout << "                       Wybierz poziom." << endl;
				cout << "--------------------------------------------------------------" << endl;
				cout << "\t\t  Wpisz odpowiednia litere:\n" << endl;
				cout << "                       \"E\" dla Easy" << endl;
				cout << "                       \"H\" dla Hard" << endl;
				cout << endl << "                       Wybor: ";
				cin >> choice;

				if (choice != 'E' && choice != 'e' && choice != 'h' && choice != 'H') {
					cout << "\nNie ma takiego trybu !" << endl;
					this_thread::sleep_for(chrono::seconds(1));
					system("cls");
					blad = true;
				}
				else {
					blad = false;
				}
			} while (blad == true);

			switch (choice) {
			case 'E':;
			case 'e':
				wybierzKarty();
				losowanie(wybraneKarty, zestawKart);
				odliczanie();
				easy(realTimePlansza, zestawKart, name);
				choice = 1;
				break;

			case 'H':;
			case 'h':
				wybierzKarty();
				losowanie(wybraneKarty, zestawKart);
				odliczanie();
				hard(realTimePlansza, zestawKart, name);
				choice = 1;
				break;

			default: choice = 1; break;
			}
		}
		if (choice == '2') {
			again = false;
		}
		else if (choice == '3') {
			cout << endl << "Ranking graczy trybow EASY oraz HARD" << endl;
			cout << "--------------------------------------------------------------" << endl;
			cout << "\t\tEASY" << endl;

			fstream EASY;
			EASY.open("RankingEasy.txt");
			if (EASY.is_open()) {
				string Easy;
				while (getline(EASY, Easy)) {
					cout << Easy << endl;
				}
				EASY.close();
			}

			cout << "--------------------------------------------------------------";
			cout << endl << "\t\tHARD" << endl;

			fstream HARD;
			HARD.open("RankingHard.txt");
			if (HARD.is_open()) {
				string Hard;
				while (getline(HARD, Hard)) {
					cout << Hard << endl;
				}
				HARD.close();
			}
			cout << "--------------------------------------------------------------" << endl;
			choice = 0;
		}
	}


	return 0;
}