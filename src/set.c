/*
	Jadro - set.c

	Copyright 2021 G. Adam Stanislav
	All rights reserved

	Redistribution and use in source and binary forms,
	with or without modification, are permitted provided
	that the following conditions are met:

	1. Redistributions of source code must retain the
	above copyright notice, this list of conditions
	and the following disclaimer.

	2. Redistributions in binary form must reproduce the
	above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or
	other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the
	names of its contributors may be used to endorse or
	promote products derived from this software without
	specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS
	AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
	WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
	STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#define	JADROLIB

#include "../include/jadro.h"

#ifdef __cplusplus
extern "C" {
#endif

// Create the set uzol associated with a label.
// If the uzol exist already, return NULL.
//
// This is because the jadro language does not
// allow the redefinition of a set.
JADRODC JADRO_uzol * JADRO_createsetuzol(JADRO_jadro *jadro, char *label, JADRO_errors *err) {
	JADRO_uzol *uzol, *last;
	ptrdiff_t idx;
	int n;

	if (err) *err = JADRO_NOERR;

	if ((jadro == NULL) || (label == NULL) ||
	((idx = JADRO_initialchar(*label)) < 0)) {
		if (err) *err = JADRO_INVAL;
		return NULL;
	}
	else if (jadro->uzly[idx] == NULL) {
		if ((uzol = JADRO_newuzol(label)) != NULL)
			uzol->set = 1;
		else if (err) *err = JADRO_EOMEM;
		jadro->uzly[idx] = uzol;
		return uzol;
	}
	else if ((n = JADRO_strcmp(jadro->uzly[idx]->label, label)) == 0) {
		if (err) *err = JADRO_DUPLI;
		return NULL;
	}
	else if (n > 0) {
		if ((uzol = JADRO_newuzol(label)) != NULL) {
			uzol->set = 1;
			uzol->next = jadro->uzly[idx];
			jadro->uzly[idx] = uzol;
		}
		else if (err) *err = JADRO_EOMEM;
		return uzol;
	}
	else for(last = jadro->uzly[idx], uzol = jadro->uzly[idx];; last = uzol, uzol = uzol->next) {
		if ((n = JADRO_strcmp(uzol->label, label)) == 0) {
			if (err) *err = JADRO_DUPLI;
			return NULL;
		}
		else if (n > 0) break;
		else if (uzol->next == NULL) {
			uzol->next = JADRO_newuzol(label);
			if (uzol->next != NULL)
				uzol->next->set = 1;
			else if (err) *err = JADRO_EOMEM;
			return uzol->next;
		}
	}
	if ((uzol = JADRO_newuzol(label)) != NULL) {
		uzol->set = 1;
		uzol->next = last->next;
		last->next = uzol;
	}
	else if (err) *err = JADRO_EOMEM;
	return uzol;
}

JADRODC bool JADRO_isglyphinset(JADRO_uzol *glyph, JADRO_uzol *set) {
	JADRO_bod *bod;
	if ((glyph != NULL) && (glyph->set == 0)) {
		if (glyph == set)	// technically, a glyph is a set of one
			return true;
		else if ((set != NULL) && (set->set != 0))
			for (bod = set->first.bod; bod; bod = bod->next)
				if (glyph == bod->uzol)
					return true;
	}
	return false;
}

JADRODC JADRO_uzol * JADRO_addlabeltoset(JADRO_jadro *jadro, char *label, JADRO_uzol *set, JADRO_errors *err) {
	return JADRO_adduzoltoset(JADRO_getuzol(jadro, label, err), set);
}

#ifdef __cplusplus
}
#endif
