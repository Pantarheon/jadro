/*
	Jadro - addtoset.c

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

JADROHID JADRO_uzol * JADRO_adduzoltoset(JADRO_uzol *uzol, JADRO_uzol *set) {
	if ((set == NULL) || (uzol == NULL) || (set->set == 0))
		// Nothing to add or to add to.
		return NULL;
	else if (uzol != set) {	// Because we do not want an endless recursion.
		if (uzol->set == 0) {
			// First of six possible scenarios: set is still empty.
			if (set->first.bod == NULL) {
				if ((set->first.bod = JADRO_newbod(uzol)) == NULL)
					// not enough memory.
					return NULL;
				set->first.bod->uzol = uzol;
			}
			// Second scenario: uzol is set->first.kern already.
			else if (uzol == set->first.bod->uzol) return uzol;
			// Third scenario: uzol lexicographicaly precedes set->first.bod->uzol.
			else if (JADRO_strcmp(uzol->label, set->first.bod->uzol->label) < 0) {
				JADRO_bod *bod = JADRO_newbod(uzol);
				if (bod == NULL) return NULL;	// Out of memory.
				bod->next = set->first.bod;
				set->first.bod = bod;
			}
			else {
				JADRO_bod *last, *bod;
				for (last = set->first.bod, bod = last->next;;last = bod, bod = bod->next) {
					// Fourth scenario: We have come to the end of chain
					if (bod == NULL) {
						if ((last->next = JADRO_newbod(uzol)) == NULL)
							// Out of memory;
							return NULL;
						break;
					}
					// Fifth scenario: The uzol is already in the chain
					else if (uzol == bod->uzol)
						break;
					// Sixth scenario: Bod is lexicographically after uzol.
					else if (JADRO_strcmp(uzol->label, bod->uzol->label) < 0) {
						JADRO_bod *temp = JADRO_newbod(uzol);
						if (temp == NULL) return NULL;
						temp->next = bod;
						last->next = temp;
						break;
					}
				}
			}
		}
		else if (uzol->first.kern != NULL) {	// No point adding an empty set.
			JADRO_bod *bod;
			for (bod = uzol->first.bod; bod; bod = bod->next)
				JADRO_adduzoltoset(bod->uzol, set);
		}
	}
	return uzol;
}

#ifdef __cplusplus
}
#endif
