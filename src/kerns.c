/*
	Jadro - kerns.c

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

// Create a next link in the chain of kerns and associate
// it with an uzol. Mark the uzol (or the entire set thereof)
// as a leading glyph.
JADRODC  JADRO_kerns * JADRO_nextkerns(JADRO_jadro *jadro, JADRO_uzol *uzol) {
	JADRO_kerns *kerns, *k;
	JADRO_bod   *bod;

	if ((jadro != NULL) && (uzol != NULL) && ((kerns = JADRO_newkerns()) != NULL)) {
		kerns->uzol = uzol;
		uzol->glyph = 1;

		if (uzol->set)
			for (bod = uzol->first.bod; bod; bod = bod->next)
				bod->uzol->glyph = 1;

		if (jadro->kerns == NULL) {
			jadro->kerns = kerns;
		}
		else for (k = jadro->kerns; ;k = k->next) {
			if (k->next == NULL) {
				k->next = kerns;
				break;
			}
		}
		return kerns;
	}
	else return NULL;
}

JADRODC  JADRO_kern * JADRO_nextkern(JADRO_kerns *kerns, int rozdiel, JADRO_uzol *uzol) {
	JADRO_kern *kern, *k;
	JADRO_bod  *bod;

	if ((kerns != NULL) && (uzol != NULL) && ((kern = JADRO_newkern()) != NULL)) {
		kern->rozdiel = rozdiel;
		kern->uzol    = uzol;
		uzol->peri    = 1;

		if (uzol->set)
			for (bod = uzol->first.bod; bod; bod = bod->next)
				bod->uzol->peri = 1;

		if (kerns->first == NULL) kerns->first = kern;
		else for (k = kerns->first; ;k = k->next) {
			if (k->next == NULL) {
				k->next = kern;
				break;
			}
		}
		return kern;
	}
	else return NULL;
}


#ifdef __cplusplus
}
#endif
