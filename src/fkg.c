/*
	Jadro - fkg.c

	Copyright 2021 G. Adam Stanislav
	All rights reserved
*/

#define	JADROLIB

#include "../include/jadro.h"

#ifdef __cplusplus
extern "C" {
#endif

JADRODC JADRO_uzol * JADRO_firstkernedglyph(JADRO_jadro *jadro) {
	jadro->abeceda = 0;
	jadro->uzol    = jadro->uzly[0];
	return JADRO_nextkernedglyph(jadro);
}

#ifdef __cplusplus
}
#endif
