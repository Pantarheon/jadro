/*
	Jadro - jadro.h

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

#ifndef	JADRO_INC
#define	JADRO_INC

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#if defined _WIN32
# define	JADROHID
#ifdef	JADROLIB
# define	JADRODC	__declspec(dllexport)
#else	// !JADROLIB
# define	JADRODC	__declspec(dllimport)
#endif	// JADROLIB
#elif defined __APPLE__ || defined linux || defined __FreeBSD__
#  define JADRODC __attribute__((visibility("default")))
# define	JADROHID __attribute__((visibility("hidden")))
#else	// !_WIN32 !__APPLE__ !linux !__FreeBSD__
# define	JADRODC
# define	JADROHID
#endif	// _WIN32

#ifdef __cplusplus
extern "C" {
#endif

#define	JADRO_INITIALCHARS	(26*2+1)

typedef enum {
	JADRO_NOERR,
	JADRO_EOMEM,
	JADRO_DUPLI,
	JADRO_INVAL
} JADRO_errors;

typedef struct JADRO_UZOL JADRUZOL;

typedef struct JADRO_BOD {
	JADRUZOL         *uzol;
	struct JADRO_BOD *next;
} JADRO_bod;

typedef struct JADRO_KERN {
	JADRUZOL          *uzol;
	struct JADRO_KERN *next;
	signed int rozdiel;
} JADRO_kern;

typedef struct JADRO_KERNS {
	JADRUZOL           *uzol;	// The (leading or peri)glyph or set thereof
	struct JADRO_KERNS *next;
	struct JADRO_KERN  *first;	// First (peri)glyph or set thereof
	unsigned int peri:1;
} JADRO_kerns;

typedef struct JADRO_UZOL {
	char *label;
	struct JADRO_UZOL *next;
	union {
		JADRO_kern    *kern;	// Used by glyphs
		JADRO_bod     *bod;		// Used by sets
	} first;
	unsigned int set:1;			// 0 = glyph, 1 = set
	unsigned int glyph:1;		// 1 = leading glyph or set thereof
	unsigned int peri:1;		// 1 = periglyph or set thereof
} JADRO_uzol;

typedef struct JADRO {
	JADRO_uzol  *uzly[JADRO_INITIALCHARS];
	JADRO_kerns *kerns;
	// The rest of the structure is private
	// and can change in different versions
	// of jadro, so do not access it
	// from applications.
	unsigned int abeceda;
	JADRO_uzol *uzol;
} JADRO_jadro;

JADRODC JADRO_jadro * JADRO_newjadro(void);
JADRODC JADRO_jadro * JADRO_freejadro(JADRO_jadro *jadro);
JADRODC JADRO_uzol  * JADRO_newuzol(char *label);
JADRODC JADRO_uzol  * JADRO_freeuzol(JADRO_uzol *uzol);
JADRODC JADRO_kern  * JADRO_newkern(void);
JADRODC JADRO_kern  * JADRO_freekern(JADRO_kern *kern);
JADRODC JADRO_kerns * JADRO_newkerns(void);
JADRODC JADRO_kerns * JADRO_freekerns(JADRO_kerns *kerns);
JADRODC JADRO_bod   * JADRO_newbod(JADRO_uzol *uzol);
JADRODC JADRO_bod   * JADRO_freebod(JADRO_bod *bod);

// If the character is valid as the first character
// of a label, it returns a non-negative number.
// Otherwise, it returns -1.
JADRODC ptrdiff_t JADRO_initialchar(char c);

// If the character is valid in a label, even if not
// as the first character, this returns a non-negative
// number. Otherwise, it returns -1.
JADRODC ptrdiff_t JADRO_char(char c);

// If a character is a digit [0-9], it returns its
// numeric value. Otherwise it returns -1.
JADRODC ptrdiff_t JADRO_digitchar(char c);

// Similar to strcmp, but expects only valid characters,
// which it compares lexicographically and returns 0
// if the strings are equal (even if they are NULL),
// a negative number if the first string is lexicographically
// before the second (or if the first is NULL but the
// second is not), and a positive number otherwise.
JADRODC int JADRO_strcmp(char const * const ch1, char const * const ch2);

// If an uzol with this label exists, return it.
// Otherwise, create it and return it.
JADRODC JADRO_uzol  * JADRO_getuzol(JADRO_jadro *jadro, char *label, JADRO_errors *err);

// Create an uzol with this label, mark it as a set, and return it.
// If such an uzol exists already, that is a syntax error, so
// return NULL instead.
JADRODC JADRO_uzol  * JADRO_createsetuzol(JADRO_jadro *jadro, char *label, JADRO_errors *err);

// Add a glyph or a set of glyphs to a set.
JADRODC JADRO_uzol * JADRO_addlabeltoset(JADRO_jadro *jadro, char *label, JADRO_uzol *set, JADRO_errors *err);

// Check if a glyph uzol is a member of a set.
JADRODC bool JADRO_isglyphinset(JADRO_uzol *glyph, JADRO_uzol *set);

// Create a next link in the chain of kerns and associate
// it with an uzol. Mark the uzol (or the entire set thereof)
// as a leading glyph.
JADRODC  JADRO_kerns * JADRO_nextkerns(JADRO_jadro *jadro, JADRO_uzol *uzol);
JADRODC  JADRO_kern * JADRO_nextkern(JADRO_kerns *kerns, int rozdiel, JADRO_uzol *uzol);

// Find the lexicographically first/next leading glyph.
JADRODC JADRO_uzol * JADRO_firstkernedglyph(JADRO_jadro *jadro);
JADRODC JADRO_uzol * JADRO_nextkernedglyph(JADRO_jadro *jadro);

// Create the data for a dictionary entry of a leading glyph.
JADRODC JADRO_uzol * JADRO_glyphentry(JADRO_jadro *jadro, JADRO_uzol *glyph);
JADRODC JADRO_uzol * JADRO_freeglyphentry(JADRO_uzol *glyph);

/*** Some useful text constants ***/
JADRODC extern const char JADRO_shebang[];	// = "#!/usr/bin/jadro\n";
JADRODC extern const char JADRO_colon[];	// = ":%s {\n";
JADRODC extern const char JADRO_setmember[];	// = "\t%s\n";
JADRODC extern const char JADRO_rightcurly[];	// = "}\n\n";
JADRODC extern const char JADRO_tilde[];	// = "~%s [\n";
JADRODC extern const char JADRO_circumflex[];	// = "^%s <\n";
JADRODC extern const char JADRO_rozdiel[];	// = "%10i  %s\n";
JADRODC extern const char JADRO_odlisnost[];	// = "\t%s\t%8i\n";
JADRODC extern const char JADRO_rightsquare[];	// = "]\n\n";
JADRODC extern const char JADRO_rightangle[];	// = ">\n\n";

JADRODC extern const char JADRO_plist[];	// = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
//	"<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
//	"<plist version=\"1.0\">\n";
JADRODC extern const char JADRO_slashplist[];	// = "</plist>\n";
JADRODC extern const char JADRO_dict[];	// = "<dict>\n";
JADRODC extern const char JADRO_slashdicts[];	// = "</dict>\n";
JADRODC extern const char JADRO_slashdict[];	// =  "\t</dict>\n";
JADRODC extern const char JADRO_keydict[];	//	"\t<key>%s</key>\n\t<dict>\n";
JADRODC extern const char JADRO_keyinteger[];	// = "\t\t<key>%s</key><integer>%i</integer>\n";

/*** Common jadro compiler errors and warnings ***/

JADRODC extern const char JADRO_TopError[];	// = "JADRO Error [%u:%u]: Expected ':', '~', or '^', not '%s'.\n";
JADRODC extern const char JADRO_SetExpected[];	// = "JADRO Error [%u:%u]: Expected a set name.\n";
JADRODC extern const char JADRO_SetFail[];	// = "JADRO Error [%u:%u]: Could not create set '%s'.\n";
JADRODC extern const char JADRO_SetRedefinition[];	// = "JADRO Error [%u:%u]: Set '%s' redefinition.\n";
JADRODC extern const char JADRO_AddToSetFail[];	// = "JADRO Error [%u:%u]: Could not add '%s' to set\n";
JADRODC extern const char JADRO_GlyphSetExpected[];	// = "JADRO Error [%u:%u]: Expected a glyph or set name.\n";
JADRODC extern const char JADRO_PeriglyphSetExpected[];	// = "JADRO Error [%u:%u]: Expected a periglyph or set name.\n";
JADRODC extern const char JADRO_GlyphCreateError[];	// = "JADRO Error [%u:%u]: Could not create glyph '%s'\n";
JADRODC extern const char JADRO_OutOfMemoryError[];	// = "JADRO Error [%u:%u]: Out of memory.\n";
JADRODC extern const char JADRO_SyntaxError[];	// = "JADRO Error [%u:%u]: Invalid label syntax '%s'.\n";
JADRODC extern const char JADRO_LeftCurlyExpected[];	// = "JADRO Error [%u:%u]: Expected '{', not '%s'.\n";
JADRODC extern const char JADRO_LeftSquareExpected[];	// = "JADRO Error [%u:%u]: Expected '[', not '%s'.\n";
JADRODC extern const char JADRO_LeftAngleExpected[];	// = "JADRO Error [%u:%u]: Expected '<', not '%s'.\n";
JADRODC extern const char JADRO_NumberExpected[];	// = "Jadro Error [%u:%u]: Expected a number, not '%s'.\n";
JADRODC extern const char JADRO_LabelBracketExpected[];	// = "JADRO Error [%u:%u]: Expected a label or '}', not '%s'.\n";
JADRODC extern const char JADRO_NumberBracketExpected[];	// = "JADRO Error [%u:%u]: Expected a number or ']', not '%s'.\n";
JADRODC extern const char JADRO_LabelAngleExpected[];	// = "JADRO Error [%u:%u]: Expected a label or '>', not '%s'.\n";
JADRODC extern const char JADRO_PeriglyphFail[];	// = "JADRO Error [%u:%u]: Could not create periglyph '%s'.\n";
JADRODC extern const char JADRO_EndOfFile[];	// = "JADRO Error: [%u:%u]: Unexpected end of file.\n";

JADRODC extern const char JADRO_DelayedSetDefinition[];	// = "Jadro Warning [%u:%u]: A set definition after kerning pair(s) defined.\n";
JADRODC extern const char JADRO_RightSquareWarning[];	// = "Jadro Warning [%u:%u]: Pretending the ']' is a '%c'.\n";
JADRODC extern const char JADRO_RightCurlyWarning[];	// = "Jadro Warning [%u:%u]: Pretending the '}' is a '%c'.\n";
JADRODC extern const char JADRO_RightAngleWarning[];	// = "Jadro Warning [%u:%u]: Pretending the '>' is a '%c'.\n";

/*** The rest is used internally by libjadro ***/

// Add an uzol or set of uzly to a set.
JADROHID JADRO_uzol * JADRO_adduzoltoset(JADRO_uzol *uzol, JADRO_uzol *set);

// Return the kern associated with a glyph and a periglyph.
// If the uzol does not exist yet, create it.
JADROHID JADRO_kern * JADRO_getkern(JADRO_uzol *glyph, JADRO_uzol *periglyph);

#ifdef __cplusplus
}
#endif

#endif	// JADRO_INC (not)defined
