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

#define PLAYER_OFFSET 720

#define maxDelta winWidth/5
#define minDelta winWidth/7

#define obsCount (int) ceil((float) winWidth / minDelta) + 2

typedef struct
{
	sfVector2f pos;
	int type;
} obstacle;

typedef struct {
	obstacle* obs;
	sfVector2f lastPos;
} obsInfo;

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

void drawBackground(sfRenderWindow* win, sfRectangleShape* bg, int score)
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


obsInfo* getObsInfo()
{
	static obsInfo d;
	static bool setup;

	if (!setup)
	{
		int i;
		d.obs = malloc((obsCount) * sizeof(obstacle));
		if (d.obs != NULL)
		{
			d.lastPos.x = winWidth; // la posizione dell'ultimo ostacolo generato
			d.lastPos.y = 0;

			for (i = 0; i < obsCount; i++)
				d.obs[i].type = -1;

			setup = true;
		}
	}

	return &d;
}

void drawObs(sfRenderWindow* win, sfRectangleShape* obsRect, int score)
{
	obsInfo* obsIn = getObsInfo();

	obstacle* obs = obsIn->obs;
	sfVector2f* lastPos = &(obsIn->lastPos);

	int i;

	float deltaPos; // la distanza tra gli ultimi ostacoli generati
	sfVector2f newPos; // la posizione del nuovo ostacolo

	for (i = 0; i < obsCount; i++) {
		// Quando un ostacolo esce dallo schermo viene "eliminato"
		if (obs[i].pos.x < score - PLAYER_OFFSET - maxDelta)
			obs[i].type = -1;
	}

	// Generazione di un nuovo ostacolo
	if (score > lastPos->x + minDelta - winWidth + PLAYER_OFFSET)
	{
		deltaPos = rand() % (maxDelta - minDelta) + minDelta;
		newPos.x = lastPos->x + deltaPos;
		newPos.y = rand() % (int) (coordMaxY - coordMinY) + coordMinY;

		// Inserisce il nuovo ostacolo nel primo "posto libero"
		i = 0;
		while (obs[i].type != -1 && i <= obsCount)
			i++;

		obs[i].pos.x = newPos.x;
		obs[i].pos.y = newPos.y;
		obs[i].type = 1;

		lastPos->x = newPos.x;
		lastPos->y = newPos.y;
	}

	for (i = 0; i < obsCount; i++) {
		if ((obs[i].type != -1) && (obs[i].pos.x - score + PLAYER_OFFSET < winWidth) && (obs[i].pos.x - score + PLAYER_OFFSET >= 0)) {
			sfRectangleShape_setPosition(obsRect, (sfVector2f) { obs[i].pos.x - score + PLAYER_OFFSET, obs[i].pos.y });
			sfRenderWindow_drawRectangleShape(win, obsRect, NULL);
		}
	}
}

void moveHor(int* score, int* stage)
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