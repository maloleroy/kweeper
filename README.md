# 🎮 Kweeper : a Minesweeper game in C with SDL 2 💣💥 

Kweeper is a 2D minesweeper I wrote in C using SDL 2 back in 2023. It uses my own rudimentary testing library [Confer](https://github.com/maloleroy/confer) to generate the Makefile and run the tests!

## Installation

### Dependecies

* GCC
* SDL 2
* Confer (only if you want to run the tests)

Here, having SDL 2 installed means having `sdl2-config` in your path, since `Makefile` uses `sdl2-config` to get the correct GCC flags.

### Building

Just use Make!
```bash
make bin/kweeper
```

## Usage

To display the help message, just type
```bash
./bin/kweeper --help
```

Here is the output:
```
Kweeper, minesweeper game in development written in C created by Malo Leroy.

Command line options
  -d, --defsize    Uses the default size for the program (1920 by 1080 px),
                    instead of fullscreen
  -h, --help       Displays this help message and exits
  -v, --verbose    Verbose mode. A lot of ouputs (for devs or bugs report)

Key presses
  Escape           Quit the game and close the window
  Space            Pause/unpause the game
  s                Virtually clicks on all cells
  r                Starta a new game

About
  -  Written by Malo Leroy
  -  Licensed GPLv3 : Visit https://www.gnu.org/licenses/gpl-3.0.html
  -  Coded in C language and compiled (if you did not compiled the code
      yourself) with GCC 9.4.0
```
