/*
	Jadro - glentry.c

	Copyright 2021 G. Adam Stanislav
	All rights reserved
*/

#define	JADROLIB

#include "../include/jadro.h"

#ifdef __cplusplus
extern "C" {
#endif

JADRODC JADRO_uzol * JADRO_glyphentry(JADRO_jadro *jadro, JADRO_uzol *glyph) {
	if ((jadro != NULL)  && (glyph != NULL) && (glyph->set == 0) && (glyph->glyph != 0)) {
		JADRO_kerns *kerns;
		JADRO_kern  *kern;
		JADRO_kern  *k;
		JADRO_bod   *bod;

		if (glyph->first.kern) glyph->first.kern = JADRO_freekern(glyph->first.kern);

		for (kerns = jadro->kerns; kerns; kerns = kerns->next) {
			if (JADRO_isglyphinset(glyph, kerns->uzol)) {
				for (kern = kerns->first; kern; kern = kern->next) {
					if (kern->uzol->set == 0) {
						k = JADRO_getkern(glyph, kern->uzol);
						if (k == NULL) return NULL;
						else {
							k->uzol    = kern->uzol;
							k->rozdiel = kern->rozdiel;
						}
					}
					else for (bod = kern->uzol->first.bod; bod; bod = bod->next) {
						k = JADRO_getkern(glyph, bod->uzol);
						if (k == NULL) return NULL;
						else {
							k->uzol    = bod->uzol;
							k->rozdiel = kern->rozdiel;
						}
					}
				}
			}
		}
		return glyph;
	}
	return NULL;
}

JADRODC JADRO_uzol * JADRO_freeglyphentry(JADRO_uzol *glyph) {
	if ((glyph == NULL) && (glyph->set)) return NULL;
	else glyph->first.kern = JADRO_freekern(glyph->first.kern);
	return glyph;
}

#ifdef __cplusplus
}
#endif
