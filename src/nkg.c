/*
	Jadro - nkg.c

	Copyright 2021 G. Adam Stanislav
	All rights reserved
*/

#define	JADROLIB

#include "../include/jadro.h"

#ifdef __cplusplus
extern "C" {
#endif

JADRODC JADRO_uzol * JADRO_nextkernedglyph(JADRO_jadro *jadro) {
	JADRO_uzol *uzol;

	for (; jadro->uzol; jadro->uzol = jadro->uzol->next) {
		if ((jadro->uzol->set == 0) && (jadro->uzol->glyph != 0)) {
			uzol = jadro->uzol;
			jadro->uzol = jadro->uzol->next;
			return uzol;
		}
	}

	for (jadro->abeceda++; jadro->abeceda < JADRO_INITIALCHARS; jadro->abeceda++) {
		for (jadro->uzol = jadro->uzly[jadro->abeceda]; jadro->uzol; jadro->uzol = jadro->uzol->next) {
			if ((jadro->uzol->set == 0) && (jadro->uzol->glyph != 0)) {
				uzol = jadro->uzol;
				jadro->uzol = jadro->uzol->next;
				return uzol;
			}
		}
	}

	return NULL;
}

#ifdef __cplusplus
}
#endif
