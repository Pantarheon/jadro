#	Copyright 2021 G. Adam Stanislav
#	All rights reserved
#
#	Redistribution and use in source and binary forms,
#	with or without modification, are permitted provided
#	that the following conditions are met:
#
#	1. Redistributions of source code must retain the
#	above copyright notice, this list of conditions
#	and the following disclaimer.
#
#	2. Redistributions in binary form must reproduce the
#	above copyright notice, this list of conditions and
#	the following disclaimer in the documentation and/or
#	other materials provided with the distribution.
#
#	3. Neither the name of the copyright holder nor the
#	names of its contributors may be used to endorse or
#	promote products derived from this software without
#	specific prior written permission.
#
#	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS
#	AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
#	WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
#	SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
#	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#	STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

CC=gcc
CFLG=-O3
CFLAGS=$(CFLG) -fPIC
PREFIX=/usr

objects = addtoset.o alloc.o char.o consts.o fkg.o glentry.o \
	glyphkern.o kerns.o nkg.o set.o strcmp.o uzol.o 

lib:	libjadro.so

build-all: install jadro

rebuild: clean build-all

jadro: jadro.o ../include/jadro.h lib
	$(CC) $(CFLG) -o jadro jadro.o -L$(PREFIX)/lib -ljadro

%.o: %.c ../include/jadro.h
	$(CC) -c $(CFLAGS) $< -o $@

libjadro.so: $(objects)
	$(CC) --shared -o libjadro.so $(objects)

install: lib
	install -p -s libjadro.so $(PREFIX)/lib
	cp -p ../include/jadro.h $(PREFIX)/include
	chown root:root $(PREFIX)/include/jadro.h

install-jadro: jadro
	install -p -s jadro $(PREFIX)/bin

install-all: install install-jadro

clean:
	rm -f jadro.o $(objects) jadro libjadro.so

