#ifndef CSFML_FRAMEWORK_H
#define CSFML_FRAMEWORK_H

#include<SFML\Graphics.h>

#include<stdarg.h>
#include<stdlib.h>

//testo dinamico

typedef struct
{
	char* ctxt;
	int cnt;
	dtDynTextCh* next;
} dtDynTextCh;

typedef struct
{
	sfText* text;
	int state;
	dtDynTextCh* states;
} dtDynText;

//Componenti di una scena

union shSceneT//tipi di scena
{
	dtDynText* dynText;
	sfText* text;
	sfRectangleShape* rect;
	sfSprite* sprite;
};

enum shSceneE//enumeratori dei tipi di scena
{
	dynText = 0,
	text,
	rect,
	sprite
};

typedef struct
{
	enum shSceneE type;
	union shSceneT T;
	scene* next;
} scene;

#endif