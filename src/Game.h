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
	bool cell; //1(true) - ����� ������, 0(false) - ������ (����������)
};

class game
{
private:
	setka_dr field[8][8]; //�������� ���������� ������ - ��� ���������
	int shashki[8][8]; //���������� � ������, -1 - ������, 0 - ������ �����, 1- �����, +/- 2 - ����. "�����".
	int kletka; //������� ������ =31
	int pole; // =18 ���������� �� ������� ������� ������� ����� �� ������ ������
	int otstupX,otstupY; //10, 0
	int color;
public:
	game(int,int,int,int,int);
	void draw(); //������� ��������� ����
	void friend curPos(int,int);
	bool incl_latter(char);
	bool incl_number(char);
	void damka(); /*�������� �� �����*/ 
	bool correct(string st); /*������� �������� ����� ����*/
	void change(string,bool&); /*������� ����*/
	int win(); /*���-������ �������?*/
	int* get_shashki(int);
	void set_shashki(int*,int);
};
#endif GAME_H