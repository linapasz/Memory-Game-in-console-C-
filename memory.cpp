#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <thread>         
#include <conio.h>

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
clock_t t1;//do odliczania czasu
clock_t t2;
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//do koloru
using namespace std;

typedef struct cords {//do przechowywania indeksów wybieranych kart oraz ich rzeczywistych wspó³rzêdnych
	int a;
	int b;
	int aReal;
	int bReal;
};

void getxy(int& a, int& b) {//pobieramy wspolrzedne naszego wskaznika
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	a = csbi.dwCursorPosition.X;
	b = csbi.dwCursorPosition.Y;
	return;
}

void gotoxy(int x, int y) {//przemieszczamy wskaznik
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}

int kursorMenu() {
	int check = 0, x, y;
	gotoxy(25, 11);
	while (!(check == 13)) {
		check = 0;
		getxy(x, y);//= 34, y=14
		switch ((check = _getch())) {//poruszamy sie do gory i do dolu
		case 72:
			gotoxy(25, y - 1);//do dolu
			break;
		case 80:
			gotoxy(25, y + 1);//do gory
			break;
		}
		getxy(x, y);

		if (y == 13) {//w zaleznosci co wybralismy
			choice = '3';
		}
		else if (y == 12) {
			choice = '2';
		}
		else if (y == 11) {
			choice = '1';
		}
		else choice = '10';
	}
	return choice;
}
cords kursorPlansza(int q, int z) {
	cords cords1 = { 0,0, 0, 0 };
	int check = 0, x, y;
	gotoxy(q, z);
	while (!(check == 13)) {
		check = 0;
		getxy(x, y);
		switch ((check = _getch())) {
		case 72:
			gotoxy(x, y - 2);
			break;
		case 80:
			gotoxy(x, y + 2);
			break;
		case 75:
			gotoxy(x - 6, y);
			break;
		case 77:
			gotoxy(x + 6, y);
			break;
		}
		cords1.bReal = y; //rzeczywiste po³o¿enie wskaznika
		cords1.aReal = x;
		switch (x) { //przerabiamy rzeczywiste wspolrzedne na indeksy do tablicy
		case 23: cords1.a = 0; break;
		case 29: cords1.a = 1; break;
		case 35: cords1.a = 2; break;
		case 41: cords1.a = 3; break;
		default: cords1.a = -1; break;
		}
		switch (y) {
		case 7: cords1.b = 0; break;
		case 9: cords1.b = 1; break;
		case 11: cords1.b = 2; break;
		case 13: cords1.b = 3; break;
		default: cords1.b = -1; break;
		}
	}
	return cords1;
}

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
	cout << "                    |  0  |  1  |  2  |  3  |\n";
	cout << "                 ------------------------------\n";
	int i;
	for (i = 0; i < 4; i++) { //przechodzimy do kolejnego wiersza
		cout << "                 ";
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
		cout << endl << "                 ------------------------------\n";
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
		cords lokalne;//nasza struktura do wskaŸników
		do {
			cout << endl; //ISTOTNE DO WSKAZNIKA, NIE USUWAC
			plansza(realTimePlansza, I, licznikKrokow);
			cout << "Wybierz karty" << endl;
			lokalne = kursorPlansza(23, 7); //tak, aby kursor wskazywa³ pierwszy obiekt na planszy, dlatego wywolujemy z x=23 i y=7
			x1 = lokalne.a; //przekazuje informacje o wspolrzednych wybranej karty
			y1 = lokalne.b; //same here

			if (x1 < 0 || y1 < 0) {
				SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "\nNie ma takiego pola!\n";
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				this_thread::sleep_for(chrono::seconds(1)); //usypianie programu na 1 sekundê
				system("cls");
				blad = true;
			}
			else if (realTimePlansza[y1][x1] == ' ') {//kiedy by³a ju¿ odgadniêta
				SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "\nJuz wybrales ta karte!\n";
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				this_thread::sleep_for(chrono::seconds(1));
				system("cls");
				blad = true;
			}
			else {
				blad = false; //wybralismy poprawna karte - wychodzimy z pêtli
			}
		} while (blad == true);//dopoki nie wybierzemy poprawnej karty blad == true i petla sie wykonuje

		realTimePlansza[y1][x1] = zestawKart[y1][x1];
		system("cls");

		do {
			cout << endl; //ISTOTNE DO WSKAZNIKA, NIE USUWAC
			plansza(realTimePlansza, I, licznikKrokow);
			cout << "Wybierz karty" << endl;
			lokalne = kursorPlansza(lokalne.aReal, lokalne.bReal);
			x2 = lokalne.a;
			y2 = lokalne.b;

			if ((x2 > 3 || x2 < 0) || (y2 > 3 || y2 < 0)) {
				SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "\nNie ma takiego pola!\n";
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				this_thread::sleep_for(chrono::seconds(1));
				system("cls");
				blad = true;
			}
			else if (realTimePlansza[y2][x2] == ' ' || (x2 == x1 && y1 == y2)) { //kiedy by³a ju¿ odkryta lub I karta to by³a ta
				SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "\nJuz wybrales ta karte!\n";
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
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
			cout << endl << "Nacisnij enter by kontynuowac" << endl;
			_getch();
			system("cls");

			realTimePlansza[y1][x1] = 'O';
			realTimePlansza[y2][x2] = 'O';
		}
		else { //jeœli trafione, to powiêkszamy licznik pêtli i pomijamy ponowne drukowanie planszy

			I++;
			realTimePlansza[y1][x1] = ' ';
			realTimePlansza[y2][x2] = ' ';
			cout << endl;
			cout << "--------------------------------------------------------------" << endl;
			cout << "                       TRAFIONY ZATOPIONY!                    " << endl;
			cout << "--------------------------------------------------------------" << endl;
			this_thread::sleep_for(chrono::seconds(2));
			system("cls");
			continue;
		}

	} while (I < 8); //wykonuje siê dopóki nie trafimy wszystkich 8 par kart
	//w trybie easy nie da siê przegraæ, wiêc zawsze wygrywamy
	int punkty = licznikKrokow;
	cout << endl;
	cout << "--------------------------------------------------------------" << endl;
	cout << "                             WYGRALES!                       " << endl;
	cout << "                          Punkty: " << punkty << endl;
	cout << "--------------------------------------------------------------" << endl;

	fstream newfile;
	newfile.open("RankingEasy.txt");
	if (newfile.is_open()) {
		newfile << name << " " << punkty << "pkt";
		newfile.close();
	}
	cout << endl; //ISTOTNE DO WSKAZNIKA, NIE USUWAC
	plansza(realTimePlansza, I, licznikKrokow);
	cout << endl << "Pudlo! Nacisnij enter by kontunuowac" << endl;
	_getch();
}

int uplywCzasu(clock_t t1, clock_t t2) { //obliczanie pozostalego czasu
	float diff = 0;
	diff = t2 - t1; //ró¿nica pomiêdzy czasem z 1. pomiaru a z 2. pomiaru
	float seconds = diff / CLOCKS_PER_SEC; //przeliczenie na sekundy
	pozostalyCzas -= seconds; //odejmujemy ile czasu ju¿ uplynê³o i dostajemy pozosta³y nam czas
	return pozostalyCzas;//zwracamy wartoœæ, ¿eby móc j¹ wyœwietliæ po wyjœciu z funkcji
}

void hard(char realTimePlansza[4][4], char zestawKart[4][4], string name) { //analogicznie do easy() + odliczanie czasu
	int I = 0;
	int licznikKrokow = 0;
	do {
		int x1;
		int y1;
		int x2;
		int y2;
		cords lokalne;
		t1 = clock();//pobieranie pierwszego czasu

		cout << "                       Pozostaly czas: " << pozostalyCzas << endl;
		do {
			plansza(realTimePlansza, I, licznikKrokow);
			cout << "Wybierz karty" << endl;
			lokalne = kursorPlansza(23, 7);
			x1 = lokalne.a;
			y1 = lokalne.b;

			if ((x1 > 3 || x1 < 0) || (y1 > 3 || y1 < 0)) {
				SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "\nNie ma takiego pola!\n";
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				this_thread::sleep_for(chrono::seconds(1));
				system("cls");
				blad = true;
			}
			else if (realTimePlansza[y1][x1] == ' ') {
				SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "\nJuz wybrales ta karte!\n";
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
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
		pozostalyCzas = uplywCzasu(t1, t2);//przepisanie wartoœci pozosta³ego czasu z funkcji
		t1 = t2;//drugi czas staje siê pierwszym do kolejnego obliczenia czasu
		if (pozostalyCzas <= 0) {//monitorowanie czasu
			goto przekroczenieCzasu;
		}

		do {
			cout << "                       Pozostaly czas: " << pozostalyCzas << " s" << endl;
			plansza(realTimePlansza, I, licznikKrokow);
			cout << "Wybierz karty" << endl;
			lokalne = kursorPlansza(lokalne.aReal, lokalne.bReal);
			x2 = lokalne.a;
			y2 = lokalne.b;

			if ((x2 > 3 || x2 < 0) || (y2 > 3 || y2 < 0)) {
				SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "\nNie ma takiego pola!\n";
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				this_thread::sleep_for(chrono::seconds(1));
				system("cls");
				blad = true;
			}
			else if (realTimePlansza[y2][x2] == ' ' || (x2 == x1 && y1 == y2)) {
				SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "\nJuz wybrales ta karte!\n";
				this_thread::sleep_for(chrono::seconds(1));
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
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
		if (pozostalyCzas <= 0) {
			goto przekroczenieCzasu;
		}

		if (zestawKart[y1][x1] != zestawKart[y2][x2]) { //jeœli nietrafione, to znowu zakrywamy
			cout << "                       Pozostaly czas: " << pozostalyCzas << endl;
			plansza(realTimePlansza, I, licznikKrokow);

			cout << endl << "Nacisnij enter by kontynuowac" << endl;
			_getch();

			system("cls");
			realTimePlansza[y1][x1] = 'O';
			realTimePlansza[y2][x2] = 'O';
		}
		else { //jeœli trafione, to powiêkszamy licznik pêtli i pomijamy ponowne drukowanie planszy

			I++;
			realTimePlansza[y1][x1] = ' ';
			realTimePlansza[y2][x2] = ' ';
			cout << endl;
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
		cout << endl;
		cout << "--------------------------------------------------------------" << endl;
		cout << "                             WYGRALES!                       " << endl;
		cout << "                          Punkty: " << punkty << endl;
		cout << "--------------------------------------------------------------" << endl;
		plansza(realTimePlansza, I, licznikKrokow);
		cout << endl << "Nacisnij enter by kontynuowac" << endl;
		_getch();


		fstream newfile;
		newfile.open("RankingHard.txt");
		if (newfile.is_open()) {
			newfile << name << " " << punkty << "pkt.\n";
			newfile.close();
		}

	}
	else {
	przekroczenieCzasu: //tutaj skaczemy jak przekroczymy czas
		cout << endl;
		cout << "--------------------------------------------------------------" << endl;
		cout << "                           PRZEGRALES! :(                      " << endl;
		cout << "                           Czas minal!" << endl;
		cout << "--------------------------------------------------------------" << endl;
		cout << endl << "Nacisnij enter by przejsc do menu" << endl;
		_getch();
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

void wybierzKarty() { //menu wyboru zestawu kart
	system("cls");
	do {
		choice = 0;
		logo();
		cout << endl << "                       Wybierz zestaw kart" << endl;
		cout << "--------------------------------------------------------------" << endl;
		cout << "                       1. Literki" << endl;
		cout << "                       2. Cyferki" << endl;
		cout << "                       3. Matma style" << endl << endl;
		choice = kursorMenu();

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

	switch (choice) {//wczytanie znaków do naszego roboczego zestawu znaków, który bêdziemy wykorzystywaæ do rozmieszczania znaków na planszy
					//przepisujemy je do wybraneKarty[], a nie korzystamy bezpoœrednio z nich, ¿eby by³ krótszy zapis w mainie, a nie milion switchy w switchu
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
		cout << endl << "--------------------------------------------------------------" << endl;
		cout << "                          GRAMY!" << endl;
		cout << "\t\t\tprzygotuj sie" << endl;
		cout << "\t\t\t     " << i << endl;
		cout << "--------------------------------------------------------------" << endl;
		Sleep(1000);
	}
	system("cls");
}


int main() {
	srand(time(NULL));
	string name;

	//ogarniêcie real time planszy, rewers obecnie stanowi¹ "0"
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			realTimePlansza[i][j] = 'O';
		}
	}

	//wczytywanie na samym pocz¹ku znaków do tablic
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

	//WYŒWIETLENIE GRY
	logo();
	cout << endl << "                           Witaj!" << endl;
	cout << "--------------------------------------------------------------" << endl;
	cout << "               Wpisz swoje imie: ";
	cin >> name;
	system("cls");//u¿ywamy, bo to nasz ma³y program i nic siê nie stanie
	choice = 1;//nasza zmienna do wybierania

	//pêtla do grania w nieskoñczonoœæ
	while (again) {
		system("cls");
		logo();
		cout << endl << "                         Witaj " << name << "!" << endl;
		cout << "--------------------------------------------------------------" << endl;
		cout << "                       1. Rozpocznij gre" << endl;
		cout << "                       2. Wyjdz" << endl;
		cout << "                       3. Ranking" << endl;
		choice = kursorMenu();

		system("cls");
		if (choice == '1') {//zaczynamy grê i przechodzimy do wyboru poziomu
			do {
				choice = '0';//zerujemy nasz¹ zmienn¹ wyboru
				logo();
				cout << endl << "                       Wybierz poziom." << endl;
				cout << "--------------------------------------------------------------" << endl;
				cout << "                        1. Easy" << endl;
				cout << "                        2. Hard" << endl;
				choice = kursorMenu();//wskazujemy wskaŸnikiem na nasz wybór

				if (choice != '1' && choice != '2') {
					cout << "\nNie ma takiego trybu !" << endl;
					this_thread::sleep_for(chrono::seconds(1));
					system("cls");
					blad = true;
				}
				else {
					blad = false;
				}
			} while (blad == true);

			switch (choice) {//wybieramy poziom
			case '1'://zale¿nie od wybranego poziomu uruchamiamy inne funkcje z gr¹
				wybierzKarty();//wczeœniej wybieraj¹c zestaw kart
				losowanie(wybraneKarty, zestawKart);
				odliczanie();
				easy(realTimePlansza, zestawKart, name);
				choice = 1;
				break;

			case '2':
				wybierzKarty();
				losowanie(wybraneKarty, zestawKart);
				odliczanie();
				hard(realTimePlansza, zestawKart, name);
				choice = 1;
				break;

			default: choice = 1; break;
			}
		}
		if (choice == '2') {//tutaj kiedy chcemy wyjœæ - nasza zmienna do nieskoñczonej pêtli zmia siê na false - zatrzymuje pêtlê
			again = false;
		}
		else if (choice == '3') {//wyœwietlamy ranking
			logo();
			cout << "                           SZYBKIE MENU" << endl << endl;
			cout << "--------------------------------------------------------------" << endl;
			cout << "                       1. Powrot" << endl << endl;

			cout << "                           Ranking graczy" << endl << endl;
			cout << "--------------------------------------------------------------" << endl;
			cout << "                             EASY" << endl << endl;

			fstream EASY;
			EASY.open("RankingEasy.txt");
			if (EASY.is_open()) {
				string Easy;
				while (getline(EASY, Easy)) {
					cout << Easy << endl;
				}
				EASY.close();
			}

			cout << "--------------------------------------------------------------" << endl;
			cout << "                             HARD" << endl << endl;

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
			choice = kursorMenu();
		}
	}


	return 0;
}