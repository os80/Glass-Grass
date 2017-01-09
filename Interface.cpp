
#include "Interface.h"


void renderHpBar(int hp, Vector2f position, Vector2f size, RenderWindow &window, Color color){
	if(hp < 0 ) // ��������� ������� ������ �� ������������
		hp = 0; // �� ��������� ��� ���������� ���������� � ������ �������� hp
	RectangleShape back(size); // ������ ����� �������
	back.setPosition(position);
	back.setFillColor(Color(50,50,50,128)); // ����� �������������� ����
	// ��� hp-bar
	RectangleShape bar(Vector2f(size.x*(hp/100.0),size.y)); // ������ ���������� �� ��������� hp � 100,
	//�.�. ����� ����� ��������������� ������� �� ��
	bar.setPosition(position);
	bar.setFillColor(color);
	
	window.draw(back);
	window.draw(bar);
	
}
void renderHpBar(int hp, float pos_x, float pos_y, float size_x, float size_y, RenderWindow &window, Color color){
	renderHpBar(hp,Vector2f(pos_x,pos_y),Vector2f(size_x,size_y),window,color);
}

void renderInterfaceBack(float x, float y, float pos_x, float pos_y, RenderWindow &window)
{
	RectangleShape InterfaceBack(Vector2f(x, y)); // ������ ����� �������
	InterfaceBack.setPosition(Vector2f(pos_x,pos_y));
	InterfaceBack.setFillColor(Color(50,50,50,128)); // ����� �������������� ����
	window.draw(InterfaceBack);
}