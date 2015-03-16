#include"Game.h"
//Ќе знаю что, нужно дл€ графики

HWND hwnd = GetConsoleWindow();
HDC hdc = GetDC(hwnd);

//ј это нужно дл€ управлени€ курсором
HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

//÷вета
COLORREF white=RGB(255,255,255), black=RGB(192,70,18), setka=RGB(251,206,132), crown=RGB(255,0,0);
using namespace std;


game::game(int k,int p,int oX,int oY,int clr)
{
	pole=p;
	kletka=k;
	otstupX=oX;
	otstupY=oY;
	color=clr;
	for(int i=0;i<8;++i)
		for(int j=0;j<8;++j)
		{
			field[i][j].x=pole+j*(kletka);
			field[i][j].y=pole+i*(kletka);
			if((i+j)%2==0)
				field[i][j].cell=true;
			else
				field[i][j].cell=false;
		};

	//»сходное положение шашек
	for(int i=0;i<8;++i)
		for(int j=0;j<8;++j)
			shashki[i][j]=0;

	for(int i=0;i<3;++i)
	{
		int j;
		if(i!=1)
			j=1;
		else
			j=0;
		for(;j<8;j+=2)
			shashki[i][j]=-1;
	}

	for(int i=5;i<8;++i)
	{
		int j;
		if(i==6)
			j=1;
		else
			j=0;
		for(;j<8;j+=2)
			shashki[i][j]=1;
	}
}
void game::draw()
{
	SelectObject(hdc,GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc,black);
	Rectangle(hdc,otstupX,otstupY,otstupX+2*pole+8*kletka,otstupY+2*pole+8*kletka);

	//–исуем  летки
	SelectObject(hdc,GetStockObject(DC_PEN));
	SelectObject(hdc,GetStockObject(DC_BRUSH));
	for(int i=0;i<8;++i)
		for(int j=0;j<8;++j)
		{
			if(field[i][j].cell)
			{
				SetDCBrushColor(hdc,white);
				SetDCPenColor(hdc,white);
			}
			else
			{
				SetDCBrushColor(hdc,black);
				SetDCPenColor(hdc,black);
			}
			Rectangle(hdc,otstupX+field[i][j].x,otstupY+field[i][j].y,otstupX+field[i][j].x+kletka,otstupY+field[i][j].y+kletka);
		};

	//–исуем сетку
	SelectObject(hdc,GetStockObject(DC_PEN));
	SetDCPenColor(hdc,setka);
	for(int i=0;i<5;++i)
	{
		MoveToEx(hdc,otstupX+i,i+otstupY,NULL);
		LineTo(hdc,2*pole+8*kletka-i+otstupX,i+otstupY);
		LineTo(hdc,2*pole+8*kletka-i+otstupX,2*pole+8*kletka-i+otstupY);
		LineTo(hdc,i+otstupX,2*pole+8*kletka-i+otstupY);
		LineTo(hdc,i+otstupX,i+otstupY);
	}

	SelectObject(hdc,GetStockObject(DC_PEN));
	SetDCPenColor(hdc,setka);
	MoveToEx(hdc,pole+otstupX,pole+otstupY,NULL);
	LineTo(hdc,8*kletka+pole+otstupX,pole+otstupY);
	LineTo(hdc,8*kletka+pole+otstupX,8*kletka+pole+otstupY);
	LineTo(hdc,pole+otstupX,8*kletka+pole+otstupY);
	LineTo(hdc,pole+otstupX,pole+otstupY);
	
	//–исуем шашки

	for(int i=0;i<8;++i)
		for(int j=0;j<8;++j)
		{
			int x1,x2,y1,y2;
			x1=otstupX+field[i][j].x+5;
			y1=otstupY+field[i][j].y+5;
			x2=otstupX+field[i][j].x+kletka-5;
			y2=otstupY+field[i][j].y+kletka-5;
			int a=i,b=j;
			if(color==-1)
			{
				a=7-i;
				b=7-j;
			}
			switch(shashki[a][b])
			{
			case 1:
				SelectObject(hdc,GetStockObject(WHITE_BRUSH));
				SelectObject(hdc,GetStockObject(WHITE_PEN));
				Ellipse(hdc,x1,y1,x2,y2);
				break;
			case -1:
				SelectObject(hdc,GetStockObject(BLACK_BRUSH));
				SelectObject(hdc,GetStockObject(BLACK_PEN));
				Ellipse(hdc,x1,y1,x2,y2);
				break;
			case 2:
				SelectObject(hdc,GetStockObject(WHITE_BRUSH));
				SelectObject(hdc,GetStockObject(WHITE_PEN));
				Ellipse(hdc,x1,y1,x2,y2);
				//–исуем корону
				SelectObject(hdc,GetStockObject(DC_BRUSH));
				SetDCBrushColor(hdc,crown);
				Ellipse(hdc,x1+7,y1+7,x2-7,y2-7);		
				break;
			case -2:
				SelectObject(hdc,GetStockObject(BLACK_BRUSH));
				SelectObject(hdc,GetStockObject(BLACK_PEN));
				Ellipse(hdc,x1,y1,x2,y2);
				//–исуем корону
				SelectObject(hdc,GetStockObject(DC_BRUSH));
				SetDCBrushColor(hdc,crown);
				Ellipse(hdc,x1+7,y1+7,x2-7,y2-7);
				break;
			};
		}
	//Ќаносим маркировку
	csbiInfo.dwCursorPosition.X=0;
	csbiInfo.dwCursorPosition.Y=2;
	for(int i=0;i<8;++i)
	{
		SetConsoleCursorPosition(hStdout,csbiInfo.dwCursorPosition);
		if(color==1)
			cout<<(8-i);
		else
			cout<<i+1;
		if((i==1)||(i==5))
			csbiInfo.dwCursorPosition.Y+=2; 
		else
			csbiInfo.dwCursorPosition.Y+=3;	
	}

	csbiInfo.dwCursorPosition.Y=24;
	csbiInfo.dwCursorPosition.X=5;
	for(int i=0;i<8;++i)
	{
		SetConsoleCursorPosition(hStdout,csbiInfo.dwCursorPosition);
		if(color==1)
			cout<<char(int('a')+i);
		else
			cout<<char(int('h')-i);
		csbiInfo.dwCursorPosition.X+=4;
	}
};
void curPos(int x,int y)
{
	csbiInfo.dwCursorPosition.X=x;
	csbiInfo.dwCursorPosition.Y=y;
	SetConsoleCursorPosition(hStdout,csbiInfo.dwCursorPosition);
}
bool game::incl_latter(char ch)
{
	if((ch>='a')&&(ch<='h'))
		return true;
	else 
		return false;
}
bool game::incl_number(char ch)
{
	if((ch>='1')&&(ch<='8'))
		return true;
	else
		return false;
}
void game::damka()
{
	for(int i=0;i<8;++i)
		if(shashki[7][i]==-1)
			shashki[7][i]=-2;
	for(int i=0;i<8;++i)
		if(shashki[0][i]==1)
			shashki[0][i]=2;
}
bool game::correct(string st)
{
	if((st=="")&&(st=="\n"))
		return false;
	if(st.length()<5)
		return false;
	if((st.length()-5)%3!=0) //проверка дл€ многошаговых ходов
		return false;
	//оп€ть-таки формула дл€ проверки корректности многошаговых ходов
	for(int i=0;i<st.length();++i)
		switch(i%3)
		{
		case 0:
			if(!incl_latter(st[i]))
				return false;
			break;
		case 1:
			if(!incl_number(st[i]))
				return false;
			break;
		case 2:
			if(st[i]!='-')
				return false;
			break;
		};
	
	if(abs(int(st[0])-int(st[3]))!=(abs(int(st[1])-int(st[4])))) //’одим только по диагонали
		return false;
	return true;
}
void game::change(string st,bool& fl)
{
	int x0,x1,y0,y1;
	
	y0=int(st[0])-int('a');
	x0=8-(int(st[1])-int('0'));
	y1=int(st[3])-int('a');
	x1=8-(int(st[4])-int('0'));

	if(shashki[x0][y0]*color<=0)
	{
		fl=false;
		return;
	}

	int j;
	int mk;
	bool f;
	bool imba;
	int my=shashki[x0][y0];
	switch(abs(shashki[x0][y0]))
	{
	case 1: //если шашка обычна€
		mk=(st.length()-5)/3+1; //"простой" ход, или multikill? 
		if(mk==1)
		{
			switch(abs(x1-x0))
			{
			case 1:
				//черные не могут ходить в сторону уменьшени€ коор. х, белые - в сторону увеличени€
				if((color==1)&&(x0<x1))
				{
					fl=false;
					return;
				}
				if((color==-1)&&(x0>x1))
				{
					fl=false;
					return;
				}
				if((shashki[x1][y1]==0)&&(abs(shashki[x0][y0])==1))
				{
					shashki[x1][y1]=shashki[x0][y0];
					shashki[x0][y0]=0;
					damka();
					fl=true;
					return;
				}
				else
					fl=false;
				return;
				break;
			case 2:
				int xh,yh;
				xh=(x1+x0)/2;
				yh=(y1+y0)/2;
				if(((shashki[xh][yh]*shashki[x0][y0])<0)&&(shashki[x1][y1]==0)&&(abs(shashki[x0][y0])==1))
				{
					shashki[x1][y1]=shashki[x0][y0];
					shashki[x0][y0]=0;
					shashki[xh][yh]=0;
					damka();
					fl=true;
					return;
				}
				else
					fl=false;
				return;
				break;
			default:
				fl=false;
				return;
			};
		}
		else
		{
			//проверка, расположение шашек не мен€етс€!
			int my=shashki[x0][y0];
			if(my==0)
			{
				fl=false;
				return;
			}
			for(int i=1;i<=mk;++i)
			{
				if(abs(x1-x0)!=2)
				{
					fl=false;
					return;
				}
				if(abs(y1-y0)!=2)
				{
					fl=false;
					return;
				}
				if( ((color==1)&&(x1==0)) || ((color==-1)&&(x1==7)))
					{
						bool flg;
						y0=int(st[0])-int('a');
						x0=8-(int(st[1])-int('0'));
						shashki[x0][y0]*=2;
						change(st,flg);
						if(flg)
						{
							fl=true;
							return;
						}
						else
						{
							shashki[x0][y0]/=2;
							fl=false;
							return;
						}
					}
				int xh,yh;
				xh=(x1+x0)/2;
				yh=(y1+y0)/2;
				if( ((shashki[xh][yh]*my)>=0) || (shashki[x1][y1]!=0) )
				{
					fl=false;
					return;
				}
				if(i!=mk)
				{
					y0=y1;
					x0=x1;
					y1=int(st[3+3*i])-int('a');
					x1=8-(int(st[4+3*i])-int('0'));

					if( ((color==1)&&(x1==0)) || ((color==-1)&&(x1==7)))
					{
						bool flg;
						y0=int(st[0])-int('a');
						x0=8-(int(st[1])-int('0'));
						shashki[x0][y0]*=2;
						change(st,flg);
						if(flg)
						{
							fl=true;
							return;
						}
						else
						{
							shashki[x0][y0]/=2;
							fl=false;
							return;
						}
					}
				}
			}
			//≈сли мы сюда попали, то все мультикиллы корректны, теперь надо сделавть все ходы

			y0=int(st[0])-int('a');
			x0=8-(int(st[1])-int('0'));
			shashki[x1][y1]=shashki[x0][y0]; // ставим шашку на конечную позицию
			shashki[x0][y0]=0; // удал€ем исходную шашку	
			y1=int(st[3])-int('a');
			x1=8-(int(st[4])-int('0'));

			for(int i=1;i<=mk;++i)
			{
				int xh,yh;
				xh=(x1+x0)/2;
				yh=(y1+y0)/2;
				shashki[xh][yh]=0; //удал€ем шашки оппонента
				if(i!=mk)
				{
					y0=y1;
					x0=x1;
					y1=int(st[3+3*i])-int('a');
					x1=8-(int(st[4+3*i])-int('0'));
				}
			}
		}
		fl=true;
		break;
	case 2: //если дамка
		fl=false; imba=false;
		mk=(st.length()-5)/3+1; //количество ходов
		j=1; f=false;
		while(j<=mk)
		{
			if(shashki[x1][y1]!=0)
			{
				fl=false;
				return;
			}
			int k=abs(x1-x0);
			/*vx,vy - вектора, отвечающие направлению движени€ дамки;
			в этом направлении провер€етс€ кажда€ клетка на "преграды" */
			int vx=(x1-x0)/k,vy=(y1-y0)/k;
			for(int i=1;i<k-1;++i)
				if(shashki[x0+i*vx][y0+i*vy]*shashki[x0][y0]>0)
				{
					fl=false;
					return;
				}
				else
					if(shashki[x0+i*vx][y0+i*vy]*shashki[x0][y0]<0)
						if(shashki[x0+(i+1)*vx][y0+(i+1)*vy]!=0)
						{
							fl=false;
							return;
						}
						else
							imba=true;
				if(shashki[x1-vx][y1-vy]*shashki[x0][y0]<0)
					imba=true;
			if(f)
			{
				//если все правильно, то теперь можно ходить
				fl=true;
				shashki[x1][y1]=shashki[x0][y0];
				shashki[x0][y0]=0;
				for(int i=1;i<k;++i)
					shashki[x0+i*vx][y0+i*vy]=0;
			}
			if(j!=mk)
			{
				if(!imba)
				{
					fl=false;
					return;
				}
				else
					imba=false;
				y0=y1;
				x0=x1;
				y1=int(st[3+3*j])-int('a');
				x1=8-(int(st[4+3*j])-int('0'));
				++j;
			}
			else
				if(!f)
				{
					j=1;
					f=true; //прогон€ем цикл еще раз, теперь примен€ем изменени€
					y0=int(st[0])-int('a');
					x0=8-(int(st[1])-int('0'));
					y1=int(st[3])-int('a');
					x1=8-(int(st[4])-int('0'));
				}
				else
					++j;
		}		
		fl=true;
		return;
		break;
	default:
		fl=false;
	};
	return;	
}
int game::win()
{
	bool f1=true,f2=true;
	for(int i=0;i<8;++i)
		for(int j=0;j<8;++j)
		{
			if(shashki[i][j]<0)
				f1=false;
			if(shashki[i][j]>0)
				f2=false;
		}
	if(f1)
		return 1;
	if(f2)
		return -1;
	return 0;
}
int* game::get_shashki(int i)
{
	int *a;
	a=shashki[i];
	return a;
}
void game::set_shashki(int *a,int i)
{
	for(int j=0;j<8;++j)
		shashki[i][j]=a[j];
}