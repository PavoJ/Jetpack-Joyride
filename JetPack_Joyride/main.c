#include"logica.c"
#include"gameClock.c"
#include"textHandler.c"

#include <SFML\Graphics.h>
#include <SFML\window.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

int menu(sfRenderWindow* win, sfVideoMode vMode, sfColor clearColor, int argc, dynText* states)
{
	int a;
	int clickedButton = -1;
	int activeButton = -1;

	sfFloatRect playR;

	sfVector2i mouse;

	//finchè il giocatore non ha ancora scelto un tasto
	while ((clickedButton==-1) && sfRenderWindow_isOpen(win))
	{
		sfEvent event;
		while (sfRenderWindow_pollEvent(win, &event))
		{
			switch (event.type)
			{
			case sfEvtClosed:
				sfRenderWindow_close(win);
				break;
			
			case sfEvtMouseMoved:
				for (a = 0; a < argc; a++)
				{
					mouse = sfMouse_getPositionRenderWindow(win);
					playR = sfText_getGlobalBounds(states[a].text);

					if (sfFloatRect_contains(&playR, (float)mouse.x, (float)mouse.y))
					{
						sfText_setColor(states[a].text, sfColor_fromRGB(70, 70, 150));
						activeButton = a;
					}
					else if (activeButton == a)
					{
						sfText_setColor(states[a].text, sfBlack);
						activeButton = -1;
					}
				}
				break;

			case sfEvtMouseButtonPressed:
				if (sfMouse_isButtonPressed(sfMouseLeft))
				{
					if (activeButton != -1)
					{
						clickedButton = activeButton;
						txtHandler_click(states[activeButton]);
					}
				}
				break;
			}
		}

		sfRenderWindow_clear(win, clearColor);

		for(a=0 ; a<argc ; a++)
			sfRenderWindow_drawText(win, states[a].text, NULL);

		sfRenderWindow_display(win);
	}
	return clickedButton;
}

void diagBox(sfRenderWindow* win, sfVideoMode vMode, char* diagText, int argc, ...)
{
	va_list list;
	va_start(list, argc);

	sfVector2f size = (sfVector2f){ vMode.width / 2, vMode.height / 2 };
	sfRectangleShape* diag = sfRectangleShape_create();
	sfRectangleShape_setSize(diag, size);
	sfRectangleShape_setOrigin(diag, (sfVector2f) { size.x/2, size.y/2 });
	sfRectangleShape_setPosition(diag, (sfVector2f) { vMode.width / 2, vMode.height / 2 });
	sfRectangleShape_setFillColor(diag, sfBlack);

	sfEvent event;
	while (sfRenderWindow_isOpen(win))
	{
		while (sfRenderWindow_pollEvent(win, &event))
		{
			switch (event.type)
			{
			case sfClose:
				sfRenderWindow_close(win);
				break;
			}
		}
		sfRenderWindow_clear(win, sfColor_fromRGB(150, 150, 150));

		sfRenderWindow_drawRectangleShape(win, diag, NULL);

		sfRenderWindow_display(win);
	}

}

void gameLoop(sfRenderWindow* win, sfVideoMode vMode)
{
	sfRectangleShape* mainChar = sfRectangleShape_create();//Player
	sfRectangleShape_setSize(mainChar, (sfVector2f) { 100, 200 });
	sfRectangleShape_setFillColor(mainChar, sfBlue);

	bool movingUp = false;//se il tasto per andare su è tenuto premuto.

	player* pl = getP();
	
	//finchè la finestra è aperta
	while (sfRenderWindow_isOpen(win))
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
				break;

			case sfEvtKeyReleased:
				if (!sfKeyboard_isKeyPressed(sfKeySpace))
					movingUp = false;
				break;
			}
		}

		if (elapsedTick())
		{
			move(movingUp);
			sfRectangleShape_setPosition(mainChar, (sfVector2f) { pl->position.x, 1080 - pl->position.y });
			/*
			system("cls");
			printf("%d - %d (%d)", pl->position.x, 1080-pl->position.y, movingUp);*/
		}

		//sovrascrivo tutta la finestra con il colore bianco
		sfRenderWindow_clear(win, sfColor_fromRGB(255, 255, 255));

		sfRenderWindow_drawRectangleShape(win, mainChar, NULL);

		//disegno i cambiamenti su schermo
		sfRenderWindow_display(win);
	}
}

int main ()
{
	//dichiarazione della finestra
	sfRenderWindow* win;

	//imposto la dimensione della finestra
	sfVideoMode vMode;
	vMode.width = 1920;
	vMode.height = 1080;

	
	//creazione della finestra
	win = sfRenderWindow_create(vMode, "Jetpack vs. Zombies", sfClose, NULL);

	inText* m;
	int inp = menu(win, vMode, sfColor_fromRGB(150, 150, 150), 5, m);
	
	switch (inp)
	{
	case 0:
		gameLoop(win, vMode);
		break;
	/*case 1:
		x=menu(win, vMode, sfColor_fromRGB(150, 150, 150), 5,wepofrjw, wefjnwiejhf, weoiufh);
		break;*/
	case 2:
		diagBox(win, vMode, "Are you sure about that?", 0);
		break;
	}
}