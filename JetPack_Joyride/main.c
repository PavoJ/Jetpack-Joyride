#include<SFML\Graphics.h>
#include<SFML\window.h>

int main()
{
	sfRenderWindow *win; //dichiarazione finestra

	sfVideoMode vMode; // size finestra
	vMode.width = 1920;
	vMode.height = 1080;

	win = sfRenderWindow_create(vMode, "hey", sfClose, NULL);
	// creazione finestra con dati imessi precedentemente

	while (sfWindow_isOpen(win))//controlla se la finestra è aperta
	{
		sfEvent event;
		while (sfRenderWindow_pollEvent(win, &event))
		{//controla se ci sono eventi
			switch (event.type)//controlla il tipo di evento
			{
			case sfEvtClosed: // tipo di vento
				sfRenderWindow_close(win); //ad es. chiusura finestra
				break;
			}
		}

		sfRenderWindow_clear(win, sfColor_fromRGB(255, 255, 255));
		// sostituisce il contenuto della finestra col colore stabilito

		sfRenderWindow_display(win);
		//mostra la finestra modificata
	}
}