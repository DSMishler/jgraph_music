# Daniel Mishler's JGraph lab

This lab takes music files in mjm format and generates images out of them.

To do so, simply observe the functionality of `make examples`. Pull. Go to
`Lab-1`. `make examples`. 'nuff said.

I suppose you could just as easily generate postscript.

## How does it work?

- your file in mjm format is converted to jgraph. Find them in the `mjms`
  directory.
    - This is done through `mjm_to_jgr.cpp` with the help of things in `symbols`
- the jgraph is converted into postscript
- postscript converted to pdf
- pdf converted to image. Find it in the `music` directory.

## MJM format (Mishler Jgraph Music format)

All files are read in the nascent `mjm` format. These files are composed
of a header, and then notes in the format of `<duration> <pitch>`.

`#comment`

`<title>`

`<author>`

`<clef>` (always treble)

`<key signature>`

`<time signature>`

`<notes>`


Notes take the following format:
`<time> <name>`


time:
- W: whole note
- H: half note
- Q: quarter note
- E: eight note

name:
- R is for rests
- Otherwise, `<name><degree>`
- examples:
    - C4 is the middle C
    - G4 is the note the clef encircles
    - F5 is the top of the staff.

### limitations
- add '#', 'b', or 'n' before a note to add the accidental. This has not been
  implemented but could and should be an extension
- only treble clef
- no ties
- no dotted rhythms
- no sixteenth notes and below. Only eighth, through whole (rests included).
- Very loosely typed and not very much error check. *it is probably very easy
  to make the output look goofy.*
- There is very little text processing and the mjm format is probably
  easy to break.
- Eighth notes don't connect. This could be a great extension.

## Notes
- Spent a good amount of time trying to get my rests and clefs looking okay.
  Would be open to feedback on it if there's something obvious I'm missing
- This has been a quite fun experience!

## Want to try something?
- Just make an mjm file and process it like you see the others processed!
