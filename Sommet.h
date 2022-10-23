#pragma once
class Sommet
{	public:
	int numero_sommet;
	int t1;//earlist time
	int t2;//latest time
	int dist;//distance à prochaine sommet
	Sommet();
	Sommet(int,int,int);
};

