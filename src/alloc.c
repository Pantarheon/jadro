/*
	Jadro - alloc.c

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

#include <stdlib.h>
#include <string.h>
#include "../include/jadro.h"

#ifdef __cplusplus
extern "C" {
#endif

JADRODC JADRO_jadro * JADRO_newjadro(void) {
	return calloc(1, sizeof(JADRO_jadro));
}

JADRODC JADRO_bod * JADRO_newbod(JADRO_uzol *uzol) {
	JADRO_bod *bod = calloc(1, sizeof(JADRO_bod));
	if (bod) bod->uzol = uzol;
	return bod;
}

JADRODC JADRO_uzol * JADRO_newuzol(char *label) {
	JADRO_uzol *uzol = (JADRO_uzol *)calloc(1, sizeof(JADRO_uzol));
	if ((uzol != NULL) && (label != NULL)) {
		uzol->label = strdup(label);
		if (uzol->label == NULL) {	// Out of memory, most likely
			free(uzol);
			return NULL;
		}
	}
	return uzol;
}

JADRODC JADRO_kern * JADRO_newkern(void) {
	return calloc(1, sizeof(JADRO_kern));
}

JADRODC JADRO_kerns * JADRO_newkerns(void) {
	return calloc(1, sizeof(JADRO_kerns));
}

JADRODC JADRO_kerns * JADRO_freekerns(JADRO_kerns *k) {
	JADRO_kerns *temp;
	JADRO_kerns *kerns = k;

	while (kerns) {
		if (kerns->first != NULL) kerns->first = JADRO_freekern(kerns->first);
		temp = kerns->next;
		free(kerns);
		kerns = temp;
	}
	return NULL;
}

// We only free ->next but not ->uzol
// because we only free the uzols when freeing the jadro.
JADRODC JADRO_kern * JADRO_freekern(JADRO_kern *k) {
	JADRO_kern *temp;
	JADRO_kern *kern = k;

	while (kern) {
		temp = kern->next;
		free(kern);
		kern = temp;
	}
	return NULL;
}

JADRODC JADRO_uzol * JADRO_freeuzol(JADRO_uzol *u) {
	JADRO_uzol *temp;
	JADRO_uzol *uzol = u;

	while (uzol) {
		if (uzol->label != NULL) {
			free (uzol->label);
			uzol->label = NULL;
		}

		if (uzol->first.kern != NULL) {
			uzol->first.bod = (uzol->set) ?
				JADRO_freebod (uzol->first.bod) :
				(JADRO_bod *)JADRO_freekern(uzol->first.kern);
//			if (uzol->set == 0)
//				uzol->first.kern = JADRO_freekern(uzol->first.kern);
//			else uzol->first.bod = JADRO_freebod (uzol->first.bod);
		}

		temp = uzol->next;
		free (uzol);
		uzol = temp;
	}
	return NULL;
}

JADRODC JADRO_bod *JADRO_freebod(JADRO_bod *bod) {
	JADRO_bod *temp;
	JADRO_bod *b = bod;

	while (b) {
		temp = b->next;
		free(b);
		b = temp;
	}
	return NULL;
}

JADRODC JADRO_jadro * JADRO_freejadro(JADRO_jadro *jadro) {
	unsigned int i;

	if (jadro) {
		if (jadro->kerns != NULL) jadro->kerns = JADRO_freekerns(jadro->kerns);

		for (i = 0; i < JADRO_INITIALCHARS; i++) jadro->uzly[i] = JADRO_freeuzol(jadro->uzly[i]);
		free(jadro);
	}
	return NULL;
}

#ifdef __cplusplus
}
#endif
