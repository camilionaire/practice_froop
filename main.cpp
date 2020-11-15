// trying for the main implementation.  see if we can do it... err.
// Group 7, CS300 frupal project
// Camilo Schaser-Hughes
// November 13, 2020
// largely stolen from my previous prototype.


#include <ncurses.h>
#include <string.h> // don't know if need both of these 2.
#include <cstring>
#include <fstream>
#include "hero.h"
#include "item.h"

using namespace std; // tossed in for ifstream

int main(int argc, char ** argv) // don't need argv necessarily
{
  WINDOW *viewport, *in_game;
  int vpheight, vpwidth, igheight, igwidth = 35;


  hero * my_hero;
  char * inputfile;
  int ch; // holds the 'char'

  initscr(); // CAN ADD IN A PART HERE FOR TERMINAL SIZE CHECK
  cbreak();   // disables line buffering
  keypad(stdscr, TRUE); // up, down, etc.  dunno if screen matters
  noecho();
  nodelay(stdscr, TRUE); // make it not wait for getch()
  curs_set(0);
  // this sets up the windows height and width
  vpheight = LINES;
  igheight = LINES;
  vpwidth = COLS - igwidth;

  // added in as a 'cheat' to not fix a bug
  if(vpwidth > KSIZE) {
    vpwidth = KSIZE;
    igwidth = COLS - vpwidth;
  }

  viewport = newwin(vpheight, vpwidth, 0, 0);
  in_game = newwin(igheight, igwidth, 0, vpwidth + 1);
  wborder(in_game, '#', 0, ' ', ' ', '#', 0, '#', 0); // should put just on left side

  // this is the file open system
  if(argc != 2)
    my_hero = new hero(viewport, in_game);
  else {
    inputfile = argv[1];
    ifstream inFile;
    inFile.open(inputfile);
    if(! inFile)
      my_hero = new hero(viewport, in_game);
    else {
      inFile.close();
      my_hero = new hero(viewport, in_game, stdscr, inputfile);
    }
  }



  my_hero->update_display();

  while ((ch = getch()) != 'q' && ch != 'Q')
  {
    switch (ch)
    {
      case KEY_LEFT: // move left
        my_hero->move_left();
        break;
      case KEY_RIGHT:
        my_hero->move_right();
        break;
      case KEY_UP:
        my_hero->move_up();
        break;
      case KEY_DOWN:
        my_hero->move_down();
        break;
    }
  }

  endwin();
  delete my_hero;

  return 0;
}
