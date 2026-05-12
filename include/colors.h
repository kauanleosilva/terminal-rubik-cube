#pragma once

#define MAROON "\x1b[38;5;88m"
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define ROSE "\x1b[38;5;201m"
#define YELLOW "\x1b[33m"
#define PURPLE "\x1b[38;5;129m"
#define RESET "\x1b[0m"

char getColor(int);
char *convertColor(char);