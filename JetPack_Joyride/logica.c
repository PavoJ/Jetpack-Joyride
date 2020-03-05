#include <stdio.h>
#include <stdlib.h>

#include <SFML\System.h>

#include <time.h>
#include <math.h>

#include "consts.h"
#include "settingsHandler.c"

#define bgCount ceil((float) winWidth / bgDim.x) + 1 // numero di sfondi su schermo
#define obsCount (int)ceil((double)( winWidth / minDelta)) + 2 // numero massimo di ostacoli su schermo

/* costanti di scroll dello schermo */
#define bgInitialVelocity .0093f * winHeight /* velocit� iniziale */
#define bgTerminalVelocity .0463f * winHeight /* velocit� massima che si pu� raggiungere */
#define bgSoftness 20000.f /* gradualit� per cui si passa da velocit� iniziale a finale */

/* costanti di quando ti muovi verso l'alto */
#define jpInitialVelocity .0046f * winHeight /* velocit� iniziale */
#define jpTerminalVelocity .0185f * winHeight /* velocit� massima che si pu� raggiungere */
#define jpSoftness 20.f /* gradualit� per cui si passa da velocit� iniziale a finale */

/* costanti di quando ti muovi verso il basso */
#define grInitialVelocity 0.f * winHeight /* velocit� iniziale */
#define grSoftness 1.2f /* gradualit� per cui si passa da velocit� iniziale a finale */

#define bgVelocityFunction(x) x / (x + 1)
#define jpVelocityFunction(x) x / (x + 1)
#define grVelocityFunction(x) x

#define PLAYER_OFFSET (-.0521f * winWidth)

#define maxDelta (.2000 * winWidth) // massima distanza fra due ostacoli
#define minDelta (.1429 * winWidth) // minima distanza fra due ostacoli

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
	sthSettings* set = sthGetSettings();
	int winWidth = set->vMode.width;
	int winHeight = set->vMode.height;

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
	sthSettings* set = sthGetSettings();
	int winWidth = set->vMode.width;
	int winHeight = set->vMode.height;

	sfVector2f bgDim = sfRectangleShape_getSize(bg);

	float i, x;
	for (i = 0.f; i < bgCount; i++)
	{
		x = (i * bgDim.x) - (float)(score % (int)bgDim.x);

		sfRectangleShape_setPosition(bg, (sfVector2f) { x, 0 });
		sfRenderWindow_drawRectangleShape(win, bg, NULL);
	}
}

obsInfo* getObsInfo()
{
	sthSettings* set = sthGetSettings();
	int winWidth = set->vMode.width;
	int winHeight = set->vMode.height;

	static obsInfo d;
	static bool setup;

	if (!setup)
	{
		int i;
		printf("%g", obsCount);
		d.obs = (obstacle*)malloc(obsCount * sizeof(obstacle));
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
	sthSettings* set = sthGetSettings();
	int winWidth = set->vMode.width;
	int winHeight = set->vMode.height;

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
		deltaPos = (float)(rand() % (int)(maxDelta - minDelta) + minDelta);
		newPos.x = lastPos->x + deltaPos;
		newPos.y = rand() % (int)(coordMaxY - coordMinY) + coordMinY;

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

void moveHor(int* score)
{
	sthSettings* set = sthGetSettings();
	int winWidth = set->vMode.width;
	int winHeight = set->vMode.height;
	(*score) += (int)(bgTerminalVelocity - bgInitialVelocity) * bgVelocityFunction(*score / bgSoftness) + bgInitialVelocity;
}

void moveVer(bool moveUp)
{
	sthSettings* set = sthGetSettings();
	int winWidth = set->vMode.width;
	int winHeight = set->vMode.height;

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
		p->velocity = -((grVelocityFunction(delta) / grSoftness) + grInitialVelocity);

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

void writeScore(int score)
{
	FILE* fp;
	errno_t err = fopen_s(&fp, "lastScore.txt", "w");

	if (fp != NULL && err == 0)
	{
		fprintf_s(fp, "%d", score);
		fclose(fp);
	}
}

int readScore()
{
	int r;
	FILE* fp;
	errno_t err = fopen_s(&fp, "lastScore.txt", "r");

	if (fp != NULL && err == 0)
	{
		fscanf_s(fp, "%d", &r);
		fclose(fp);
	}
	else
	{
		r = 0;
	}

	return r;
}