#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//�������� sfml ������ "���", ������� � �������� �������

void setPlayerCoordinateForView(float x, float y) { //������� ��� ���������� ��������� ������
	float tempX = x; float tempY = y;//��������� ����� ������ � ��������� ��, ����� ������ ����

	if (x < 320) tempX = 320;//������� �� ���� ����� �������
	if (y < 240) tempY = 240;//������� �������
	if (y > 7500) tempY = 7500;//������ �������	

	view.setCenter(tempX, tempY); //������ �� �������, ��������� ��� ����������. 
	
}


void viewmap(int time) { //������� ��� ����������� ������ �� �����. ��������� ����� sfml
	

	if (Keyboard::isKeyPressed(Keyboard::D)) {
		view.move(0.1*time, 0);//�������� ����� ������ (�� ����, ����� �� ������� ����� - �� ���� �����)
	}

	if (Keyboard::isKeyPressed(Keyboard::S)) {
		view.move(0, 0.1*time);//�������� ����� ���� (�� ����, ����� �� ������� ����� - �� ���� �����)
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		view.move(-0.1*time, 0);//�������� ����� ����� (�� ����, ����� �� ������� ����� - �� ���� �����)
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		view.move(0, -0.1*time);//�������� ����� ������ (�� ����, ����� �� ������� ����� - �� ���� �����)
	}

}

void changeview(){


	if (Keyboard::isKeyPressed(Keyboard::U)) {
		view.zoom(1.1000f); //������������, ����������
		//view.zoom(1.0006f); //���� ����� ����������� ��������������
	}

	if (Keyboard::isKeyPressed(Keyboard::Y)) {
		//view.setRotation(90);//����� �� ������ ������� ������
		//view.rotate(1);//���������� ������������ ������ (������������� �������� - � �������� �������)
		view.zoom(0.9000f);
	}


	if (Keyboard::isKeyPressed(Keyboard::I)) {
		view.setSize(640, 480);//������������� ������ ������ (��� ��������)
	}

	if (Keyboard::isKeyPressed(Keyboard::P)) {
		view.setSize(800, 600);//�������� ������ ������
		view.move(125, 200);
	}


	//if (Keyboard::isKeyPressed(Keyboard::Q)) {
	//	view.setViewport(sf::FloatRect(0, 0, 0.5f, 1));//����� ������� �������� ���������� ����� ��� ���� �� �����. ����� ������ ������� ��� ���� ������ View � ����������� � ���� ���������� ������ 2.
	//}

}
