#ifndef TEXTHANDLER_H
#define TEXTHANDLER_H 1

#include"textHandler.c"

dynText txtHandler_createText(int charSize, sfVector2f pos, int statec, const char** sts);

//cambia stato del testo (da chiamare quando viene cliccato)
void txtHandler_click(dynText txt);

#endif