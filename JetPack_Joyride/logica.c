#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define jetpackAcceleration 25

#define coordMaxY 1080
#define coordMinY 0
#define PlayerHigh 15.f

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
		p.position.x = 30;
		p.position.y = 10;
		p.gravity = 12;
		p.velocity = 0;
		p.acceleration = 0;

		setup = true;
	}

	return &p;
}

void move(bool moveUp)
{
	player* p = getPS();

	p->acceleration = -p->gravity;

	/* tasto "up" premuto */
	if (moveUp)
		p->acceleration += jetpackAcceleration;

	p->velocity += p->acceleration;

	/* non collide con il pavimento e il soffitto */
	if (!(((p->position.y + p->velocity) >= coordMaxY) && ((p->position.y - PlayerHigh + p->velocity) <= coordMinY)))
		p->position.y += (int)p->velocity;
}
