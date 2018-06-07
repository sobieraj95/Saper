#include<SFML/Graphics.hpp>
#include<iostream>
#include<sstream>
#include<cmath>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<fstream>

enum STATUS { MENU = 0, GRA = 1, CZY_WYGRANA = 2, EXIT = 3 };
enum STATUS_GRY { W_TOKU = 0, MINA = 1, ODKRYTA_PLANSZA = 2, RANKING = 3 };
using namespace std;

struct pole
{
	int wartosc;
	bool odkryte;
	bool trafiona;
};

void generuj_plansze(int szerokosc, pole** plansza)
{
	for (int x = 0; x < szerokosc; x++)
	for (int y = 0; y < szerokosc; y++)
	{
		plansza[x][y].wartosc = 0;
		plansza[x][y].odkryte = false;
		plansza[x][y].trafiona = false;
	}
}

void sprawdz_czy_koniec(int szerokosc, pole** plansza, STATUS_GRY &status_gry, int miny, STATUS &status)
{
	int poz_nieodkryte_pola = szerokosc*szerokosc - miny;
	for (int x = 0; x < szerokosc; x++)
	for (int y = 0; y < szerokosc; y++)
	{
		if (plansza[x][y].wartosc < 9 && plansza[x][y].odkryte == true)
			poz_nieodkryte_pola--;
		else if (plansza[x][y].wartosc == 9 && plansza[x][y].odkryte == true)
		{
			status = MENU;
			status_gry = MINA;
			return;
		}
	}
	if (poz_nieodkryte_pola == 0)
	{

		status = MENU;
		status_gry = ODKRYTA_PLANSZA;
	}
}

void losuj_pozycje(int szerokosc, pole** plansza, int ilosc)
{
	time_t t;
	int poz_x, poz_y;

	srand((unsigned)time(&t));

	while (ilosc > 0)
	{
		poz_x = rand() % szerokosc;
		poz_y = rand() % szerokosc;

		if (plansza[poz_x][poz_y].wartosc != 9)
		{
			plansza[poz_x][poz_y].wartosc = 9;

			for (int k = -1; k < 2; k++)
			for (int l = -1; l < 2; l++)
			{
				if ((poz_x + l)<0 || (poz_y + k)<0) continue;
				if ((poz_x + l)>szerokosc - 1 || (poz_y + k)>szerokosc - 1) continue;

				if (plansza[poz_x + l][poz_y + k].wartosc == 9) continue;
				plansza[poz_x + l][poz_y + k].wartosc += 1;
			}
			ilosc--;
		}
	}
}

void odkryj_plansze(int x, int y, int szerokosc, pole** plansza)
{

	if (x<0 || x>szerokosc - 1) return;
	if (y<0 || y>szerokosc - 1) return;

	if (plansza[x][y].wartosc == 9)
	{
		plansza[x][y].trafiona = true;
		for (int x = 0; x < szerokosc; x++)
		for (int y = 0; y < szerokosc; y++)
		{
			plansza[x][y].odkryte = true;
		}
	}

	if (plansza[x][y].odkryte == true) return;

	if (plansza[x][y].wartosc != 9 && plansza[x][y].odkryte == false)
		plansza[x][y].odkryte = true;

	if (plansza[x][y].wartosc != 0) return;

		odkryj_plansze(x - 1, y - 1, szerokosc, plansza),
		odkryj_plansze(x - 1, y, szerokosc, plansza),
		odkryj_plansze(x - 1, y + 1, szerokosc, plansza),
		odkryj_plansze(x + 1, y - 1, szerokosc, plansza),
		odkryj_plansze(x + 1, y, szerokosc, plansza),
		odkryj_plansze(x + 1, y + 1, szerokosc, plansza),
		odkryj_plansze(x, y - 1, szerokosc, plansza),
		odkryj_plansze(x, y, szerokosc, plansza),
		odkryj_plansze(x, y + 1, szerokosc, plansza);
}

int main()
{
	//ZMIENNE MENU
	STATUS status = MENU;
	STATUS_GRY status_gry = W_TOKU;
	int koniec = 0;
	int szerokosc;
	int menu_poz_x = 0, menu_poz_y = 0, ilosc_min;
	int rank_pos_y;
	clock_t mierz_czas;
	double czas_koncowy;

	std::size_t maxTextLength = 6;
	sf::Font font;
	sf::Font font_menu;
	font.loadFromFile("Verdana.ttf");
	font_menu.loadFromFile("big_rage.ttf");

	sf::Text menu;
	menu.setFont(font_menu);
	menu.setCharacterSize(20);
	menu.setFillColor(sf::Color::White);

	sf::Text menu_exit;
	menu_exit.setFont(font_menu);
	menu_exit.setCharacterSize(20);
	menu_exit.setFillColor(sf::Color::Red);
	menu_exit.setString("WYJSCIE");
	menu_exit.setPosition(70, 200);

	sf::Texture logo;
	logo.loadFromFile("logo.png");
	sf::Sprite logoS;
	logoS.setTexture(logo);
	logoS.setPosition(20, 20);

	//ZMIENNE GRA
	int odkr_x, odkr_y;
	int poz_myszy_x, poz_myszy_y;

	std::string str;
	sf::Texture ikona;
	ikona.loadFromFile("przycisk.png");
	sf::Sprite ikonaS;
	ikonaS.setTexture(ikona);

	sf::Text bufor;
	bufor.setFont(font);
	bufor.setCharacterSize(13);
	bufor.setFillColor(sf::Color::Black);

	sf::Text bufor2;
	bufor2.setFont(font);
	bufor2.setCharacterSize(13);
	bufor2.setFillColor(sf::Color::Black);

	sf::Text przegrales_tekst;
	przegrales_tekst.setFont(font_menu);
	przegrales_tekst.setCharacterSize(30);
	przegrales_tekst.setFillColor(sf::Color::Red);

	//ZMIENNE RANKING
	sf::Text ranking_text;
	ranking_text.setFont(font_menu);
	ranking_text.setCharacterSize(20);
	ranking_text.setPosition(53, 120);
	ranking_text.setFillColor(sf::Color::Black);
	std::string ranking_string;

	sf::Text ranking;
	ranking.setFont(font_menu);
	ranking.setCharacterSize(20);
	ranking.setFillColor(sf::Color::White);

	sf::Text bufor_ranking;
	bufor_ranking.setFont(font_menu);
	bufor_ranking.setCharacterSize(20);
	bufor_ranking.setFillColor(sf::Color::Yellow);
	string buf_rank;
	string tab[100];
	//PETLA GLOWNA
	do
	{
		ranking.setString("");
		if (status == MENU)
		{
			ranking.setString("");
			//------------------MENU------------------------ 
			sf::RenderWindow okno(sf::VideoMode(200, 230, 32), "SAPER");
			while (okno.isOpen())
			{
				if (status_gry == MINA || status_gry == W_TOKU)
				{
					sf::Event event;
					while (okno.pollEvent(event))
					{
						if (event.type == sf::Event::Closed)
						{
							okno.close();
							status = EXIT;
						}
						if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
						{
							sf::Vector2i menu_poz = sf::Mouse::getPosition(okno);
							if (menu_poz.x > 40 && menu_poz.y > 80 && menu_poz.x < 170 && menu_poz.y < 100)
							{
								ilosc_min = 10;
								szerokosc = 9;
								status = GRA;
								mierz_czas = clock();
								okno.close();

							}
							else if (menu_poz.x > 40 && menu_poz.y > 110 && menu_poz.x < 170 && menu_poz.y < 130)
							{
								ilosc_min = 35;
								szerokosc = 16;
								status = GRA;
								mierz_czas = clock();
								okno.close();
							}
							else if (menu_poz.x > 75 && menu_poz.y > 140 && menu_poz.x < 135 && menu_poz.y < 160)
							{
								ilosc_min = 55;
								szerokosc = 18;
								status = GRA;
								mierz_czas = clock();
								okno.close();
							}
							else if (menu_poz.x > 70 && menu_poz.y > 170 && menu_poz.x < 140 && menu_poz.y < 190)
							{
								status = MENU;
								status_gry = RANKING;
							}
							else if (menu_poz.x > 70 && menu_poz.y > 200 && menu_poz.x < 140 && menu_poz.y < 220)
							{
								status = EXIT;
								okno.close();
							}

						}
					}
					okno.clear();
					menu.setFillColor(sf::Color::White);
					menu.setString("POCZATKUJACY");
					menu.setPosition(40, 80);
					okno.draw(menu);
					menu.setString("ZAAWANSOWANY");
					menu.setPosition(40, 110);
					okno.draw(menu);
					menu.setString("MISTRZ");
					menu.setPosition(75, 140);
					okno.draw(menu);
					menu.setString("RANKING");
					menu.setFillColor(sf::Color::Green);
					menu.setPosition(70, 170);
					okno.draw(menu);
					okno.draw(menu_exit);
					okno.draw(logoS);
					okno.display();

				}
				else if (status_gry == ODKRYTA_PLANSZA)
				{
					sf::Event event3;
					while (okno.pollEvent(event3))
					{
						if (event3.type == sf::Event::Closed)
						{
							status_gry = W_TOKU;
							status = MENU;
							okno.close();
						}
						if (event3.type == sf::Event::MouseButtonPressed && event3.mouseButton.button == sf::Mouse::Left)
						{
							sf::Vector2i menu_poz = sf::Mouse::getPosition(okno);
							if (menu_poz.x > 90 && menu_poz.y > 179 && menu_poz.x < 110 && menu_poz.y < 200)
							{
								if (status_gry != MINA)
								{
									std::fstream plik;
									plik.open("ranking.txt", ios::app | ios::out);
									czas_koncowy = czas_koncowy / 1000;
									ostringstream czas_string;
									czas_string << czas_koncowy;
									if (plik.good() == true)
									{

										plik << ranking_string << "*" << endl;
										plik << czas_string.str() << "*" << endl;
										plik.close();
									}
								}
								status = MENU;
								status_gry = W_TOKU;
								okno.close();
							}
						}
						if (event3.type == sf::Event::TextEntered)
						{
							sf::Uint32 unicode = event3.text.unicode;
							if (unicode == 8 && ranking_string.size() > 0)
							{
								ranking_string.erase(ranking_string.size() - 1, 1);
								ranking_text.setString(ranking_string);
							}
							else if (unicode >= 97 && unicode <= 122 && ranking_string.size() < 10)
							{
								ranking_string += static_cast<char>(unicode);
								ranking_text.setString(ranking_string);
							}
						}
					}

					okno.clear();
					logoS.setPosition(20, 20);
					okno.draw(logoS);
					if (status_gry == MINA)
					{
						ranking.setFillColor(sf::Color::Red);
						ranking.setString("PRZEGRANA");
						ranking.setPosition(57, 70);
					}
					else{
						ranking.setFillColor(sf::Color::Green);
						ranking.setString("WYGRALES");
						ranking.setPosition(62, 70);
					}

					ranking.setPosition(57, 70);
					okno.draw(ranking);
					ranking.setString("OK");
					ranking.setPosition(95, 180);
					okno.draw(ranking);
					ranking.setFillColor(sf::Color::White);
					ranking.setString("podaj nick: ");
					ranking.setPosition(55, 90);
					okno.draw(ranking);
					sf::RectangleShape rectangle(sf::Vector2f(100, 26));
					rectangle.setPosition(50, 120);
					okno.draw(rectangle);
					okno.draw(ranking_text);
					okno.display();
				}
				else if (status_gry == RANKING)
				{
					rank_pos_y = 50;
					sf::Event event4;
					while (okno.pollEvent(event4))
					{

						if (event4.type == sf::Event::MouseButtonPressed && event4.mouseButton.button == sf::Mouse::Left)
						{
							sf::Vector2i menu_poz = sf::Mouse::getPosition(okno);
							if (menu_poz.x > 95 && menu_poz.y > 180 && menu_poz.x < 105 && menu_poz.y < 200)
							{
								status = MENU;
								status_gry = W_TOKU;
								okno.close();
							}
						}
						
						std::fstream plik;
						plik.open("ranking.txt", ios::in);
						for (int i = 0; i<10; i+=2)				
							getline(plik, tab[i], '*');
	
						okno.clear();
						for (int i = 0; i<10; i += 2)
						{
							bufor_ranking.setString(tab[i]);
							bufor_ranking.setPosition(20, rank_pos_y);
							okno.draw(bufor_ranking);

							bufor_ranking.setString(tab[i + 1]);
							bufor_ranking.setPosition(120, rank_pos_y);
							okno.draw(bufor_ranking);
							rank_pos_y += 20;
						}
						ranking.setString("RANKING");
						ranking.setPosition(70, 20);
						okno.draw(ranking);
						ranking.setString("OK");
						ranking.setPosition(95, 180);
						okno.draw(ranking);
						okno.display();
					}

				}
			}
		}
		else if (status == GRA)
		{
			pole** plansza = (pole**)malloc(szerokosc*sizeof(pole *));
			for (int i = 0; i < szerokosc; i++)
				plansza[i] = (pole*)malloc(szerokosc * sizeof(pole));

			generuj_plansze(szerokosc, plansza);
			losuj_pozycje(szerokosc, plansza, ilosc_min);

			sf::RenderWindow okno_gra(sf::VideoMode(szerokosc * 20, szerokosc * 20, 32), "SAPER");


			while (okno_gra.isOpen())
			{

				sf::Event event2;
				while (okno_gra.pollEvent(event2))
				{
					if (event2.type == sf::Event::Closed)
					{
						status = EXIT;
						okno_gra.close();
					}
					if (event2.type == sf::Event::MouseButtonPressed && event2.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i pozycjaMyszy = sf::Mouse::getPosition(okno_gra);
						poz_myszy_x = pozycjaMyszy.x;
						poz_myszy_y = pozycjaMyszy.y;
						odkr_x = poz_myszy_x / 20;
						odkr_y = poz_myszy_y / 20;
						odkryj_plansze(odkr_y, odkr_x, szerokosc, plansza);
					}
				}
				okno_gra.clear();

				for (int x = 0; x < szerokosc; x++)
				{
					for (int y = 0; y < szerokosc; y++)
					{
						ostringstream ss2;
						ss2 << plansza[x][y].wartosc;
						string str2 = ss2.str();
						bufor2.setPosition(y * 20 + 6, x * 20 + 3);
						bufor2.setString(str2);
						ikonaS.setPosition(y * 20, x * 20);
						if (plansza[x][y].odkryte == false)
						{
							ikona.loadFromFile("przycisk.png");
							ikonaS.setTexture(ikona);
							okno_gra.draw(ikonaS);
						}
						else if (plansza[x][y].odkryte == true)
						{
							if (plansza[x][y].wartosc == 9 && plansza[x][y].trafiona == false)
							{
								ikona.loadFromFile("mina.png");
								ikonaS.setTexture(ikona);
								okno_gra.draw(ikonaS);
							}
							else if (plansza[x][y].wartosc == 9 && plansza[x][y].trafiona == true)
							{
								ikona.loadFromFile("mina_trafiona.png");
								ikonaS.setTexture(ikona);
								okno_gra.draw(ikonaS);

							}
							else if (plansza[x][y].wartosc < 9)
							{
								ikona.loadFromFile("przycisk_wcisniety.png");
								ikonaS.setTexture(ikona);
								okno_gra.draw(ikonaS);
							}
							okno_gra.draw(bufor2);
						}
					}
				}

				for (int i = 20; i < szerokosc * 20; i += 20)
				{
					sf::Vertex line[2] =
					{
						sf::Vertex(sf::Vector2f(i, 0)),
						sf::Vertex(sf::Vector2f(i, szerokosc * 20))
					};
					okno_gra.draw(line, 2, sf::Lines);

					sf::Vertex line2[2] =
					{
						sf::Vertex(sf::Vector2f(0, i)),
						sf::Vertex(sf::Vector2f(szerokosc * 20, i))
					};
					okno_gra.draw(line2, 2, sf::Lines);
				}
				sprawdz_czy_koniec(szerokosc, plansza, status_gry, ilosc_min, status);
				okno_gra.display();
				if (status == MENU)
				{					
					czas_koncowy = (clock() - mierz_czas);
					Sleep(2000);
					okno_gra.close();
				}
				

			}
		}
	} while (status != EXIT);
	_getch();
	return 0;
}