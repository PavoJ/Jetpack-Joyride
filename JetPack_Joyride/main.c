#include"csfml_framework/sceneHandler.c"
#include"consts.h"
#include"logica.c"

#include <SFML\Graphics.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

void gameLoop(sfRenderWindow* win)
{
	player* pl = getP();

	//texture
	sfTexture* bgTexture = sfTexture_createFromFile("basicbg.png", NULL);
	sfTexture* playerTexture = sfTexture_createFromFile("player.png", NULL);
	sfTexture* obsTexture = sfTexture_createFromFile("zapbar.png", NULL);

	scene* s = shNewScene();

	//Giocatore
	sfRectangleShape* mainChar = shAppendRectangleS(s, pl->position, (sfVector2f) { playerWidth, playerHeight }, true);
	sfRectangleShape_setTexture(mainChar, playerTexture, false);
	
	//sfondo
	sfRectangleShape* bg = shAppendRectangleS(s, (sfVector2f) { 0, 0 }, (sfVector2f) { winWidth, winHeight }, false);
	sfRectangleShape_setTexture(bg, bgTexture, false);

	//calcolo la dimensione della texture dell'ostacolo
	sfVector2u obsSize = sfTexture_getSize(obsTexture);

	//ostacolo
	sfRectangleShape* obs = shAppendRectangleS(s, (sfVector2f) { 0, 0 }, (sfVector2f) { obsSize.x * 3, obsSize.y * 3 }, false);
	sfRectangleShape_setTexture(obs, obsTexture, true);

	//orologio di gioco (per aggiornarlo ogni tick)
	sfClock* clk = sfClock_create();
	sfTime elapsedTime;

	bool movingUp = false;//se il tasto per andare su è tenuto premuto.

	int score = 0;//tiene conto della distanza percorsa orizzontalmente
	int stage = 1;//tiene conto dello stadio a cui si trovano i giocatori nel livello
	
	bool playing = true;
	//finchè la finestra è aperta
	while (sfRenderWindow_isOpen(win) && playing)
	{
		sfEvent event;
		while (sfRenderWindow_pollEvent(win, &event))//ciclo tra tutti gli eventi accumulati
		{
			//switch per i vari tipi di evento
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

			moveHor(&score, &stage);
			moveVer(movingUp);

			sfRectangleShape_setPosition(mainChar, pl->position);
			
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
}

//x è il numero del testo
//y è il numero dele stringhe su schermo
#define textPos(x, y)  (sfVector2f){winWidth/2, ((float)winHeight/(y+2))*x}

int main ()
{
	sfRenderWindow* win;

	sfVideoMode vMode;
	vMode.width = winWidth;
	vMode.height = winHeight;
	
	//creazione della finestra di dimensioni vMode
	win = sfRenderWindow_create(vMode, "Jetpack vs. Zombies", sfClose, NULL);
	//sfRenderWindow_setFramerateLimit(win, 61);

	player* p = getP();

	sfFont* defaultF = sfFont_createFromFile("defaultFont.ttf");
	int fontSize = 70;
	
	scene* s;

	sfTexture* mCT = sfTexture_createFromFile("Player.png", NULL);
	sfTexture* bgT = sfTexture_createFromFile("basicbg.png", NULL);

	sfRectangleShape* mainChar;
	sfRectangleShape* bg;
	
	int inp;

	bool playing = true;
	while (playing && sfRenderWindow_isOpen(win))
	{
		s = shNewScene();

		bg = shAppendRectangleS(s, (sfVector2f) { 0, 0 }, (sfVector2f) {winWidth, winHeight}, true);
		sfRectangleShape_setTexture(bg, bgT, false);

		mainChar = shAppendRectangleS(s, p->position, (sfVector2f) { playerWidth, playerHeight }, true);
		sfRectangleShape_setTexture(mainChar, mCT, false);

		shAppendText(s, textPos(1.f, 2.f), sfWhite, defaultF, fontSize, "Gioca", true);
		shAppendText(s, textPos(2.f, 2.f), sfWhite, defaultF, fontSize, "Impostazioni", true);

		inp = shSceneLoop(s, win, vMode, sfWhite);
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

			dtDynText* sounds = shAppendDynText(s, textPos(1.f, 3.f), sfWhite, defaultF, fontSize, true, 2, "Suoni<on>", "Suoni<off>");
			

			shSceneLoop(s, win, vMode, sfWhite);


			break;
		}
	}
}