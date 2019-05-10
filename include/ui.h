// ui.h
#include "cpu.h"

/* MACROS */
#define FLUSH "\e[0m"

/* Reg Color Pallet */
/*
#define C1 "\e[48;5;46m"
#define C2 "\e[48;5;40m"
#define C3 "\e[48;5;34m"
#define C4 "\e[48;5;154m"
#define C5 "\e[48;5;226m"
#define C6 "\e[48;5;220m"
#define C7 "\e[48;5;214m"
#define C8 "\e[48;5;208m"
#define C9 "\e[48;5;202m"
#define C10 "\e[48;5;196m"
*/

/* Blue Color Pallet */
#define C1 "\e[48;5;51m"
#define C2 "\e[48;5;45m"
#define C3 "\e[48;5;39m"
#define C4 "\e[48;5;33m"
#define C5 "\e[48;5;27m"
#define C6 "\e[48;5;20m"
#define C7 "\e[48;5;19m"
#define C8 "\e[48;5;18m"
#define C9 "\e[48;5;17m"
#define C10 "\e[48;5;16m"

#define graphY 13
#define graphX 74

/* FUNCTION PROTOTYPES */
void print_cpu_core_graph(struct CPU cpu);
