/*
	DynText è un oggetto di testo dinamico
	è un espansione al concetto di testo di CSFML sfText, in quanto non contiene solo una stringa,
	ma più stringhe che ciclano se una condizione è avverata (Per gli scopi di questo programma lo usiamo principalmente
		se il giocatore clicca il testo).
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

dtDynTextCh* dtDynTextChAppend(dtDynTextCh* DT, const char* str)
{
	while (DT->next != NULL)
		DT = DT->next;

	DT->next = malloc(sizeof(dtDynTextCh));

	if (DT->next != NULL) DT->next->next = NULL;

	if (DT != NULL)
	{
		DT->ctxt = malloc((strlen(str)+1)*sizeof(char));//+1 a strlen perchè contiamo anche il carattere '\n'
		
		char* STRtmp = DT->ctxt;
		
		if (STRtmp != NULL) strcpy_s(STRtmp, (strlen(str)+1) * sizeof(char), str);
		else printf("Errore nell'allocazione di memoria per la stringa %s", str);
	}
	else {
		printf("c'e' stato un errore nell'append di un elemento di testo DynTextCh");
	}

	return DT;
}

void shCenterText(sfText* t);

void dtDynTextUpdate(dtDynText* DTTC)
{
	dtDynTextCh* DTCtmp = DTTC->states;

	//arrivo allo stato corrente
	int Ctmp;
	for (Ctmp = DTTC->state; Ctmp != 0; Ctmp--)
		DTCtmp = DTCtmp->next;

	//aumento lo stato di 1
	DTTC->state++;
	if (DTCtmp->next == NULL)//se gli stati sono finiti
	{
		//vado al primo stato
		DTCtmp = DTTC->states;
		DTTC->state = 0;
	}

	sfText_setString(DTTC->sText.text, DTCtmp->ctxt);
	shCenterText(DTTC->sText.text);
}