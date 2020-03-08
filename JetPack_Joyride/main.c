#include"csfml_framework/sceneHandler.c"
#include"consts.h"
#include"logica.c"
#include"settingsHandler.c"

#include <SFML\Graphics.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include<string.h>

char* pointsStr(int score, int highScore)
{
	static char* scoreTxt;
	static bool setup;

	const int MSize = strlen("Punteggio:0000000000\nRecord:0000000000")*sizeof(char);

	if (scoreTxt == NULL || !setup)
	{
		scoreTxt = malloc(MSize);
		setup = true;
	}
	
	if(scoreTxt != NULL)
		snprintf(scoreTxt, MSize, "%s%d\n%s%d", "Punteggio:", score, "Record:", highScore);

	return scoreTxt;
}

void gameLoop(sfRenderWindow* win)
{
	sthSettings* set = sthGetSettings();
	int winWidth = set->vMode.width;
	int winHeight = set->vMode.height;

	player* pl = getP();

	//texture
	sfTexture* bgTexture = sfTexture_createFromFile("basicbg.png", NULL);
	sfTexture* playerTexture = sfTexture_createFromFile("player.png", NULL);
	sfTexture* obsTexture = sfTexture_createFromFile("zapbar.png", NULL);

	scene* s = shNewScene();

	int score = 0;//tiene conto della distanza percorsa orizzontalmente
	const int highScore = readScore();//record
	int stage = 1;//tiene conto dello stadio a cui si trovano i giocatori nel livello

	sfFont* defaultF = sfFont_createFromFile("defaultFont.ttf");
	int fontSize = 45;
	
	//Giocatore (non è uno sprite per alcune limitazioni di CSFML)
	sfRectangleShape* mainChar = shAppendRectangleS(s, pl->position, (sfVector2f) { (playerWidth/1920)*winWidth, (playerHeight/1080)*winHeight }, true);
	sfRectangleShape_setTexture(mainChar, playerTexture, true);
	
	//scritta del punteggio
	sfText* scoreText = shAppendText(s, (sfVector2f) { 0, 0 }, sfBlue, defaultF, fontSize, pointsStr(score, highScore), true, false);

	//sfondo
	sfRectangleShape* bg = shAppendRectangleS(s, (sfVector2f) { 0, 0 }, (sfVector2f) { winWidth, winHeight }, false);
	sfRectangleShape_setTexture(bg, bgTexture, false);

	//calcolo la dimensione della texture dell'ostacolo
	sfVector2u obsSize = sfTexture_getSize(obsTexture);

	//ostacolo
	sfRectangleShape* obs = shAppendRectangleS(s, (sfVector2f) { 0, 0 }, (sfVector2f) { (obsSize.x * 3. / 1920.)*winWidth, (obsSize.y * 3. / 1920.)*winHeight }, false);
	sfRectangleShape_setTexture(obs, obsTexture, true);

	//orologio di gioco (per aggiornarlo ogni tick)
	sfClock* clk = sfClock_create();
	sfTime elapsedTime;

	bool movingUp = false;//se il tasto per andare su è tenuto premuto.
	
	bool playing = true;
	//finchè la finestra è aperta
	while (sfRenderWindow_isOpen(win) && playing)
	{
		sfEvent event;
		while (sfRenderWindow_pollEvent(win, &event))//ciclo tra tutti gli eventi
		{
			switch (event.type)
			{
			case sfEvtClosed://chiusura finestra
				sfRenderWindow_close(win);
				break;

			case sfEvtKeyPressed://se un tasto è stato cliccato
				if (sfKeyboard_isKeyPressed(sfKeySpace))
					movingUp = true;
				else if (sfKeyboard_isKeyPressed(sfKeyEscape))
					playing = false;
				break;

			case sfEvtKeyReleased://se un tasto è stato lasciato
				if (!sfKeyboard_isKeyPressed(sfKeySpace))
					movingUp = false;
				break;
			}
		}

		elapsedTime = sfClock_getElapsedTime(clk);
		if (sfTime_asMilliseconds(elapsedTime) > tick)
		{
			sfClock_restart(clk);

			playing = !collisionCheck(score, mainChar, obs);

			moveHor(&score);//muovo gli ostacoli
			moveVer(movingUp);//muovo il giocatore
			
			sfRectangleShape_setPosition(mainChar, pl->position);

			sfText_setString(scoreText, pointsStr(score, highScore));

			//printf("%f - %f (%d)\n", pl->position.x, pl->position.y, movingUp);
		}

		sfRenderWindow_clear(win, sfWhite);

		drawBackground(win, bg, score);
		drawObs(win, obs, score);
		shRenderScene(s, win);
	}

	shSceneDestroy(s);

	sfTexture_destroy(bgTexture);
	sfTexture_destroy(playerTexture);
	sfTexture_destroy(obsTexture);

	sfClock_destroy(clk);

	pl->position.y = coordMaxY - playerHeight;

	if (score > highScore) writeScore(score);
}

//x è il numero del testo
//y è il numero dele stringhe su schermo
#define textPos(x, y)  (sfVector2f){winWidth/2, ((float)winHeight/(y+2))*x}

int main ()
{
	srand(time(NULL));

	sthSettings* set = sthGetSettings();
	int winWidth = set->vMode.width;
	int winHeight = set->vMode.height;

	sfRenderWindow* win;
	
	//creazione della finestra di dimensioni vMode
	win = sfRenderWindow_create(set->vMode, "Jetpack vs. Zombies", sfClose, NULL);
	//sfRenderWindow_setFramerateLimit(win, 61);

	player* p = getP();

	sfFont* defaultF = sfFont_createFromFile("defaultFont.ttf");
	int fontSize = 70;
	
	scene* s;

	sfTexture* mCT = sfTexture_createFromFile("Player.png", NULL);
	sfTexture* bgT = sfTexture_createFromFile("basicbg.png", NULL);

	sfRectangleShape* mainChar;
	sfRectangleShape* bg;
	
	int inp;//input dell'utente
	int a;//contatore per for

	bool playing = true;
	while (playing && sfRenderWindow_isOpen(win))
	{
		s = shNewScene();

		bg = shAppendRectangleS(s, (sfVector2f) { 0, 0 }, (sfVector2f) {winWidth, winHeight}, true);
		sfRectangleShape_setTexture(bg, bgT, false);

		mainChar = shAppendRectangleS(s, p->position, (sfVector2f) { (playerWidth / 1920.f)* winWidth, (playerHeight / 1080.f)* winHeight }, true);
		sfRectangleShape_setTexture(mainChar, mCT, false);

		shAppendText(s, textPos(1.f, 2.f), sfWhite, defaultF, fontSize, "Gioca", true, true);
		shAppendText(s, textPos(2.f, 2.f), sfWhite, defaultF, fontSize, "Impostazioni", true, true);

		inp = shSceneLoop(s, win, set->vMode, sfWhite);
		shSceneDestroy(s);

		switch (inp)
		{
		case 2://"Gioca"
			gameLoop(win);
			//playing = false;
			break;
		case 3://"Impostazioni"
			s = shNewScene();

			bg = shAppendRectangleS(s, (sfVector2f) { 0, 0 }, (sfVector2f) { winWidth, winHeight }, true);
			sfRectangleShape_setTexture(bg, bgT, false);

			dtDynText* sounds = shAppendDynText(s, textPos(1.f, 4.f), sfWhite, defaultF, fontSize, true, true, 2, "Suoni<on>", "Suoni<off>");
			
			//mi annoto tutte le possibili risoluzioni per la macchina
			int vModeCnt;
			char buff[25];
			sfVideoMode* vModeArr = sfVideoMode_getFullscreenModes(&vModeCnt);

			dtDynText* res = shAppendDynText(s, textPos(2.f, 4.f), sfWhite, defaultF, fontSize, true, true, 0);

			//aggiungo ogni tipo di risoluzione al testo dinamico
			dtDynTextCh* DTC = res->states;
			for (a = 0; a < vModeCnt; a++)
			{
				sprintf_s(buff, 25 * sizeof(char), "Risoluzione<%dx%d>", vModeArr[a].width, vModeArr[a].height);
				dtDynTextChAppend(DTC, buff);
			}
			sfText_setString(res->sText.text, res->states->ctxt);
			shCenterText(res->sText.text);

			dtDynText* fullScreen = shAppendDynText(s, textPos(3.f, 4.f), sfWhite, defaultF, fontSize, true, true, 2, "Schermo intero<off>", "Schermo intero<on>");

			shAppendText(s, textPos(4.f, 4.f), sfWhite, defaultF, fontSize, "Applica", true, true);

			shSceneLoop(s, win, set->vMode, sfWhite);

			if (vModeArr[res->state].width != winWidth ||
				vModeArr[res->state].height != winHeight)
			{
				set->vMode = vModeArr[res->state];
				winWidth = set->vMode.width;
				winHeight = set->vMode.height;

				sfRenderWindow_destroy(win);
				win = sfRenderWindow_create(set->vMode, "Jetpack vs. Zombies", sfClose, NULL);

				sthWriteSettings(set);
			}

			break;
		}
	}
}