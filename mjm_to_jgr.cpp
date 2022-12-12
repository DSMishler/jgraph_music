#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

typedef struct note
{
   int length;   // amount of space (passed to jgraph) and consistent with
                 // time (so an eighth note is always half a quarter note)
   char letter;  // A, B, G, etc.
   int degree;   // 1, 5, 3. Usually 4 or 5.
   int position; // y-coordinate. height. position = name - 'A' + degree * 7
   string symbol;// name of file that the note's symbol is stored in.
} mjmnote_t;

void staff_lines_at(int height, int start, int len);
void bar_line_at(int pos, int base);
void key_signature_at(int pos, int base, int nsharps, int nflats);
void clef_at(int pos, int base, char clef);
void time_signature_at(int x, int y, int num, int denom);

// possible TODO: for cleanliness could get rid of need for this.
#define MAXLINE 80

// notes are defined as their position relative to A0. This brings them to the
// treble clef.
#define TREBLE_E4_OFFSET 32
#define EIGHTH_NOTE_LEN  6

int main(void)
{
   ///////////////////////////////// processing ////////////////////////////////
   string line, title, author;
   char cstring[MAXLINE];
   int sretval;



   char clef;
   int num_sharps;
   int num_flats;
   int ts_num;
   int ts_denom;
   vector <mjmnote_t> notes;

   // cout << "Mishler mjm to jgraph\n";
   // possibly read all comments in a way that indicates I've never used
   // C++ before
   do
   {
      getline(cin, line);
      sretval = sscanf(line.c_str(), " %s", cstring);
   } while (sretval != 1 || cstring[0] == '#');

   // get title
   title = line;
   // and author
   getline(cin, author);

   // get clef
   getline(cin, line);
   sretval = sscanf(line.c_str(), " %s", cstring);
   if(strcmp(cstring, "treble") == 0)
   {
      clef = 't';
   }
   else
   {
      cout << "error: cannot understand clef\n";
      return 2;
   }

   // get key signature
   getline(cin, line);
   sretval = sscanf(line.c_str(), " %d# %db", &num_sharps, &num_flats);



   // get time signature
   getline(cin, line);
   sretval = sscanf(line.c_str(), " %d/%d", &ts_num, &ts_denom);


   // now we are in notes territory
   int total_length = 0;
   while(getline(cin, line))
   {
      mjmnote_t note;
      char notelen[MAXLINE], notename[MAXLINE];
      sscanf(line.c_str(), " %s %s", notelen, notename);

      note.letter = notename[0];
      if (note.letter == 'R')
      {
         // send it to B5
         note.degree = 5;
         note.position = 'B' - 'A' + note.degree * 7;
      }
      else
      {
         note.degree = notename[1] - '0';
         note.position = note.letter - 'A' + note.degree * 7;
      }
      switch (notelen[0])
      {
      case 'E':
         note.length = EIGHTH_NOTE_LEN;
         note.symbol = "./symbols/note_eighth.ps";
         if (note.letter == 'R')
         {
             note.symbol = "./symbols/rest_eighth.ps";
         }
         break;
      case 'Q':
         note.length = 2 * EIGHTH_NOTE_LEN;
         note.symbol = "./symbols/note_quarter.ps";
         if (note.letter == 'R')
         {
             note.symbol = "./symbols/rest_quarter.ps";
         }
         break;
      case 'H':
         note.length = 4 * EIGHTH_NOTE_LEN;
         note.symbol = "./symbols/note_half.ps";
         if (note.letter == 'R')
         {
             note.symbol = "./symbols/rest_half.ps";
         }
         break;
      case 'W':
         note.length = 8 * EIGHTH_NOTE_LEN;
         note.symbol = "./symbols/note_whole.ps";
         if (note.letter == 'R')
         {
             note.symbol = "./symbols/rest_whole.ps";
         }
         break;
      default:
         cout << "error: unrecognized note name\n";
         break;
      }
      total_length += note.length;
      notes.push_back(note);
   }

   ///////////////////////////////// generating ////////////////////////////////

   // have a basis case of 4/4 time, adjust from there.
   int measure_length = 48;
   measure_length *= ts_num;
   measure_length /= ts_denom;

   int plot_start = -64;
   int head_space = 32;
   int line_length = ((200) / measure_length) * measure_length + head_space;
   int space_for_notes = ((line_length-head_space)/measure_length) * measure_length;
   int staff_spacing = 14;
   int num_staffs;

   num_staffs = (total_length + (space_for_notes-1)) / space_for_notes;


   
   int xmax, ymax, xmin, ymin;
   xmin = 0;
   ymin = 0;
   ymax = 35;
   xmax = 100;
   // for whatever reason it breaks ratios super bad if I don't have
   // x = [0,100] and y = [0,35]. Not investigating further. Just building
   // the treble clef. We're building off these ratios as a result of growing
   // pains. Maybe one day if I use this more this would be a great target
   // to optimize for.
   cout << "(* " << clef << " clef *)" << endl;
   cout << "(* " << num_sharps << " sharps *)" << endl;
   cout << "(* " << num_flats << " flats *)" << endl;
   cout << "(* " << ts_num << "/" << ts_denom << " time *)" << endl;
   cout << "newgraph" << endl;
   cout << "xaxis min " << xmin << " max " << xmax << " nodraw" << endl;
   cout << "yaxis min " << ymin << " max " << ymax << " nodraw" << endl;

   // titles and author
   // title: center aligned
   printf("newstring font Times-Roman fontsize %d x %d y %d hjc vjc :\n",
           14,
           plot_start + line_length/2,
           num_staffs * staff_spacing - 2);
   printf("%s\n", title.c_str());
   // author: right aligned
   printf("newstring font Courier fontsize %d x %d y %d hjr vjc :\n",
           12,
           plot_start + line_length,
           num_staffs * staff_spacing - 2);
   printf("%s\n", author.c_str());

   // first; put in the notes (if you do this last, you get them messing with
   // the staff lines!)
   int start_time = 0;
   int cur_time = start_time;
   int which_row, note_pos;
   int i, j;
   for(i = 0; (unsigned) i < notes.size(); i++)
   {
      which_row = (cur_time) / space_for_notes;
      // all notes naturally move to the middle of their alloted space.
      // (centered at self.length/2)
      note_pos = (cur_time % space_for_notes) + plot_start + head_space
                  + notes[i].length/2;
      cout << "newcurve eps " << notes[i].symbol;
      cout << " marksize 80 20 pts " << endl;
      cout << note_pos;
      cout << " ";
      cur_time += notes[i].length;
      cout << notes[i].position - TREBLE_E4_OFFSET +
              (num_staffs-which_row-1)*14;
      cout << endl;
   }

   // place time signature, key signature, clef, and bar lines for each
   // staff line.
   for(i = 0; i < num_staffs; i++)
   {
      key_signature_at(plot_start + 12, i*staff_spacing + 2,
                       num_sharps, num_flats);
      clef_at(plot_start + 6, i*staff_spacing + 2, clef);
      time_signature_at(plot_start + 28, i*staff_spacing+4, ts_num, ts_denom);
      staff_lines_at(i*staff_spacing, plot_start, line_length);
      for(j = plot_start + head_space + measure_length;
          j <= plot_start+line_length;
          j+= measure_length)
      {
         bar_line_at(j, i*staff_spacing);
      }
   }

   return 0;
}

void bar_line_at(int pos, int base)
{
   cout << "(* bar line *)" << endl;
   cout << "newline poly pfill 1 pts" << endl;
   cout << pos << " " << base << " " << pos << " " << base + 8 << endl;
}

void staff_lines_at(int height, int start, int len)
{
   cout << "(* staff *)" << endl;
   cout << "newline poly pfill 1 pts" << endl;
   cout << start << " " << 0 + height << " "
        << start + len << " " << 0 + height << endl;
   cout << "newline poly pfill 1 pts" << endl;
   cout << start << " " << 2 + height << " " <<
           start + len << " " << 2 + height << endl;
   cout << "newline poly pfill 1 pts" << endl;
   cout << start << " " << 4 + height << " " <<
           start + len << " " << 4 + height << endl;
   cout << "newline poly pfill 1 pts" << endl;
   cout << start << " " << 6 + height << " " <<
           start + len << " " << 6 + height << endl;
   cout << "newline poly pfill 1 pts" << endl;
   cout << start << " " << 8 + height << " " <<
           start + len << " " << 8 + height << endl;
}

// base is f on the top of staff. That way every accidental happens on the staff
// thus, pos is the position of g
void key_signature_at(int pos, int base, int nsharps, int nflats)
{
   // not error checking for one of nsharps or nflats being 0,
   // but for all intents and purposes at most one should be nonzero.
   if(nsharps > 0)
   {
      int sharp_yoffset = 6; // starting with F#
      int sharp_xoffset = 0;
      int i;
      cout << "newcurve eps symbols/sharp.ps marksize 8 5 pts" << endl;
      for(i = 0; i < nsharps; i++)
      {
         cout << pos + sharp_xoffset  << " " << base + sharp_yoffset << endl;
         sharp_xoffset += 2;
         sharp_yoffset = (sharp_yoffset+4) % 7; // circle of fifths
      }
   }
   if(nflats > 0)
   {
      int flat_yoffset = 2; // starting with Bb
      int flat_xoffset = 0;
      int i;
      cout << "newcurve eps symbols/flat.ps marksize 8 5 pts" << endl;
      for(i = 0; i < nflats; i++)
      {
         cout << pos + flat_xoffset  << " " << base + flat_yoffset << endl;
         flat_xoffset += 2;
         flat_yoffset = (flat_yoffset+3) % 7; // circle of fourths
      }
   }
}

// treble clef (G clef) is centered on G.
void clef_at(int pos, int base, char clef)
{
   if (clef == 't')
   {
      cout << "newcurve eps symbols/clef_treble.ps marksize 25 20 pts" << endl;
   }
   else
   {
      cout << "break jgraph" << endl;
   }
   cout << pos << " " << base << endl;
}

void time_signature_at(int x, int y, int num, int denom)
{
   
   printf("newstring font Times-Roman fontsize %d x %d y %d hjc vjc :\n",
           30, x, y+2);
   printf("%d\n", num);
   printf("newstring font Times-Roman fontsize %d x %d y %d hjc vjc :\n",
           30, x, y-2);
   printf("%d\n", denom);
}
