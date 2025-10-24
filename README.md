# Dungeon Lord

Dungeon Lord is a terminal-based dungeon game written in C for Windows. The player navigates a grid, avoids a pursuing foe, retrieves a sword, and reaches the goal. The game includes basic AI, color effects, and cheat codes.

---

### NOTE:
Although the game is mostly functional, there may be bugs. Well - there are bugs, I know there are, but I don't know when I'll get to them. You are welcome to improve or modify the source code however you like. This was something I whipped together in about four hours.
---



## Features

- Grid-based movement
- Foe with line-of-sight detection and chase logic
- Sword pickup with visual strobe effect
- Level progression
- Cheat mode for skipping levels

## Requirements

- Windows OS
- GCC compiler (e.g., MinGW)
- Console that supports ANSI colors

## Build Instructions

1. Clone or download the repository.
2. Make sure the following files are present in the `src/` directory:
   - `main.c`
   - `levels.c`
   - `levels.h`
3. Open Git Bash or your preferred terminal.
4. Compile the game:
   ```bash
   gcc src/main.c src/levels.c -o DungeonLord.exe


##Game Overview

- **Player (@)**: Starts in the center of the map. Can acquire a sword (S) and reach the goal (G).
- **Foe (F)**: Actively hunts the player if spotted. Retreats or chases based on proximity and line of sight.
- **Sword (S)**: Grants visual strobe effects and may influence gameplay logic.
- **Goal (G)**: Reaching it progresses the level.
- **Cheat Mode**: Enter special characters to skip levels or exit cheat mode.

## How to Play

- Use WASD to move.
- Avoid the foe unless armed.
- Reach the goal to advance.
- Press special characters in cheat mode:
  - `L`  Skip level
  - `P`  Exit cheat mode

##  Build Instructions (Windows)

### Requirements

- GCC (MinGW or similar)
- Windows terminal with ANSI color support

### Build

```bash

gcc main.c levels.c -o DungeonLord.exe

```



