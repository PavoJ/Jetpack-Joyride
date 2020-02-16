#include<stdarg.h>

#include<SFML\Graphics.h>

/*Piani:
	Enum per la gestione delle risposte ("Si\No", "Ok", "Cancella")
*/

/*
void dbDisplay(sfRenderWindow* win, sfVideoMode vMode, char* diagText, int argc, ...)
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

}*/