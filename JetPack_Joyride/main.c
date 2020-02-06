#include<SFML\Graphics.h>
#include<SFML\window.h>

int main()
{
	sfRenderWindow *win;

	sfVideoMode vMode;
	vMode.width = 1920;
	vMode.height = 1080;

	win = sfRenderWindow_create(vMode, "hey", sfClose, NULL);
	
	sfCircleShape* c = sfCircleShape_create();

	sfCircleShape_setFillColor(c, sfColor_fromRGB(0, 0, 0));
	sfCircleShape_setRadius(c, 500);

	sfVector2f circle;
	circle.x = 0;
	circle.y = 0;

	sfCircleShape_setPosition(c, circle);

	while (sfWindow_isOpen(win))
	{
		sfRenderWindow_clear(win, sfColor_fromRGB(255, 255, 255));

		sfRenderWindow_drawCircleShape(win, c, NULL);

		sfRenderWindow_display(win);
	}
}