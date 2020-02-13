// /*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* valori esterni dipendenti dalla finestra di gioco */
#define coordMaxY 560
#define coordMinY 40.
#define playerHeight 200.

/* costanti di quando ti muovi verso l'alto */
#define jpInitialVelocity 5. /* velocità iniziale */
#define jpTerminalVelocity 20. /* velocità massima che si può raggiungere */
#define jpSoftness 20. /* gradualità per cui si passa da velocità iniziale a finale */

/* costanti di quando ti muovi verso il basso */
#define grInitialVelocity 0. /* velocità iniziale */
#define grTerminalVelocity -50. /* velocità massima che si può raggiungere */
#define grSoftness 30. /* gradualità per cui si passa da velocità iniziale a finale */

#define jpVelocityFunction(x) x / (x + 1)
#define grVelocityFunction(x) x

typedef struct
{
	float x;
	float y;
} coords;

typedef struct
{
	coords position;
	float velocity;
	bool oldMoveUp;

} player;

player* getP()
{
	static player p;
	static bool setup; /* false */

	if (!setup)
	{
		p.position.x = 130.;
		p.position.y = 10.;
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