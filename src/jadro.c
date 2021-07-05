/*
	Jadro - jadro.c

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

#ifdef	JADROLIB
#undef	JADROLIB
#endif

#include <stdio.h>
#include "../include/jadro.h"

#define	BUFFERBYTES	128
typedef enum {LABEL, NUMBER, COLON, TILDE, CIRCUM, MINUS, PLUS, LSQUARE, RSQUARE, LCURLY, RCURLY, LANGLE, RANGLE, JADERR, JEOF} parse;

int jeof(unsigned int line, unsigned int col) {
	fprintf(stderr, JADRO_EndOfFile, line, col);
	return JEOF;
}

int skiptoeol(FILE *input, unsigned int *line, unsigned int *col) {
	for (;;) {
		++*col;
		switch (fgetc(input)) {
			case EOF:
				return EOF;
				break;
			case '\t':
				*col = (*col+3)&~3;
				break;
			case '\n':
				++*line;
				*col = 0;
				return 0;
				break;
			case '\r':
				*col = 0;
				break;
		}
	}
}

int ignorechars(FILE *input, unsigned int *line, unsigned int *col) {
	unsigned int count = 1;

	while ((count) && (!feof(input))) {
		++*col;
		switch(fgetc(input)) {
			case '\n':
				++*line;
				*col = 0;
				break;
			case '\t':
				*col = (*col+3)&~3;
				break;
			case '\r':
				*col = 0;
				break;
			case '(':
				count++;
				break;
			case ')':
				count--;
				break;
			case '#':
				if (!skiptoeol(input, line, col))
					break;
				// else break through to case EOF.
			case EOF:
				return JEOF;
				break;
		}
	}
	return (feof(input)) ? EOF : 0;
}

int skipblanks(FILE *input, unsigned int *line, unsigned int *col) {
	int c, d;

	while (!feof(input)) {
		++*col;
		c = fgetc(input);
		if (c == '\n') {++*line;*col=0;}
		else if (c == '#') {
			*col = 0;
			while (!feof(input)) {
				if ((c = fgetc(input)) == '\n') {
					++*line;
					break;
				}
			}
		}
		else if (c == '\t') *col = (*col+3)&~3;
		else if (c == '\r') *col = 0;
		else if ((c == '(') && (d = ignorechars(input, line, col))) return d;
		else if (c > ' ') return c;
	}
	return c;
}

int gettoken(FILE *input, unsigned int *line, unsigned int *col, char *string, unsigned bytes, unsigned int *ln, unsigned int *cl) {
	int i;
	int c, d;

	c = skipblanks(input, line, col);
	*cl = *col;
	*ln = *line;
	*string = (char)c;
	string[1] = '\0';
	d = 0;

	switch (c) {
		case EOF:
			return EOF;
			break;
		case JEOF:
			return JEOF;
			break;
		case ':':
			return COLON;
			break;
		case '~':
			return TILDE;
			break;
		case '^':
			return CIRCUM;
			break;
		case '-':
			return MINUS;
			break;
		case '+':
			return PLUS;
			break;
		case '[':
			return LSQUARE;
			break;
		case ']':
			return RSQUARE;
			break;
		case '{':
			return LCURLY;
			break;
		case '}':
			return RCURLY;
			break;
		case '<':
			return LANGLE;
			break;
		case '>':
			return RANGLE;
			break;
/* Actually, this cannot happen since skipblanks
   handles it before returning here!
		case '(':
			if (ignorechars(input, line, col)) return JEOF;
			break;
*/
		case ')':
			return c;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			for (i = 1; i < bytes-1;) {
				if (JADRO_digitchar(c = fgetc(input)) >= 0) {
					string[i] = (char)c;
					++*col;
					i++;
				}
				else if (c == '(') {
					++*col;
					if (d = ignorechars(input, line, col))
						break;
				}
				else {
					ungetc(c, input);
					break;
				}
			}
			string[i] = '\0';
			return (d) ? d : NUMBER;
			break;
		default:
			for (i = 1; i < bytes-1;) {
				if (JADRO_char(c = fgetc(input)) >= 0) {
					string[i] = (char)c;
					++*col;
					i++;
				}
				else if (c == '(') {
					++*col;
					if (d = ignorechars(input, line, col))
						break;
				}
				else {
					ungetc(c, input);
					break;
				}
			}
			string[i] = '\0';
			return (d) ? d : (JADRO_initialchar(*string) < 0) ? JADERR : LABEL;
			break;
	}
}

void printkerns(JADRO_jadro *jadro, FILE *output) {
	JADRO_kerns *kerns;
	JADRO_kern  *kern;
	
	for (kerns = jadro->kerns; kerns; kerns = kerns->next) {
		int peri = (kerns->peri);
		fprintf(output, (peri) ? JADRO_circumflex : JADRO_tilde, kerns->uzol->label);
		for (kern = kerns->first; kern; kern = kern->next) {
			if (peri) fprintf(output, JADRO_odlisnost, kern->uzol->label, kern->rozdiel);
			else fprintf(output, JADRO_rozdiel, kern->rozdiel, kern->uzol->label);
		}
		fprintf(output,  (peri) ? JADRO_rightangle : JADRO_rightsquare);
	}
}

// Create preliminary kerning dictionary entry.

int circum (JADRO_jadro *jadro, FILE *input, unsigned int *line, unsigned int *col, char *string, unsigned bytes, unsigned int *ln, unsigned int *cl) {
	JADRO_uzol *glyph, *periglyph;
	JADRO_kerns *kerns;
	JADRO_kern  *kern;
	JADRO_errors err;
	int token = gettoken(input, line, col, string, bytes, ln, cl);
	int c, k;
	unsigned lin, co;

	if (token != LABEL) {
		fprintf(stderr, JADRO_PeriglyphSetExpected, *line, *col);
		return ((token == EOF) || (token == JEOF)) ? JEOF : JADERR;
	}

	periglyph = JADRO_getuzol(jadro, string, &err);

	if (periglyph == NULL) {
		switch (err) {
			case JADRO_NOERR:
				fprintf(stderr, JADRO_PeriglyphFail, *ln, *cl, string);
				break;
			case JADRO_EOMEM:
				fprintf(stderr, JADRO_OutOfMemoryError, *ln, *cl);
				break;
			case JADRO_INVAL:
				fprintf(stderr, JADRO_SyntaxError, *ln, *cl, string);
				break;
		}
		return JADERR;
	}
	else if (kerns = JADRO_nextkerns(jadro, periglyph)) {
		kerns->peri = 1;
		token = gettoken(input, line, col, string, bytes, ln, cl);
		if (token != LANGLE) {
			fprintf(stderr, JADRO_LeftAngleExpected, *line, *col, string);
			return ((token == EOF) || (token == JEOF)) ? JEOF : JADERR;
		}
		// Add kerning pairs to uzol.
		for (;;) {
			token = gettoken(input, line, col, string, bytes, ln, cl);
			if (token == RANGLE) {
				break;
			}
			else {
				if (token == LABEL) {
					glyph = JADRO_getuzol(jadro, string, &err);
					if (glyph == NULL) {
						switch (err) {
							case JADRO_NOERR:
								fprintf(stderr, JADRO_GlyphCreateError, *ln, *cl, string);
								break;
							case JADRO_EOMEM:
								fprintf(stderr, JADRO_OutOfMemoryError, *ln, *cl, string);
								break;
							case JADRO_INVAL:
								fprintf(stderr, JADRO_SyntaxError, string, *ln, *cl);
								break;
						}
						return JADERR;
					}
				}
				else if (token == RCURLY) {
					fprintf(stderr, JADRO_RightCurlyWarning, *line, *col, '>');
					break;
				}
				else if (token == RSQUARE) {
					fprintf(stderr, JADRO_RightSquareWarning, *line, *col, '>');
					break;
				}
				else {
					fprintf(stderr, JADRO_LabelAngleExpected, *line, *col, string);
					return ((token == EOF) || (token == JEOF)) ? JEOF : JADERR;
				}

				token = gettoken(input, line, col, string, bytes, ln, cl);

				if (token == MINUS) {
					lin = *line; co = *col;
					if ((c = gettoken(input, line, col, string+1, bytes-1, ln, cl)) != NUMBER) {
						fprintf(stderr, JADRO_NumberExpected, lin, co, string);
						return ((c == EOF) || (c == JEOF)) ? JEOF : JADERR;
					}
					else k = atoi(string);
				}
				else if (token == PLUS) {
					lin = *line; co = *col;
					if ((c = gettoken(input, line, col, string, bytes, ln, cl)) != NUMBER) {
						fprintf(stderr, JADRO_NumberExpected, lin, co, string);
						return ((c == EOF) || (c == JEOF)) ? JEOF : JADERR;
					}
					else k = atoi(string);
				}
				else if (token == NUMBER) k = atoi(string);
				else {
					fprintf(stderr, JADRO_NumberExpected, *ln, *col, string);
					return ((token == JEOF) || (token == EOF)) ? JEOF : JADERR;
				}
				kern = JADRO_nextkern(kerns, k, glyph);
			}
		}
	}

	return 0;
}

int tilde (JADRO_jadro *jadro, FILE *input, unsigned int *line, unsigned int *col, char *string, unsigned bytes, unsigned int *ln, unsigned int *cl) {
	JADRO_uzol *glyph, *periglyph;
	JADRO_kerns *kerns;
	JADRO_kern  *kern;
	JADRO_errors err;
	int token = gettoken(input, line, col, string, bytes, ln, cl);
	int c, k;
	unsigned lin, co;

	if (token != LABEL) {
		fprintf(stderr, JADRO_GlyphSetExpected, *line, *col);
		return ((token == EOF) || (token == JEOF)) ? JEOF : JADERR;
	}

	glyph = JADRO_getuzol(jadro, string, &err);

	if (glyph == NULL) {
		switch (err) {
			case JADRO_NOERR:
				fprintf(stderr, JADRO_GlyphCreateError, *ln, *cl, string);
				break;
			case JADRO_EOMEM:
				fprintf(stderr, JADRO_OutOfMemoryError, *ln, *cl);
				break;
			case JADRO_INVAL:
				fprintf(stderr, JADRO_SyntaxError, *ln, *cl, string);
				break;
		}
		return JADERR;
	}
	else if (kerns = JADRO_nextkerns(jadro, glyph)) {
		token = gettoken(input, line, col, string, bytes, ln, cl);
		if (token != LSQUARE) {
			fprintf(stderr, JADRO_LeftSquareExpected, *line, *col, string);
			return ((token == EOF) || (token == JEOF)) ? JEOF : JADERR;
		}
		// Add kerning pairs to uzol.
		for (;;) {
			token = gettoken(input, line, col, string, bytes, ln, cl);
			if (token == RSQUARE) {
				break;
			}
			else {
				if (token == MINUS) {
					lin = *line; co = *col;
					if ((c = gettoken(input, line, col, string+1, bytes-1, ln, cl)) != NUMBER) {
						fprintf(stderr, JADRO_NumberExpected, lin, co, string);
						return ((c == EOF) || (c == JEOF)) ? JEOF : JADERR;
					}
					else k = atoi(string);
				}
				else if (token == PLUS) {
					lin = *line; co = *col;
					if ((c = gettoken(input, line, col, string, bytes, ln, cl)) != NUMBER) {
						fprintf(stderr, JADRO_NumberExpected, lin, co, string);
						return ((c == EOF) || (c == JEOF)) ? JEOF : JADERR;
					}
					else k = atoi(string);
				}
				else if (token == NUMBER) k = atoi(string);
				else if (token == RCURLY) {
					fprintf(stderr, JADRO_RightCurlyWarning, *line, *col, ']');
					break;
				}
				else if (token == RANGLE) {
					fprintf(stderr, JADRO_RightAngleWarning, *line, *col, ']');
					break;
				}
				else {
					fprintf(stderr, JADRO_NumberBracketExpected, *line, *col, string);
					return ((token == EOF) || (token == JEOF)) ? JEOF : JADERR;
				}
				token = gettoken(input, line, col, string, bytes, ln, cl);
				if (token == LABEL) {
					periglyph = JADRO_getuzol(jadro, string, &err);
					if (periglyph == NULL) {
						switch (err) {
							case JADRO_NOERR:
								fprintf(stderr, JADRO_PeriglyphFail, *ln, *cl, string);
								break;
							case JADRO_EOMEM:
								fprintf(stderr, JADRO_OutOfMemoryError, *ln, *cl, string);
								break;
							case JADRO_INVAL:
								fprintf(stderr, JADRO_SyntaxError, string, *ln, *cl);
								break;
						}
						return JADERR;
					}
					else {
						kern = JADRO_nextkern(kerns, k, periglyph);
					}
				}
				else if  (token == JEOF) return JEOF;
			}
		}
	}

	return 0;
}

// Create a set. If it exists already, return error.
// Else fill it up.
int colon(JADRO_jadro *jadro, FILE *input, FILE *output, unsigned int *line, unsigned int *col, char *string, unsigned bytes, unsigned int *ln, unsigned int *cl) {
	JADRO_uzol * set, *uzol;
	JADRO_errors err;
	int token = gettoken(input, line, col, string, BUFFERBYTES, ln, cl);

	if (token != LABEL) {
		fprintf(stderr, JADRO_SetExpected, *line, *col, string);
		return ((token == EOF) || (token == JEOF)) ? JEOF : JADERR;
	}
	set = JADRO_createsetuzol(jadro, string, &err);
	if (set == NULL) {
		switch (err) {
			case JADRO_NOERR:
				fprintf(stderr, JADRO_SetFail, *ln, *cl, string);
				break;
			case JADRO_EOMEM:
				fprintf(stderr, JADRO_OutOfMemoryError, *ln, *cl, string);
				break;
			case JADRO_DUPLI:
				fprintf(stderr, JADRO_SetRedefinition, *ln, *cl, string);
				break;
			case JADRO_INVAL:
				fprintf(stderr, JADRO_SyntaxError, *ln, *cl, string);
				break;
		}
		return JADERR;
	}

	token = gettoken(input, line, col, string, BUFFERBYTES, ln, cl);
	if (token != LCURLY) {
		fprintf(stderr, JADRO_LeftCurlyExpected, *line, *col, string);
		return ((token == EOF) || (token == JEOF)) ? JEOF : JADERR;
	}
	else if (output) fprintf(output, JADRO_colon, set->label);
	// Add child uzly to set.
	for (;;) {
		token = gettoken(input, line, col, string, bytes, ln, cl);
		if (token == RCURLY) {
			break;
		}
		else if (token == RSQUARE) {
		fprintf(stderr, JADRO_RightSquareWarning, *line, *col, '}');
			break;
		}
		else if (token == RANGLE) {
		fprintf(stderr, JADRO_RightAngleWarning, *line, *col, '}');
			break;
		}
		else if (token == LABEL) {
			uzol = JADRO_addlabeltoset(jadro, string, set, &err);
			if (uzol == NULL) fprintf(stderr, JADRO_AddToSetFail, ln, cl, string);
			else if (output)
				fprintf(output, JADRO_setmember, uzol->label);
		}
		else {
			fprintf(stderr, JADRO_LabelBracketExpected, *line, *col, string);
			return ((token == EOF) || (token == JEOF)) ? JEOF : JADERR;
		}
	}
	if (output) fprintf(output, JADRO_rightcurly);
	return 0;
}

int parser(JADRO_jadro *jadro, FILE *input, FILE *output) {
	unsigned int line = 1;
	unsigned int col  = 0;
	unsigned int lastline = 1;
	unsigned int lastcol  = 0;
	unsigned int cl, ln;
	char string[BUFFERBYTES];
	int token = 0;
	unsigned int didtilde = 0;

	while ((token = gettoken(input, &line, &col, string, BUFFERBYTES, &ln, &cl)) != EOF) {
		// Only three tokens are valid here, COLON, TILDE, CIRCUM.
		// Anything else at this point is syntax error.
		switch (token) {
			case COLON:
				if (didtilde) {
					fprintf(stderr, JADRO_DelayedSetDefinition, line, col);
				}
				if (colon(jadro, input, output, &line, &col, string, BUFFERBYTES, &ln, &cl) == JEOF)
					return jeof(line, col);
				break;
			case TILDE:
				didtilde++;
				if (tilde(jadro, input, &line, &col, string, BUFFERBYTES, &ln, &cl) == JEOF)
					return jeof(line, col);
				break;
			case CIRCUM:
				didtilde++;
				if (circum(jadro, input, &line, &col, string, BUFFERBYTES, &ln, &cl) == JEOF)
					return jeof(line, col);
				break;
			case JEOF:
				return jeof(line, col);
				break;
			default:
				fprintf(stderr, JADRO_TopError, ln, cl, string);
				if (feof(input)) return jeof(line, col);
				break;
		}
	}

	return 0;
}

int main(int argc, char *argv[]) {
	JADRO_jadro *jadro;
	FILE *input  = stdin;
	FILE *output = stdout;
	char *inpt   = NULL;
	char *outpt  = NULL;
	JADRO_uzol *uzol, *glyph;
	JADRO_kern *kern;
	unsigned int i;
	int j = 0;

	for (i = 1; i < argc; i++) {
		if (
		(*argv[i] == '-')
#ifdef _WIN
			|| (*argv[i] == '/')
#endif
		)
		switch (argv[i][1]) {
			case 'j':
				j = 1;
				break;
			case 'i':
				if (argv[i][2]) inpt = &(argv[i][2]);
				else (inpt) = NULL;
				break;
			case 'o':
				if (argv[i][2]) outpt = &(argv[i][2]);
				else (outpt) = NULL;
				break;
			default:
				fprintf(stderr, "I do not understand '%s'.\n", argv[i]);
				break;
		}
		else if (inpt == NULL) inpt = argv[i];
		else if (outpt == NULL) outpt = argv[i];
		else fprintf(stderr, "I do not understand '%s'.\n", argv[i]);
	}

	if ((inpt) && ((input = fopen(inpt, "rt")) == NULL)) {
		fprintf(stderr, "Cannot open input file '%s'", inpt);
		return 1;
	}
	if ((outpt) && ((output = fopen(outpt, "wb"))) == NULL) {
		fprintf(stderr, "Cannot create output file '%s'", outpt);
		if (input != stdin) fclose(input);
		return 1;
	}

	jadro = JADRO_newjadro();

	if (j) fprintf(output, JADRO_shebang);
	else fprintf(output, "%s%s", JADRO_plist, JADRO_dict);
	if (parser(jadro, input, (j) ? output : NULL)) {
		JADRO_freejadro(jadro);
		if (input != stdin) fclose(input);
		if (output != stdout) fclose(stdout);
		return -1;
	}

	if (input != stdin) fclose(input);
	
	if (j) printkerns(jadro, output);
	else {
		for (uzol = JADRO_firstkernedglyph(jadro); uzol; uzol = JADRO_nextkernedglyph(jadro)) {
			glyph = JADRO_glyphentry(jadro, uzol);
			if (glyph) {
				fprintf(output, JADRO_keydict, glyph->label);
				for (kern=glyph->first.kern; kern; kern = kern->next) {
					fprintf(output, JADRO_keyinteger, kern->uzol->label, kern->rozdiel);
				}
				fprintf(output, JADRO_slashdict);
				JADRO_freeglyphentry(glyph);
			}
			else fprintf(stderr, "'%' failed\n", uzol->label);
		}
		fprintf(output, "%s%s", JADRO_slashdicts, JADRO_slashplist);
	}

	JADRO_freejadro(jadro);
	if (output != stdout) fclose(stdout);

	return 0;
}
