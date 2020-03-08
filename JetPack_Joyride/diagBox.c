/*#include"settingsHandler.c"
#include<SFML\Graphics.h>
#include<math.h>

enum txtOpts
{
	NONE = 0,
	YES_NO,
	CANCEL_OK
};

constexpr double PHI = 1.61803398874989;

bool dbDisplay(sfRenderWindow* win, sfColor color, enum txtOpts opt, const char* diagText)
{
	sthSettings* set = sthGetSettings();
	int winWidth = set->vMode.width;
	int winHeight = set->vMode.height;

	sfRectangleShape* dBox = sfRectangleShape_create();

	sfRectangleShape_setSize(dBox, (sfVector2f){ (float)winWidth / 2.f, (float)winHeight / 2.f });
}*/