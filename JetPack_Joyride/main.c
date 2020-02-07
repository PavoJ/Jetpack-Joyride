#include"logica.h"
#include"gameClock.h"

#include <SFML\Graphics.h>
#include <SFML\window.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

int menu(sfRenderWindow* win, sfVideoMode vMode, sfColor clearColor, int argc, ...)
{
	int clickedButton = -1;
	int activeButton = -1;

	va_list list;
	va_start(list, argc);
	
	sfFont* defaultFont = sfFont_createFromFile("defaultFont.ttf");

	sfText** options = malloc(argc*sizeof(sfText*));

	sfFloatRect rect;
	int a;
	for (a = 0; a < argc; a++)
	{
		options[a] = sfText_create();

		sfText_setString(options[a], va_arg(list, char*));
		sfText_setColor(options[a], sfBlack);

		sfText_setFont(options[a], defaultFont);
		sfText_setCharacterSize(options[a], 70);

		rect = sfText_getLocalBounds(options[a]);

		sfText_setOrigin(options[a], (sfVector2f) { rect.width / 2, rect.height / 2 });

		sfText_setPosition(options[a], (sfVector2f) { (vMode.width / 2), (vMode.height / (argc + 1)) * (a + 1) });
	}

	sfFloatRect playR;

	sfVector2i mouse;

	//finchè il giocatore non ha ancora scelto un tasto
	while (clickedButton==-1)
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
					playR = sfText_getGlobalBounds(options[a]);

					if (sfFloatRect_contains(&playR, (float)mouse.x, (float)mouse.y))
					{
						sfText_setColor(options[a], sfColor_fromRGB(70, 70, 150));
						activeButton = a;
					}
					else
					{
						sfText_setColor(options[a], sfBlack);
						if (activeButton == a)
						{
							activeButton = -1;
						}
					}
				}
				break;

			case sfEvtMouseButtonPressed:
				if (sfMouse_isButtonPressed(sfMouseLeft))
				{
					if (activeButton != -1)
					{
						clickedButton = activeButton;
					}
				}
				break;
			}
		}

		sfRenderWindow_clear(win, clearColor);

		for(a=0 ; a<argc ; a++)
			sfRenderWindow_drawText(win, options[a], NULL);

		sfRenderWindow_display(win);
	}

	return clickedButton;
}

void gameLoop(sfRenderWindow* win, sfVideoMode vMode)
{
	sfRectangleShape* mainChar = sfRectangleShape_create();//Player
	sfRectangleShape_setSize(mainChar, (sfVector2f) { 50, 100 });
	sfRectangleShape_setFillColor(mainChar, sfBlue);

	bool movingUp = false;//se il tasto per andare su è tenuto premuto.

	player* pl = getPS();
	
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

	int inp = menu(win, vMode, sfColor_fromRGB(150, 150, 150), 3, "Gioca", "Impostazioni", "Esci");
	switch (inp)
	{
	case 0:
		gameLoop(win, vMode);
	}
}