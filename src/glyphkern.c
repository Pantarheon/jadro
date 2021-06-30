/*
	Jadro - kern.c

	Copyright 2021 G. Adam Stanislav
	All rights reserved
*/

#define	JADROLIB

#include "../include/jadro.h"

#ifdef __cplusplus
extern "C" {
#endif

// Return the kern associated with a glyph and a periglyph.
// If the uzol does not exist yet, create it.
JADROHID JADRO_kern * JADRO_getkern(JADRO_uzol *glyph, JADRO_uzol *periglyph) {
	JADRO_kern *kern, *k, *last;
	int n;

	// This is only for glyphs, not sets.
	if ((glyph == NULL) || (glyph->set) || (periglyph == NULL)) return NULL;

	// If the glyph has no kerns, create one.
	if (glyph->first.kern == NULL) {
		if ((kern = JADRO_newkern()) != NULL) {
			kern->uzol = periglyph;
			glyph->first.kern = kern;
		}
	}
	// Else if the first kern is what we are looking for, return it.
	else if ((n = JADRO_strcmp(periglyph->label, glyph->first.kern->uzol->label)) == 0)
		kern = glyph->first.kern;
	// Else if the first kern follows periglyph lexicographically, insert.
	else if (n < 0) {
		if ((kern = JADRO_newkern()) != NULL) {
			kern->uzol = periglyph;
			kern->next = glyph->first.kern;
			glyph->first.kern = kern;
		}
	}
	else for (last = glyph->first.kern, k = last->next;; last = k, k = last->next) {
		if (k == NULL) {	// End of chain
			if ((kern = JADRO_newkern()) != NULL)
				kern->uzol = periglyph;
			last->next = kern;
			break;
		}
		else if ((n = JADRO_strcmp(periglyph->label, k->uzol->label)) == 0) {
			kern = k;
			break;
		}
		else if (n < 0) {
			if ((kern = JADRO_newkern()) != NULL) {
				kern->uzol = periglyph;
				kern->next = k;
				last->next = kern;
			}
			break;
		}
	}

	return kern;
}

#ifdef __cplusplus
}
#endif
