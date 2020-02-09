#include<SFML\Window.h>
#include<SFML\Graphics.h>

#include<stdlib.h>
#include<stdbool.h>
#include<stdarg.h>

typedef struct {
	char** inStates;//i vari stati che può assumere la struttura
	int stateCounter;//il contatore degli stati
} inText;//interactive text

typedef struct {
	sfText* text;//il testo, font, grandezza e posizione nella finestra
	inText states;//i vari stati che può assumere il testo
	dynText* next;//prossimo elemento della lista
} dynText;

void txtHandler_center(sfText* txt)
{
	sfFloatRect rect = sfText_getLocalBounds(txt);

	sfText_setOrigin(txt, (sfVector2f) { rect.width / 2, rect.height / 2 });
}

// charSize è la dimensione del testo
// pos la posizione
// statec corrisponde agli stati della struttura di stringhe
// cnt è il contenuto della struttura
void txtHandler_appendText(dynText* txt, int charSize, sfVector2f pos, int statec, char** cnt)
{
	dynText* tmp = txt;
	sfFont* defaultFont = sfFont_createFromFile("defaultFont.ttf");

	//salto all'ultimo elemento della lista
	while (tmp != NULL)
		tmp = tmp->next;
	
	tmp = malloc(sizeof(dynText));
	if (tmp == NULL)
		printf("Errore: allocazione di text non riuscita");
	else
	{
		tmp->next = NULL;
		   
		tmp->text = sfText_create();
		tmp->states.stateCounter = 0;
		   
		tmp->states.inStates = txt;
		tmp->states.stateCounter = 0;

		sfText_setString(tmp->text, tmp->states.inStates[0][0]);
		sfText_setColor(tmp->text, sfBlack);
		sfText_setFont(tmp->text, defaultFont);
		sfText_setCharacterSize(tmp->text, charSize);

		txtHandler_center(tmp->text);
		sfText_setPosition(tmp->text, pos);
	}
}

void txtHandler_click(dynText dtxt)
{
	sfText_setString(dtxt.text, dtxt.states.inStates[dtxt.states.stateCounter]);
	txtHandler_center(dtxt.text);
}
