#include <stdio.h>
#include <stdlib.h>

#include<SFML\System.h>

#include <time.h>

#include "consts.h"

/* costanti di quando ti muovi verso l'alto */
#define jpInitialVelocity 5.f /* velocit� iniziale */
#define jpTerminalVelocity 20.f /* velocit� massima che si pu� raggiungere */
#define jpSoftness 20.f /* gradualit� per cui si passa da velocit� iniziale a finale */

/* costanti di quando ti muovi verso il basso */
#define grInitialVelocity 0.f /* velocit� iniziale */
#define grTerminalVelocity -50.f /* velocit� massima che si pu� raggiungere */
#define grSoftness 30.f /* gradualit� per cui si passa da velocit� iniziale a finale */

#define jpVelocityFunction(x) x / (x + 1)
#define grVelocityFunction(x) x

player* getP()
{
	static player p;
	static bool setup; /* false */

	if (!setup)
	{
		p.position.x = 130.f;
		p.position.y = 10.f;
		p.velocity = 0;

		setup = true;
	}

	return &p;
}

void moveVer(bool moveUp)
{
	player* p = getP();

	static int delta;
	static bool oldMoveUp;

	delta += 1;

	if (moveUp != oldMoveUp)
		delta = 0;

	oldMoveUp = moveUp;

	if (moveUp)
		p->velocity = (jpTerminalVelocity - jpInitialVelocity) * jpVelocityFunction(delta / jpSoftness) + jpInitialVelocity;
	else
		p->velocity = (grTerminalVelocity - grInitialVelocity) * jpVelocityFunction(delta / grSoftness) + grInitialVelocity;

	p->position.y -= p->velocity;

	if (p->position.y <= coordMinY)
	{
		p->position.y = coordMinY;
		delta = 0;
	}

	if (p->position.y >= coordMaxY - playerHeight)
	{
		p->position.y = coordMaxY - playerHeight;
		delta = 0;
	}

}