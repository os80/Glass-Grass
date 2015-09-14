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

std::list<Items*>  items;//������ ������, ���� ���� ������ �������.�������� ������.
std::list<Items*>::iterator it_items;//�������� ����� ��������� �� ��-��� ������


int GetItemsCount(){
	return items.size();
}

void SetItem(float x, float y, int value){ //���, ����������, ��������
	
	Image swordImage;
	swordImage.loadFromFile("images/items.png");
	items.push_back(new Item(swordImage,x,y,32,32,"Sword"));
}

void SetViewItems(int VisibleDist,float px, float py, RenderWindow &window){ // ���������� ������ ��, ��� ����� � ������
	for (it_items = items.begin(); it_items != items.end(); it_items++)
	{
		if ((*it_items)->x < px+VisibleDist && (*it_items)->x > px-VisibleDist)  
			if ((*it_items)->y < py+VisibleDist && (*it_items)->y > py-VisibleDist)
				window.draw((*it_items)->sprite); //������ items �������				
	}
}