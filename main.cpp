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

float SetRandomSpeed(int r) //������ �������� ����� ��������� �� 0 �� speed/1000
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
srand(time(0)); 	//������ ������ ������ ������.
int VisibleDist=480;//��������� ��������� �������
int CountSheep=150; //��������� ��������� ����
int CountWolf=10; 	//��������� ���������� ������
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
//easyEnemyImage.createMaskFromColor(Color(255, 0, 0));//������� ����� �� �����.�� ����� ���������� ����� ���������� ��������. ��� ����� ���� � ��������� - �������� ������ �������:)

Player p(heroImage,1000,1000,32,32,3,0);//������ ������ ������. x=1000 -��������� ����������. 32x32 - ������ ��������
std::list<Entity*>  entities;//������ ������, ���� ���� ������ �������.�������� ������.
std::list<Entity*>::iterator it;//�������� ����� ��������� �� ��-��� ������
std::list<Entity*>::iterator it2; // ��� �������� ������� ���� � ������

for (int i = 0; i < CountSheep; i++)//���������� �� ��������� ����� �������(� ������ �� �����)
{
	int r=abs(rand()%(HEIGHT_MAP*32)-200); //��������� ���������� ��������� �����
	int r2=abs(rand()%(HEIGHT_MAP*32)-200);
	entities.push_back(new Enemy(sheepImage,r,r2,32,32,1,rand()%10));//� ���������� �� � ������
}

for (int i = 0; i < CountWolf; i++)//���������� �� ��������� ����� �������(� ������ �� ������)
{
	int r=abs(rand()%(HEIGHT_MAP*32)-200); //��������� ���������� ��������� �����
	int r2=abs(rand()%(HEIGHT_MAP*32)-200);
	entities.push_back(new Enemy(wolfImage,r,r2,32,32,2,rand()%10));//� ���������� �� � ������
}

//HWND hWnd = GetConsoleWindow();//����� ����� �������, (�������� ������� (�������� ������ ��� windows))
//ShowWindow(hWnd, SW_HIDE);//�������� �������

RenderWindow window(VideoMode(640, 480), "Glass and Grass");
view.reset(FloatRect(0, 0, 640, 480));
window.setFramerateLimit(60); //������������� ������� ������ � �������

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
s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //���������� �����, ������� �������� ��������
//s_quest.setScale(0.6f, 0.6f);//���� ��������� ��������, => ������ ���� ������
bool showMissionText = true;//���������� ����������, ���������� �� ��������� ������ ������ �� ������

	
SetMap(); // ������ �����

	while (window.isOpen())
	{
		//playMusic("ding.flac");
		changeview();
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds(); 
		else  { view.move(0.8, 0); }
		clock.restart();
		time = time / 800;
		
		//if(rand()%1000<2)SetGrassOnMap(); // ���� �� ��������� �� ����� ����� � ��������� �����.
		countGrass++;
		if(countGrass > 1000){ SetGrassOnMap(); countGrass=0; } //��������� �� ����� ����� � ��������� �����.
	
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				for (it = entities.begin(); it != entities.end();) {Entity *b = *it; it = entities.erase(it); delete b;}
				window.close();
			}
			
			if (event.type == Event::KeyPressed)//������� ������� �������
			if ((event.key.code == Keyboard::Tab)) {//���� ������� ���					
				switch (showMissionText) {//�������������, ����������� �� ���������� ���������� showMissionText
				case true: {
							   std::ostringstream playerHealthString;//������ �������� ������
							   playerHealthString << p.health; //������� � ������ �������� 
							   std::ostringstream playerTime;//������ �������� ������
							   playerTime << time; //������� � ������ �������� 
							   std::ostringstream sheep;
							   sheep << entities.size();
							   std::ostringstream task;//������ ������ ������
							   task << getTextMission(getCurrentMission(p.getplayercoordinateX()));//���������� ������� getTextMission (��� ���������� ����� ������), ������� ��������� � �������� ��������� ������� getCurrentMission(������������ ����� ������), � ��� ��� �-��� ��������� � �������� ��������� ������� p.getplayercoordinateX() (��� �-��� ���������� ��� ���������� ������)
							   text.setString("��������: " + playerHealthString.str());
							   //text.setString("�����: " + playerTime.str() + "\n" + task.str());
							   text.setString("����: " + sheep.str());
							   
							   showMissionText = false;//��� ������ ��������� ������ ��� ��� �� ������ �� ������
							   break;//������� , ����� �� ��������� ������� "false" (������� ����)
				}
				case false: {
								text.setString("");//���� �� ������ ������� ���, �� ���� ���� ����� ������
								showMissionText = true;// � ��� ������ ��������� ����� ������ ������� ��� � �������� ����� �� �����
								break;
				}
				}
			}
		}
	
		if (!showMissionText) {
			text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//������� ����� ����� ���������� �����
			s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//������� ���� ��� �����			
			window.draw(s_quest); window.draw(text); //������ ������ ������ (��� ��� ������ ������). � ����� � �����. ��� ��� �������� �� ���������� ����������, ������� ������ ���� ��������� �� ������� ������� ���
		}
		
		p.update(time);
		
/////////////////////////// ��������////////////////////////////////
		//for (it = entities.begin(); it != entities.end();)//������� ��� ���������� �� ������ �� �����
		//{
		//	Entity *b = *it;//��� ��������, ����� �� ������ (*it)->
		//	b->update(time);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
		//	if (b->life == false)
		//	{ 
		//		if(b->name==1) CountSheep--;
		//		if(b->name==2) CountWolf--;
		//		//std::cout << "DEAD " << b->age <<"\n";
		//		it = entities.erase(it); delete b; 
		//		
		//	}// ���� ���� ������ �����, �� ������� ���			
		//	else it++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		//}
				

		window.setView(view);
		window.clear();


//		if ((getCurrentMission(p.getplayercoordinateX())) == 0) 
//		{ 	//���� ������� ������ 0, �� ������ ����� ��� ���
			//��������� ����� �������� ��������� �������:
			
		//for (it = entities.begin(); it != entities.end(); it++)
		//{
		//	if ((*it)->x < p.x+VisibleDist && (*it)->x > p.x-VisibleDist)  
		//		if ((*it)->y < p.y+VisibleDist && (*it)->y > p.y-VisibleDist)
		//			window.draw((*it)->sprite); //������ entities �������
		//}
		
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

//		if ((getCurrentMission(p.getplayercoordinateX())) >= 1) { //��� ����� ����. ��������� ������� �� ������.
//																	//������� �� �������, � �� �� �����������.
//        for (int i = 0; i < HEIGHT_MAP; i++) //���� �� ������
//                for (int j = 0; j < WIDTH_MAP; j++) // �� ������
//                {
//                    switch (TileMap[i][j]) //�������� switch case, ������� �� �������� � ���������� � ��������� �������� � ����������� �� ����������. ������ �������������� if-��.
//                    {
//                    case Floor: //�����
//                        s_map.setTextureRect(IntRect(200, 8, 32, 32));
//                        break;
//                    case Stone: //������
//                        s_map.setTextureRect(IntRect(328, 200, 32, 32));
//                        break;
//                    case Brick: // ������
//                        s_map.setTextureRect(IntRect(104, 142, 32, 32));
//                        break;
//                    case Rock: // �����
//                        s_map.setTextureRect(IntRect(296, 232, 32, 32));
//                        break;
//					case Grass: // �����
//                        s_map.setTextureRect(IntRect(40, 8, 32, 32));
//                        break;
//                    };
//                    s_map.setPosition(j * 32, i * 32); 
//					window.draw(s_map);
//                }
//		}

					
//////////////////////////////////������������ ��������//////////////////////////////////////////
	LimitUpdateSex+=1;
	if(LimitUpdateSex>0)
	{			
		for (it = entities.begin(); it != entities.end(); it++) 
		{
			for (it2 = it; it2 != entities.end(); it2++) 
				{
				if(it2==it) continue;	
				//////////////////////////������������ �������� � �������///////////////////////////
				//if ((*it)->getRect().intersects(p.getRect()))//���� ������������� ������� ������� ������������ � �������
				//{
				//	if ((*it)->name == 1) //� ��� ���� ��� ������� Sheep,��..
				//	{
				//
				//		////////������������ �����
				//		if ((*it)->dx > 0)//���� ���� ���� ������
				//			(*it)->x = (*it)->x - 1; //����������� ��� �� ������ ����� (�������)
				//		if ((*it)->dx < 0)//���� ���� ���� �����
				//			(*it)->x = (*it)->x + 1; //���������� - ����������� ������
				//		if ((*it)->dy > 0)//���� ���� ���� ����
				//			(*it)->y = (*it)->y - 1; //����������� ��� �� ������ ����� (�������)
				//		if ((*it)->dy < 0)//���� ���� ���� �����
				//			(*it)->y = (*it)->y + 1; //���������� - ����������� ������
				//		(*it)->dx = 0;//�������������
				//		(*it)->dy = 0;
				//		(*it2)->dx = 0;//�������������
				//		(*it2)->dy = 0;
				//		///////������������ ������
				//		//if (p.dx < 0) { p.x = (*it)->x + (*it)->w;}//���� ����������� � ������ � ����� ���� ����� �� ����������� ������
				//		//if (p.dx > 0) { p.x = (*it)->x - (*it)->w;}//���� ����������� � ������ � ����� ���� ������ �� ����������� ������
				//		
				//
				//	}				
				//}
				//////////////////////////����� ������������ �������� � �������///////////////////////////
				
					
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
					 //���� ����� �� ������
					
					if((*it)->name==1 && (*it2)->name==1 && rand()<20 &&
					abs((*it)->x-(*it2)->x)<(*it)->SeeSheepRadius && 
					abs((*it)->y-(*it2)->y)<(*it)->SeeSheepRadius)
					{
					if((*it)->x<(*it2)->x)	(*it)->dx =  SetRandomSpeed(50); 
					else 					(*it)->dx = -SetRandomSpeed(50);
					if((*it)->y<(*it2)->y) 	(*it)->dy =  SetRandomSpeed(50); 
					else 					(*it)->dy = -SetRandomSpeed(50);
					}//���� ����� � �����
					
					//if((*it)->getRect().intersects((*it2)->getRect())) //���� ����������� ��� �������
					if((*it)->x-32 < (*it2)->x && (*it)->x+32 > (*it2)->x)
					if((*it)->y-32 < (*it2)->y && (*it)->y+32 > (*it2)->y)
					{ 
						if ((*it)->name == 1 && (*it2)->name == 1 && (*it)->readyToChild==true && (*it2)->readyToChild==true)
						{
							//std::cout << "satiety before: " << (*it)->satiety << ", age: " << (*it)->age << ", health: "<<  (*it)->health << "\n"; //������� � ������� ������� � ������� ���� ��� �����������
							(*it)->satiety-=5;
							(*it2)->satiety-=5;
							CountSheep+=3;
							entities.push_back(new Enemy(sheepImage,(*it)->x,(*it)->y,32,32,1,0));
							entities.push_back(new Enemy(sheepImage,(*it)->x,(*it)->y,32,32,1,0));
							entities.push_back(new Enemy(sheepImage,(*it)->x,(*it)->y,32,32,1,0));
						}	
						
						if ((*it)->name == 1 && (*it2)->name == 2) 
						{
							if((*it)->age>3 && (*it2)->health<=100) // ���� ��� ���� ��������
							{
								//std::cout << "KILL sheep " << (*it)->satiety << (*it2)->satiety << " age " << (*it)->age <<"\n"; //������� � ������� ���� �������� ����
								(*it2)->health+=10;
								(*it)->life=false;
							}
							if((*it)->age<3 && (*it2)->health<=100) // ���� ��� ���� ��������
							{
								//std::cout << "KILL sheep " << (*it)->satiety << (*it2)->satiety << " age " << (*it)->age <<"\n"; //������� � ������� ���� �������� ����
								(*it2)->health+=3;
								(*it)->life=false;
							}	
						}
						
						if ((*it)->name==2 && (*it2)->name==2 && (*it)->readyToChild==true && (*it2)->readyToChild==true)//���� ����������� ��� ������� � ��� ��� �����
						{		
							//std::cout << "More wolf! " << "age- " << (*it)->age << ", health- "<<  (*it)->health << "\n"; //������� � ������� ������� � ������� ���� ��� �����������
							(*it) ->eros=0;
							(*it2)->eros=0;
							CountWolf += 3;
							entities.push_back(new Enemy(wolfImage,(*it)->x,(*it)->y,32,32,2,0));
							entities.push_back(new Enemy(wolfImage,(*it)->x,(*it)->y,32,32,2,0));
							entities.push_back(new Enemy(wolfImage,(*it)->x,(*it)->y,32,32,2,0));

						}
						
						////////������������ �����
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
					}// ���� ���� ������ �����, �� ������� ���
					
				}
			//}
			
			
			//if ((*it)->getRect().intersects(p.getRect()))//���� ������������� ������� ������� ������������ � �������
			if((*it)->x-32 < p.x && (*it)->x+32 > p.x)
			if((*it)->y-32 < p.y && (*it)->y+32 > p.y)
			{
				if ((*it)->name == 2) //� ��� ���� ��� ������� ����,��..
				{							
				p.health -= 0.1;
				if(ballSound.getStatus() != sf::Sound::Playing)ballSound.play();
				}
			}		

		}
		LimitUpdateSex=0;
	}
///////////////////////////////�������������� ���������//////////////////////////////////////
if(CountWolf<=0)
	{
	CountWolf=10;	
	for (int i = 0; i < CountWolf; i++)//���������� �� ��������� ����� �������(� ������ �� ������)
		{
		int r=abs(rand()%(HEIGHT_MAP*32)-200); //��������� ���������� ��������� �����
		int r2=abs(rand()%(HEIGHT_MAP*32)-200);
		entities.push_back(new Enemy(wolfImage,r,r2,32,32,2,rand()%10));//� ���������� �� � ������
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////	
	
		//������������ ������ ��� �������, ��� ����� � ������� ������
		for (it = entities.begin(); it != entities.end(); it++)
		{
			(*it)->update(time);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
			
			if ((*it)->x < p.x+VisibleDist && (*it)->x > p.x-VisibleDist)  
				if ((*it)->y < p.y+VisibleDist && (*it)->y > p.y-VisibleDist)
					window.draw((*it)->sprite); //������ entities �������
		}		
		
		//��� � ����������� ����� ���,  � ������ ������ ���� ���: renderInterfaceBack(0, 0, ViewSizeX, 30, window, Color::Green) � renderInterfaceBack(10, 10, 60, 10, window, Color::Grey) ������ ��� �������� - ����������, ������ ��� - ������
		renderInterfaceBack(view.getSize().x, 30, (view.getCenter().x - view.getSize().x / 2), (view.getCenter().y - view.getSize().y / 2), window); //��� ���������� ������� ������� � ����������. ���������� ��� ��� ���.
		renderHpBar(p.health, (view.getCenter().x - view.getSize().x / 2)+10, (view.getCenter().y - view.getSize().y / 2)+10, 60, 10, window, Color::Green); //��� �� ���������. ������� ����������, ����� ������ 60x10px

		std::ostringstream sheep;
		sheep << CountSheep;	
		std::ostringstream wolf;
		wolf << CountWolf;			
		text.setPosition((view.getCenter().x - view.getSize().x / 2)+100, (view.getCenter().y - view.getSize().y / 2)+5);
		text.setString("����: " + sheep.str() + ". ������: " + wolf.str());
		
		window.draw(p.sprite);	//������ ������		
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