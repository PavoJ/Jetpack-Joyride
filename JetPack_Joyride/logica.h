#ifndef LOGICA_H
#define LOGICA_H 1

#include"logica.c"

/* moveUp è true se il tasto su è premuto */
void move (bool moveUp);
/* move:
	mette la posizione y nella struct del personaggio in base al suo movimento
	( velocità esponenziale )
*/


player* getP();

#endif