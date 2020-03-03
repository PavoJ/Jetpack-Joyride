/*
	scene è una lista contenente gli elementi che compongono una schermata (menù principale, impostazioni, gioco...)
	il tipo scene non va confuso con una pila, dato che non è LIFO
*/

//#include"csfml_framework.h"
#include"dynText.c"

#include<SFML\Graphics.h>

#include<stdarg.h>
#include<stdlib.h>
#include<stdbool.h>

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
	bool draw;
	struct scene* next;
} scene;

scene* shNewScene()
{
	scene* Stmp = malloc(sizeof(scene));
	if (Stmp != NULL)
	{
		Stmp->type = empty;
		Stmp->next = NULL;
	}
	else
	{
		fprintf_s(stderr, "c'e' stato un errore nell'allocazione di una nuova scena\n");
	}

	return Stmp;
}

//Imposto il puntatore della scena al primo elemento vuoto
scene* shSetLast(scene* s)
{
	if (s->type != empty)
		s = shSetLast(s->next);
	else
		s->next = shNewScene();	

	return s;
}

void shRenderScene(scene* s, sfRenderWindow* win)
{
	union shSceneT Stype;

	Stype = s->T;

	if (s->draw)
	{
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
	}
	
	s = s->next;
	if (s->type != empty) shRenderScene(s, win);
	else sfRenderWindow_display(win);
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
	sfVector2f pos = (sfVector2f){ FRtmp.width / 2.f, FRtmp.height / 2.f };

	sfText_setOrigin(t, pos);
}

//esegue un append ad una scena s un elemento di testo dinamico (vedi dynText.c)
//pos è la positione del testo
//color è il colore del testo
//font è il font del testo
//textSize è la dimensione del testo
//drawOnRender determina se l'oggetto deve essere disegnato dalla funzione shRenderScene
//argc è il numero di stati che può assumere il testo
//di seguito vanno inserite stringhe di numero argc, sono i vari stati che può assumere il testo
//centerText determina se va centrato il testo
dtDynText* shAppendDynText(scene* s, sfVector2f pos, sfColor color, sfFont* font, unsigned int textSize, bool drawOnRender, bool centerText, int argc, ...)
{
	scene* Stmp = shSetLast(s);

	Stmp->type = dTxt;
	Stmp->draw = drawOnRender;
	
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

			//inserisco la lista di stati nelle variabili apposite
			va_list list;
			va_start(list, argc);
			char* STRtmp;
			int a;

			DTtmp->states = malloc(sizeof(dtDynTextCh));

			if (DTtmp->states != NULL)
			{
				DTtmp->states->ctxt = NULL;
				DTtmp->states->next = NULL;

				for (a = 0; a < argc; a++)
				{
					STRtmp = va_arg(list, char*);
					dtDynTextChAppend(DTtmp->states, STRtmp);
				}

				sfText_setString(Ttmp, DTtmp->states->ctxt);
			}
			va_end(list);
			
			//imposto il testo
			sfText_setPosition(Ttmp, pos);
			sfText_setFont(Ttmp, font);
			sfText_setCharacterSize(Ttmp, textSize);
			if(centerText) shCenterText(Ttmp);

			sfText_setFillColor(Ttmp, color);
			DTtmp->sText.defaultColor = color;
		}
	}
	return DTtmp;
}

//esegue un append di un elemento di testo alla scena s
//pos è la coordinata del testo, il testo viene centrato automaticamente
//font è il font utilizzato
//textSize è la dimensione del testo su schermo
//drawOnRender determina se l'oggetto deve essere disegnato dalla funzione shRenderScene
//centerText determina se va centrato il testo
sfText* shAppendText(scene* s, sfVector2f pos, sfColor color, sfFont* font, unsigned int textSize, const char* str, bool drawOnRender, bool centerText)
{
	scene* Stmp = shSetLast(s);

	Stmp->type = sTxt;
	Stmp->draw = drawOnRender;

	sfText* Ttmp = NULL;

	Stmp->T.sText = malloc(sizeof(dtText));
	dtText* STtemp = Stmp->T.sText;
	if (STtemp != NULL)
	{
		STtemp->text = sfText_create();
		Ttmp = STtemp->text;

		sfText_setPosition(Ttmp, pos);
		sfText_setFont(Ttmp, font);
		sfText_setCharacterSize(Ttmp, textSize);
		sfText_setString(Ttmp, str);
		if(centerText) shCenterText(Ttmp);

		sfText_setFillColor(Ttmp, color);
		STtemp->defaultColor = color;
	}

	return Ttmp;
}

//esegue un append di un rettangolo alla scena s
//pos è la coordinata del rettangolo, come origine ha il punto in alto a sinistra
//dim è la larghezza ed altezza del rettangolo
//drawOnRender determina se l'oggetto deve essere disegnato dalla funzione shRenderScene
sfRectangleShape* shAppendRectangleS(scene* s, sfVector2f pos, sfVector2f dim, bool drawOnRender)
{
	scene* Stmp = shSetLast(s);
	
	Stmp->type = rec;
	Stmp->draw = drawOnRender;

	Stmp->T.rect = sfRectangleShape_create();
	sfRectangleShape* RStmp = Stmp->T.rect;

	sfRectangleShape_setSize(RStmp, dim);
	sfRectangleShape_setPosition(RStmp, pos);
	
	return RStmp;
}

//esegue un append di uno sprite alla scena s
//texture è la texture utilizzata dallo sprite
//pos è la coordinata dello sprite, come origine ha il punto in alto a sinistra
//drawOnRender determina se l'oggetto deve essere disegnato dalla funzione shRenderScene
sfSprite* shAppendSprite(scene* s, sfTexture* texture, sfVector2f pos, bool drawOnRender)
{
	scene* Stmp = shSetLast(s);
	
	Stmp->type = spr;
	Stmp->draw = drawOnRender;

	sfSprite* Sptmp = Stmp->T.sprite;
	Sptmp = sfSprite_create();
	sfSprite_setTexture(Sptmp, texture, false);
	sfSprite_setPosition(Sptmp, pos);

	return Sptmp;
}

void shSceneDestroy(scene* s)
{
	scene* Stmp;
	union shSceneT* STtmp;

	Stmp = s;
	s = s->next;

	STtmp = &Stmp->T;
	switch (Stmp->type)
	{
	case dTxt:
		free(STtmp->dynText->states);
		sfText_destroy(STtmp->dynText->sText.text);
		free(STtmp->dynText);
		break;
	case sTxt:
		sfText_destroy(STtmp->sText->text);
		free(STtmp->sText);
		break;
	case rec:
		sfRectangleShape_destroy(STtmp->rect);
		break;
	case spr:
		sfSprite_destroy(STtmp->sprite);
		break;
	}
	free(Stmp);

	if (s != NULL) shSceneDestroy(s);
}