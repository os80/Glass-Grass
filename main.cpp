#define _WIN32_WINNT 0x0500
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "mission.h"
#include <windows.h>
#include <ctime>
#include <string>
#include <vector>
#include <list>
#include "view.h"
#include "interface.h"

#include <SFML/Audio.hpp>
#include <iomanip>
#include <iostream>
#include <string>

using namespace sf;

std::string CountLion;
int it;

#include "map.h"
#include "enemy.h"

float SetRandomSpeed(int r) //делаем скорость юнита случайной от 0 до speed/1000
{
	//r*=1000;
	float a;
	a=rand()%r;
	a/=1000;
	return abs(a);
}
	
void playMusic(const std::string& filename)
{
    // Load an ogg music file
    sf::Music music;
    if (!music.openFromFile("resources/" + filename))
	{std::cout << " "  << " No music"       << std::endl;
	return;
	}        

    if(music.getStatus() != sf::Music::Playing)music.play();
}
	
int main()
{
Clock clock;
Clock gameTimeClock;
int gameTime = 0;
int LimitUpdateSex=0;
//float LimitUpdateCount=0;
srand(time(0)); 	//делаем рандом всегда разным.
int VisibleDist=480;//дистанция отрисовки существ
int CountSheep=150; //начальное количетво овец
int CountWolf=10; 	//Начальное количество волков
int countGrass=0;

    // Load the sounds used in the game
    sf::SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
        return EXIT_FAILURE;
    sf::Sound ballSound(ballSoundBuffer);	

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

Player p(heroImage,1000,1000,32,32,3,0);//объект класса игрока. x=1000 -начальная координата. 32x32 - размер текстуры
std::list<Entity*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
std::list<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка
std::list<Entity*>::iterator it2; // для проверки встречи друг с другом

for (int i = 0; i < CountSheep; i++)//проходимся по элементам этого вектора(а именно по овцам)
{
	int r=abs(rand()%(HEIGHT_MAP*32)-200); //начальные координаты появления юнита
	int r2=abs(rand()%(HEIGHT_MAP*32)-200);
	entities.push_back(new Enemy(sheepImage,r,r2,32,32,1,rand()%10));//и закидываем их в список
}

for (int i = 0; i < CountWolf; i++)//проходимся по элементам этого вектора(а именно по волкам)
{
	int r=abs(rand()%(HEIGHT_MAP*32)-200); //начальные координаты появления юнита
	int r2=abs(rand()%(HEIGHT_MAP*32)-200);
	entities.push_back(new Enemy(wolfImage,r,r2,32,32,2,rand()%10));//и закидываем их в список
}

//HWND hWnd = GetConsoleWindow();//берем текущ консоль, (скрывает консоль (работает только под windows))
//ShowWindow(hWnd, SW_HIDE);//скрывает консоль

RenderWindow window(VideoMode(640, 480), "Glass and Grass");
view.reset(FloatRect(0, 0, 640, 480));
window.setFramerateLimit(60); //устанавливаем частоту кадров в секунду

std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
for (std::size_t i = 0; i < modes.size(); ++i)
{
	sf::VideoMode mode = modes[i];
	std::cout << "Mode #" << i << ": "
			<< mode.width << "x" << mode.height << " - "
			<< mode.bitsPerPixel << " bpp" << std::endl;
}
// Create a window with the same pixel depth as the desktop
sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
window.create(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Glass and Grass");

Font font;
font.loadFromFile("CyrilicOld.TTF");
Text text("", font, 15);
text.setColor(Color::White);

Image quest_image;
quest_image.loadFromFile("images/missionbg.jpg");
quest_image.createMaskFromColor(Color(0, 0, 0));
Texture quest_texture;
quest_texture.loadFromImage(quest_image);
Sprite s_quest;
s_quest.setTexture(quest_texture);
s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //приведение типов, размеры картинки исходные
//s_quest.setScale(0.6f, 0.6f);//чуть уменьшили картинку, => размер стал меньше
bool showMissionText = true;//логическая переменная, отвечающая за появление текста миссии на экране

	
SetMap(); // делаем карту

	while (window.isOpen())
	{
		//playMusic("ding.flac");
		changeview();
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds(); 
		else  { view.move(0.8, 0); }
		clock.restart();
		time = time / 800;
		
		//if(rand()%1000<2)SetGrassOnMap(); // шанс на появление на карте травы в случайном месте.
		countGrass++;
		if(countGrass > 1000){ SetGrassOnMap(); countGrass=0; } //появление на карте травы в случайном месте.
	
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				for (it = entities.begin(); it != entities.end();) {Entity *b = *it; it = entities.erase(it); delete b;}
				window.close();
			}
			
			if (event.type == Event::KeyPressed)//событие нажатия клавиши
			if ((event.key.code == Keyboard::Tab)) {//если клавиша ТАБ					
				switch (showMissionText) {//переключатель, реагирующий на логическую переменную showMissionText
				case true: {
							   std::ostringstream playerHealthString;//строка здоровья игрока
							   playerHealthString << p.health; //заносим в строку здоровье 
							   std::ostringstream playerTime;//строка здоровья игрока
							   playerTime << time; //заносим в строку здоровье 
							   std::ostringstream sheep;
							   sheep << entities.size();
							   std::ostringstream task;//строка текста миссии
							   task << getTextMission(getCurrentMission(p.getplayercoordinateX()));//вызывается функция getTextMission (она возвращает текст миссии), которая принимает в качестве аргумента функцию getCurrentMission(возвращающую номер миссии), а уже эта ф-ция принимает в качестве аргумента функцию p.getplayercoordinateX() (эта ф-ция возвращает Икс координату игрока)
							   text.setString("Здоровье: " + playerHealthString.str());
							   //text.setString("время: " + playerTime.str() + "\n" + task.str());
							   text.setString("овцы: " + sheep.str());
							   
							   showMissionText = false;//эта строка позволяет убрать все что мы вывели на экране
							   break;//выходим , чтобы не выполнить условие "false" (которое ниже)
				}
				case false: {
								text.setString("");//если не нажата клавиша таб, то весь этот текст пустой
								showMissionText = true;// а эта строка позволяет снова нажать клавишу таб и получить вывод на экран
								break;
				}
				}
			}
		}
	
		if (!showMissionText) {
			text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//позиция всего этого текстового блока
			s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//позиция фона для блока			
			window.draw(s_quest); window.draw(text); //рисуем спрайт свитка (фон для текста миссии). а затем и текст. все это завязано на логическую переменную, которая меняет свое состояние от нажатия клавиши ТАБ
		}
		
		p.update(time);
		
/////////////////////////// Убийства////////////////////////////////
		//for (it = entities.begin(); it != entities.end();)//говорим что проходимся от начала до конца
		//{
		//	Entity *b = *it;//для удобства, чтобы не писать (*it)->
		//	b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
		//	if (b->life == false)
		//	{ 
		//		if(b->name==1) CountSheep--;
		//		if(b->name==2) CountWolf--;
		//		//std::cout << "DEAD " << b->age <<"\n";
		//		it = entities.erase(it); delete b; 
		//		
		//	}// если этот объект мертв, то удаляем его			
		//	else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		//}
				

		window.setView(view);
		window.clear();


//		if ((getCurrentMission(p.getplayercoordinateX())) == 0) 
//		{ 	//Если текущая миссия 0, то рисуем карту вот так
			//отрисовка карты меняется следующим образом:
			
		//for (it = entities.begin(); it != entities.end(); it++)
		//{
		//	if ((*it)->x < p.x+VisibleDist && (*it)->x > p.x-VisibleDist)  
		//		if ((*it)->y < p.y+VisibleDist && (*it)->y > p.y-VisibleDist)
		//			window.draw((*it)->sprite); //рисуем entities объекты
		//}
		
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
		
//	int Countwoter, SizewoterH, SizewoterW;
//	Countwoter=10;//rand()%10;
//	SizewoterH=2+rand()%3;
//	SizewoterW=2+rand()%2;
//	
//	for(int i=0; i<Countwoter; i++){
//		int x_w=1+(rand()%HEIGHT_MAP-2);
//		int y_w=1+(rand()%WIDTH_MAP-2);
//		for(int x=0; x/32<5; x+=32)
//		for(int y=0; y/32<5; y+=32)
//		{
//			s_map.setTextureRect(IntRect(488+x, 200+y, 32, 32));
//			s_map.setPosition(x_w,y_w); 
//			window.draw(s_map);
//	
//		}
//	}
//		}

//		if ((getCurrentMission(p.getplayercoordinateX())) >= 1) { //тут будет ночь. Програмно сделать всё темнее.
//																	//условие по таймеру, а не по координатам.
//        for (int i = 0; i < HEIGHT_MAP; i++) //цикл по высоте
//                for (int j = 0; j < WIDTH_MAP; j++) // по ширине
//                {
//                    switch (TileMap[i][j]) //оператор switch case, смотрит на значение в переменной и выполняет действия в зависимости от содержания. замена многочисленным if-ам.
//                    {
//                    case Floor: //земля
//                        s_map.setTextureRect(IntRect(200, 8, 32, 32));
//                        break;
//                    case Stone: //камень
//                        s_map.setTextureRect(IntRect(328, 200, 32, 32));
//                        break;
//                    case Brick: // кирпич
//                        s_map.setTextureRect(IntRect(104, 142, 32, 32));
//                        break;
//                    case Rock: // скалы
//                        s_map.setTextureRect(IntRect(296, 232, 32, 32));
//                        break;
//					case Grass: // трава
//                        s_map.setTextureRect(IntRect(40, 8, 32, 32));
//                        break;
//                    };
//                    s_map.setPosition(j * 32, i * 32); 
//					window.draw(s_map);
//                }
//		}

					
//////////////////////////////////столкновения объектов//////////////////////////////////////////
	LimitUpdateSex+=1;
	if(LimitUpdateSex>0)
	{			
		for (it = entities.begin(); it != entities.end(); it++) 
		{
			for (it2 = it; it2 != entities.end(); it2++) 
				{
				if(it2==it) continue;	
				//////////////////////////столкновение объектов с игроком///////////////////////////
				//if ((*it)->getRect().intersects(p.getRect()))//если прямоугольник спрайта объекта пересекается с игроком
				//{
				//	if ((*it)->name == 1) //и при этом имя объекта Sheep,то..
				//	{
				//
				//		////////выталкивание врага
				//		if ((*it)->dx > 0)//если враг идет вправо
				//			(*it)->x = (*it)->x - 1; //отталкиваем его от игрока влево (впритык)
				//		if ((*it)->dx < 0)//если враг идет влево
				//			(*it)->x = (*it)->x + 1; //аналогично - отталкиваем вправо
				//		if ((*it)->dy > 0)//если враг идет вниз
				//			(*it)->y = (*it)->y - 1; //отталкиваем его от игрока влево (впритык)
				//		if ((*it)->dy < 0)//если враг идет вверх
				//			(*it)->y = (*it)->y + 1; //аналогично - отталкиваем вправо
				//		(*it)->dx = 0;//останавливаем
				//		(*it)->dy = 0;
				//		(*it2)->dx = 0;//останавливаем
				//		(*it2)->dy = 0;
				//		///////выталкивание игрока
				//		//if (p.dx < 0) { p.x = (*it)->x + (*it)->w;}//если столкнулись с врагом и игрок идет влево то выталкиваем игрока
				//		//if (p.dx > 0) { p.x = (*it)->x - (*it)->w;}//если столкнулись с врагом и игрок идет вправо то выталкиваем игрока
				//		
				//
				//	}				
				//}
				//////////////////////////КОНЕЦ столкновение объектов с игроком///////////////////////////
				
					
					if((*it)->name==1 && (*it2)->name==2)
						if(abs((*it)->x-(*it2)->x)<(*it)->SeeSheepRadius && 
						abs((*it)->y-(*it2)->y)<(*it)->SeeSheepRadius)
						{
							if((*it)->x < (*it2)->x) {(*it)->dx = -SetRandomSpeed(50); (*it)->state2=1;}
							else 					 {(*it)->dx =  SetRandomSpeed(50); (*it)->state2=2;}
							if((*it)->y < (*it2)->y) {(*it)->dy = -SetRandomSpeed(50); (*it)->state2=3;}
							else 					 {(*it)->dy =  SetRandomSpeed(50); (*it)->state2=4;}
						}
						//else (*it)->state2=0;
					 //ОВЦЫ БЕГУТ ОТ ВОЛКОВ
					
					if((*it)->name==1 && (*it2)->name==1 && rand()<20 &&
					abs((*it)->x-(*it2)->x)<(*it)->SeeSheepRadius && 
					abs((*it)->y-(*it2)->y)<(*it)->SeeSheepRadius)
					{
					if((*it)->x<(*it2)->x)	(*it)->dx =  SetRandomSpeed(50); 
					else 					(*it)->dx = -SetRandomSpeed(50);
					if((*it)->y<(*it2)->y) 	(*it)->dy =  SetRandomSpeed(50); 
					else 					(*it)->dy = -SetRandomSpeed(50);
					}//овцы бегут к овцам
					
					//if((*it)->getRect().intersects((*it2)->getRect())) //если столкнулись два объекта
					if((*it)->x-32 < (*it2)->x && (*it)->x+32 > (*it2)->x)
					if((*it)->y-32 < (*it2)->y && (*it)->y+32 > (*it2)->y)
					{ 
						if ((*it)->name == 1 && (*it2)->name == 1 && (*it)->readyToChild==true && (*it2)->readyToChild==true)
						{
							//std::cout << "satiety before: " << (*it)->satiety << ", age: " << (*it)->age << ", health: "<<  (*it)->health << "\n"; //выводим в консоль сытость и возраст овец при размножении
							(*it)->satiety-=5;
							(*it2)->satiety-=5;
							CountSheep+=3;
							entities.push_back(new Enemy(sheepImage,(*it)->x,(*it)->y,32,32,1,0));
							entities.push_back(new Enemy(sheepImage,(*it)->x,(*it)->y,32,32,1,0));
							entities.push_back(new Enemy(sheepImage,(*it)->x,(*it)->y,32,32,1,0));
						}	
						
						if ((*it)->name == 1 && (*it2)->name == 2) 
						{
							if((*it)->age>3 && (*it2)->health<=100) // если эта овца взрослая
							{
								//std::cout << "KILL sheep " << (*it)->satiety << (*it2)->satiety << " age " << (*it)->age <<"\n"; //выводим в консоль факт поедания овцы
								(*it2)->health+=10;
								(*it)->life=false;
							}
							if((*it)->age<3 && (*it2)->health<=100) // если эта овца взрослая
							{
								//std::cout << "KILL sheep " << (*it)->satiety << (*it2)->satiety << " age " << (*it)->age <<"\n"; //выводим в консоль факт поедания овцы
								(*it2)->health+=3;
								(*it)->life=false;
							}	
						}
						
						if ((*it)->name==2 && (*it2)->name==2 && (*it)->readyToChild==true && (*it2)->readyToChild==true)//если столкнулись два объекта и они оба волки
						{		
							//std::cout << "More wolf! " << "age- " << (*it)->age << ", health- "<<  (*it)->health << "\n"; //выводим в консоль сытость и возраст овец при размножении
							(*it) ->eros=0;
							(*it2)->eros=0;
							CountWolf += 3;
							entities.push_back(new Enemy(wolfImage,(*it)->x,(*it)->y,32,32,2,0));
							entities.push_back(new Enemy(wolfImage,(*it)->x,(*it)->y,32,32,2,0));
							entities.push_back(new Enemy(wolfImage,(*it)->x,(*it)->y,32,32,2,0));

						}
						
						////////выталкивание врага
						if ((*it)->dx > 0)	(*it)->x = (*it)->x - 0.1;
						if ((*it)->dx < 0)	(*it)->x = (*it)->x + 0.1;
						if ((*it)->dy > 0)	(*it)->y = (*it)->y - 0.1;
						if ((*it)->dy < 0)	(*it)->y = (*it)->y + 0.1;	
						
						//if ((*it)->dx > 0)	{(*it)->x = (*it)->x - 1; (*it)->state2=0; (*it)->dx=0;}
						//if ((*it)->dx < 0)	{(*it)->x = (*it)->x + 1; (*it)->state2=0; (*it)->dx=0;}
						//if ((*it)->dy > 0)	{(*it)->y = (*it)->y - 1; (*it)->state2=0; (*it)->dy=0;}
						//if ((*it)->dy < 0)	{(*it)->y = (*it)->y + 1; (*it)->state2=0; (*it)->dy=0;}								
							
					}		
					
					if((*it)->name==2 && (*it2)->name==2 && (*it)->readyToChild==true && (*it2)->readyToChild==true)
					{
						if((*it)->x<(*it2)->x) (*it)->dx = SetRandomSpeed(60); else (*it)->dx = -SetRandomSpeed(60);
						if((*it)->y<(*it2)->y) (*it)->dy = SetRandomSpeed(60); else (*it)->dy = -SetRandomSpeed(60);						
					}
					
					//}
					
					if ((*it2)->life == false)
					{ 
						if((*it2)->name==1) CountSheep--;
						if((*it2)->name==2) CountWolf--;
						//std::cout << "DEAD " << b->age <<"\n";
						it2 = entities.erase(it2); //delete b; 				
					}// если этот объект мертв, то удаляем его
					
				}
			//}
			
			
			//if ((*it)->getRect().intersects(p.getRect()))//если прямоугольник спрайта объекта пересекается с игроком
			if((*it)->x-32 < p.x && (*it)->x+32 > p.x)
			if((*it)->y-32 < p.y && (*it)->y+32 > p.y)
			{
				if ((*it)->name == 2) //и при этом имя объекта Волк,то..
				{							
				p.health -= 0.1;
				if(ballSound.getStatus() != sf::Sound::Playing)ballSound.play();
				}
			}		

		}
		LimitUpdateSex=0;
	}
///////////////////////////////восстановление популяции//////////////////////////////////////
if(CountWolf<=0)
	{
	CountWolf=10;	
	for (int i = 0; i < CountWolf; i++)//проходимся по элементам этого вектора(а именно по волкам)
		{
		int r=abs(rand()%(HEIGHT_MAP*32)-200); //начальные координаты появления юнита
		int r2=abs(rand()%(HEIGHT_MAP*32)-200);
		entities.push_back(new Enemy(wolfImage,r,r2,32,32,2,rand()%10));//и закидываем их в список
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////	
	
		//отрисовываем только тех существ, кто рядом с главным героем
		for (it = entities.begin(); it != entities.end(); it++)
		{
			(*it)->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			
			if ((*it)->x < p.x+VisibleDist && (*it)->x > p.x-VisibleDist)  
				if ((*it)->y < p.y+VisibleDist && (*it)->y > p.y-VisibleDist)
					window.draw((*it)->sprite); //рисуем entities объекты
		}		
		
		//тут я прикручивал чужой код,  В идеале должно быть так: renderInterfaceBack(0, 0, ViewSizeX, 30, window, Color::Green) и renderInterfaceBack(10, 10, 60, 10, window, Color::Grey) Первые два значения - координаты, вторые два - размер
		renderInterfaceBack(view.getSize().x, 30, (view.getCenter().x - view.getSize().x / 2), (view.getCenter().y - view.getSize().y / 2), window); //тут перепутаны местами размеры и координаты. Исправлять сил уже нет.
		renderHpBar(p.health, (view.getCenter().x - view.getSize().x / 2)+10, (view.getCenter().y - view.getSize().y / 2)+10, 60, 10, window, Color::Green); //тут всё правильно. Сначала координаты, потом размер 60x10px

		std::ostringstream sheep;
		sheep << CountSheep;	
		std::ostringstream wolf;
		wolf << CountWolf;			
		text.setPosition((view.getCenter().x - view.getSize().x / 2)+100, (view.getCenter().y - view.getSize().y / 2)+5);
		text.setString("Овец: " + sheep.str() + ". Волков: " + wolf.str());
		
		window.draw(p.sprite);	//рисуем игрока		
		window.draw(text);		
		
		//minimap.setCenter((view.getCenter().x - view.getSize().x / 2)+50, (view.getCenter().y - view.getSize().y / 2)+50);
		//minimap.setSize(200, 200);
		//view.setViewport(sf::FloatRect(0, 0, 1, 1));
		//minimap.setViewport(sf::FloatRect(100, 100, 400, 200));
		//minimap.zoom(4.f);
		//window.setView(minimap);
		//window.draw(s_map);
		//window.setView(minimap);
		//window.draw(minimap);	
		//window.setView(view);
		window.display();
	}

	return 0;
}