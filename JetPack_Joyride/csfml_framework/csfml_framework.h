#ifndef CSFML_FRAMEWORK_H
#define CSFML_FRAMEWORK_H 1

#include<SFML\Graphics.h>

#include"sceneHandler.c"
#include"dynText.c"

/*
#include<stdarg.h>
#include<stdlib.h>

//testo dinamico

typedef struct dtDynTextCh
{
	char* ctxt;
	int cnt;
	struct dtDynTextCh* next;
} dtDynTextCh;
extern dtDynTextCh;

typedef struct dtDynText
{
	sfText* text;
	int state;
	struct dtDynTextCh* states;
} dtDynText;
extern dtDynText;

//Componenti di una scena

union shSceneT//tipi di scena
{
	dtDynText* dynText;
	sfText* text;
	sfRectangleShape* rect;
	sfSprite* sprite;
};

enum shSceneE//enumeratori dei tipi di scena
{
	dynText = 0,
	txt,
	rec,
	spr
};

typedef struct
{
	enum shSceneE type;
	union shSceneT T;
	scene* next;
} scene;
extern scene;

//dynText.c

//esegue un append di una stringa ad un elemento di testo dinamico DTTC
//str è la stringa da aggiungere alla lista
void dtDynTextChAppend(dtDynTextCh* DTTC, const char* str);

//altera lo stato di un elemento di testo dinamico DTTC
//da chiamare se l'elemento viene cliccato
void dtDynTextUpdate(dtDynText* DTTC);


//sceneHandler.c

//esegue un append di un elemento di testo dinamico alla scena s
//pos è la coordinata del testo, il testo viene centrato automaticamente
//font è il font utilizzato
//textSize è la dimensione del testo su schermo
//argc è il numero di stati che può assumere il testo
//in seguito vanno messi gli stati del testo
dtDynText* shAppendDynText(dtDynText* s, sfVector2f pos, sfFont* font, unsigned int textSize, int argc, ...);

//Renderizza la scena s nella finestra win
void shRenderScene(scene* s, sfRenderWindow* win);

//trovo il primo spazio libero nella lista s
scene* shSetLast(scene* s);

//imposta l'origine del testo al suo centro
void shCenterText(sfText* text);

//esegue un append di un elemento di testo alla scena s
//pos è la coordinata del testo, il testo viene centrato automaticamente
//font è il font utilizzato
//textSize è la dimensione del testo su schermo
sfText* shAppendText(scene* s, sfVector2f pos, sfFont* font, unsigned int textSize, const char* str);

//esegue un append di un rettangolo alla scena s
//pos è la coordinata del rettangolo, come origine ha il punto in alto a sinistra
//dim è la larghezza ed altezza del rettangolo
sfRectangleShape* shAppendRectangleS(scene* s, sfVector2f pos, sfVector2f dim, sfColor c);

//esegue un append di uno sprite alla scena s
//texture è la texture utilizzata dallo sprite
//pos è la coordinata dello sprite, come origine ha il punto in alto a sinistra
//dim è la larghezza ed altezza dello sprite
sfSprite* shAppendSprite(scene* s, sfTexture* texture, sfVector2f pos);
*/

#endif //CSFML_FRAMEWORK_H