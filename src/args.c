#include <args.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

const char *help_message =
	"Kweeper, minesweeper game in development written in C created by Sap.\n"
	"\nCommand line options\n"
	"  -d, --defsize    Uses the default size for the program (1920 by 1080 px),\n"
	"                    instead of fullscreen\n"
	"  -h, --help       Displays this help message and exits\n"
	"  -v, --verbose    Verbose mode. A lot of ouputs (for devs or bugs report)\n"
	"\nKey presses\n"
	"  Escape           Quit the game and close the window\n"
	"  Space            Pause/unpause the game\n"
	"  s                Virtually clicks on all cells\n"
	"  r                Starta a new game\n"
	"\nAbout\n"
	"  -  Written by Sap\n"
	"  -  Licensed GPLv3 : Visit https://www.gnu.org/licenses/gpl-3.0.html\n"
	"  -  Coded in C language and compiled (if you did not compiled the code\n"
	"      yourself) with GCC 9.4.0\n";

bool contains_char(char const *str, const char c) {
	while (*str) {
		if (*str++ == c)
			return true;
	}
	return false;
}

bool is_long_flag(char const *str, const int argc, char *argv[]) {
	for (int arg = 0; arg < argc; arg++) {
		if (argv[arg] != NULL && argv[arg][0] == '-' && argv[arg][1] == '-' && !strcmp(argv[arg]+2, str))
			return true;
	}
	return false;
}

bool is_short_flag(const char c, const int argc, char *argv[]) {
	for (int arg = 0; arg < argc; arg++) {
		if (argv[arg] != NULL && argv[arg][0] == '-' && argv[arg][1] != '-' && contains_char(argv[arg]+1, c))
			return true;
	}
	return false;
}


bool is_flag(char const *str, const int argc, char *argv[]) {
	return is_short_flag(str[0], argc, argv) || is_long_flag(str, argc, argv) || false;
}
