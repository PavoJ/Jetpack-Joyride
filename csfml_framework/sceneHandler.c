#include"csfml_framework.h"

dtDynText* shAppendDynText(dtDynText* s, sfVector2f pos, sfFont* font, unsigned int textSize, int argc, ...)
{
	scene* Stmp = shSetLast(s);
	
	Stmp->type = dynText;
	
	dtDynText* DTtmp = Stmp->T.dynText;
	DTtmp = malloc(sizeof(dynText));
	DTtmp->states->cnt = 0;

	sfText* Ttmp = DTtmp->text;
	Ttmp = sfText_create();
	
	shCenterText(Ttmp);
	sfText_setPosition(Ttmp, pos);
	sfText_setFont(Ttmp, font);
	sfText_setCharacterSize(Ttmp, textSize);
	
	va_list list;
	va_start(list, argc);
	char* STRtmp;
	int a;
	for (a = 0; a < argc; a++)
	{
		STRtmp = va_arg(list, char*);
		shDynTextChAppend(DTtmp->states, STRtmp);
	}
	va_end(list);
	
	return DTtmp;
}



void renderScene(scene* s, sfRenderWindow* win)
{
	scene* Stmp = s;
	while (Stmp != NULL)
	{
		switch (Stmp->type)
		{
		case dynText:
			sfRenderWindow_drawText(win, Stmp->T.text, NULL);
			break;
		case text:
			sfRenderWindow_drawText(win, Stmp->T.text, NULL);
			break;
		case rect:
			sfRenderWindow_drawRectangleShape(win, Stmp->T.rect, NULL);
			break;
		case sprite:
			sfRenderWindow_drawSprite(win, Stmp->T.sprite, NULL);
			break;
		}

		Stmp = Stmp->next;
	}
	sfRenderWindow_display(win);
}

//trovo il primo spazio libero nella lista s
scene* shSetLast(scene* s)
{
	scene* Stmp = s;
	while (Stmp != NULL)
		Stmp = Stmp->next;
	Stmp = malloc(sizeof(scene));
	return Stmp;
}

//imposta l'origine del testo al suo centro
void shCenterText(sfText* text)
{
	sfFloatRect FRtmp = sfText_getLocalBounds(text);
	sfVector2f pos = (sfVector2f){ FRtmp.width/2, FRtmp.height/2 };

	sfText_setOrigin(text, pos);
}

//esegue un append di un elemento di testo alla scena s
//pos è la coordinata del testo, il testo viene centrato automaticamente
//font è il font utilizzato
//textSize è la dimensione del testo su schermo
sfText* shAppendText(scene* s, sfVector2f pos, sfFont* font, unsigned int textSize, const char* str)
{
	scene* Stmp = shSetLast(s);

	Stmp->type = text;

	sfText* Ttmp = Stmp->T.text;
	Ttmp = sfText_create();
	shCenterText(Ttmp);
	sfText_setPosition(Ttmp, pos);
	sfText_setFont(Ttmp, font);
	sfText_setCharacterSize(Ttmp, textSize);
	sfText_setString(Ttmp, str);

	return Ttmp;
}

//esegue un append di un rettangolo alla scena s
//pos è la coordinata del rettangolo, come origine ha il punto in alto a sinistra
//dim è la larghezza ed altezza del rettangolo
sfRectangleShape* shAppendRectangleS(scene* s, sfVector2f pos, sfVector2f dim, sfColor c)
{
	scene* Stmp = shSetLast(s);
	
	Stmp->type = rect;
	
	sfRectangleShape* RStmp = Stmp->T.rect;
	RStmp = sfRectangleShape_create();
	sfRectangleShape_setFillColor(RStmp, c);
	sfRectangleShape_setSize(RStmp, dim);
	sfRectangleShape_setPosition(RStmp, pos);
	
	return RStmp;
}

//esegue un append di uno sprite alla scena s
//texture è la texture utilizzata dallo sprite
//pos è la coordinata dello sprite, come origine ha il punto in alto a sinistra
//dim è la larghezza ed altezza dello sprite
sfSprite* shAppendSprite(scene* s, sfTexture* texture, sfVector2f pos)
{
	scene* Stmp = shSetLast(s);
	
	Stmp->type = sprite;

	sfSprite* Sptmp = s->T.sprite;
	Sptmp = sfSprite_create();
	sfSprite_setTexture(Sptmp, texture, 0);
	sfSprite_setPosition(Sptmp, pos);

	return Sptmp;
}