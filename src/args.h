#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

#define verb(format, ...) \
	if (get_context()->verbose) { \
		printf(format __VA_OPT__(,) __VA_ARGS__); \
	}

extern const char *help_message;

bool contains_char(char const *str, const char c);

bool is_flag(char const *str, const int argc, char *argv[]);
bool is_short_flag(const char c, const int argc, char *argv[]);
bool is_long_flag(char const *str, const int argc, char *argv[]);

#endif
