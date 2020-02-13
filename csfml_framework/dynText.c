#include"csfml_framework.h"

void dtDynTextChAppend(dtDynTextCh* DTTC, const char* str)
{
	dtDynTextCh* DTTCtmp = DTTC;
	while (DTTCtmp != NULL)
		DTTCtmp = DTTCtmp->next;

	DTTCtmp = malloc(sizeof(dtDynTextCh));

	char* STRtmp = DTTCtmp->ctxt;
	STRtmp = malloc(sizeof(str));
	STRtmp = str;
}

void dtDynTextUpdate(dtDynText* DTTC)
{
	dtDynTextCh* DTCtmp = DTTC->states;

	int a;
	int Ctmp = DTTC->states->cnt;
	for (Ctmp += 1; Ctmp != 0; Ctmp--)
		DTCtmp = DTCtmp->next;

	if (DTCtmp == NULL)
		DTCtmp = DTTC;
		
	sfText_setString(DTTC->text, DTCtmp->ctxt);
}