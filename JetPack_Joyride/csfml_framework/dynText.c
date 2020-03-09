/*
	DynText è un oggetto di testo dinamico
	è un espansione al concetto di testo di CSFML sfText, in quanto non contiene solo una stringa,
	ma più stringhe che ciclano se una condizione è avverata (di default se il giocatore clicca il testo, 
	nella funzione shSceneLoop di sceneHandler.c).
*/

#include<SFML\Graphics.h>

#include<stdarg.h>
#include<stdlib.h>
#include<stdio.h>

#include<string.h>

typedef struct dtText
{
	sfText* text;
	sfColor defaultColor;
} dtText;

//testo dinamico

typedef struct dtDynTextCh
{
	char* ctxt;
	struct dtDynTextCh* next;
} dtDynTextCh;

typedef struct dtDynText
{
	dtText sText;

	int state;//stato corrente
	int stateT;//stati totali
	
	dtDynTextCh* states;//lista degli stati
} dtDynText;

dtDynTextCh* dtChGetLast(dtDynTextCh* DT)
{
	if (DT->next != NULL) DT = dtChGetLast(DT->next);
	
	return DT;
}

dtDynTextCh* dtChAppend(dtDynText* DTTC, const char* str)
{
	dtDynTextCh* DT = dtChGetLast(DTTC->states);

	DT->next = malloc(sizeof(dtDynTextCh));

	if (DT->next != NULL) DT->next->next = NULL;

	if (DT != NULL)
	{
		DT->ctxt = malloc((strlen(str)+1)*sizeof(char));//+1 a strlen perchè contiamo anche il carattere '\n'
		
		char* STRtmp = DT->ctxt;
		
		if (STRtmp != NULL)
		{
			DTTC->stateT++;
			strcpy_s(STRtmp, (strlen(str) + 1) * sizeof(char), str);
		}
		else printf("Errore nell'allocazione di memoria per la stringa %s", str);
	}
	else {
		fprintf_s(stderr, "c'e' stato un errore nell'append di un elemento di testo DynTextCh");
	}

	return DT;
}

void shCenterText(sfText* t);

//Imposta lo stato del testo dinamico DTTC a state
//come return da un puntatore allo stato
dtDynTextCh* dtSetState(dtDynText* DTTC, int state)
{
	dtDynTextCh* DTCtmp = DTTC->states;

	if (state < 0) state += DTTC->stateT;

	DTTC->state = state;

	//arrivo allo stato corrente
	int Ctmp;
	for (Ctmp = DTTC->state; Ctmp>0; Ctmp--)
	{
		DTCtmp = DTCtmp->next;

		if (DTCtmp->next == NULL)//se gli stati sono finiti
		{
			//vado al primo stato
			DTCtmp = DTTC->states;
			DTTC->state = 0;
		}
	}

	sfText_setString(DTTC->sText.text, DTCtmp->ctxt);
	shCenterText(DTTC->sText.text);

	return DTCtmp;
}