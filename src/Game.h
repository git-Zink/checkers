#include<Windows.h>
#include<iostream>
#include<string>
#include<cmath>
using namespace std;

#ifndef GAME_H
#define GAME_H

struct setka_dr
{
	int x;
	int y;
	bool cell; //1(true) - белая клетка, 0(false) - черная (коричневая)
};

class game
{
private:
	setka_dr field[8][8]; //содержит координаты клеток - для отрисовки
	int shashki[8][8]; //информация о шашках, -1 - черная, 0 - клетка пуста, 1- белая, +/- 2 - соот. "дамки".
	int kletka; //Размеры клеток =31
	int pole; // =18 расстояние от внешней границы бежевой каймы до начала клеток
	int otstupX,otstupY; //10, 0
	int color;
public:
	game(int,int,int,int,int);
	void draw(); //Функция отрисовки поля
	void friend curPos(int,int);
	bool incl_latter(char);
	bool incl_number(char);
	void damka(); /*Проверка на дамку*/ 
	bool correct(string st); /*Функция проверка ввода хода*/
	void change(string,bool&); /*Функция шага*/
	int win(); /*Кто-нибудь выиграл?*/
	int* get_shashki(int);
	void set_shashki(int*,int);
};
#endif GAME_H