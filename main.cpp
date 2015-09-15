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
srand(time(0)); 	//делаем рандом всегда разным.
int VisibleDist=360;//дистанция отрисовки существ
int CountSheep=500; //начальное количетво овец
int CountWolf=10; 	//Начальное количество волков
int CountItem=5;	//Начальное количество предметов на карте


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
//easyEnemyImage.createMaskFromColor(Color(255, 0, 0));//сделали маску по цвету.но лучше изначально иметь прозрачную картинку. как найду комп с фотошопом - исправлю спрайт шамаича:)

Player p(heroImage,400,400,32,32,"Player1");//объект класса игрока. x=1000 -начальная координата. 32x32 - размер текстуры
std::list<Entity*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
std::list<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка
std::list<Entity*>::iterator it2; // для проверки встречи друг с другом


for (int i = 0; i < CountSheep; i++)//проходимся по элементам этого вектора(а именно по овцам)
{
	int r=abs(rand()%(HEIGHT_MAP*32)-200); //начальные координаты появления юнита
	int r2=abs(rand()%(HEIGHT_MAP*32)-200);
	entities.push_back(new Enemy(sheepImage,r,r2,32,32,"Sheep"));//и закидываем их в список
}
for (int i = 0; i < CountWolf; i++)//проходимся по элементам этого вектора(а именно по волкам)
{
	int r=abs(rand()%(HEIGHT_MAP*32)-200);  //начальные координаты появления юнита
	int r2=abs(rand()%(HEIGHT_MAP*32)-200);
	entities.push_back(new Enemy(wolfImage,r,r2,32,32,"Wolf"));//и закидываем их в список
}

SetItem(450,450,20, "Sword");
//HWND hWnd = GetConsoleWindow();//берем текущ консоль, (скрывает консоль (работает только под windows))
//ShowWindow(hWnd, SW_HIDE);//скрывает консоль



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
window.setFramerateLimit(60); //устанавливаем частоту кадров в секунду

Font font;
font.loadFromFile("CyrilicOld.ttf");
Text text("", font, 16);
text.setColor(Color::White);

SetMap(); // делаем карту

	while (window.isOpen())
	{
	
		changeview();
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds(); 
		else  { view.move(0.8, 0); }
		clock.restart();
		time = time / 800;
		
		if(rand()%100<5)SetGrassOnMap(); // шанс на появление на карте травы в случайном месте.
		//SetGrassOnMap(); //появление на карте травы в случайном месте.
		
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
		
/////////////////////////// Убийства////////////////////////////////
		for (it = entities.begin(); it != entities.end();)//говорим что проходимся от начала до конца
		{
			Entity *b = *it;//для удобства, чтобы не писать (*it)->
			b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			if (b->life == false)
			{ 
				std::cout << "DEAD from noneat " << b->age <<"\n";
				it = entities.erase(it); delete b; 
				
			}// если этот объект мертв, то удаляем его			
			else {
				it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
			}
		}
	

		window.setView(view);
		window.clear();
		
        for (int i = 0; i < HEIGHT_MAP; i++) //цикл по высоте
		  if (i < (p.y+VisibleDist)/32 && i > (p.y-VisibleDist)/32)
                for (int j = 0; j < WIDTH_MAP; j++) // по ширине
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
		
//////////////////////////////////столкновения объектов//////////////////////////////////////////
		LimitUpdateSex+=0.1;
		if(LimitUpdateSex>0.4)
		{
		for (it = entities.begin(); it != entities.end(); it++) 
		{
			
			if((*it)->satiety>10 && (*it)->age>3) // если эта овца сыта и подходящего возраста
			{
				for (it2 = it; it2 != entities.end(); it2++) //столкновения овец
				{
					if ((*it)->getRect() != (*it2)->getRect())//при этом это должны быть разные прямоугольники
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "Sheep") && ((*it2)->name == "Sheep"))//если столкнулись два объекта и они оба овцы
					{
						if((*it2)->satiety>10 && (*it2)->age>3) // если эта овца сыта и подходящего возраста
						{
							std::cout << "satiety before " << (*it)->satiety << " age " << (*it)->age << "health "<<  (*it)->health << "\n"; //выводим в консоль сытость и возраст овец при размножении
							(*it)->satiety-=5;
							(*it2)->satiety-=5;
							entities.push_back(new Enemy(sheepImage,(*it)->x,(*it)->y,32,32,"Sheep"));
							//std::cout << "satiety " << (*it)->satiety << (*it2)->satiety <<"\n"; //выводим в консоль сытость овец при размножении
						}
					}
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "Sheep") && ((*it2)->name == "Wolf"))//если столкнулись два объекта и они враги
					{
						if((*it)->age>3 && (*it2)->health<90) // если эта овца взрослая
							{
								std::cout << "KILL sheep " << (*it)->satiety << (*it2)->satiety << " age " << (*it)->age <<"\n"; //выводим в консоль факт поедания овцы
								(*it2)->health+=10;
								Entity *b = *it;
								it = entities.erase(it); delete b; 
								//std::cout << "satiety " << (*it)->satiety << (*it2)->satiety <<"\n"; //выводим в консоль сытость овец при размножении
							}
							else if((*it2)->health<90)
							{
								std::cout << "KILL little sheep " << (*it)->satiety << (*it2)->satiety << " age " << (*it)->age <<"\n"; //выводим в консоль факт поедания овцы
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
		
		//отрисовываем только тех существ, кто рядом с главным героем
		for (it = entities.begin(); it != entities.end(); it++)
		{
			if ((*it)->x < p.x+VisibleDist && (*it)->x > p.x-VisibleDist)  
				if ((*it)->y < p.y+VisibleDist && (*it)->y > p.y-VisibleDist){
					renderHpBar((*it)->satiety*10, (*it)->x, (*it)->y-3, 32, 2, window, Color::Blue); //полоски жизни над существами
					renderHpBar((*it)->health, (*it)->x, (*it)->y, 32, 2, window, Color::Green); //полоски жизни над существами
					window.draw((*it)->sprite); //рисуем entities объекты
				}
		}
		
		SetViewItems(VisibleDist, p.x, p.y, window, time); //рисуем предметы
		
				
		window.draw(p.sprite);	//рисуем игрока		
		
		//тут я прикручивал чужой код, поэтому начинается совсем уж лютый пиздец, хуже, чем весь остальной. В идеале должно быть так: renderInterfaceBack(0, 0, ViewSizeX, 30, window, Color::Green) и renderInterfaceBack(10, 10, 60, 10, window, Color::Grey) Первые два значения - координаты, вторые два - размер
		renderInterfaceBack(view.getSize().x, 30, (view.getCenter().x - view.getSize().x / 2), (view.getCenter().y - view.getSize().y / 2), window); //тут перепутаны местами размеры и координаты. Исправлять сил уже нет.
		renderHpBar(p.health, (view.getCenter().x - view.getSize().x / 2)+10, (view.getCenter().y - view.getSize().y / 2)+10, 60, 10, window, Color::Green); //тут всё правильно. Сначала координаты, потом размер 60x10px
		
				std::ostringstream sheep;
				sheep << entities.size();
				std::ostringstream items;
				items << GetItemsSize();
				text.setPosition((view.getCenter().x - view.getSize().x / 2)+100, (view.getCenter().y - view.getSize().y / 2)+5);
				text.setString("существ: " + sheep.str() + ", предметов: " + items.str());
				window.draw(text);
				
		window.display();
	}
	entities.clear();
//		for (it = entities.begin(); it != entities.end();)//при выходе из игры чистим список существ. 
//		{
//			Entity *b = *it;
//			it = entities.erase(it); delete b; 
//			it++;
//		}
	return 0;
}