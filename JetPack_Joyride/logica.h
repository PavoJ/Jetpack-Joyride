#ifndef LOGICA_H
#define LOGICA_H 1

#include"logica.c"

/* moveUp � true se il tasto su � premuto */
void move (bool moveUp);
/* move:
	mette la posizione y nella struct del personaggio in base al suo movimento
	( velocit� esponenziale )
*/


player* getP();

#endif