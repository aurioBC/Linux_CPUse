/*==============================================================================
|   SOURCE:     cpu.c
|
|   DATE:       Sept 9, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Module for acquiring CPU usage info and calculating usage
|               percentage.
|=============================================================================*/
#include "../include/cpu.h"
#include <sys/sysinfo.h>
#include <stdlib.h>


/*------------------------------------------------------------------------------
|   FUNCTION:   void zero(struct CPU *cpu)
|                   *cpu : struct to zero out
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Zero's out a "*cpu" struct.
|-----------------------------------------------------------------------------*/
void zero(struct CPU *cpu)
{
    for(int i = 0; i < SIZE; i++)
    {
        cpu->core[i].empty = 1;
        cpu->core[i].user = 0;
        cpu->core[i].nice = 0;
        cpu->core[i].sys = 0;
        cpu->core[i].idle = 0;
        cpu->core[i].iowait = 0;
        cpu->core[i].irq = 0;
        cpu->core[i].soft_irq = 0;
        cpu->core[i].steal = 0;
        cpu->core[i].guest = 0;
        cpu->core[i].guest_nice = 0;
        cpu->core[i].usage = 0;
    }
}


/*------------------------------------------------------------------------------
|   FUNCTION:   void get_info(FILE *fp, struct CPU *cpu)
|                   *fp : file pointer
|                   *cpu : pointer to CPU struct to fill
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Reads file "*fp" line by line and fills struct "*cpu".
|-----------------------------------------------------------------------------*/
void get_info(FILE *fp, struct CPU *cpu)
{
    char *line = NULL;
    ssize_t nread;
    size_t len = 0;
    int line_num = 0;

    // get number of processors
    cpu->num_of_cores = get_nprocs();

    // iterate thru file line by line
    while ((nread = getline(&line, &len, fp)) != -1)
    {
        // check if done getting CPU info
        if(line_num == (cpu->num_of_cores + 1))
            break;

        line_num++;

        // add CPU info to CPU_Info struct
        add_info(line, cpu);
    }

    free(line);
}


/*------------------------------------------------------------------------------
|   FUNCTION:   void add_info(char *line, struct CPU *cpu)
|                   *line : line of CPU usage info to add
|                   *cpu : pointer to CPU struct to fill
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Goes through "*cpu" struct calls add() to fill an empty
|               element.
|-----------------------------------------------------------------------------*/
void add_info(char *line, struct CPU *cpu)
{
    for(int i = 0; i < SIZE; i++)
        if(cpu->core[i].empty)
        {
            add(line, cpu, i);
            break;
        }
}


/*------------------------------------------------------------------------------
|   FUNCTION:   void add(char *line, struct CPU *cpu, int pos)
|                   *line : line of CPU usage info to add
|                   *cpu : pointer to CPU struct to fill
|                   pos : current position in Core struct array
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Adds CPU usage info from "*line" to "*cpu" struct at "pos".
|-----------------------------------------------------------------------------*/
void add(char *line, struct CPU *cpu, int pos)
{
    cpu->core[pos].empty = 0; // struct element not empty
    sscanf(line, "%s %f %f %f %f %f %f %f %f %f %f", cpu->core[pos].core_num, &cpu->core[pos].user, &cpu->core[pos].nice,
                                                     &cpu->core[pos].sys, &cpu->core[pos].idle, &cpu->core[pos].iowait,
                                                     &cpu->core[pos].irq, &cpu->core[pos].soft_irq, &cpu->core[pos].steal,
                                                     &cpu->core[pos].guest, &cpu->core[pos].guest_nice);
}

//----------
// TESTING |
//----------
/*------------------------------------------------------------------------------
|   FUNCTION:   void print_cpu_info(struct CPU cpu)
|                   cpu : CPU struct to print
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Prints the contents of "cpu" struct
|-----------------------------------------------------------------------------*/
void print_cpu_info(struct CPU cpu)
{
    for(int pos = 0; pos < SIZE; pos++)
    {
        if(cpu.core[pos].empty)
            break;

        printf("\n%s %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f", cpu.core[pos].core_num, cpu.core[pos].user, cpu.core[pos].nice,
                                                         cpu.core[pos].sys, cpu.core[pos].idle, cpu.core[pos].iowait,
                                                         cpu.core[pos].irq, cpu.core[pos].soft_irq, cpu.core[pos].steal,
                                                         cpu.core[pos].guest, cpu.core[pos].guest_nice);
    }
    printf("\n\n");
}


/*------------------------------------------------------------------------------
|   FUNCTION:   void copy_cpu_info(struct CPU curr, struct CPU *prev)
|
|   DATE:       Sept 10, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Prints the contents of "cpu" struct
|-----------------------------------------------------------------------------*/
void copy_cpu_info(struct CPU src, struct CPU *dst)
{
    for(int pos = 0; pos < SIZE; pos++)
    {
        dst->core[pos].user = src.core[pos].user;
        dst->core[pos].nice = src.core[pos].nice;
        dst->core[pos].sys = src.core[pos].sys;
        dst->core[pos].idle = src.core[pos].idle;
        dst->core[pos].iowait = src.core[pos].iowait;
        dst->core[pos].irq = src.core[pos].irq;
        dst->core[pos].soft_irq = src.core[pos].soft_irq;
        dst->core[pos].steal = src.core[pos].steal;
        dst->core[pos].guest = src.core[pos].guest;
        dst->core[pos].guest_nice = src.core[pos].guest_nice;
    }
}


/*------------------------------------------------------------------------------
|   FUNCTION:   void copy_cpu_info(struct CPU curr, struct CPU *prev)
|
|   DATE:       Sept 10, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Prints the contents of "cpu" struct
|-----------------------------------------------------------------------------*/
void calc(struct CPU prev, struct CPU *curr)
{
    {
        float prev_idle, curr_idle;
        float prev_non_idle, curr_non_idle;
        float prev_total, curr_total;
        float total_delta, idle_delta;

        for(int pos = 0; pos < SIZE; pos++)
        {
            if(prev.core[pos].empty)
                break;

            // PrevIdle=$((previdle + previowait))
            // Idle=$((idle + iowait))
            prev_idle = prev.core[pos].idle + prev.core[pos].iowait;
            curr_idle = curr->core[pos].idle + curr->core[pos].iowait;

            // PrevNonIdle=$((prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal))
            // NonIdle=$((user + nice + system + irq + softirq + steal))
            prev_non_idle = prev.core[pos].user + prev.core[pos].nice + prev.core[pos].sys + prev.core[pos].irq + prev.core[pos].soft_irq + prev.core[pos].steal;
            curr_non_idle = curr->core[pos].user + curr->core[pos].nice + curr->core[pos].sys + curr->core[pos].irq + curr->core[pos].soft_irq + curr->core[pos].steal;

            // PrevTotal=$((PrevIdle + PrevNonIdle))
            // Total=$((Idle + NonIdle))
            prev_total = prev_idle + prev_non_idle;
            curr_total = curr_idle + curr_non_idle;

            // totald=$((Total - PrevTotal))
            // idled=$((Idle - PrevIdle))
            total_delta = curr_total - prev_total;
            idle_delta = curr_idle - prev_idle;

            // CPU_Percentage=$(awk "BEGIN {print ($totald - $idled)/$totald*100}")
            curr->core[pos].usage = ((total_delta - idle_delta)/total_delta) * 100;

            /* TESTING */
            /*if(pos == 1)
                curr->core[pos].usage = 15.3;
            if(pos == 2)
                curr->core[pos].usage = 84.2;
            if(pos == 3)
                curr->core[pos].usage = 7.8;
            if(pos == 4)
                curr->core[pos].usage = 45.9;
            if(pos == 5)
                curr->core[pos].usage = 89.2;
            if(pos == 6)
                curr->core[pos].usage = 92.3;
            if(pos == 7)
                curr->core[pos].usage = 99.9;
            if(pos == 8)
                curr->core[pos].usage = 37.3;*/
        }
    }
}
