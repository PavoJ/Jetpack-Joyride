#include"logica.c"
#include"gameClock.c"
#include"textHandler.c"

#include <SFML\Graphics.h>
#include <SFML\window.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

int menu(sfRenderWindow* win, sfVideoMode vMode, sfColor clearColor, dynText states)
{
	int a;
	int clickedButton = -1;//tasto scelto dal giocatore
	int activeButton = -1;//tasto che ha il mouse sopra

	dynText *statec = &states;//per ciclare tra il testo

	sfVector2i mouse;//coordinate del mouse
	sfFloatRect playR;//per calcolare intersezioni tra il mouse e il testo

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
				while(statec != NULL)
				{
					mouse = sfMouse_getPositionRenderWindow(win);
					playR = sfText_getGlobalBounds(statec->text);

					//se il mouse è sopra a del testo
					if (sfFloatRect_contains(&playR, (float)mouse.x, (float)mouse.y))
					{
						sfText_setColor(statec->text, sfColor_fromRGB(70, 70, 150));
						activeButton = a;
					}
					//se il mouse ha lasciato il testo
					else if (activeButton == a)
					{
						sfText_setColor(statec->text, sfBlack);
						activeButton = -1;
					}
					statec = statec->next;
				}
				statec = &states;
				break;

			case sfEvtMouseButtonPressed:
				//se il giocatore clicca il tasto sinistro del mouse
				if (sfMouse_isButtonPressed(sfMouseLeft))
				{
					//se ha il mouse sopra a del testo
					if (activeButton != -1)
					{
						//TODO: da cambiare la parte commentata: non deve uscire subito, ma deve controllare
						//se ci sono degli stati da ciclare
						//clickedButton = activeButton;
						while(activeButton!=0)
						{
							statec = statec->next;
							activeButton--;
						}
						txtHandler_click(*statec);
						statec = &states;
						
					}
				}
				break;
			}
		}

		sfRenderWindow_clear(win, clearColor);

		while (statec != NULL)
		{
			sfRenderWindow_drawText(win, statec->text, NULL);
			statec = statec->next;
		}
		statec = &states;

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
	sfRectangleShape_setSize(mainChar, (sfVector2f) { 150, 200 });
	sfRectangleShape_setFillColor(mainChar, sfBlue);

	bool movingUp = false;//se il tasto per andare su � tenuto premuto.

	player* pl = getP();
	
	//finch� la finestra � aperta
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

	dynText m;
	sfVector2f pos;
	int a, statec, iNum=0;
	char totStates[5][3][20] = {
		{"Gioca", "Fra ma sei sicuro?", '\0'},
		{"Impostazioni", '\0'},
		{"Crediti", '\0'},
		{"Esci", '\0'},
		{'\0'}};
	
	while(totStates[iNum][0][0]!='\0') iNum++;
	for(a=0 ; a<iNum ; a++)
	{
		pos = (sfVector2f){vMode.width/2, (vMode.height/iNum)*(a+1)};

		statec=0;
		while(totStates[a][statec][0]!='\0') statec++;

		txtHandler_appendText(&m, 70, pos, statec+1, totStates[a][0]);
	}

	
	menu(win, vMode, sfColor_fromRGB(150, 150, 150), m);
	/*
	switch (inp)
	{
	case 0:
		gameLoop(win, vMode);
		break;
	case 1:
		x=menu(win, vMode, sfColor_fromRGB(150, 150, 150), 5,wepofrjw, wefjnwiejhf, weoiufh);
		break;
	case 2:
		diagBox(win, vMode, "Are you sure about that?", 0);
		break;
	}*/
}