#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объявили sfml объект "вид", который и является камерой

void setPlayerCoordinateForView(float x, float y) { //функция для считывания координат игрока
	float tempX = x; float tempY = y;//считываем коорд игрока и проверяем их, чтобы убрать края

	if (x < 320) tempX = 320;//убираем из вида левую сторону
	if (y < 240) tempY = 240;//верхнюю сторону
	if (y > 7500) tempY = 7500;//нижнюю сторону	

	view.setCenter(tempX, tempY); //следим за игроком, передавая его координаты. 
	
}


void viewmap(int time) { //функция для перемещения камеры по карте. принимает время sfml
	

	if (Keyboard::isKeyPressed(Keyboard::D)) {
		view.move(0.1*time, 0);//скроллим карту вправо (см урок, когда мы двигали героя - всё тоже самое)
	}

	if (Keyboard::isKeyPressed(Keyboard::S)) {
		view.move(0, 0.1*time);//скроллим карту вниз (см урок, когда мы двигали героя - всё тоже самое)
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		view.move(-0.1*time, 0);//скроллим карту влево (см урок, когда мы двигали героя - всё тоже самое)
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		view.move(0, -0.1*time);//скроллим карту вправо (см урок, когда мы двигали героя - всё тоже самое)
	}

}

void changeview(){


	if (Keyboard::isKeyPressed(Keyboard::U)) {
		view.zoom(1.1000f); //масштабируем, уменьшение
		//view.zoom(1.0006f); //тоже самое помедленнее соответственно
	}

	if (Keyboard::isKeyPressed(Keyboard::Y)) {
		//view.setRotation(90);//сразу же задает поворот камере
		//view.rotate(1);//постепенно поворачивает камеру (отрицательное значение - в обратную сторону)
		view.zoom(0.9000f);
	}


	if (Keyboard::isKeyPressed(Keyboard::I)) {
		view.setSize(640, 480);//устанавливает размер камеры (наш исходный)
	}

	if (Keyboard::isKeyPressed(Keyboard::P)) {
		view.setSize(800, 600);//например другой размер
		view.move(125, 200);
	}


	//if (Keyboard::isKeyPressed(Keyboard::Q)) {
	//	view.setViewport(sf::FloatRect(0, 0, 0.5f, 1));//таким образом делается раздельный экран для игры на двоих. нужно только создать ещё один объект View и привязывать к нему координаты игрока 2.
	//}

}
