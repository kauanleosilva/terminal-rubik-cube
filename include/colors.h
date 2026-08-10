#pragma once

#define MAROON "\x1b[38;5;88m"
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define ROSE "\x1b[38;5;201m"
#define YELLOW "\x1b[33m"
#define PURPLE "\x1b[38;5;129m"
#define BG_MAROON "\x1b[48;5;88m"
#define BG_BLUE "\x1b[44m"
#define BG_GREEN "\x1b[42m"
#define BG_ROSE "\x1b[48;5;201m"
#define BG_YELLOW "\x1b[43m"
#define BG_PURPLE "\x1b[48;5;129m"
#define RESET "\x1b[0m"

char *convertColor(char, char);