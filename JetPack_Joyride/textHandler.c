#include<SFML\Window.h>
#include<SFML\Graphics.h>

typedef struct {
	sfText* text;//il testo, font, grandezza e posizione nella finestra
	
	char** states;//i vari stati che può assumere il testo
	int stateC;//il contatore degli stati
} dynText;

dynText* txtHandler_createText()
