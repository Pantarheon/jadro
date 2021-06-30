/*
	Jadro - consts.c

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

JADRODC const char JADRO_shebang[] = "#!/usr/bin/jadro\n";
JADRODC const char JADRO_colon[] = ":%s {\n";
JADRODC const char JADRO_setmember[] = "\t%s\n";
JADRODC const char JADRO_rightcurly[] = "}\n\n";
JADRODC const char JADRO_tilde[] = "~%s [\n";
JADRODC const char JADRO_rozdiel[] = "%10i  %s\n";
JADRODC const char JADRO_rightsquare[] = "]\n\n";

JADRODC const char JADRO_plist[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	"<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
	"<plist version=\"1.0\">\n";
JADRODC const char JADRO_slashplist[] = "</plist>\n";
JADRODC const char JADRO_dict[] = "<dict>\n";
JADRODC const char JADRO_slashdicts[] = "</dict>\n";
JADRODC const char JADRO_slashdict[] =  "\t</dict>\n";
JADRODC const char JADRO_keydict[] = "\t<key>%s</key>\n\t<dict>\n";
JADRODC const char JADRO_keyinteger[] = "\t\t<key>%s</key><integer>%i</integer>\n";

/*** Common jadro compiler errors and warnings ***/

JADRODC const char JADRO_TopError[] = "JADRO Error [%u:%u]: Expected ':' or '~', not '%s'.\n";
JADRODC const char JADRO_SetExpected[] = "JADRO Error [%u:%u]: Expected a set name.\n";
JADRODC const char JADRO_SetFail[] = "JADRO Error [%u:%u]: Could not create set '%s'.\n";
JADRODC const char JADRO_SetRedefinition[] = "JADRO Error [%u:%u]: Set '%s' redefinition.\n";
JADRODC const char JADRO_AddToSetFail[] = "JADRO Error [%u:%u]: Could not add '%s' to set\n";
JADRODC const char JADRO_GlyphSetExpected[] = "JADRO Error [%u:%u]: Expected a glyph or set name.\n";
JADRODC const char JADRO_GlyphCreateError[] = "JADRO Error [%u:%u]: Could not create glyph '%s'.\n";
JADRODC const char JADRO_OutOfMemoryError[] = "JADRO Error [%u:%u]: Out of memory.\n";
JADRODC const char JADRO_SyntaxError[] = "JADRO Error [%u:%u]: Invalid label syntax '%s'.\n";
JADRODC const char JADRO_LeftCurlyExpected[] = "JADRO Error [%u:%u]: Expected '{', not '%s'.\n";
JADRODC const char JADRO_LeftSquareExpected[] = "JADRO Error [%u:%u]: Expected '[', not '%s'.\n";
JADRODC const char JADRO_NumberExpected[] = "JADRO Error [%u:%u]: Expected a number, not a '%s'.\n";
JADRODC const char JADRO_LabelBracketExpected[] = "JADRO Error [%u:%u]: Expected a label or '}', not '%s'.\n";
JADRODC const char JADRO_NumberBracketExpected[] = "JADRO Error [%u:%u]: Expected a number or ']', not '%s'.\n";
JADRODC const char JADRO_PeriglyphFail[] = "JADRO Error [%u:%u]: Could not create periglyph '%s'.\n";
JADRODC const char JADRO_EndOfFile[] = "JADRO Error: [%u:%u]: Unexpected end of file.\n";

JADRODC const char JADRO_DelayedSetDefinition[] = "Jadro Warning [%u:%u]: A set definition after kerning pair(s) defined.\n";
JADRODC const char JADRO_RightSquareWarning[] = "Jadro Warning [%u:%u]: Pretending the ']' is a '}'.\n";
JADRODC const char JADRO_RightCurlyWarning[] = "Jadro Warning [%u:%u]: Pretending the '}' is a ']'.\n";

#ifdef __cplusplus
}
#endif
