const int HEIGHT_MAP = 300; //������ ����� ������
const int WIDTH_MAP = 300; //������ ����� ������
enum mapObjects { Brick , Floor, Stone, Rock, House, Well, Tree, Grass, 
Coast1, Coast2, Coast3, Coast4, Coast5, Coast8, Coast9, Coast10, Coast11, 
Coast12, Woter1, Woter2 }; // �������� �����
mapObjects TileMap[HEIGHT_MAP][WIDTH_MAP]; // �������� ������� ��� �����. �� ��������� �� ���������� ������ ��������� ���� - ��������.  

int SetMap()
{
	mapObjects Object; 
	for (int i = 1; i < HEIGHT_MAP-1; i++)	//���� �� ������, �������� ��������, ��� i ����� �� 1, �� HEIGHT_MAP-1 ����� �� ������� ������ �� ��������.
	for (int j = 1; j < WIDTH_MAP-1; j++) //���� �� ������, ���������� ��� �������.
	{				
		Object  = mapObjects(rand()%8); //������� ��������� ������ �� ������ ������� ����������
		if (Object == Brick) Object = Floor; // ���� ��������� �������� ��������� ������ - ������ ��� �� ���, ����� ��� ����� ������� �������?
		if (Object == Tree ) {if (rand()%100<2)Object = Tree; else Object=Floor;}//������ �������� ��������
		if (Object == House) {if (rand()%100>200)Object = House; else Object=Floor;}
		if (Object == Well)  {if (rand()%100<2)Object = Well; else Object=Floor;}
		TileMap[i][j] = Object; 

	}
	return TileMap[HEIGHT_MAP][WIDTH_MAP];			
}


int SetGrassOnMap(){	//����� ����� ������ �� ����� � ��������� �����
int i;
int j;	
i=rand()%HEIGHT_MAP;
if(i<2) i=1;
if(i>HEIGHT_MAP-2) i=HEIGHT_MAP-2;
j=rand()%WIDTH_MAP;
if(j<2) j=1;
if(j>WIDTH_MAP-2) j=WIDTH_MAP-2;
if(TileMap[i][j] == Floor || TileMap[i][j] == Stone) TileMap[i][j] = Grass;
return TileMap[i][j];
}
