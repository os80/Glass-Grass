#include <SFML/Graphics.hpp>

using namespace sf;
class Entity {
public:
virtual void update(float time) = 0;
	enum { left, right, up, down, jump, stay } state;//добавляем тип перечисления - состояние объекта
	float age, health, dx, dy, x, y, moveTimer, a, CurrentFrame;//добавили переменную таймер для будущих целей
	double speed;
	int w,h,r,satiety;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;//враги могут быть разные, мы не будем делать другой класс для врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени

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
	//enum { left, right, up, down, jump, stay } state;//добавляем тип перечисления - состояние объекта
	int playerScore;//эта переменная может быть только у игрока

	   Player(Image &image, float X, float Y,int W,int H,String Name):Entity(image,X,Y,W,H,Name){
		   playerScore = 0; state = stay;
		   if (name == "Player1"){
			   sprite.setTextureRect(IntRect(32, 192, w, h));
		   }
	   }

	   void control() //движение игрока клавишами
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

	   void checkCollisionWithMap(float Dx, float Dy)//функция проверки столкновений с картой
	   {
		   for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
		   for (int j = x / 32; j<(x + w) / 32; j++)
		   {
			   if (TileMap[i][j] == Brick)//если элемент наш тайлик кирпича? то
			   {
				   if (Dy>0){ y = i * 32 - h;	dy = 0; }	//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
				   if (Dy<0){ y = i * 32 + 32;  dy = 0; }	//столкновение с верхними краями карты(может и не пригодиться)
				   if (Dx>0){ x = j * 32 - w; }				//с правым краем карты
				   if (Dx<0){ x = j * 32 + 32;}				// с левым краем карты
			   }
		   }
	   }
	    
	   void update(float time)
	   {
		   control();//функция управления персонажем
		   switch (state)//тут делаются различные действия в зависимости от состояния
		   {
		   case right:
				{
				dx = speed; 
		   		CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (CurrentFrame > 3) CurrentFrame = 0; // если пришли к третьему кадру - откидываемся назад.
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 192, 32, 32)); //проходимся по координатам Х. получается начинаем рисование с координаты Х равной 0,96,96*2, и опять 0
				break;
				}//состояние идти вправо
		   case left:
				{
				dx = -speed; 
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 160, 32, 32)); 
				break;
				}//состояние идти влево
		   case up:
				{
				dy= -speed; 
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 224, 32, 32)); 
				break;
				}//будет состояние поднятия наверх (например по лестнице)
		   case down:
				{
				dy= speed; 
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 128, 32, 32)); 
				break;
				}//будет состояние во время спуска персонажа (например по лестнице)
		   case stay: {CurrentFrame=0; break;}//и здесь тоже		
		   }
		   x += dx*time;
		   checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
		   y += dy*time;
		   checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
		   sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
		   if (health <= 0){ life = false; }
		   if (!isMove){ speed = 0; }
		   //if (!onGround) { dy = dy + 0.0015*time; }//убираем и будем всегда притягивать к земле
		   if (life) { setPlayerCoordinateForView(x, y); }
		   //dy = dy + 0.0015*time;//постоянно притягиваемся к земле
		   
		   
	   } 
	float getplayercoordinateX(){	
		return x;
	}
	float getplayercoordinateY(){
		return y;
	}
};

/////////////////////////////////////класс врага//////////////////////////////////////////////////
class Enemy :public Entity{
public:
	
	Enemy(Image &image, float X, float Y,int W,int H,String Name):Entity(image,X,Y,W,H,Name){ 

		if (name == "Sheep"){
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0;//сначала враг стоит на месте.
			dy = 0;
			speed = 50;
			satiety = 5;
			age=rand()%10;
			
		}
		if (name == "Wolf"){
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0;//сначала враг стоит на месте.
			dy = 0;
			speed = 10;
			satiety = 5;
			age=rand()%10;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if (TileMap[i][j] == Brick)//если элемент наш тайлик кирпича, то
			{
				if (Dy>0){ y = i * 32 - h; 	dy = 0; }//sprite.scale(-1, 1);}//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
				if (Dy<0){ y = i * 32 + 32; dy = 0; }//sprite.scale(-1, 1);}//столкновение с верхними краями карты(может и не пригодиться)
				if (Dx>0){ x = j * 32 - w; 	dx = 0; }//sprite.scale(-1, 1);}//с правым краем карты
				if (Dx<0){ x = j * 32 + 32; dx = 0; }//sprite.scale(-1, 1);}// с левым краем карты
			}
		}
	}
	
	float SetRandomSpeed(int r) //делаем скорость юнита случайной от 0 до speed/1000
	{
		//r*=1000;
		a=rand()%r;
		a/=1000;
		return abs(a);
	};
	void update(float time)
	{
		if (name == "Sheep") //для персонажа с таким именем логика будет такой.
		{	
			//RectangleShape HpBar(sf::Vector2f(100*(health/100.0),10));
			//HpBar.setPosition(x,y);
			//window.draw(HpBar);
			moveTimer += time;
			if (moveTimer>3000+rand()%9000)//меняет направление раз в несколько секунд
			{ 
				switch (rand()%7) //чем число больше пяти, тем чаще будет стоять на месте.
				{
				case 1: {dy = SetRandomSpeed(speed);  state=down;  dx=0; break;} // если убрать dx=0, то будет ходить по диагонали, но анимации такой нет и будет криво выглядеть.
				case 2: {dy = -SetRandomSpeed(speed); state=up;    dx=0; break;}
				case 3: {dx = SetRandomSpeed(speed);  state=right; dy=0; break;}
				case 4: {dx = -SetRandomSpeed(speed); state=left;  dy=0; break;}
				default: {state=stay;}
				}
			moveTimer = 0; 
			}
			
			switch (state)//анимация в зависимости от состояния.
			{
			case right:
				{
		   		CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (CurrentFrame > 3) CurrentFrame = 0; // если пришли к третьему кадру - откидываемся назад.
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 64, 32, 32)); //проходимся по координатам Х. получается начинаем рисование с координаты Х равной 0,96,96*2, и опять 0
				break;
				}//состояние идти вправо
			case left:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 32, 32, 32)); 
				break;
				}//состояние идти влево
			case up:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 96, 32, 32)); 
				break;
				}//состояние поднятия наверх
			case down:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 0, 32, 32)); 
				break;
				}//состояние во время спуска персонажа
			case stay: {CurrentFrame=0; dx=dy=0; break;}//стоп анимация.		
			}	
			x += dx*time;
			checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
			y += dy*time;
			checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
			
			//ест траву
			for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
			for (int j = x / 32; j<(x + w) / 32; j++)			
			{
				if (TileMap[i][j] == Grass)//если элемент наш тайлик трава, то съедаем её.				
				{
				if(health<100)health+=5;
				satiety++; //Поели? Сытость возросла
				if(rand()%2==1)	TileMap[i][j] = Floor; //меняем съеденую траву на землю или камни
				else 			TileMap[i][j] = Stone;
				state=stay;
				}
			}
			health -= 0.01; // постепенно уменьшаем здоровье.
			age += 0.001;	// постепенно увеличиваем возраст.
			if(age<3)sprite.setScale(0.5f, 0.5f); else sprite.setScale(0.9f, 0.9f); //молодые овечки маленькие, взрослые - большие
			if (health <= 0)life = false;
			if (age > 20) 	life = false;
		}
		if (name == "Wolf") //для персонажа с таким именем логика будет такой.
		{	
				
			moveTimer += time;
			if (moveTimer>4000+rand()%9000)//меняет направление раз в несколько секунд
			{ 
				switch (rand()%5) //чем число больше пяти, тем чаще будет стоять на месте.
				{
				case 1: {dy = 0.05;  state=down;  dx=0; break;} // если убрать dx=0, то будет ходить по диагонали, но анимации такой нет и будет криво выглядеть.
				case 2: {dy = -0.05; state=up;    dx=0; break;}
				case 3: {dx = 0.05;  state=right; dy=0; break;}
				case 4: {dx = -0.05; state=left;  dy=0; break;}
				default: {state=stay;}
				}
			moveTimer = 0; 
			}
			
			switch (state)//анимация в зависимости от состояния.
			{
			case right:
				{
		   		CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (CurrentFrame > 3) CurrentFrame = 0; // если пришли к третьему кадру - откидываемся назад.
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 64, 32, 32)); //проходимся по координатам Х. получается начинаем рисование с координаты Х равной 0,96,96*2, и опять 0
				break;
				}//состояние идти вправо
			case left:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 32, 32, 32)); 
				break;
				}//состояние идти влево
			case up:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 96, 32, 32)); 
				break;
				}//состояние поднятия наверх
			case down:
				{
				CurrentFrame += 0.005*time; 
				if (CurrentFrame > 3) CurrentFrame = 0; 
				sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 0, 32, 32)); 
				break;
				}//состояние во время спуска персонажа
			case stay: {CurrentFrame=0; dx=dy=0; break;}//стоп анимация.		
			}	
			x += dx*time;
			checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
			y += dy*time;
			checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
			
			health -= 0.001; // постепенно уменьшаем здоровье.
			age += 0.001;	// постепенно увеличиваем возраст.
			if (health <= 0)life = false;
			if (age > 40) 	life = false;
		}
	}
};