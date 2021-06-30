# JADRO

Jadro is a tool for the creation of a `kerning.plist` file,
which are used in [unified font objects](https://unifiedfontobject.org/versions/ufo3/kerning.plist/).

## Jadro Files

To use `jadro`, first create a file in the `jadro` language. An example
file named [faulty.jadro](./faulty.jadro) is included. The file contains
a few deliberate mistakes, but we can run it with the `jadro` compiler.

Under Windows we would type,

```
C:\some\path> jadro -j faulty.jadro > nul
```

Or under Unix and compatibles,

```
$ jadro -j faulty.jadro > /dev/null
```

Since the file is not perfect, we will see this,

```
Jadro Warning [312:1]: A set definition after kerning pair(s) defined.
Jadro Warning [343:1]: A set definition after kerning pair(s) defined.
Jadro Warning [353:1]: A set definition after kerning pair(s) defined.
Jadro Warning [386:1]: A set definition after kerning pair(s) defined.
```

No errors, just warnings. But to see the same file with no warnings,
we can use `jadro` itself to help us,

```
$ jadro -j faulty.jadro good.jadro
```

It is the `-j` switch that instructs the `jadro` compiler to send a
correct version of the input into the output.

If we do not use the `-j` switch, `jadro` will convert the input
to the `kerning.plist` format mentioned above,

```
$ jadro good.jadro kerning.plist
```

This time we will see no warnings. By the way, the `good.jadro` is
16,274 bytes long, while the `kerning.plist` is 479,976 bytes long.

Clearly, it is simpler to write our kerning values in the `jadro`
language than in the `kerning.plist` format and have `jadro` do
the hard work.

## The Jadro Language

The `jadro` language is very simple. For starters, it has no variables
or expressions, just constants. It also has no procedures, just
statements.

It allows you to produce kerning pairs of individual glyphs (a rather
tedious undertaking), or of sets of glyphs (a much simpler task), as
well as the combination of the two.

Any glyph or set thereof must have a name, which starts with either
an underline (`_`) or a letter of the Roman alphabet (as used by English).
It can then follow by any number of letters, numbers, underlines, and dots.

So, any of these are valid names (this just follows the rules set by Adobe
for glyph names):

```
_
A
a
OE
oe
f_f_i
a.sc
uni102F
```

And here are two invalid names:

```
.a
102F.uni
```

Whenever the `jadro` compiler encounters a valid name it has not seen
before, it assumes it is the name of a `glyph`. Glyph names need not be
defined before use. Indeed, the _cannot_ be defined before use.

A `set` of glyphs is defined and declared at the same time by preceding
its name by a colon (`:`), and following the name by a list of glyphs
and/or sets enclosed in brackets (`{}`) and separated by blank space.

Because everything is a constant, once the brackets close, the `set` is,
well, set. It cannot be changed. A set can contain other sets, as long as
they have been defined and declared earlier. There are no forward set
definitions in `jadro`, mostly to prevent set including a set that includes
the including set, which would produce an endless recursion.

Once it is defined and declared, the `set` consists of a union of all glyphs
included in it and in any of the set included. It is OK to include sets with
overlapping members, because a union ignores any duplicates.

For example, we could create these sets,

```
:CapitalOs {
	O
	Obreve
	OE
	Ocaron
	Oacute
	Ohorn
}

:MoreOs {
	O
	Ohungarumlaut
	Ocircumflex
	Omacron
	Otilde
}

:Os {
	O
	CapitalOs
	MoreOs
	Odieresis
}
```

As a result the `Os set` contains `O`, `Oacute`, `Obreve`, `Ocaron`,
`Ocircumflex`, `Odieresis`, `OE`, `Ohorn`, `Ohungarumlaut`, `Omacron`,
and `Otilde`. Please note that it does _not_ include the `sets` `CapitalOs`
and `MoreOs`, though it does include all the `glyphs` from those `sets`.

To create `kerning` pairs, we start with a tilde (`~`), follow it by
the name of a `glyph` or set of glyphs, then an array of values and
`periglyphs` or sets of periglyphs.

> Note that in `jadro` terminology, the first glyph of the kerned pair
> (which would be the left glyph in left-to-right writing systems, but
> the right glyph in the right-to-left writing systems) is just called
> a `glyph`. The second glyph of the pair, we call a `periglyph`.
>
> You will not find that word in the dictionary, as I made it up
> just to be able to distinguish between the two glyphs in the pair.
>
> In other words, regardless of the writing direction, in the kerned
> pair, a `glyph` is followed by a `periglyph`.

For example,

```
~a [
	-25 Os
	-17 Omacron
]

~Os [-23 a]
```

This tells `jadro` that whenever the glyph `a` is followed by any periglyph
from the `Os` set it should be rendered 25 font units closer than it would
be if not for kerning.

But what about `Omacron`? After all it is a member of the `Os` set. `Jadro`
has a simple rule: Any kerning pair that contradicts any `previous` pair
prevails. So, in `jadro` we have a list a of rules potentially followed
by a list of exceptions.

At any rate, if we feed `jadro` with the first example (of the various sets)
followed by the second (the `~a []` example), it will create this plist,

```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>a</key>
	<dict>
		<key>O</key><integer>-25</integer>
		<key>Oacute</key><integer>-25</integer>
		<key>Obreve</key><integer>-25</integer>
		<key>Ocaron</key><integer>-25</integer>
		<key>Ocircumflex</key><integer>-25</integer>
		<key>Odieresis</key><integer>-25</integer>
		<key>OE</key><integer>-25</integer>
		<key>Ohorn</key><integer>-25</integer>
		<key>Ohungarumlaut</key><integer>-25</integer>
		<key>Omacron</key><integer>-17</integer>
		<key>Otilde</key><integer>-25</integer>
	</dict>
	<key>O</key>
	<dict>
		<key>a</key><integer>-23</integer>
	</dict>
	<key>Oacute</key>
	<dict>
		<key>a</key><integer>-23</integer>
	</dict>
	<key>Obreve</key>
	<dict>
		<key>a</key><integer>-23</integer>
	</dict>
	<key>Ocaron</key>
	<dict>
		<key>a</key><integer>-23</integer>
	</dict>
	<key>Ocircumflex</key>
	<dict>
		<key>a</key><integer>-23</integer>
	</dict>
	<key>Odieresis</key>
	<dict>
		<key>a</key><integer>-23</integer>
	</dict>
	<key>OE</key>
	<dict>
		<key>a</key><integer>-23</integer>
	</dict>
	<key>Ohorn</key>
	<dict>
		<key>a</key><integer>-23</integer>
	</dict>
	<key>Ohungarumlaut</key>
	<dict>
		<key>a</key><integer>-23</integer>
	</dict>
	<key>Omacron</key>
	<dict>
		<key>a</key><integer>-23</integer>
	</dict>
	<key>Otilde</key>
	<dict>
		<key>a</key><integer>-23</integer>
	</dict>
</dict>
</plist>
```

## Comments and Wipes

The `octothorp` a.k.a. the `number sign` (`#`) starts a comment
which goes all the way to the end of the line. This is very common
in many scripting languages. The `jadro` compiler treats it as blank space.

Anything placed in regular parentheses (`()`) is treated as if it
was not even there. The parentheses can also be nested, so you can use
them to wipe large parts of a `.jadro` file.

To make that clear, take this example,

```
:SomeSet {
O# just the letter O
O(K, hahaha!
All this is (ignored).
Yes, really)circumflex
}
```

The above is identical to,

```
:SomeSet { O Ocircumflex }
```

## Getting Jadro

If you have 64-bit Windows, just dowload the contents of the
[win64](./win64) folder, or at least the `jadro.exe` and `jadro.dll`
files, which you need to place somewhere on the system included
in the `path` environment variable.

If you have Windows, just get everything from here, then type,

```
$ sudo make install-all
```
