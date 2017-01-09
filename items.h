#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


using namespace sf;
class Items {
public:
virtual void update(float time) = 0;
	//эффекты предмета	
	bool bless;//святой (не хороший)
	bool dark;//проклятый (не плохой)
	int blind;//ослепление
	float pace;//замедление
	bool stop;//остановка
	bool disorientation;//дезориентация
	bool silence;//безмолвие
	bool rust;//ржавчина
	bool rot;//гниение
	int str;//сила
	int intellect;//ум
	bool gasp;//удушье (может быть нивелировано жабрами :) они под подмышками вырастают)
	bool fear;//страх
	//
	//
	bool gills;//жабры (плотная одежда не даёт ими дышать)
	bool nightvision;//ночное зрение
	//float pace; //ускорение	
	bool vigilance;//зоркость
	int antidote;//антидот (снимает эффекты)
	//
	//
	int armor;//показатель брони (может быть у щита или доспеха, а можно и банкой с этим отрицательным эффектом во вражину кинуть)
	float bigorlittle;//разбухание
	//скукоживание
	int damage;//урон
	int cure;//лечение
	bool identification;//опознание
	int modifyhealth;//запас здоровья
	int modifymana;//запас маны
	float createlive;//создание живых
	float createitem;//создание предметов
	int fire;//огонь
	int ice;//лёд
	int elec;//электричество
	int poison;//яд
	//
	//
	int mass;//вес
	int ligth;//свет
	bool needle;//иглы (без перчаток такое не взять, а можно и себя баффнуть)
	enum {Mbone,Mwood,Msteel,Mskin,Mpaper,Mglass,Mgrass,Mwater,Mrock,Mdiamond,Mgold,Mbronze} material;//материал
	enum {bottle,sword,bone,shield,wand,ring,chest,skin,scroll,gem, container} tipe; //тип предмета
	bool distance;//дистаннционный эффект
	bool mud;//пачкает руки (оставляет следы на руках или в контейнере. Разные следы могут реагировать)
	//bool container;//контейнер
	int rigidity;//жескость
	bool wave;//звук (флейта призыва или изгнания, гремящие доспехи)
	int value;//ценность (скажем, это коэффициент силы его эффекта)
	bool invisibility;//невидимость (тяжелый невидимый камень будет ощущатся. А можно и вирус болезни этим сделать, совместив с ядом, следами, дистанционной атакой и нулевым весом)
	float period;//время существования
	bool improved;//улучшаемость
	String letters;//письмена
	String nameitem;//имя (есть имя для движка, а есть имя дл игрока. может быть сгенерено в зависимости от эффектов)

	
	Texture texture;
	Sprite sprite;
	String name;
	float x,y,w,h,CurrentFrame;
	
	Items(Image &image, float X, float Y,int W,int H, String Name)
	{
		x = X; y = Y; w = W; h = H; name = Name;
		CurrentFrame=0;				
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect(){return FloatRect(x, y, w, h);}
	
};


	
class Item :public Items {
	public:
		Item(Image &image, float X, float Y,int W,int H,String Name):Items(image,X,Y,W,H,Name){
			sprite.setTextureRect(IntRect(96, 48, w, h));	
			
		};
		
		void update(float time){	
		sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
		};
};

int GetItemsSize();
void SetItem(float x, float y, int value, String tipe);
void SetViewItems(int VisibleDist,float px, float py, RenderWindow &window, float time);