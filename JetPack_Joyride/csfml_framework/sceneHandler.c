//#include"csfml_framework.h"
#include"dynText.c"

#include<SFML\Graphics.h>

#include<stdarg.h>
#include<stdlib.h>

//Componenti di una scena

union shSceneT//tipi di scena
{
	dtDynText* dynText;
	dtText* sText;
	sfRectangleShape* rect;
	sfSprite* sprite;
};

enum shSceneE//enumeratori dei tipi di scena
{
	empty = -1,//vuoto
	dTxt,//testo dinamico
	sTxt,//testo normale
	rec,//rettangolo
	spr//sprite
};

typedef struct scene
{
	enum shSceneE type;
	union shSceneT T;
	struct scene* next;
} scene;

scene* newScene()
{
	scene* Stmp = malloc(sizeof(scene));
	if (Stmp != NULL)
	{
		Stmp->type = empty;
		Stmp->next = NULL;
	}
	else
	{
		printf("c'e' stato un errore nell'allocazione di una nuova scena\n");
	}

	return Stmp;
}

//Imposto il puntatore della scena al primo elemento vuoto
scene* shSetLast(scene* s)
{
	while (s->type != empty) s = s->next;

	s->next = newScene();

	return s;
}

void shRenderScene(scene* s, sfRenderWindow* win)
{
	union shSceneT Stype;
	while (s->type != empty)
	{
		Stype = s->T;
		switch (s->type)
		{
		case dTxt:
			sfRenderWindow_drawText(win, Stype.dynText->sText.text, NULL);
			break;
		case sTxt:
			sfRenderWindow_drawText(win, Stype.sText->text, NULL);
			break;
		case rec:
			sfRenderWindow_drawRectangleShape(win, Stype.rect, NULL);
			break;
		case spr:
			sfRenderWindow_drawSprite(win, Stype.sprite, NULL);
			break;
		}

		s = s->next;
	}
	sfRenderWindow_display(win);
}

//mette una scena s in una window loop, e riporta un valore pari al tasto premuto
//win è un puntatore alla finestra
//vMode è la grandezza della finestra
//clearColor è il colore di sottofondo
int shSceneLoop(scene* s, sfRenderWindow* win, sfVideoMode vMode, sfColor clearColor)
{
	int a;
	int clickedButton = -1;//tasto scelto dal giocatore
	int activeButton = -1;//tasto che ha il mouse sopra

	scene* Stmp;

	dtText Ttmp;

	sfVector2i mouse;//coordinate del mouse
	sfFloatRect playR;//per calcolare intersezioni tra il mouse e il testo

	sfEvent event;

	//finchè il giocatore non ha ancora scelto un tasto
	while ((clickedButton == -1) && sfRenderWindow_isOpen(win))
	{
		while (sfRenderWindow_pollEvent(win, &event))
		{
			switch (event.type)
			{
			case sfEvtClosed:
				sfRenderWindow_close(win);
				break;

			case sfEvtMouseMoved:
				Stmp = s;
				a = 0;
				while (Stmp->type != empty)
				{
					if (Stmp->type == sTxt || Stmp->type == dTxt)
					{
						Ttmp = (Stmp->type == sTxt) ? (*Stmp->T.sText) : Stmp->T.dynText->sText;

						mouse = sfMouse_getPositionRenderWindow(win);
						playR = sfText_getGlobalBounds(Ttmp.text);

						//se il mouse è sopra a del testo
						if (sfFloatRect_contains(&playR, (float)mouse.x, (float)mouse.y))
						{
							sfText_setColor(Ttmp.text, sfColor_fromRGB(70, 70, 150));
							activeButton = a;
						}
						else if (activeButton == a)
						{
							sfText_setColor(Ttmp.text, Ttmp.defaultColor);
							activeButton = -1;
						}
					}
					Stmp = Stmp->next;
					a++;
				}
				break;

			case sfEvtMouseButtonPressed:
				//se il giocatore clicca il tasto sinistro del mouse
				if (sfMouse_isButtonPressed(sfMouseLeft))
				{
					//se ha il mouse sopra a del testo
					if (activeButton != -1)
					{
						//vai all'elemento cliccato
						Stmp = s;
						for (a = 0; a != activeButton; a++)
							Stmp = Stmp->next;

						//se e' un elemento di testo dinamico, cambia il suo stato
						if (Stmp->type == dTxt)
							dtDynTextUpdate(Stmp->T.dynText);
						else if (Stmp->type == sTxt)
							clickedButton = activeButton;
					}
				}
				break;

			}
		}

		sfRenderWindow_clear(win, clearColor);

		shRenderScene(s, win);
	}
	return clickedButton;
}

//imposta l'origine del testo al suo centro
void shCenterText(sfText* t)
{
	sfFloatRect FRtmp = sfText_getLocalBounds(t);
	sfVector2f pos = (sfVector2f){ FRtmp.width / 2.f, -FRtmp.height / 2.f };

	sfText_setOrigin(t, pos);
}

dtDynText* shAppendDynText(scene* s, sfVector2f pos, sfColor color, sfFont* font, unsigned int textSize, int argc, ...)
{
	scene* Stmp = shSetLast(s);
	Stmp->type = dTxt;
	
	Stmp->T.dynText = malloc(sizeof(dtDynText));
	dtDynText* DTtmp = Stmp->T.dynText;

	if (DTtmp != NULL)
	{
		DTtmp->stateT = argc;
		DTtmp->state = 1;
		
		DTtmp->sText.text = sfText_create();
		if (DTtmp->sText.text != NULL)
		{
			sfText* Ttmp = DTtmp->sText.text;

			shCenterText(Ttmp);
			sfText_setPosition(Ttmp, pos);
			sfText_setFont(Ttmp, font);
			sfText_setCharacterSize(Ttmp, textSize);

			sfText_setFillColor(Ttmp, color);
			DTtmp->sText.defaultColor = color;

			va_list list;
			va_start(list, argc);

			char* STRtmp;
			int a;

			DTtmp->states = malloc(sizeof(dtDynTextCh));
			if (DTtmp->states != NULL)
			{
				DTtmp->states->next = NULL;

				for (a = 0; a < argc; a++)
				{
					STRtmp = va_arg(list, char*);
					dtDynTextChAppend(DTtmp->states, STRtmp);
				}

				sfText_setString(Ttmp, DTtmp->states->ctxt);
			}

			va_end(list);
		}
	}
	return DTtmp;
}

//esegue un append di un elemento di testo alla scena s
//pos è la coordinata del testo, il testo viene centrato automaticamente
//font è il font utilizzato
//textSize è la dimensione del testo su schermo
sfText* shAppendText(scene* s, sfVector2f pos, sfColor color, sfFont* font, unsigned int textSize, const char* str)
{
	scene* Stmp = shSetLast(s);
	Stmp->type = sTxt;

	sfText* Ttmp = NULL;

	Stmp->T.sText = malloc(sizeof(dtText));
	dtText* STtemp = Stmp->T.sText;
	if (STtemp != NULL)
	{
		STtemp->text = sfText_create();
		Ttmp = STtemp->text;

		shCenterText(Ttmp);
		sfText_setPosition(Ttmp, pos);
		sfText_setFont(Ttmp, font);
		sfText_setCharacterSize(Ttmp, textSize);
		sfText_setString(Ttmp, str);

		sfText_setFillColor(Ttmp, color);
		STtemp->defaultColor = color;
	}

	return Ttmp;
}

//esegue un append di un rettangolo alla scena s
//pos è la coordinata del rettangolo, come origine ha il punto in alto a sinistra
//dim è la larghezza ed altezza del rettangolo
sfRectangleShape* shAppendRectangleS(scene* s, sfVector2f pos, sfVector2f dim, sfColor c)
{
	scene* Stmp = shSetLast(s);
	
	Stmp->type = rec;
	
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
	
	Stmp->type = spr;

	sfSprite* Sptmp = Stmp->T.sprite;
	Sptmp = sfSprite_create();
	sfSprite_setTexture(Sptmp, texture, 0);
	sfSprite_setPosition(Sptmp, pos);

	return Sptmp;
}