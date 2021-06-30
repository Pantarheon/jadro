/*
	Jadro - char.c

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

#include <stddef.h>
#include "../include/jadro.h"

#define	JADRO_CHARS	(JADRO_INITIALCHARS+10+1)

#ifdef __cplusplus
extern "C" {
#endif

static const char JADRO_validchars[JADRO_CHARS] =
"."
"_"
"AaBbCcDdEeFfGgHhIiJjKkLlMm"
"NnOoPpQqRrSsTtUuVvWwXxYyZz"
"0123456789";

// If we have a legal character, return its offset
// in the string of valid characters. Otherwise, return -1.

JADRODC ptrdiff_t JADRO_char(char c) {
	ptrdiff_t pos;

	for (pos = JADRO_CHARS - 1; pos > -1; pos--)
		if (JADRO_validchars[pos] == c) break;
	return pos;
}

JADRODC ptrdiff_t JADRO_initialchar(char c) {
	ptrdiff_t pos;

	for (pos = JADRO_INITIALCHARS - 1; pos > -1; pos--)
		if (JADRO_validchars[pos+1] == c) break;
	return pos;
}

JADRODC ptrdiff_t JADRO_digitchar(char c) {
	ptrdiff_t pos;

	for (pos =  9; pos > -1; pos--)
		if (JADRO_validchars[pos+1+JADRO_INITIALCHARS] == c) break;
	return pos;
}


#ifdef __cplusplus
}
#endif
