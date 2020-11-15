// hero implementation, practice for cs300 frupal project
// November 13, 2020
// Camilo Schaser-Hughes

#include <ncurses.h> // dunno if i need this here
#include <string.h>
#include <fstream>
#include "item.h"
#include "hero.h"

using namespace std; // do we NEED this here, probably not...

hero::hero(WINDOW * vp, WINDOW * gm) {

  xpos = 25;
  ypos = 25; // arbitrary starting position
  whiffles = 1000;
  energy = 100;
  binoculars = false;

  gamemenu = gm;
  viewport = vp;
  
  for(int i = 0; i < KSIZE; ++i)
    for(int j = 0; j < KSIZE; ++j)
    {
      map[i][j].square = MEADOW;
      map[i][j].viewed = false; // afterwards will fix this to false
      map[i][j].feature = NULL;
    }

  start_color(); // dunno if this'll work, taken from Huidong
	init_pair(1, COLOR_BLACK, COLOR_GREEN); // MEADOW enum = 1
  init_pair(2, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(3, COLOR_BLACK, COLOR_BLUE);
  init_pair(4, COLOR_BLACK, COLOR_WHITE);
  init_pair(5, COLOR_BLACK, COLOR_BLACK);
	init_pair(9, COLOR_YELLOW, COLOR_RED);
}

hero::hero(WINDOW * vp, WINDOW * gm, WINDOW * standard, char * filename) {

  gamemenu = gm;
  viewport = vp;
  stdwindow = standard;

  for(int i = 0; i < KSIZE; ++i)
    for(int j = 0; j < KSIZE; ++j)
    {
      map[i][j].square = MEADOW;
      map[i][j].viewed = false; // afterwards will fix this to false
      map[i][j].feature = NULL;
    }

  load_file(filename);
    
  start_color(); // dunno if this'll work, taken from Huidong
	init_pair(1, COLOR_BLACK, COLOR_GREEN); // MEADOW enum = 1
  init_pair(2, COLOR_BLACK, COLOR_CYAN);
  init_pair(3, COLOR_BLACK, COLOR_BLUE);
  init_pair(4, COLOR_BLACK, COLOR_WHITE);
  init_pair(5, COLOR_BLACK, COLOR_BLACK);
	init_pair(9, COLOR_YELLOW, COLOR_RED);
}

void hero::load_file(char * filename) {
  
  int i, j, temp;
  char title[100];
  int temp_en, temp_co;

  ifstream inFile;
  inFile.open(filename);

// get's hero's information
  inFile.ignore(100, '\n');
  inFile >> ypos;
  inFile.get();
  inFile >> xpos;
  inFile.get();
  inFile >> whiffles;
  inFile.get();
  inFile >> energy;
  inFile.ignore(100, '\n');

  inFile.ignore(100, '\n');
  do {
    inFile >> temp;
    inFile.get();
    inFile >> i;
    inFile.get();
    inFile >> j;
    inFile.ignore(100, '\n');

    if(temp != 999)
      map[i][j].square = static_cast<terrain>(temp);
  } while(temp != 999);

  do {
    inFile >> temp;
    inFile.get();
    inFile >> i;
    inFile.get();
    inFile >> j;
    if(temp == 2) { // its a 2 for binoculars in enum
      // TODO FIGURE THIS THING OUT
      map[i][j].feature = new Binoculars();
      inFile.ignore(100, '\n');
    }
    else if(temp == 1) { // it's a 1 for food in THIS enum
      inFile.get();
      inFile >> temp_co;
      inFile.get();
      inFile >> temp_en;
      inFile.get();
      inFile.get(title, 100, ';');
      map[i][j].feature = new Food(temp_co, temp_en, title);
      inFile.ignore(100, '\n');
    }
  } while(temp != 999);

  inFile.close();
}

void hero::update_display() {
  refresh();
  display_viewport();
  display_gamemenu();

  if(map[ypos][xpos].feature) {
    obj_type temp = map[ypos][xpos].feature->return_type();
    map[ypos][xpos].feature->display_info(gamemenu);
    wrefresh(gamemenu);

    switch(temp) {
      case(BINOCULARS):
        binoculars = true;
        delete map[ypos][xpos].feature;
        map[ypos][xpos].feature = NULL;
        break;
      case(UNKNOWN):
        break;
      case(FOOD):
        int ch;
        Food * ptr;
        ptr = dynamic_cast<Food*>(map[ypos][xpos].feature);
        nodelay(stdwindow, false);
        if((ch = getch()) == 'y') {
          energy += ptr->get_energy();
          whiffles -= ptr->get_cost();
          delete map[ypos][xpos].feature;
          map[ypos][xpos].feature = NULL;
          wrefresh(gamemenu);
        }
        nodelay(stdwindow, true);
        break;
      case(TREASURE):
        break;
    }
//    map[ypos][xpos].feature->implement();
  }
}

void hero::display_viewport() {
  int leftx, topy;
  scroll_view(leftx, topy);
  look_around();
  display_frupal(leftx, topy);
  display_hero(leftx, topy);
  // XXX THIS CODE IS FOR TESTING THE SCROLLING POSITION
  mvwprintw(viewport, 0, 0, "leftx: %d topyy: %d", leftx, topy);
  // END THE TESTING CODE
  wrefresh(viewport);
}

void hero::scroll_view(int& leftx, int& topy) {

  int centerx, rightx, centery, bottomy, rows, cols;

  // THIS IS THE START OF A COMPLICATED SCROLLING FUNCTION
  // WILL BE MATCHED BY AN EQUALLY COMPLICATED HERO PLACEMENT FUNCTION
  getmaxyx(viewport, rows, cols);
  centery = rows / 2;
  centerx = cols / 2;

  leftx = xpos - centerx;
  rightx = xpos + centerx;

  if(cols >= KSIZE)
    leftx = 0;
  else if(leftx < 0)
  {
    leftx = 0;
  }
  else if(rightx > KSIZE) {
    leftx = KSIZE - cols;
  }

  topy = ypos - centery;
  bottomy = ypos + centery;

  if(rows >= KSIZE)
    topy = 0;
  else if(topy < 0)
  {
    topy = 0;
  }
  else if(bottomy > KSIZE) {
    topy = KSIZE - rows;
  }
  // END OF SCROLLING MECHANISM
}

void hero::look_around() {
  int i, j;
  int sight = 1;
  sight += binoculars;

  for(i = ypos - sight; i <= ypos + sight; ++i)
    for(j = xpos - sight; j <= xpos + sight; ++j) {
      if(i >= 0 && j >= 0 && i < KSIZE && j < KSIZE)
        map[i][j].viewed = true;
    }
}

void hero::display_frupal(int leftx, int topy) { 

  int i, j, color, rows, cols;

  getmaxyx(viewport, rows, cols);
  
  // added in the extra check for KSIZE to fix a bug, was looking at i = 128
  for(i = topy; i < topy + rows && i < KSIZE; ++i)
    for(j = leftx; j < leftx + cols && j < KSIZE; ++j)
    {
      // this converts the enum type to an int match to color pair
      if(map[i][j].viewed)
        color = map[i][j].square;
      else
        color = UNSEEN;
      wattron(viewport, COLOR_PAIR(color));
      if(map[i][j].feature)
        mvwaddch(viewport, i - topy, j - leftx, map[i][j].feature->return_char());
      else
      {
        mvwaddch(viewport, i - topy, j - leftx, ' ');
      }
      wattroff(viewport, COLOR_PAIR(color));
    }
}

void hero::display_hero(int leftx, int topy) {

  int yloc, xloc;
  yloc = ypos - topy;
  xloc = xpos - leftx;
  
  wattron(viewport, COLOR_PAIR(9));
  mvwaddch(viewport, yloc, xloc, '@');
  wattroff(viewport, COLOR_PAIR(9));
}

void hero::display_gamemenu() {
  int width, height;
  getmaxyx(gamemenu, height, width);
  werase(gamemenu);
  wborder(gamemenu, '#', 0, ' ', ' ', '#', 0, '#', 0);
  mvwprintw(gamemenu, 1, (width - strlen("Menu:")) / 2, "Menu:");
  // XXX THIS PORTION IS FOR TESTING!!! TAKE OUT WHEN DONE!!!
  mvwprintw(gamemenu, height - 5, 2, "x-position: %d", xpos);
  mvwprintw(gamemenu, height - 4, 2, "y-position: %d", ypos);
  // THIS IS THE END OF THE TESTING PORTION!!!
  mvwprintw(gamemenu, height - 3, 2, "Whiffles: %d", whiffles);
  mvwprintw(gamemenu, height - 2, 2, "Energy: %d", energy);

  wrefresh(gamemenu);
}

void hero::move_up() {
  --ypos;
  if(!energy_cost())
  {
    ++ypos;
    --energy;
  }
  update_display();
}

void hero::move_down() {
  ++ypos;
  if(!energy_cost())
  {
    --ypos;
    --energy;
  }
  update_display();
}

void hero::move_left() {
  --xpos;
  if(!energy_cost())
  {
    ++xpos;
    --energy;
  }
  update_display();
}

void hero::move_right() {
  ++xpos;
  if(!energy_cost())
  {
    --xpos;
    --energy;
  }
  update_display();
}

bool hero::energy_cost() {

  if (xpos < 0 || xpos >= KSIZE || ypos < 0 || ypos >= KSIZE)
    return false;
  else if(map[ypos][xpos].square == MEADOW)
    --energy; 
  else if(map[ypos][xpos].square == SWAMP)
    energy -= 2;
  else if(map[ypos][xpos].square == WATER || map[ypos][xpos].square == WALL)
    return false;

  return true;
}


