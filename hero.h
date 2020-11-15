// based of Huidong's design
// practice for CS300 frupal project
// Camilo Schaser-Hughes
// November 13, 2020


#define KSIZE 128
class item;

enum terrain {NONE, MEADOW, SWAMP, WATER, WALL, UNSEEN};

struct grovnick {

  enum terrain square;
  bool viewed;
  item * feature;
  
};

class hero {

public:
  
  hero(WINDOW * vp, WINDOW * gm);
  hero(WINDOW * vp, WINDOW * gm, WINDOW * standard, char * filename);

  void update_display();
  
  void move_up();
  void move_down();
  void move_left();
  void move_right();
  
private:

  int xpos;
  int ypos;
  int whiffles;
  int energy;
  bool binoculars;

  grovnick map[KSIZE][KSIZE];

  WINDOW * viewport;
  WINDOW * gamemenu;
  WINDOW * stdwindow;

  // these are display functions
  void display_viewport();
  void display_frupal(int leftx, int topy);
  void display_hero(int leftx, int topy);
  void display_gamemenu();

  // these are maintenance functions
  bool energy_cost();
  void scroll_view(int& leftx, int& topy);
  void look_around();
  void load_file(char * filename);

};
