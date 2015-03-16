#include"Game.h"
#include<stdio.h>
#include<iostream>
using namespace std;
int message=0;
string s_ip="127.0.0.1";
void transferTo(int a[],char ch[])
{
	for(int i=0;i<8;++i)
		switch(a[i])
		{
			case 1:
				ch[i]='1';
				break;
			case 2:
				ch[i]='2';
				break;
			case -1:
				ch[i]='*';
				break;
			case -2:
				ch[i]='#';
				break;
			case 0:
				ch[i]='0';
				break;
		};
}
void transferFrom(char ch[],int a[])
{
	for(int i=0;i<8;++i)
		switch(ch[i])
		{
		case '1':
			a[i]=1;
			break;
		case '2':
			a[i]=2;
			break;
		case '*':
			a[i]=-1;
			break;
		case '#':
			a[i]=-2;
			break;
		case '0':
			a[i]=0;
			break;
		};
}
void main()
{
// Дела сетевые...
	int a; //для ф-ции win();
	curPos(0,4);
	unsigned int server_s;
	sockaddr_in server_addr;
	unsigned int client_s;
	sockaddr_in client_addr;
	in_addr client_ip_addr;
	int addr_len;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested=2;

	//Initialize Winsock
	err = WSAStartup(wVersionRequested,&wsaData);
	if (err!=0)
	{
		printf("WSAStartup failed with error: %d\n",err);
		system("pause");
		return;
	}
	cout<<"Welcome to Checkers Online!\n";
	cout<<"Do you want be a server (then type 1), or client (then type 2)?\n";
	cout<<"Remember, server plays for white\n";
	char ch;
	cin>>ch;
	while((ch!='1')&&(ch!='2'))
	{
		cout<<"Incorrect symbol, try again\n";
		cin>>ch;
	}
	if(ch=='1')
	{
		//Create a SOCKET
		server_s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(server_s==INVALID_SOCKET)
		{
			printf("Error");
			WSACleanup();
			system("pause");
			return;
		}
		server_addr.sin_family=AF_INET;
		server_addr.sin_port=htons(27015);
		server_addr.sin_addr.s_addr=inet_addr(s_ip.c_str());

		cout<<"Wait for second player...\n";
		bind(server_s,(sockaddr*)&server_addr,sizeof(server_addr));
		listen(server_s,1);
		addr_len=sizeof(client_addr);
		client_s=accept(server_s,(sockaddr*)&client_addr,&addr_len);
		memcpy(&client_ip_addr,&client_addr.sin_addr.s_addr,4);
		cout<<"Connection to player2 was established\n";
		system("pause");
		game g(31,18,10,0,1);
		do
		{
			system("cls");
			curPos(0,2);
			g.draw();
			curPos(38,0);
			cout<<"Info: ";
			switch(message)
			{
				case 0:
					cout<<"For step type something like e2-e4";
					break;
				case -1:
					cout<<"Wrong input, try again";
					message=0;
					break;
			};
			curPos(38,1);
			string step;
			cin>>step;
			curPos(38,1);		
			if(step[0]=='/')
			{
				if(step=="/redraw")
				{
					system("cls");
					message=0;
					g.draw();
				}
				else
					if(step=="/exit")
					{
						closesocket(server_s);
						closesocket(client_s);
						WSACleanup();
						return;
					}
					else
					{
						system("cls");
						g.draw();
						message=-1;
					}	
			}
			else
				if(!g.correct(step))
				{
					system("cls");
					g.draw();
					message=-1;
				}			
				else
				{
					system("cls");
					bool f;
					g.change(step,f);
					if(f)
					{
						message=0;
						g.draw();
						for(int i=0;i<8;++i)
						{
							char out_buf[8];
							int *a;
							a=g.get_shashki(i);
							transferTo(g.get_shashki(i),out_buf);
							send(client_s,out_buf,8,0);
						}
						system("cls");
						g.draw();
						system("cls");
						curPos(38,0);
						g.draw();
						curPos(38,0);
						cout<<"Info: Wait your opponent...";
						a=g.win();
						switch(a)
						{
						case 1:
							system("cls");
							g.draw();
							curPos(38,0);
							cout<<"Info: Yeah, you win!!! ^_^";
							curPos(38,1);
							system("pause");
							closesocket(server_s);
							closesocket(client_s);
							WSACleanup();
							return;
							break;
						case -1:
							system("cls");
							g.draw();
							curPos(38,0);
							cout<<"Info: Oh no, you lose! %(";
							curPos(38,1);
							system("pause");
							closesocket(server_s);
							closesocket(client_s);
							WSACleanup();
							return;
							break;
						};
						for(int i=0;i<8;++i)
						{
							char in_buf[8];
							int iResult=recv(client_s,in_buf,8,0);
							if(iResult>0)
							{
								int a[8];
								transferFrom(in_buf,a);
								g.set_shashki(a,i);
							}
							else
							{
								system("cls");
								g.draw();
								curPos(38,0);
								cout<<"Info: disconnect";
								curPos(38,1);
								closesocket(server_s);
								closesocket(client_s);
								WSACleanup();
								system("pause");
								return;
							}
						}
						a=g.win();
						switch(a)
						{
						case 1:
							system("cls");
							g.draw();
							curPos(38,0);
							cout<<"Info: Yeah, you win!!! ^_^";
							curPos(38,1);
							system("pause");
							closesocket(server_s);
							closesocket(client_s);
							WSACleanup();
							return;
							break;
						case -1:
							system("cls");
							g.draw();
							curPos(38,0);
							cout<<"Info: Oh no, you lose! %(";
							curPos(38,1);
							system("pause");
							closesocket(server_s);
							closesocket(client_s);
							WSACleanup();
							return;
							break;
						};
						message=0;
					}
					else
					{
						system("cls");
						g.draw();
						message=-1;
					}
				}
		}while(1);
	}
	if(ch=='2')
	{
		server_s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		server_addr.sin_family=AF_INET;
		server_addr.sin_port=htons(27015);
		server_addr.sin_addr.s_addr=inet_addr(s_ip.c_str());

		if(connect(server_s,(sockaddr*)&server_addr,sizeof(server_addr))==SOCKET_ERROR)
		{
			printf("Failed to conect\n");
			closesocket(server_s);
			WSACleanup();
			system("pause");
			return;
		}
		cout<<"Connection to player2 was established\n";
		system("pause");
		game g(31,18,10,0,-1);
	
		system("cls");
		curPos(38,0);
		g.draw();
		curPos(38,0);
		cout<<"Info: Wait your opponent...";
		for(int i=0;i<8;++i)
		{
			char in_buf[8];
			int iResult=recv(server_s,in_buf,8,0);
			if(iResult>0)
			{
				int a[8];
				transferFrom(in_buf,a);
				g.set_shashki(a,i);
			}
			else
			{
				system("cls");
				g.draw();
				curPos(38,0);
				cout<<"Info: disconnect";
				curPos(38,1);
				closesocket(server_s);
				closesocket(client_s);
				WSACleanup();
				system("pause");
				return;
			}
		}
		message=0;

		do
		{
			system("cls");
			g.draw();
			curPos(38,0);
			cout<<"Info: ";
			switch(message)
			{
				case 0:
					cout<<"For step type something like e2-e4";
					break;
				case -1:
					cout<<"Wrong input, try again";
					message=0;
					break;
			};
			curPos(38,1);
			string step;
			cin>>step;
			curPos(38,1);		
			if(step[0]=='/')
			{
				if(step=="/redraw")
				{
					system("cls");
					message=0;
					g.draw();
				}
				else
					if(step=="/exit")
					{
						closesocket(server_s);
						closesocket(client_s);
						WSACleanup();
						return;
					}
					else
					{
						system("cls");
						g.draw();
						message=-1;
					}	
			}
			else
				if(!g.correct(step)) 
				{
					system("cls");
					g.draw();
					message=-1;
				}			
				else
				{
					system("cls");
					bool f;
					g.change(step,f);
					if(f)
					{
						message=0;
						g.draw();
						for(int i=0;i<8;++i)
						{
							char out_buf[8];
							transferTo(g.get_shashki(i),out_buf);
							send(server_s,out_buf,8,0);
						}
						a=g.win();
						switch(a)
						{
						case 1:
							system("cls");
							g.draw();
							curPos(38,0);
							cout<<"Info: Oh no, you lose! %(";
							curPos(38,1);
							system("pause");
							closesocket(server_s);
							closesocket(client_s);
							WSACleanup();
							return;
							break;
						case -1:
							system("cls");
							g.draw();
							curPos(38,0);
							cout<<"Info: Yeah, you win!!! ^_^";
							curPos(38,1);
							system("pause");
							closesocket(server_s);
							closesocket(client_s);
							WSACleanup();
							return;
							break;
						};
						system("cls");
						curPos(38,0);
						g.draw();
						curPos(38,0);
						cout<<"Info: Wait your opponent...";
						for(int i=0;i<8;++i)
						{
							char in_buf[8];
							int iResult=recv(server_s,in_buf,8,0);
							if(iResult>0)
							{
								int a[8];
								transferFrom(in_buf,a);
								g.set_shashki(a,i);
							}
							else
							{
								system("cls");
								g.draw();
								curPos(38,0);
								cout<<"Info: disconnect";
								curPos(38,1);
								closesocket(server_s);
								closesocket(client_s);
								WSACleanup();
								system("pause");
								return;
							}
						}
						a=g.win();
						switch(a)
						{
						case 1:
							system("cls");
							g.draw();
							curPos(38,0);
							cout<<"Info: Oh no, you lose! %(";
							curPos(38,1);
							system("pause");
							closesocket(server_s);
							closesocket(client_s);
							WSACleanup();
							return;
							break;
						case -1:
							system("cls");
							g.draw();
							curPos(38,0);
							cout<<"Info: Yeah, you win!!! ^_^";
							curPos(38,1);
							system("pause");
							closesocket(server_s);
							closesocket(client_s);
							WSACleanup();
							return;
							break;
						};
						message=0;
					}
					else
					{
						system("cls");
						g.draw();
						message=-1;
					}
				}
		}while(1);
	}
}