// sample grovnick tile and enum typeset.
// now named item.h - grovnick moved to hero.h
// Nov 13, 2020
// Camilo Schaser-Hughes

#include <ncurses.h>

enum obj_type {UNKNOWN, FOOD, BINOCULARS, TREASURE};

class item {
public:
  item();
  item(obj_type type_set, char * name_set, char a_char);

  virtual ~item();

  virtual void display_info(WINDOW * window) const;
  virtual void implement();
  char return_char() const;
  obj_type return_type() const;
  
protected:
  enum obj_type type;
  char * name;
  char print_char;
};

class Binoculars: public item {
public:
  Binoculars();
  virtual void display_info(WINDOW * window) const;
  virtual void implement();
private:

};

class Food: public item {
public:
  Food();
  Food(int a_cost, int some_energy, char * a_name);
  virtual void display_info(WINDOW * window) const;
  virtual void implement();

  int get_cost();
  int get_energy();

private:
  int cost;
  int energy;

};
