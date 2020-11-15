

#include "item.h"
#include "hero.h" // i don't need this here
#include <string.h>
#include <ncurses.h>


item::item(): type(UNKNOWN), name(0), print_char('*') {}

item::item(obj_type type_set, char * name_set, char a_char) {

  name = new char[strlen(name_set) + 1];
  strcpy(name, name_set);

  type = type_set;
  print_char = a_char;
}

item::~item() {
  if(name)
    delete [] name;
}

void item::display_info(WINDOW * window) const {

}

void item::implement() {

}

char item::return_char() const {
  return print_char;
}

obj_type item:: return_type() const {
  return type;
}

Binoculars::Binoculars(): item() {

  type = BINOCULARS;
  name = new char[strlen("Binoculars of Vision!") + 1];
  strcpy(name, "Binoculars of Vision!");
  print_char = 'B';
}

void Binoculars::display_info(WINDOW * window) const {
  int rows, cols;
  item::display_info(window); // just in case need it.
  getmaxyx(window, rows, cols);

  mvwprintw(window, rows / 2 - 1, (cols - strlen("You found the:")) / 2, "You found the:");
  mvwprintw(window, rows / 2, (cols - strlen(name)) / 2, name);
}

void Binoculars::implement() {
  item::implement(); // what used for? maybe nothing

}

Food::Food(): item(), cost(0), energy(0) {

}
Food::Food(int a_cost, int some_energy, char * a_name):
        item(FOOD, a_name, 'F'), cost(a_cost), energy(some_energy) {

}

void Food::display_info(WINDOW * window) const {

  int rows, cols;
  item::display_info(window); // just in case need it.
  getmaxyx(window, rows, cols);

  mvwprintw(window, rows / 2 - 2, (cols - strlen("You found a:")) / 2, "You found a:");
  mvwprintw(window, rows / 2 - 1, (cols - strlen(name)) / 2, name);
  mvwprintw(window, rows / 2, (cols - strlen("It costs: XX whiffles.")) / 2, "It costs %d whiffles.", cost);
  mvwprintw(window, rows / 2 + 1, (cols - strlen("It gives XX energy.")) / 2, "It gives %d energy.", energy);
  mvwprintw(window, rows / 2 + 2, (cols - strlen("Press (Y) to buy!")) / 2, "Press (Y) to buy!");

}

void Food::implement() {

}

int Food::get_cost() {
  return cost;
}

int Food::get_energy() {
  return energy;
}


