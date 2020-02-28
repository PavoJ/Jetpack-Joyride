#include"consts.h"
#include<SFML\Graphics.h>
#include<stdbool.h>
#include<stdio.h>

#define settingsDir ".\\settings.txt"

typedef struct
{
	bool sounds;
	sfVideoMode vMode;
} settings;

settings readSettings()
{
	settings Sret;
	int soundsT;

	FILE* set;

	fopen_s(&set, settingsDir, "r");
	if (set != NULL)
	{
		if (fscanf_s(set, "%i\n%lux%lu", &soundsT, &Sret.vMode.width, &Sret.vMode.height) == 3)
		{
			Sret.sounds = soundsT;
		}
		else
		{
			Sret.sounds = false;
			Sret.vMode.width = winWidth;
			Sret.vMode.height = winHeight;
		}
	}
}

settings* getSettings()
{
	static settings St;
	static bool setup;

	if (!setup)
	{
		St = readSettings();
		setup = true;
	}
}