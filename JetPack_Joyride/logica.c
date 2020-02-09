#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* valori esterni dipendenti dalla finestra di gioco */
#define coordMaxY 1080
#define coordMinY 40
#define PlayerHigh 15.f

/* valori "0" ad altezza totale schermo 1080 */
#define jpAcceleration0 8
#define jpVelocity0 3

/* coordMaxY : x = 1080 : jpacceleration0 */
#define jetpackAcceleration (coordMaxY * jpAcceleration0) / 1080
#define jpVelocity (coordMaxY * jpVelocity0) / 1080

typedef struct
{
	float x;
	float y;
} coords;

typedef struct
{
	coords position;
	float velocity;
	/* posizioni:
	0: accellerazione
	1: decellerazione 
	2: numero di aggiunta all'accellerazione
	3: numero di aggiunta all'decellerazione 
	*/
	float acceleration[4];
} player;

player* getP()
{
	static player p;
	static bool setup; /* false */

	if (!setup)
	{
		p.position.x = 130;
		p.position.y = 10;
		p.velocity = jpVelocity;
		p.acceleration[0] = jetpackAcceleration;
		p.acceleration[1] = jetpackAcceleration;
		p.acceleration[2] = 0;
		p.acceleration[3] = 0;
		setup = true;
	}

	return &p;
}

void move(bool moveUp)
{
	player* p = getP();

	// /*
	short int choice = 0;
	short int signMove;

	// se viene premuto il tasto su
	if (moveUp)
		choice = 1;

	// resetto l'accellerazione o la decellerazione
	if (p->acceleration[choice + 2])
		p->acceleration[choice + 2] -= 0.5;

	// se è 0 diventa 1 e viceversa
	choice = (choice + 1) % 2;

	// calcolo finale dell'accellerazione
	p->acceleration[choice] = jetpackAcceleration + (p->velocity * p->acceleration[choice + 2] * p->acceleration[choice + 2]);

	// aumento l'accellerazione corrente
	p->acceleration[choice + 2] += 0.5;

	signMove = (choice + 1) % 2;

	if(!choice)
		signMove = -1;

	// alla fine di tutto quello che fa la funzione la y viene spostata del valore della velocità che sia positiva o negativa 
	p->position.y = signMove * p->acceleration[choice];

	// */
}

/*
PROGRAMMA FUNZIONANTE DI PARTENZA

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* valori esterni dipendenti dalla finestra di gioco /
#define coordMaxY 1080
#define coordMinY 40
#define PlayerHigh 15.f

/* valori "0" ad altezza totale schermo 1080 /
#define jpAcceleration0 15
#define jpGravity0 12

/* coordMaxY : x = 1080 : jpacceleration0 /
#define jetpackAcceleration (coordMaxY * jpAcceleration0) / 1080

/* coordMaxY : x = 1080 : jpGravity0 /
#define jpGravity (coordMaxY * jpGravity0) / 1080

typedef struct
{
	float x;
	float y;
} coords;

typedef struct
{
	coords position;
	float gravity;
	float velocity;
	float acceleration;
} player;

player* getP()
{
	static player p;
	static bool setup; /* false /

	if (!setup)
	{
		p.position.x = 130;
		p.position.y = 10;
		p.gravity = jpGravity;
		p.velocity = 0;
		p.acceleration = 0;

		setup = true;
	}

	return &p;
}

void move(bool moveUp)
{
	player* p = getP();

	p->acceleration = -p->gravity;

	/* tasto "up" premuto /
	if (moveUp)
		p->acceleration += jetpackAcceleration;

	/* da aggiungere la parte delle collisioni con paviomento e soffitto /

	p->velocity += p->acceleration;
	p->position.y += (int)p->velocity;
}
*/