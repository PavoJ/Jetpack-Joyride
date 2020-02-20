#ifndef CONSTS_H
#define CONSTS_H 1

#include<stdbool.h>

#define tick 35 //millisecondi

#define winWidth 960
#define winHeight 540

/* valori esterni dipendenti dalla finestra di gioco */
#define coordMaxY (winHeight-(float)winHeight/25.f)
#define coordMinY ((float)winHeight/25.f)

#define playerWidth 60.f
#define playerHeight 90.f

typedef struct
{
	sfVector2f position;//vettore di 2 float, della SFML. L'ho cambiato per una compabilità migliore
	float velocity;
	bool oldMoveUp;
} player;

#endif //CONSTS_H