#define _WIN32_WINNT 0x0500
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <ctime>
#include <string>
#include <vector>
#include <list>
#include "view.h"
#include "interface.h"
#include "items.h"
#include "map.h"
#include "enemy.h"

using namespace sf;
int main()
{
Clock clock;
Clock gameTimeClock;
int gameTime = 0;
float LimitUpdateSex=0;
float LimitUpdateCount=0;
srand(time(0)); 	//������ ������ ������ ������.
int VisibleDist=360;//��������� ��������� �������
int CountSheep=500; //��������� ��������� ����
int CountWolf=10; 	//��������� ���������� ������
int CountItem=5;	//��������� ���������� ��������� �� �����


Image map_image;
map_image.loadFromFile("images/map2.png");
map_image.createMaskFromColor(Color(48, 120, 128));
Texture map;
map.loadFromImage(map_image);
Sprite s_map;
s_map.setTexture(map);

Image heroImage;
heroImage.loadFromFile("images/hero_by_palinor.png");

Image sheepImage;
sheepImage.loadFromFile("images/sheep.png");

Image wolfImage;
wolfImage.loadFromFile("images/wolf.png");

Image sheepBraunImage;
sheepBraunImage.loadFromFile("images/rpg_maker_vx_wolf_neo_style_by_neofox462.png");
//easyEnemyImage.createMaskFromColor(Color(255, 0, 0));//������� ����� �� �����.�� ����� ���������� ����� ���������� ��������. ��� ����� ���� � ��������� - �������� ������ �������:)

Player p(heroImage,400,400,32,32,"Player1");//������ ������ ������. x=1000 -��������� ����������. 32x32 - ������ ��������
std::list<Entity*>  entities;//������ ������, ���� ���� ������ �������.�������� ������.
std::list<Entity*>::iterator it;//�������� ����� ��������� �� ��-��� ������
std::list<Entity*>::iterator it2; // ��� �������� ������� ���� � ������


for (int i = 0; i < CountSheep; i++)//���������� �� ��������� ����� �������(� ������ �� �����)
{
	int r=abs(rand()%(HEIGHT_MAP*32)-200); //��������� ���������� ��������� �����
	int r2=abs(rand()%(HEIGHT_MAP*32)-200);
	entities.push_back(new Enemy(sheepImage,r,r2,32,32,"Sheep"));//� ���������� �� � ������
}
for (int i = 0; i < CountWolf; i++)//���������� �� ��������� ����� �������(� ������ �� ������)
{
	int r=abs(rand()%(HEIGHT_MAP*32)-200);  //��������� ���������� ��������� �����
	int r2=abs(rand()%(HEIGHT_MAP*32)-200);
	entities.push_back(new Enemy(wolfImage,r,r2,32,32,"Wolf"));//� ���������� �� � ������
}

SetItem(450,450,20, "Sword");
//HWND hWnd = GetConsoleWindow();//����� ����� �������, (�������� ������� (�������� ������ ��� windows))
//ShowWindow(hWnd, SW_HIDE);//�������� �������



std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
for (std::size_t i = 0; i < modes.size(); ++i)
{
	sf::VideoMode mode = modes[i];
	//std::cout << "Mode #" << i << ": "
	//<< mode.width << "x" << mode.height << " - "
	//<< mode.bitsPerPixel << " bpp" << std::endl;
}
// Create a window with the same pixel depth as the desktop
sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
//window.create(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Glass and Grass");
//RenderWindow window(VideoMode(640, 480), "Glass and Grass");
RenderWindow window(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Glass Grass");
view.reset(FloatRect(0, 0, 640, 480));
window.setFramerateLimit(60); //������������� ������� ������ � �������

Font font;
font.loadFromFile("CyrilicOld.ttf");
Text text("", font, 16);
text.setColor(Color::White);

SetMap(); // ������ �����

	while (window.isOpen())
	{
	
		changeview();
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds(); 
		else  { view.move(0.8, 0); }
		clock.restart();
		time = time / 800;
		
		if(rand()%100<5)SetGrassOnMap(); // ���� �� ��������� �� ����� ����� � ��������� �����.
		//SetGrassOnMap(); //��������� �� ����� ����� � ��������� �����.
		
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				for (it = entities.begin(); it != entities.end();) {Entity *b = *it; it = entities.erase(it); delete b;}
				window.close();
			}
		}
	
		
		p.update(time);
		
/////////////////////////// ��������////////////////////////////////
		for (it = entities.begin(); it != entities.end();)//������� ��� ���������� �� ������ �� �����
		{
			Entity *b = *it;//��� ��������, ����� �� ������ (*it)->
			b->update(time);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
			if (b->life == false)
			{ 
				std::cout << "DEAD from noneat " << b->age <<"\n";
				it = entities.erase(it); delete b; 
				
			}// ���� ���� ������ �����, �� ������� ���			
			else {
				it++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
			}
		}
	

		window.setView(view);
		window.clear();
		
        for (int i = 0; i < HEIGHT_MAP; i++) //���� �� ������
		  if (i < (p.y+VisibleDist)/32 && i > (p.y-VisibleDist)/32)
                for (int j = 0; j < WIDTH_MAP; j++) // �� ������
                {
					if (j < (p.x+VisibleDist)/32 && j > (p.x-VisibleDist)/32)
					{
                    switch (TileMap[i][j]) //�������� switch case, ������� �� �������� � ���������� � ��������� �������� � ����������� �� ����������. ������ �������������� if-��.
                    {
                    case Floor: //�����
                        s_map.setTextureRect(IntRect(200, 8, 32, 32));
                        break;
                    case Stone: //������
                        s_map.setTextureRect(IntRect(328, 200, 32, 32));
                        break;
                    case Brick: // ������. ������ �����
                        //s_map.setTextureRect(IntRect(104, 142, 32, 32));
						s_map.setTextureRect(IntRect(200, 8, 32, 32)); //��� ���� ������� ����� �� ���������� �� �������
                        break;
                    case Rock: // �����
                        s_map.setTextureRect(IntRect(296, 232, 32, 32));
                        break;
					case Grass: // �����
                        s_map.setTextureRect(IntRect(40, 8, 32, 32));
                        break;
                    };
                    s_map.setPosition(j * 32, i * 32); 
					window.draw(s_map);
					}
                }
		for(int x=0; x/32<5; x+=32)
		for(int y=0; y/32<5; y+=32)
		{
			
			TileMap[40+x/32][40+y/32]=Brick; // ������ ������� ��� ���, ��� � ����� ���� ������ ���� ������
	
		}
		
		for(int x=0; x/32<5; x+=32)
		for(int y=0; y/32<5; y+=32)
		{
			s_map.setTextureRect(IntRect(512+x, 8+y, 32, 32));
			s_map.setPosition(1280+x,1280+y); 
			window.draw(s_map);
	
		}
		
//////////////////////////////////������������ ��������//////////////////////////////////////////
		LimitUpdateSex+=0.1;
		if(LimitUpdateSex>0.4)
		{
		for (it = entities.begin(); it != entities.end(); it++) 
		{
			
			if((*it)->satiety>10 && (*it)->age>3) // ���� ��� ���� ���� � ����������� ��������
			{
				for (it2 = it; it2 != entities.end(); it2++) //������������ ����
				{
					if ((*it)->getRect() != (*it2)->getRect())//��� ���� ��� ������ ���� ������ ��������������
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "Sheep") && ((*it2)->name == "Sheep"))//���� ����������� ��� ������� � ��� ��� ����
					{
						if((*it2)->satiety>10 && (*it2)->age>3) // ���� ��� ���� ���� � ����������� ��������
						{
							std::cout << "satiety before " << (*it)->satiety << " age " << (*it)->age << "health "<<  (*it)->health << "\n"; //������� � ������� ������� � ������� ���� ��� �����������
							(*it)->satiety-=5;
							(*it2)->satiety-=5;
							entities.push_back(new Enemy(sheepImage,(*it)->x,(*it)->y,32,32,"Sheep"));
							//std::cout << "satiety " << (*it)->satiety << (*it2)->satiety <<"\n"; //������� � ������� ������� ���� ��� �����������
						}
					}
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "Sheep") && ((*it2)->name == "Wolf"))//���� ����������� ��� ������� � ��� �����
					{
						if((*it)->age>3 && (*it2)->health<90) // ���� ��� ���� ��������
							{
								std::cout << "KILL sheep " << (*it)->satiety << (*it2)->satiety << " age " << (*it)->age <<"\n"; //������� � ������� ���� �������� ����
								(*it2)->health+=10;
								Entity *b = *it;
								it = entities.erase(it); delete b; 
								//std::cout << "satiety " << (*it)->satiety << (*it2)->satiety <<"\n"; //������� � ������� ������� ���� ��� �����������
							}
							else if((*it2)->health<90)
							{
								std::cout << "KILL little sheep " << (*it)->satiety << (*it2)->satiety << " age " << (*it)->age <<"\n"; //������� � ������� ���� �������� ����
								(*it2)->health+=3;
								Entity *b = *it;
								it = entities.erase(it); delete b; 
								
							}
							
					}
				}
			}
		}
		LimitUpdateSex=0;
		}
		
		//������������ ������ ��� �������, ��� ����� � ������� ������
		for (it = entities.begin(); it != entities.end(); it++)
		{
			if ((*it)->x < p.x+VisibleDist && (*it)->x > p.x-VisibleDist)  
				if ((*it)->y < p.y+VisibleDist && (*it)->y > p.y-VisibleDist){
					renderHpBar((*it)->satiety*10, (*it)->x, (*it)->y-3, 32, 2, window, Color::Blue); //������� ����� ��� ����������
					renderHpBar((*it)->health, (*it)->x, (*it)->y, 32, 2, window, Color::Green); //������� ����� ��� ����������
					window.draw((*it)->sprite); //������ entities �������
				}
		}
		
		SetViewItems(VisibleDist, p.x, p.y, window, time); //������ ��������
		
				
		window.draw(p.sprite);	//������ ������		
		
		//��� � ����������� ����� ���, ������� ���������� ������ �� ����� ������, ����, ��� ���� ���������. � ������ ������ ���� ���: renderInterfaceBack(0, 0, ViewSizeX, 30, window, Color::Green) � renderInterfaceBack(10, 10, 60, 10, window, Color::Grey) ������ ��� �������� - ����������, ������ ��� - ������
		renderInterfaceBack(view.getSize().x, 30, (view.getCenter().x - view.getSize().x / 2), (view.getCenter().y - view.getSize().y / 2), window); //��� ���������� ������� ������� � ����������. ���������� ��� ��� ���.
		renderHpBar(p.health, (view.getCenter().x - view.getSize().x / 2)+10, (view.getCenter().y - view.getSize().y / 2)+10, 60, 10, window, Color::Green); //��� �� ���������. ������� ����������, ����� ������ 60x10px
		
				std::ostringstream sheep;
				sheep << entities.size();
				std::ostringstream items;
				items << GetItemsSize();
				text.setPosition((view.getCenter().x - view.getSize().x / 2)+100, (view.getCenter().y - view.getSize().y / 2)+5);
				text.setString("�������: " + sheep.str() + ", ���������: " + items.str());
				window.draw(text);
				
		window.display();
	}
	entities.clear();
//		for (it = entities.begin(); it != entities.end();)//��� ������ �� ���� ������ ������ �������. 
//		{
//			Entity *b = *it;
//			it = entities.erase(it); delete b; 
//			it++;
//		}
	return 0;
}