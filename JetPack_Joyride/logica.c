#include <stdio.h>
#include <stdlib.h>

#include<SFML\System.h>

#include <time.h>
#include <math.h>

#include "consts.h"

#define bgCount ceil((float) winWidth / bgDim.x) + 1 // numero di sfondi su schermo

#define horScreenVelocity 15// velocità di scroll dello schermo
#define stageLength 1000 //lunghezza di uno stadio all'interno di un livello

/* costanti di quando ti muovi verso l'alto */
#define jpInitialVelocity 5.f /* velocità iniziale */
#define jpTerminalVelocity 20.f /* velocità massima che si può raggiungere */
#define jpSoftness 20.f /* gradualità per cui si passa da velocità iniziale a finale */

/* costanti di quando ti muovi verso il basso */
#define grInitialVelocity 0.f /* velocità iniziale */
#define grTerminalVelocity -50.f /* velocità massima che si può raggiungere */
#define grSoftness 30.f /* gradualità per cui si passa da velocità iniziale a finale */

#define jpVelocityFunction(x) x / (x + 1)
#define grVelocityFunction(x) x

player* getP()
{
	static player p;
	static bool setup; /* false */

	if (!setup)
	{
		p.position.x = 130.f;
		p.position.y = coordMaxY - playerHeight;
		p.velocity = 0;

		setup = true;
	}

	return &p;
}

void drawBackground(sfRenderWindow* win, sfRectangleShape* bg, unsigned int score)
{
	sfVector2f bgDim = sfRectangleShape_getSize(bg);

	float i, x;
    for (i = 0.f; i < bgCount; i++)
    {
        x = (i * bgDim.x) - (float)(score % (int)bgDim.x);

		sfRectangleShape_setPosition(bg, (sfVector2f){x, 0});
		sfRenderWindow_drawRectangleShape(win, bg, NULL);
    }
}

void moveHor(unsigned int* score, unsigned int* stage)
{
	(*score) += horScreenVelocity;
	if (!((*score) % stageLength))
		(*stage)++;
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