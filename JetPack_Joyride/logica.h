#ifndef LOGICA_C
#define LOGICA_C

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define jetpackAcceleration 20

#define coordMaxY 1080
#define coordMinY 0
#define PlayerHigh 15.f

/* moveUp è true se il tasto su è premuto */
void move (bool moveUp);
/* move:
	mette la posizione y nella struct del personaggio in base al suo movimento
	( velocità esponenziale )
*/

typedef struct
{
	int x;
	int y;
} coords;

typedef struct
{
	coords position;
	float gravity;
	float velocity;
	float acceleration;
} player;

player* getPS()
{
	static player p;
	static bool setup;

	if (!setup)
	{
		p.position.x = 10;
		p.position.y = 10;
		p.gravity = 10;
		p.velocity = 0;
		p.acceleration = 0;

		setup = true;
	}
	
	return &p;
}

void move (bool moveUp)
{
	player *p = getPS();

	p->acceleration = -p->gravity;

	/* tasto "up" premuto */
	if (moveUp)
		p->acceleration += jetpackAcceleration;
	
	p->velocity += p->acceleration;

	/* non collide con il pavimento e il soffitto */
	if (!( ((p->position.y + p->velocity) >= coordMaxY) && ((p->position.y - PlayerHigh + p->velocity) <= coordMinY) ))
		p->position.y += p->velocity;
}

#endif