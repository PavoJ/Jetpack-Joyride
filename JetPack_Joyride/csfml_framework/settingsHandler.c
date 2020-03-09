#ifndef SETTINGSHANDLER_C
#define SETTINGSHANDLER_C 1

#include<SFML\Graphics.h>
#include<stdbool.h>
#include<stdio.h>
#include<windows.h>

#define settingsDir ".\\Settings.txt"

typedef struct
{
	bool sounds;
	sfVideoMode vMode;
} sthSettings;

void sthWriteSettings(sthSettings*);

sthSettings* sthReadSettings()
{
	sthSettings* Sret;

	FILE* set;

	int sounds;

	Sret = malloc(sizeof(sthSettings));
	if (Sret != NULL)
	{
		errno_t err = fopen_s(&set, settingsDir, "r");

		switch (err)
		{
		case 0://nessun errore
			if (fscanf_s(set, "%i\n%lux%lu", &sounds, &Sret->vMode.width, &Sret->vMode.height) == 3)
			{
				Sret->sounds = sounds;
			}
			else//se non ci sono impostazioni impostate dall'utente
			{
				//attiva il suono
				Sret->sounds = false;

				//scegli la risoluzione migliore possibile
				Sret->vMode = sfVideoMode_getFullscreenModes(NULL)[0];

				sthWriteSettings(Sret);
			}
			fclose(set);
			break;
			
		case ENOENT://se il file non esiste
			//attiva il suono
			Sret->sounds = false;

			//scegli la risoluzione migliore possibile
			Sret->vMode = sfVideoMode_getFullscreenModes(NULL)[0];

			sthWriteSettings(Sret);
			break;

		default://altri errori
			fprintf_s(stderr, "Errore nella lettura del file delle impostazioni\n");
			break;
		}
	}

	return Sret;
}

sthSettings* sthGetSettings()
{
	static sthSettings* st;
	static bool setup;

	if (!setup)
	{
		st = sthReadSettings();
		setup = true;
	}

	return st;
}

void sthWriteSettings(sthSettings* st)
{
	FILE* set;
	errno_t err = fopen_s(&set, settingsDir, "w");

	if(err == 0 && set != NULL)
	{
		fprintf_s(set, "%i\n%lux%lu", st->sounds, st->vMode.width, st->vMode.height);
		fclose(set);
	}
	else {
		fprintf_s(stderr, "Errore nella scrittura del file delle impostazioni\n");
	}
}

#endif //SETTINGSHANDLER_C