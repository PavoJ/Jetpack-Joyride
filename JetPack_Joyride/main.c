#include"csfml_framework/sceneHandler.c"
#include"consts.h"
#include"logica.c"

#include <SFML\Graphics.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void gameLoop(sfRenderWindow* win)
{
	//texture
	sfTexture* bgTexture = sfTexture_createFromFile("Untitled.jpg", NULL);
	sfTexture* playerTexture = sfTexture_createFromFile("Player.png", NULL);
	//sfTexture* obsTexture = sfTexture_createFromFile("Obstacle.png", NULL);

	//giocatore
	sfRectangleShape* mainChar = sfRectangleShape_create();//Player
	sfRectangleShape_setSize(mainChar, (sfVector2f) { playerWidth, playerHeight });
	sfRectangleShape_setTexture(mainChar, playerTexture, 0);
	
	//sfondo
	sfRectangleShape* bg = sfRectangleShape_create();
	sfRectangleShape_setSize(bg, (sfVector2f) { winWidth, winHeight });
	sfRectangleShape_setTexture(bg, bgTexture, false);

	//sfRectangleShape* obs = sfRectangleShape_create();
	//sfRectangleShape_setTexture(obs, obsTexture, true);

	sfClock* clk = sfClock_create();
	sfTime elapsedTime;

	bool movingUp = false;//se il tasto per andare su è tenuto premuto.
	
	player* pl = getP();

	unsigned int score = 0;//tiene conto della distanza percorsa orizzontalmente
	unsigned int stage = 1;//tiene conto dello stadio a cui si trovano i giocatori nel livello
	
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

			case sfEvtKeyPressed:
				if (sfKeyboard_isKeyPressed(sfKeySpace))
					movingUp = true;
				else if (sfKeyboard_isKeyPressed(sfKeyEscape))
					playing = false;
				break;

			case sfEvtKeyReleased:
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
		sfRenderWindow_drawRectangleShape(win, mainChar, NULL);

		//disegno i cambiamenti su schermo
		sfRenderWindow_display(win);
	}

	sfTexture_destroy(bgTexture);
	sfTexture_destroy(playerTexture);
	//sfTexture_destroy(obsTexture);

	sfRectangleShape_destroy(mainChar);
	sfRectangleShape_destroy(bg);
	//sfRectangleShape_destroy(obs);

	sfClock_destroy(clk);
}

//x è il numero del testo
//y è il numero dele stringhe su schermo
#define textPos(x, y)  (sfVector2f){winWidth/2, ((float)winHeight/y)*x}

int main ()
{
	sfRenderWindow* win;

	sfVideoMode vMode;
	vMode.width = winWidth;
	vMode.height = winHeight;
	
	//creazione della finestra di dimensioni vMode
	win = sfRenderWindow_create(vMode, "Jetpack vs. Zombies", sfClose, NULL);
	//sfRenderWindow_setFramerateLimit(win, 61);

	sfFont* defaultF = sfFont_createFromFile("defaultFont.ttf");
	int fontSize = 70;
	
	scene* s;
	
	int inp;

	bool playing = true;
	while (playing)
	{
		s = newScene();

		shAppendText(s, textPos(1.f, 2.f), sfWhite, defaultF, fontSize, "Gioca");
		shAppendText(s, textPos(2.f, 2.f), sfWhite, defaultF, fontSize, "mhh, yes.");

		inp = shSceneLoop(s, win, vMode, sfColor_fromRGB(150, 150, 150));
		shSceneDestroy(s);

		switch (inp)
		{
		case 0://"Gioca"
			gameLoop(win);
			//playing = false;
			break;
		}
	}
}