#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <ctime>
#include <string>
#include <vector>
#include <list>
#include "items.h"

using namespace sf;

std::list<Items*>  items;//������ ������, ���� ���� ������ �������
std::list<Items*>::iterator it_items;//�������� ����� ��������� �� ��-��� ������


int GetItemsSize(){
	return items.size();
}

void SetItem(float x, float y, int value, String tipe){ //���, ����������, ��������	
	Image swordImage;
	swordImage.loadFromFile("images/items.png");
	swordImage.createMaskFromColor(Color(185, 224, 177));//������� ����� �� �����.
	items.push_back(new Item(swordImage,x,y,24,24, tipe));
}

void SetViewItems(int VisibleDist,float px, float py, RenderWindow &window, float time){ // ���������� ������ ��, ��� ����� � ������
	for (it_items = items.begin(); it_items != items.end(); it_items++)
	{
		if ((*it_items)->x < px+VisibleDist && (*it_items)->x > px-VisibleDist)  
			if ((*it_items)->y < py+VisibleDist && (*it_items)->y > py-VisibleDist){
				(*it_items)->update(time);
				window.draw((*it_items)->sprite); //������ items �������	
				//std::cout << "x= " << (*it_items)->x << " y= " << (*it_items)->y << " px= " << px << " py= " << py <<"\n";
			}
	}
}