/*==============================================================================
|   SOURCE:     ui.c
|
|   DATE:
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Module for printing results to screen.
|=============================================================================*/
#include "../include/ui.h"
#include <stdio.h>
#include <ncurses.h>


/*------------------------------------------------------------------------------
|   FUNCTION:   void print_cpu_core_graph(struct CPU cpu)
|                   cpu : cpu info to print
|
|   DATE:       Sept 4, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Print CPU core usage info
|-----------------------------------------------------------------------------*/
void print_cpu_core_graph(struct CPU cpu)
{
    printf("\033[1;1H"); // return curser to top left
    fflush(stdout);

    for(int pos = 0; pos < SIZE; pos++)
    {
        if(cpu.core[pos].empty)
            break;

        printf("\r                                                        ", cpu.core[pos].core_num);

        if(pos == 0)
        {
            printf("\rCPU: %.1f%%   Cores: %d\n\n", cpu.core[pos].usage, cpu.num_of_cores);
        }
        else
        {
            printf("\r%s|", cpu.core[pos].core_num);
            if(cpu.core[pos].usage < 5)
                printf("%s %s %.1f%%\n\n", C1, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 5 && cpu.core[pos].usage < 10)
                printf("%s %s %s %.1f%%\n\n", C1, C1, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 10 && cpu.core[pos].usage < 15)
                printf("%s %s %s %s %.1f%%\n\n", C1, C1, C2, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 15 && cpu.core[pos].usage < 20)
                printf("%s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 20 && cpu.core[pos].usage < 25)
                printf("%s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 25 && cpu.core[pos].usage < 30)
                printf("%s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 30 && cpu.core[pos].usage < 35)
                printf("%s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 35 && cpu.core[pos].usage < 40)
                printf("%s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 40 && cpu.core[pos].usage < 45)
                printf("%s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 45 && cpu.core[pos].usage < 50)
                printf("%s %s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, C5, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 50 && cpu.core[pos].usage < 55)
                printf("%s %s %s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, C5, C6, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 55 && cpu.core[pos].usage < 60)
                printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, C5, C6, C6, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 60 && cpu.core[pos].usage < 65)
                printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, C5, C6, C6, C7, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 65 && cpu.core[pos].usage < 70)
                printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, C5, C6, C6, C7, C7, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 70 && cpu.core[pos].usage < 75)
                printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, C5, C6, C6, C7, C7, C8, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 75 && cpu.core[pos].usage < 80)
                printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, C5, C6, C6, C7, C7, C8, C8, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 80 && cpu.core[pos].usage < 85)
                printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, C5, C6, C6, C7, C7, C8, C8, C9, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 85 && cpu.core[pos].usage < 90)
                printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, C5, C6, C6, C7, C7, C8, C8, C9, C9, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 90 && cpu.core[pos].usage < 95)
                printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, C5, C6, C6, C7, C7, C8, C8, C9, C9, C10, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 95 && cpu.core[pos].usage <=100)
                printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %.1f%%\n\n", C1, C1, C2, C2, C3, C3, C4, C4, C5, C5, C6, C6, C7, C7, C8, C8, C9, C9, C10, C10, cpu.core[pos].usage, FLUSH);
        }
    }
}
