#define _WIN32_WINNT 0x0500
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <ctime>
#include <string>
#include <vector>
#include <list>

#include "map.h"

using namespace sf;



//const int HEIGHT_MAP = 60; //������ ����� ������
//const int WIDTH_MAP = 60; //������ ����� ������
enum mapObjects { Brick , Floor, Stone, Rock, House, Well, Tree, Grass, 
Coast1, Coast2, Coast3, Coast4, Coast5, Coast8, Coast9, Coast10, Coast11, 
Coast12, Woter1, Woter2 }; // �������� �����
mapObjects TileMap[HEIGHT_MAP][WIDTH_MAP]; // �������� ������� ��� �����. �� ��������� �� ���������� ������ ��������� ���� - ��������.  

//int InitWindow()
//{
//	RenderWindow window(VideoMode(640, 480), "Glass and Grass");
//	view.reset(FloatRect(0, 0, 640, 480));
//	window.setFramerateLimit(60); //������������� ������� ������ � �������
//	
//	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
//	for (std::size_t i = 0; i < modes.size(); ++i)
//	{
//		sf::VideoMode mode = modes[i];
//		std::cout << "Mode #" << i << ": "
//				<< mode.width << "x" << mode.height << " - "
//				<< mode.bitsPerPixel << " bpp" << std::endl;
//	}
//	// Create a window with the same pixel depth as the desktop
//	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
//	window.create(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Glass and Grass");
//}

int GetHeightMap(){return HEIGHT_MAP;}

int GetWidthMap() {return WIDTH_MAP;}

int SetMap()
{
	mapObjects Object; 
	for (int i = 1; i < HEIGHT_MAP-1; i++)	//���� �� ������, �������� ��������, ��� i ����� �� 1, �� HEIGHT_MAP-1 ����� �� ������� ������ �� ��������.
	for (int j = 1; j < WIDTH_MAP-1; j++) //���� �� ������, ���������� ��� �������.
	{				
		Object  = mapObjects(rand()%8); //������� ��������� ������ �� ������ ������� ����������
		if (Object == Brick) Object = Floor; // ���� ��������� �������� ��������� ������ - ������ ��� �� ���, ����� ��� ����� ������� �������?
		if (Object == Tree ) {if (rand()%100<2)Object = Tree; else Object=Floor;}//������ �������� ��������
		if (Object == House) {if (rand()%100>200)Object = House; else Object=Floor;}
		if (Object == Well)  {if (rand()%100<2)Object = Well; else Object=Floor;}
		TileMap[i][j] = Object; 

	}
	return TileMap[HEIGHT_MAP][WIDTH_MAP];			
}


int SetGrassOnMap(){	//����� ����� ������ �� ����� � ��������� �����
int i;
int j;	
i=rand()%HEIGHT_MAP;
if(i<2) i=1;
if(i>HEIGHT_MAP-2) i=HEIGHT_MAP-2;
j=rand()%WIDTH_MAP;
if(j<2) j=1;
if(j>WIDTH_MAP-2) j=WIDTH_MAP-2;
TileMap[i][j] = Grass;
return TileMap[i][j];
}

inline int PutMap(){



	for (int i = 0; i < GetWidthMap(); i++) //���� �� ������
		  if (i < (p.y+VisibleDist)/32 && i > (p.y-VisibleDist)/32)
                for (int j = 0; j < GetWidthMap(); j++) // �� ������
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
					case Coast1: // �����
                        s_map.setTextureRect(IntRect(488, 200, 32, 32));
                        break;
					case Coast2: // �����
                        s_map.setTextureRect(IntRect(520, 200, 32, 32));
                        break;
					case Coast3: // �����
                        s_map.setTextureRect(IntRect(552, 200, 32, 32));
                        break;
					case Coast4: // �����
                        s_map.setTextureRect(IntRect(584, 200, 32, 32));
                        break;
					case Coast5: // �����
                        s_map.setTextureRect(IntRect(488, 232, 32, 32));
                        break;
					case Woter1: // ����
                        s_map.setTextureRect(IntRect(520, 232, 32, 32));
                        break;
					case Woter2: // ����
                        s_map.setTextureRect(IntRect(552, 232, 32, 32));
                        break;
					case Coast8: // �����
                        s_map.setTextureRect(IntRect(584, 232, 32, 32));
                        break;
					case Coast9: // �����
                        s_map.setTextureRect(IntRect(488, 264, 32, 32));
                        break;
					case Coast10: // �����
                        s_map.setTextureRect(IntRect(520, 264, 32, 32));
                        break;
					case Coast11: // �����
                        s_map.setTextureRect(IntRect(552, 264, 32, 32));
                        break;
					case Coast12: // �����
                        s_map.setTextureRect(IntRect(584, 264, 32, 32));
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

}