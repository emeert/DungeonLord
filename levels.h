#ifndef LEVELS_H
#define LEVELS_H

#define WIDTH 20
#define HEIGHT 10
#define LEVELS 10

extern char level_maps[LEVELS][HEIGHT][WIDTH]; // Declare level array
extern const char *level_names[] ;
extern const int sword_positions[LEVELS][2];


void load_next_level();
int level_name(int currentLevel);

#endif