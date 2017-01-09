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



//const int HEIGHT_MAP = 60; //размер карты высота
//const int WIDTH_MAP = 60; //размер карты ширина
enum mapObjects { Brick , Floor, Stone, Rock, House, Well, Tree, Grass, 
Coast1, Coast2, Coast3, Coast4, Coast5, Coast8, Coast9, Coast10, Coast11, 
Coast12, Woter1, Woter2 }; // Элементы карты
mapObjects TileMap[HEIGHT_MAP][WIDTH_MAP]; // создание массива для карты. по умолчанию он заполнится первым элементом типа - кирпичом.  

//int InitWindow()
//{
//	RenderWindow window(VideoMode(640, 480), "Glass and Grass");
//	view.reset(FloatRect(0, 0, 640, 480));
//	window.setFramerateLimit(60); //устанавливаем частоту кадров в секунду
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
	for (int i = 1; i < HEIGHT_MAP-1; i++)	//цикл по высоте, обратите внимание, что i берем от 1, до HEIGHT_MAP-1 чтобы не портить бордюр из кирпичей.
	for (int j = 1; j < WIDTH_MAP-1; j++) //цикл по ширине, аналогично без бордюра.
	{				
		Object  = mapObjects(rand()%8); //создаем случайный объект из нашего массива переменных
		if (Object == Brick) Object = Floor; // если случайным объектом получился кирпич - меняем его на пол, зачем нам стены посреди комнаты?
		if (Object == Tree ) {if (rand()%100<2)Object = Tree; else Object=Floor;}//ставим немножко деревьев
		if (Object == House) {if (rand()%100>200)Object = House; else Object=Floor;}
		if (Object == Well)  {if (rand()%100<2)Object = Well; else Object=Floor;}
		TileMap[i][j] = Object; 

	}
	return TileMap[HEIGHT_MAP][WIDTH_MAP];			
}


int SetGrassOnMap(){	//новая трава растет на карте в случайном месте
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



	for (int i = 0; i < GetWidthMap(); i++) //цикл по высоте
		  if (i < (p.y+VisibleDist)/32 && i > (p.y-VisibleDist)/32)
                for (int j = 0; j < GetWidthMap(); j++) // по ширине
                {
					if (j < (p.x+VisibleDist)/32 && j > (p.x-VisibleDist)/32)
					{
                    switch (TileMap[i][j]) //оператор switch case, смотрит на значение в переменной и выполняет действия в зависимости от содержания. замена многочисленным if-ам.
                    {
                    case Floor: //земля
                        s_map.setTextureRect(IntRect(200, 8, 32, 32));
                        break;
                    case Stone: //камень
                        s_map.setTextureRect(IntRect(328, 200, 32, 32));
                        break;
                    case Brick: // кирпич. Бордюр карты
                        //s_map.setTextureRect(IntRect(104, 142, 32, 32));
						s_map.setTextureRect(IntRect(200, 8, 32, 32)); //эта зона запрета ничем не отличается от пейзажа
                        break;
                    case Rock: // скалы
                        s_map.setTextureRect(IntRect(296, 232, 32, 32));
                        break;
					case Grass: // трава
                        s_map.setTextureRect(IntRect(40, 8, 32, 32));
                        break;
					case Coast1: // берег
                        s_map.setTextureRect(IntRect(488, 200, 32, 32));
                        break;
					case Coast2: // берег
                        s_map.setTextureRect(IntRect(520, 200, 32, 32));
                        break;
					case Coast3: // берег
                        s_map.setTextureRect(IntRect(552, 200, 32, 32));
                        break;
					case Coast4: // берег
                        s_map.setTextureRect(IntRect(584, 200, 32, 32));
                        break;
					case Coast5: // берег
                        s_map.setTextureRect(IntRect(488, 232, 32, 32));
                        break;
					case Woter1: // вода
                        s_map.setTextureRect(IntRect(520, 232, 32, 32));
                        break;
					case Woter2: // вода
                        s_map.setTextureRect(IntRect(552, 232, 32, 32));
                        break;
					case Coast8: // берег
                        s_map.setTextureRect(IntRect(584, 232, 32, 32));
                        break;
					case Coast9: // берег
                        s_map.setTextureRect(IntRect(488, 264, 32, 32));
                        break;
					case Coast10: // берег
                        s_map.setTextureRect(IntRect(520, 264, 32, 32));
                        break;
					case Coast11: // берег
                        s_map.setTextureRect(IntRect(552, 264, 32, 32));
                        break;
					case Coast12: // берег
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
			
			TileMap[40+x/32][40+y/32]=Brick; // кладем кирпичи под дом, что б через него нельзя было ходить
	
		}
		
		for(int x=0; x/32<5; x+=32)
		for(int y=0; y/32<5; y+=32)
		{
			s_map.setTextureRect(IntRect(512+x, 8+y, 32, 32));
			s_map.setPosition(1280+x,1280+y); 
			window.draw(s_map);
	
		}

}