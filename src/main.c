/*==============================================================================
|   SOURCE:     main.c
|
|   DATE:       Sept 9, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       This program displays CPU usage in the shell environment.
|
|   NOTES:      - uses 256 color (some terminal emulators don't support this)
|=============================================================================*/
#include "../include/ui.h"
#include "../include/fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*==============================================================================
|   FUNCTION:   int main()
|
|   DATE:       Sept 9, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Main entry point of program
|=============================================================================*/
int main()
{
    printf("\033[2J"); /*clear screen*/

    
    FILE *file;
    struct CPU cpu_usage_prev, cpu_usage_curr;
    int first = 1;

    zero(&cpu_usage_prev);
    zero(&cpu_usage_curr);

    while(1)
    {
        if(first) // if first time running thru program
        {
            // sample data
            if(open_file(STATFILE, &file) == -1)
                return 1;
            get_info(file, &cpu_usage_prev);
            fclose(file);
            first = 0;
        }
        else
        {
            // Save sample data from prev interval
            copy_cpu_info(cpu_usage_curr, &cpu_usage_prev);
            zero(&cpu_usage_curr);
        }

        sleep(1);

        // sample data
        if(open_file(STATFILE, &file) == -1)
            return 1;
        get_info(file, &cpu_usage_curr);
        fclose(file);

        // calculate and print CPU usage info
        calc(cpu_usage_prev, &cpu_usage_curr);
        print_cpu_core_graph(cpu_usage_curr);
    }

    return 0;
}
