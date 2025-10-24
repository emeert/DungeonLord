#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "levels.h"
#include <strings.h>



#define WIDTH 20
#define HEIGHT 10

int movemade;
int spotted_by_foe;
int score;
int next_level_started;
int game_level;
int foe_chase_steps;
int player_has_goal;

int foe_retreating;
int foe_retreat_steps;
int player_Has_Sword;
int foe_has_been_slain;
int strobe_Effect;
int CheatCodLisPressed; 

char input;

void init_Reset_Vars(void){

movemade = 0;
spotted_by_foe = 0;
next_level_started = 0;
game_level = 0;
foe_chase_steps = 0;
player_has_goal = 0;
foe_retreating = 0;
foe_retreat_steps = 0;
player_Has_Sword = 0; 
strobe_Effect = 0;
CheatCodLisPressed = 0;

}




typedef struct {
    int x;
    int y;
} Entity;
void clear_screen(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD topLeft = {0, 0};
    DWORD written, cells;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    cells = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', cells, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cells, topLeft, &written);
    SetConsoleCursorPosition(hConsole, topLeft);
}


void set_color(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}


int has_line_of_sight(char map[HEIGHT][WIDTH], Entity from, Entity to)
{
    int dx = to.x - from.x;
    int dy = to.y - from.y;
    
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;
    
    float x = from.x;
    float y = from.y;
    
    for (int i = 0; i <= steps; i++)
    {
        int ix = (int)(x + 0.5f);
        int iy = (int)(y + 0.5f);
        
        if (map[iy][ix] == '#' && !(ix == from.x && iy == from.y))
        return 0; // Wall blocks view
        
        x += x_inc;
        y += y_inc;
    }
    
    return 1; // Clear line of sight
}




void render(char map[HEIGHT][WIDTH], Entity player, Entity foe, Entity goal, Entity sword)
{
    clear_screen();
    
    
    // Display the text above the play area
    printf("8888b.  88   88 88b 88  dP\"\"b8 888888  dP\"Yb  88b 88     88      dP\"Yb  88\"\"Yb 8888b.\n");
    printf(" 8I  Yb 88   88 88Yb88 dP   `\" 88__   dP   Yb 88Yb88     88     dP   Yb 88__dP  8I  Yb\n");
    printf(" 8I  dY Y8   8P 88 Y88 Yb  \"88 88\"\"   Yb   dP 88 Y88     88  .o Yb   dP 88\"Yb   8I  dY\n");
    printf("8888Y\"  `YbodP' 88  Y8  YboodP 888888  YbodP  88  Y8     88ood8  YbodP  88  Yb 8888Y\"\n\n");
    
    printf("By Elijah Meert\n\n");
    printf("version 1.2.0\n\n");
    
    
    
    
    
    for(int y = 0; y < HEIGHT; y++)
    {
        for(int x = 0; x < WIDTH; x++)
        {
            if (x == player.x && y == player.y)
            {
                if(player_Has_Sword)
                {           
                    if(strobe_Effect)
                    {
                        set_color(3); // Aqua Cyan color
                        strobe_Effect = 0;
                    }
                    else
                    {
                        set_color(6); // Yellow color
                        strobe_Effect = 1;
                    }
                    
                }
                else
                {
                    set_color(9); // Blue color
                }
                printf("@");
                set_color(7); // Default color
            }
            else if (x == foe.x && y == foe.y)
            {
                set_color(12); // Red color
                printf("F");
                set_color(7); // Default color
            }
            else if (x == goal.x && y == goal.y)
            {
                set_color(10); // Green color
                printf("G");
                set_color(7); // Default color
            }
            else if (x == sword.x && y == sword.y)
            {
                set_color(14); // Yellow color
                printf("S");
                set_color(7); // Reset to default
            }
            else
            {
                printf("%c", map[y][x]);
            }
        }
        printf("\n");
    }
}

void move_foe_randomly(char map[HEIGHT][WIDTH], Entity *foe)
{
    int directions[4] = {0, 1, 2, 3};

    // Shuffle directions
    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        int temp = directions[i];
        directions[i] = directions[j];
        directions[j] = temp;
    }

    for (int i = 0; i < 4; i++) {
        switch (directions[i]) {
            case 0: // Up
                if (foe->y > 1 && map[foe->y - 1][foe->x] != '#') {
                    foe->y--;
                    return;
                }
                break;
            case 1: // Down
                if (foe->y < HEIGHT - 2 && map[foe->y + 1][foe->x] != '#') {
                    foe->y++;
                    return;
                }
                break;
            case 2: // Left
                if (foe->x > 1 && map[foe->y][foe->x - 1] != '#') {
                    foe->x--;
                    return;
                }
                break;
            case 3: // Right
                if (foe->x < WIDTH - 2 && map[foe->y][foe->x + 1] != '#') {
                    foe->x++;
                    return;
                }
                break;
        }
    }
}





void move_foe(char map[HEIGHT][WIDTH], Entity *foe, Entity player, Entity goal)
{
    // Exit early if the foe is despawned
    if (foe->x < 0 && foe->y < 0)
        return;

    int distance = abs(foe->x - player.x) + abs(foe->y - player.y);
    int distance_to_goal = abs(foe->x - goal.x) + abs(foe->y - goal.y);

    if (distance <= 5 && has_line_of_sight(map, *foe, player))
    {
        spotted_by_foe = 1;
        foe_chase_steps = 0;
    }

    if (spotted_by_foe)
    {
        int dx = player.x - foe->x;
        int dy = player.y - foe->y;

        if (abs(dx) >= abs(dy))
        {
            if (dx > 0 && foe->x < WIDTH - 2 && map[foe->y][foe->x + 1] != '#')
                foe->x++;
            else if (dx < 0 && foe->x > 1 && map[foe->y][foe->x - 1] != '#')
                foe->x--;
            else if (dy > 0 && foe->y < HEIGHT - 2 && map[foe->y + 1][foe->x] != '#')
                foe->y++;
            else if (dy < 0 && foe->y > 1 && map[foe->y - 1][foe->x] != '#')
                foe->y--;
        }
        else
        {
            if (dy > 0 && foe->y < HEIGHT - 2 && map[foe->y + 1][foe->x] != '#')
                foe->y++;
            else if (dy < 0 && foe->y > 1 && map[foe->y - 1][foe->x] != '#')
                foe->y--;
            else if (dx > 0 && foe->x < WIDTH - 2 && map[foe->y][foe->x + 1] != '#')
                foe->x++;
            else if (dx < 0 && foe->x > 1 && map[foe->y][foe->x - 1] != '#')
                foe->x--;
        }
    }
    else if (distance_to_goal <= 5)
    {
        int dx = goal.x - foe->x;
        int dy = goal.y - foe->y;

        if (abs(dx) >= abs(dy))
        {
            if (dx > 0 && foe->x < WIDTH - 2 && map[foe->y][foe->x + 1] != '#')
                foe->x++;
            else if (dx < 0 && foe->x > 1 && map[foe->y][foe->x - 1] != '#')
                foe->x--;
            else if (dy > 0 && foe->y < HEIGHT - 2 && map[foe->y + 1][foe->x] != '#')
                foe->y++;
            else if (dy < 0 && foe->y > 1 && map[foe->y - 1][foe->x] != '#')
                foe->y--;
        }
        else
        {
            if (dy > 0 && foe->y < HEIGHT - 2 && map[foe->y + 1][foe->x] != '#')
                foe->y++;
            else if (dy < 0 && foe->y > 1 && map[foe->y - 1][foe->x] != '#')
                foe->y--;
            else if (dx > 0 && foe->x < WIDTH - 2 && map[foe->y][foe->x + 1] != '#')
                foe->x++;
            else if (dx < 0 && foe->x > 1 && map[foe->y][foe->x - 1] != '#')
                foe->x--;
        }
    }
    else
    {
        move_foe_randomly(map, foe);
    }
}





void spawn_entities(Entity *foe, Entity *goal, Entity *player, Entity *sword, char map[HEIGHT][WIDTH])
{
    player->x = 10;
    player->y = 4;
    
    // Spawn foe
    do {
        foe->x = rand() % (WIDTH - 2) + 1;
        foe->y = rand() % (HEIGHT - 2) + 1;
    } while ((abs(foe->x - player->x) + abs(foe->y - player->y) <= 5) ||
    map[foe->y][foe->x] == '#');
    
    // Spawn goal
    do {
        goal->x = rand() % (WIDTH - 2) + 1;
        goal->y = rand() % (HEIGHT - 2) + 1;
    } while ((abs(goal->x - player->x) + abs(goal->y - player->y) <= 5) ||
    map[goal->y][goal->x] == '#' ||
    (goal->x == foe->x && goal->y == foe->y));  // Prevent overlap

    sword->x = sword_positions[game_level][0];
    sword->y = sword_positions[game_level][1];

    
}

void despawn_entity(Entity *entity) {
    entity->x = -1;
    entity->y = -1;
}




int main()
{
    init_Reset_Vars(); //Initialize vars to 0
    srand(time(NULL));


    void cheat(void)
    {
        printf(" Welcome to the cheat mode\n");
        printf(" Cheat Codes are entered via case sensative special characters\n");
        printf(" type L to jump a level\n");
        printf(" type P to exit cheat mode\n");
        printf("Enter your code and press Enter: ");
        input = getchar();
        
        switch(input)
        {
            case 'L':
            {
                CheatCodLisPressed = 1;
            }
            break;

            case 'P':
            {
                break;
            }
            break;
        }

    }
    
    while(1)
    {
        char map[HEIGHT][WIDTH];
        memcpy(map, level_maps[game_level], sizeof(map));
        
        Entity player = {10, 4};
        Entity foe;
        Entity goal;
        Entity sword; 
        
        // Call spawn_entities with the map array
        spawn_entities(&foe, &goal, &player, &sword, map);

    
        
        while(1)
        {
            
            if(next_level_started)
            {
                player_has_goal = 0;
                player_Has_Sword = 0;
                
                // Call spawn_entities with the map array
                spawn_entities(&foe, &goal, &player, &sword, map);
                next_level_started = 0;
                spotted_by_foe = 0;
            }
            
            render(map, player, foe, goal, sword);
            level_name(game_level);
            
            if (movemade)
            {
                printf("player x = %d, player y = %d\n", player.x, player.y);
                printf("foe x = %d, foe y = %d\n", foe.x, foe.y);
                movemade = 0;
            }
            
            

            move_foe(map, &foe, player, goal);
           


            if ((player.x == goal.x && player.y == goal.y)|| (CheatCodLisPressed))
            {
         
                printf("Coin Collected!\n");
                foe_has_been_slain = 0;
                if(CheatCodLisPressed) CheatCodLisPressed = 0;
                player_has_goal = 1;
                score += 100;
                if (game_level < 9)
                {
                    game_level++;
                    load_next_level();
                    memcpy(map, level_maps[game_level], sizeof(map)); // Load new level
                    next_level_started = 1;
                    printf("score = %d\n", score);
                }
                else
                {
                    clear_screen();
                    printf("\n\n🎉 Congratulations! You completed all levels! 🎉\n");
                    printf("Final Score: %d\n", score);
                    printf("Press 'r' to restart or 'q' to quit .\n");
                    input = _getch();
                    if( input == 'r'|| input == 'R')
                    {
                        init_Reset_Vars(); //Reset vars 
                        break; //Restart
                    }
                    else
                    {
                        if(input == 'q'|| input == 'Q')
                        {
                            return 0; // Quit
                        }
                    }
                }
            }

            if (player.x == sword.x && player.y == sword.y)
            {
                player_Has_Sword = 1;
                printf("Sword Collected\n");
                despawn_entity(&sword);
            }

            if ((player.x == foe.x && player.y == foe.y) && !player_has_goal)
            {
                if(player_Has_Sword)
                {
                    despawn_entity(&foe);
                    score += 500;
                    fflush(stdout); // Ensure all output is printed before waiting for input
                    foe_has_been_slain = 1;
                }
                else
                {

                    printf("Game Over! The foe caught you.\n");
                    printf("Press 'r' to restart or q to quit.\n");
                    fflush(stdout); // Ensure all output is printed before waiting for input
                    input = _getch();
                    if (input == 'r' || input == 'R')
                        break;
                    else if (input == 'q' || input == 'Q')
                        return 0;
                    continue; // Skip rest of loop to avoid printing extra messages
                }
            }

                    

            
            
            if(!input)
            {
                printf("Press WASD for movement Controls.\n");
                printf("If you do not have a sword 'S', then you should avoid the Foe 'F'\n");
                printf("If you have the sword 'S',  then you can slay the Foe for 500 pts F\n");  
                printf("Collect the gold Coins G to win level\n");
                printf("Each Gold Coin 'G' is worth 100pts\n");
            }
            
            
            if (spotted_by_foe)
            {
                foe_chase_steps++;
                
                if (foe_chase_steps > 10)
                {
                    spotted_by_foe = 0; // Foe gives up
                    printf("You Evade!!");
                    
                    foe_chase_steps = 0;
                }
                else
                {
                    // Foe chases player (existing logic)
                    if(!player_Has_Sword)
                    {
                        printf("Run you have been spotted by the foe!!");
                    }
                    else if(foe_has_been_slain)
                    {
                        printf("Thy Foe has been slain!!");
                    }
                  
                }
            }
            
            input = _getch();
            
            switch(input)
            {
                case 'w':
                {
                    if (player.y > 1 && map[player.y - 1][player.x] != '#')
                    {
                        player.y--;
                        movemade = 1;
                    }
                }
                break;
                
                case 's':
                {
                    if (player.y < HEIGHT - 2 && map[player.y + 1][player.x] != '#')
                    {
                        player.y++;
                        movemade = 1;
                    }
                }
                break;
                
                case 'a':
                {
                    if (player.x > 1 && map[player.y][player.x - 1] != '#')
                    {
                        player.x--;
                        movemade = 1;
                    }
                }
                break;
                
                case 'd':
                {
                    if (player.x < WIDTH - 2 && map[player.y][player.x + 1] != '#')
                    {
                        player.x++;
                        movemade = 1;
                    }
                }
                break;

                case 'C':
                {
                    cheat();
                }
                break;
                
                
                case 'q':
                {
                    printf("Game Over! You quit the game.\n");
                    return 0; // End the game
                }
                break;
                
            }
            
            
   
            
            
        }
    }
    return 0;
}