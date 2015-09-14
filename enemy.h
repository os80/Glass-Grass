#include <SFML/Graphics.hpp>

using namespace sf;
class Entity {
public:
virtual void update(float time) = 0;
	enum { left, right, up, down, jump, stay } state;//��������� ��� ������������ - ��������� �������
	float age, health, dx, dy, x, y, moveTimer, a, CurrentFrame;//�������� ���������� ������ ��� ������� �����
	double speed;
	int w,h,r,satiety;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;//����� ����� ���� ������, �� �� ����� ������ ������ ����� ��� �����.����� ���� �������� ������ �� ����� � ����� ������� ���� �������� � update � ����������� �� �����

	Entity(Image &image, float X, float Y,int W,int H,String Name)
	{
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0; 
		CurrentFrame=0;		
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect(){return FloatRect(x, y, w, h);}
};


class Player :public Entity {
public:
	//enum { left, right, up, down, jump, stay } state;//��������� ��� ������������ - ��������� �������
	int playerScore;//��� ���������� ����� ���� ������ � ������

	   Player(Image &image, float X, float Y,int W,int H,String Name):Entity(image,X,Y,W,H,Name){
		   playerScore = 0; state = stay;
		   if (name == "Player1"){
			   sprite.setTextureRect(IntRect(32, 192, w, h));
		   }
	   }

	   void control() //�������� ������ ���������
	   {
		   //if (Keyboard::isKeyPressed) 
		   //{
			   if (Keyboard::isKeyPressed(Keyboard::Left)) 		{state = left; 	if (Keyboard::isKeyPressed(Keyboard::LShift)) speed = 0.1; else speed = 0.05;}
			   else if (Keyboard::isKeyPressed(Keyboard::Right)){state = right; if (Keyboard::isKeyPressed(Keyboard::LShift)) speed = 0.1; else speed = 0.05;}
			   else if (Keyboard::isKeyPressed(Keyboard::Up)) 	{state = up; 	if (Keyboard::isKeyPressed(Keyboard::LShift)) speed = 0.1; else speed = 0.05;}
			   else if (Keyboard::isKeyPressed(Keyboard::Down)) {state = down; 	if (Keyboard::isKeyPressed(Keyboard::LShift)) speed = 0.1; else speed = 0.05;}
			   //if (Keyboard::isKeyPressed(Keyboard::LShift)) speed = 0.1;
			   //std::cout << "press "<<"\n";
		   //}
		   else {state=stay; speed=0; dx=dy=0;}
	   } 

	   void checkCollisionWithMap(float Dx, float Dy)//������� �������� ������������ � ������
	   {
		   for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
		   for (int j = x / 32; j<(x + w) / 32; j++)
		   {
			   if (TileMap[i][j] == Brick)//���� ������� ��� ������ �������? ��
			   {
				   if (Dy>0){ y = i * 32 - h;	dy = 0; }	//�� Y ����=>���� � ���(����� �� �����) ��� ������. � ���� ������ ���� ���������� ��������� � ��������� ��� �� �����, ��� ���� ������� ��� �� �� ����� ��� ����� ����� ����� �������
				   if (Dy<0){ y = i * 32 + 32;  dy = 0; }	//������������ � �������� ������ �����(����� � �� �����������)
				   if (Dx>0){ x = j * 32 - w; }				//� ������ ����� �����
				   if (Dx<0){ x = j * 32 + 32;}				// � ����� ����� �����
			   }
		   }
	   }
	    
	   void update(float time)
	   {
		   control();//������� ���������� ����������
		   switch (state)//��� �������� ��������� �������� � ����������� �� ���������
		   {
		   case right:
				{
				dx = speed; 
		   		CurrentFrame += 0.005*time; //������ ��� ����������� �� "������". ���������� ������� �� ���� �������� ������������ ������� � ��������. ������� 0.005 ����� �������� �������� ��������
				if (CurrentFrame > 3) CurrentFrame = 0; // ���� ������ � �������� ����� - ������������ �����.
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 192, 32, 32)); //���������� �� ����������� �. ���������� �������� ��������� � ���������� � ������ 0,96,96*2, � ����� 0
				break;
				}//��������� ���� ������
		   case left:
				{
				dx = -speed; 
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 160, 32, 32)); 
				break;
				}//��������� ���� �����
		   case up:
				{
				dy= -speed; 
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 224, 32, 32)); 
				break;
				}//����� ��������� �������� ������ (�������� �� ��������)
		   case down:
				{
				dy= speed; 
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 128, 32, 32)); 
				break;
				}//����� ��������� �� ����� ������ ��������� (�������� �� ��������)
		   case stay: {CurrentFrame=0; break;}//� ����� ����		
		   }
		   x += dx*time;
		   checkCollisionWithMap(dx, 0);//������������ ������������ �� �
		   y += dy*time;
		   checkCollisionWithMap(0, dy);//������������ ������������ �� Y
		   sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
		   if (health <= 0){ life = false; }
		   if (!isMove){ speed = 0; }
		   //if (!onGround) { dy = dy + 0.0015*time; }//������� � ����� ������ ����������� � �����
		   if (life) { setPlayerCoordinateForView(x, y); }
		   //dy = dy + 0.0015*time;//��������� ������������� � �����
		   
		   
	   } 
	float getplayercoordinateX(){	
		return x;
	}
	float getplayercoordinateY(){
		return y;
	}
};

/////////////////////////////////////����� �����//////////////////////////////////////////////////
class Enemy :public Entity{
public:
	
	Enemy(Image &image, float X, float Y,int W,int H,String Name):Entity(image,X,Y,W,H,Name){ 

		if (name == "Sheep"){
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0;//������� ���� ����� �� �����.
			dy = 0;
			speed = 50;
			satiety = 5;
			age=rand()%10;
			
		}
		if (name == "Wolf"){
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0;//������� ���� ����� �� �����.
			dy = 0;
			speed = 10;
			satiety = 5;
			age=rand()%10;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if (TileMap[i][j] == Brick)//���� ������� ��� ������ �������, ��
			{
				if (Dy>0){ y = i * 32 - h; 	dy = 0; }//sprite.scale(-1, 1);}//�� Y ����=>���� � ���(����� �� �����) ��� ������. � ���� ������ ���� ���������� ��������� � ��������� ��� �� �����, ��� ���� ������� ��� �� �� ����� ��� ����� ����� ����� �������
				if (Dy<0){ y = i * 32 + 32; dy = 0; }//sprite.scale(-1, 1);}//������������ � �������� ������ �����(����� � �� �����������)
				if (Dx>0){ x = j * 32 - w; 	dx = 0; }//sprite.scale(-1, 1);}//� ������ ����� �����
				if (Dx<0){ x = j * 32 + 32; dx = 0; }//sprite.scale(-1, 1);}// � ����� ����� �����
			}
		}
	}
	
	float SetRandomSpeed(int r) //������ �������� ����� ��������� �� 0 �� speed/1000
	{
		//r*=1000;
		a=rand()%r;
		a/=1000;
		return abs(a);
	};
	void update(float time)
	{
		if (name == "Sheep") //��� ��������� � ����� ������ ������ ����� �����.
		{	
			//RectangleShape HpBar(sf::Vector2f(100*(health/100.0),10));
			//HpBar.setPosition(x,y);
			//window.draw(HpBar);
			moveTimer += time;
			if (moveTimer>3000+rand()%9000)//������ ����������� ��� � ��������� ������
			{ 
				switch (rand()%7) //��� ����� ������ ����, ��� ���� ����� ������ �� �����.
				{
				case 1: {dy = SetRandomSpeed(speed);  state=down;  dx=0; break;} // ���� ������ dx=0, �� ����� ������ �� ���������, �� �������� ����� ��� � ����� ����� ���������.
				case 2: {dy = -SetRandomSpeed(speed); state=up;    dx=0; break;}
				case 3: {dx = SetRandomSpeed(speed);  state=right; dy=0; break;}
				case 4: {dx = -SetRandomSpeed(speed); state=left;  dy=0; break;}
				default: {state=stay;}
				}
			moveTimer = 0; 
			}
			
			switch (state)//�������� � ����������� �� ���������.
			{
			case right:
				{
		   		CurrentFrame += 0.005*time; //������ ��� ����������� �� "������". ���������� ������� �� ���� �������� ������������ ������� � ��������. ������� 0.005 ����� �������� �������� ��������
				if (CurrentFrame > 3) CurrentFrame = 0; // ���� ������ � �������� ����� - ������������ �����.
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 64, 32, 32)); //���������� �� ����������� �. ���������� �������� ��������� � ���������� � ������ 0,96,96*2, � ����� 0
				break;
				}//��������� ���� ������
			case left:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 32, 32, 32)); 
				break;
				}//��������� ���� �����
			case up:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 96, 32, 32)); 
				break;
				}//��������� �������� ������
			case down:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 0, 32, 32)); 
				break;
				}//��������� �� ����� ������ ���������
			case stay: {CurrentFrame=0; dx=dy=0; break;}//���� ��������.		
			}	
			x += dx*time;
			checkCollisionWithMap(dx, 0);//������������ ������������ �� �
			y += dy*time;
			checkCollisionWithMap(0, dy);//������������ ������������ �� Y
			sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
			
			//��� �����
			for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
			for (int j = x / 32; j<(x + w) / 32; j++)			
			{
				if (TileMap[i][j] == Grass)//���� ������� ��� ������ �����, �� ������� �.				
				{
				if(health<100)health+=5;
				satiety++; //�����? ������� ��������
				if(rand()%2==1)	TileMap[i][j] = Floor; //������ �������� ����� �� ����� ��� �����
				else 			TileMap[i][j] = Stone;
				state=stay;
				}
			}
			health -= 0.01; // ���������� ��������� ��������.
			age += 0.001;	// ���������� ����������� �������.
			if(age<3)sprite.setScale(0.5f, 0.5f); else sprite.setScale(0.9f, 0.9f); //������� ������ ���������, �������� - �������
			if (health <= 0)life = false;
			if (age > 20) 	life = false;
		}
		if (name == "Wolf") //��� ��������� � ����� ������ ������ ����� �����.
		{	
				
			moveTimer += time;
			if (moveTimer>4000+rand()%9000)//������ ����������� ��� � ��������� ������
			{ 
				switch (rand()%5) //��� ����� ������ ����, ��� ���� ����� ������ �� �����.
				{
				case 1: {dy = 0.05;  state=down;  dx=0; break;} // ���� ������ dx=0, �� ����� ������ �� ���������, �� �������� ����� ��� � ����� ����� ���������.
				case 2: {dy = -0.05; state=up;    dx=0; break;}
				case 3: {dx = 0.05;  state=right; dy=0; break;}
				case 4: {dx = -0.05; state=left;  dy=0; break;}
				default: {state=stay;}
				}
			moveTimer = 0; 
			}
			
			switch (state)//�������� � ����������� �� ���������.
			{
			case right:
				{
		   		CurrentFrame += 0.005*time; //������ ��� ����������� �� "������". ���������� ������� �� ���� �������� ������������ ������� � ��������. ������� 0.005 ����� �������� �������� ��������
				if (CurrentFrame > 3) CurrentFrame = 0; // ���� ������ � �������� ����� - ������������ �����.
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 64, 32, 32)); //���������� �� ����������� �. ���������� �������� ��������� � ���������� � ������ 0,96,96*2, � ����� 0
				break;
				}//��������� ���� ������
			case left:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 32, 32, 32)); 
				break;
				}//��������� ���� �����
			case up:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 96, 32, 32)); 
				break;
				}//��������� �������� ������
			case down:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 0, 32, 32)); 
				break;
				}//��������� �� ����� ������ ���������
			case stay: {CurrentFrame=0; dx=dy=0; break;}//���� ��������.		
			}	
			x += dx*time;
			checkCollisionWithMap(dx, 0);//������������ ������������ �� �
			y += dy*time;
			checkCollisionWithMap(0, dy);//������������ ������������ �� Y
			sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
			
			health -= 0.001; // ���������� ��������� ��������.
			age += 0.001;	// ���������� ����������� �������.
			if (health <= 0)life = false;
			if (age > 40) 	life = false;
		}
	}
};