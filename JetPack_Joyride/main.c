#include<SFML\Graphics.h>
#include<SFML\window.h>
#include<stdio.h>

void mainMenu()
{
	//dichiaro la finestra
	sfRenderWindow* win;

	//imposto la dimensione della finestra
	sfVideoMode vMode;
	vMode.width = 800;
	vMode.height = 600;

	//creazione della finestra
	win = sfRenderWindow_create(vMode, "Menu' Principale", sfClose, NULL);
	
	sfFont *defaultFont = sfFont_createFromFile("defaultFont.ttf");

	sfText* options[3];
	const char *optionsT[] = { "Gioca", "Impostazioni", "Esci" };

	int a;
	for (a = 0; a < 3; a++)
	{
		options[a] = sfText_create();

		sfText_setString(options[a], optionsT[a]);
		sfText_setColor(options[a], sfBlack);
		sfText_setPosition(options[a], (sfVector2f) { 40, 100*(a+1) });
		sfText_setFont(options[a], defaultFont);
		sfText_setCharacterSize(options[a], 70);
	}

	sfFloatRect playR;

	sfVector2i mouse;

	while (sfRenderWindow_isOpen(win))
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
				for (a = 0; a < 3; a++)
				{
					mouse = sfMouse_getPositionRenderWindow(win);
					playR = sfText_getGlobalBounds(options[a]);

					if (sfFloatRect_contains(&playR, (float)mouse.x, (float)mouse.y))
						sfText_setColor(options[a], sfColor_fromRGB(150, 150, 150));
					else
						sfText_setColor(options[a], sfBlack);
				}
				break;
			}
		}

		sfRenderWindow_clear(win, sfWhite);

		for(a=0 ; a<3 ; a++)
			sfRenderWindow_drawText(win, options[a], NULL);

		sfRenderWindow_display(win);
	}
}

void gameLoop()
{
	//dichiaro la finestra
	sfRenderWindow* win;

	//imposto la dimensione della finestra
	sfVideoMode vMode;
	vMode.width = 1920;
	vMode.height = 1080;

	//creazione della finestra
	win = sfRenderWindow_create(vMode, "hey", sfClose, NULL);

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
			}
		}

		//sovrascrivo tutta la finestra con il colore bianco
		sfRenderWindow_clear(win, sfColor_fromRGB(255, 255, 255));


		//disegno i cambiamenti su schermo
		sfRenderWindow_display(win);
	}
}

int main()
{
	mainMenu();

	
}