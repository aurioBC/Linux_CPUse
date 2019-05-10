/*==============================================================================
|   SOURCE:     findr.c
|
|   DATE:
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:
|=============================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

/* MACROS */
#define SIZE 16
#define STATFILE "/proc/stat"
#define LO "\e[48;5;46m"
#define HI "\e[48;5;196m"
#define MID_LO "\e[48;5;226m"
#define MID_HI "\e[48;5;220m"
#define LO_1 "\e[48;5;118m"
#define LO_2 "\e[48;5;155m"
#define LO_3 "\e[48;5;191m"
#define HI_1 "\e[48;5;214m"
#define HI_2 "\e[48;5;208m"
#define HI_3 "\e[48;5;202m"
#define FLUSH "\e[0m"

/* STRUCTS */
struct Core
{
    char core_num[SIZE];
    float empty;      // to determine if struct is empty or not
    float user;
    float nice;
    float sys;
    float idle;
    float iowait;
    float irq;
    float soft_irq;
    float steal;
    float guest;
    float guest_nice;
    float usage;
};

struct CPU
{
    struct Core core[SIZE];
};

/* FUNCTION PROTOTYPES */
int open_file(char *fn, FILE **fp);
void get_info(FILE *fp, struct CPU *cpu);
void add_info(char *line, struct CPU *cpu);
void add(char *line, struct CPU *cpu, int pos);
void calc_cpu_usage(struct CPU *cpu);
void zero(struct CPU *cpu);
void print_cpu_info(struct CPU cpu);
void print_cpu_info_graph(struct CPU cpu);
void test();

void calc(struct CPU prev_info, struct CPU *curr_info, struct CPU *res);


/*==============================================================================
|   FUNCTION:   int main()
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Main entry point of program
|=============================================================================*/
int main()
{
    struct CPU prev_cpu_info;
    struct CPU curr_cpu_info;
    struct CPU cpu_res;
    FILE *file;

    while(1)
    {
        printf("-------------------------------------");
        zero(&prev_cpu_info);
        zero(&curr_cpu_info);
        zero(&cpu_res);

        // first interval
        if(open_file(STATFILE, &file) == -1)
            return 1;
        get_info(file, &prev_cpu_info);
        fclose(file);

        sleep(3);

        // second interval
        if(open_file(STATFILE, &file) == -1)
            return 1;
        get_info(file, &curr_cpu_info);
        fclose(file);

        // calculate and print
        calc(prev_cpu_info, &curr_cpu_info, &cpu_res);
        print_cpu_info_graph(curr_cpu_info);
        printf("-------------------------------------");
    }

    /*
    struct CPU cpu;
    FILE *file;

    // zero out the CPU struct
    zero(&cpu);

    // open proc/stat
    if(open_file(STATFILE, &file) == -1)
        return 1;

    // get info from proc/stat
    get_info(file, &cpu);

    // calculate stats
    calc_cpu_usage(&cpu);

    // print CPU stats
    print_cpu_info_graph(cpu);

    fclose(file);
    */
    printf("\n");
    return 0;
}


void calc(struct CPU prev_info, struct CPU *curr_info, struct CPU *res)
{
    float prev_idle, curr_idle;
    float prev_non_idle, curr_non_idle;
    float prev_total, curr_total;
    float total_delta, idle_delta;

    for(int pos = 0; pos < SIZE; pos++)
    {
        if(prev_info.core[pos].empty)
            break;

        if(pos != 0)
        {
            // PrevIdle=$((previdle + previowait))
            // Idle=$((idle + iowait))
            prev_idle = prev_info.core[pos].idle + prev_info.core[pos].iowait;
            curr_idle = curr_info->core[pos].idle + curr_info->core[pos].iowait;

            // PrevNonIdle=$((prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal))
            // NonIdle=$((user + nice + system + irq + softirq + steal))
            prev_non_idle = prev_info.core[pos].user + prev_info.core[pos].nice + prev_info.core[pos].sys + prev_info.core[pos].irq + prev_info.core[pos].soft_irq + prev_info.core[pos].steal;
            curr_non_idle = curr_info->core[pos].user + curr_info->core[pos].nice + curr_info->core[pos].sys + curr_info->core[pos].irq + curr_info->core[pos].soft_irq + curr_info->core[pos].steal;

            // PrevTotal=$((PrevIdle + PrevNonIdle))
            // Total=$((Idle + NonIdle))
            prev_total = prev_idle + prev_non_idle;
            curr_total = curr_idle + curr_non_idle;

            // totald=$((Total - PrevTotal))
            // idled=$((Idle - PrevIdle))
            total_delta = curr_total - prev_total;
            idle_delta = curr_idle - prev_idle;

            // CPU_Percentage=$(awk "BEGIN {print ($totald - $idled)/$totald*100}")
            curr_info->core[pos].usage = ((total_delta - idle_delta)/total_delta) * 100;

            //printf("\n%s %.2f%%\n", curr_info.core[pos].core_num, res->core[pos].usage);
        }
    }
}


/*------------------------------------------------------------------------------
|   FUNCTION:
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:
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
|   FUNCTION:   int open_file(char *fn, FILE **fp)
|                   *fn : file directory
|                   **fp : file to open
|
|   DATE:       Sept 4, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Opens file
|-----------------------------------------------------------------------------*/
int open_file(char *fn, FILE **fp)
{
    // validate operation
    if((*fp = fopen(fn, "r")) == NULL)
    {
        printf("Failure to open\n\n");
        return -1;
    }

    return 0;
}


/*------------------------------------------------------------------------------
|   FUNCTION:
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:
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
|   FUNCTION:
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:
|-----------------------------------------------------------------------------*/
void add(char *line, struct CPU *cpu, int pos)
{
    cpu->core[pos].empty = 0; // struct element not empty
    sscanf(line, "%s %f %f %f %f %f %f %f %f %f %f", cpu->core[pos].core_num, &cpu->core[pos].user, &cpu->core[pos].nice,
                                                     &cpu->core[pos].sys, &cpu->core[pos].idle, &cpu->core[pos].iowait,
                                                     &cpu->core[pos].irq, &cpu->core[pos].soft_irq, &cpu->core[pos].steal,
                                                     &cpu->core[pos].guest, &cpu->core[pos].guest_nice);
}


/*------------------------------------------------------------------------------
|   FUNCTION:
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:
|-----------------------------------------------------------------------------*/
void get_info(FILE *fp, struct CPU *cpu)
{
    char *line = NULL;
    ssize_t nread;
    size_t len = 0;
    int line_num = 0;
    int processors;

    // get number of processors
    processors = get_nprocs();

    // iterate thru file line by line
    while ((nread = getline(&line, &len, fp)) != -1)
    {
        // check if done getting CPU info
        if(line_num == (processors + 1))
            break;

        line_num++;

        // add CPU info to CPU_Info struct
        add_info(line, cpu);
    }

    free(line);
}


/*------------------------------------------------------------------------------
|   FUNCTION:
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:
|-----------------------------------------------------------------------------*/
void test()
{
    printf("\n");
    printf("CPU1: %s  %s  %s  %s  %s  %s  %s  %s  %s  %s  %s 100%%", LO, LO_1, LO_2, LO_3, MID_LO, MID_HI, HI_1, HI_2, HI_3, HI, FLUSH);
    printf("\n\n");
    printf("100%%\n\n %s  %s\n %s  %s\n %s  %s\n %s  %s\n %s  %s\n %s  %s\n %s  %s\n %s  %s\n %s  %s\n %s  %s\n\nCPU1\n\n", HI, FLUSH, HI_3, FLUSH, HI_2, FLUSH, HI_1, FLUSH, MID_HI, FLUSH, MID_LO, FLUSH, LO_3, FLUSH, LO_2, FLUSH, LO_1, FLUSH, LO, FLUSH);
}


/*------------------------------------------------------------------------------
|   FUNCTION:
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:
|-----------------------------------------------------------------------------*/
void print_cpu_info(struct CPU cpu)
{
    float time_since_boot;
    float idle_time_since_boot;
    float usage_time_since_boot;

    for(int pos = 0; pos < SIZE; pos++)
    {
        if(cpu.core[pos].empty)
            break;

        printf("\n%s %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f", cpu.core[pos].core_num, cpu.core[pos].user, cpu.core[pos].nice,
                                                         cpu.core[pos].sys, cpu.core[pos].idle, cpu.core[pos].iowait,
                                                         cpu.core[pos].irq, cpu.core[pos].soft_irq, cpu.core[pos].steal,
                                                         cpu.core[pos].guest, cpu.core[pos].guest_nice);

        time_since_boot = cpu.core[pos].user + cpu.core[pos].nice + cpu.core[pos].sys + cpu.core[pos].idle +
                          cpu.core[pos].iowait + cpu.core[pos].irq + cpu.core[pos].soft_irq + cpu.core[pos].steal;
        idle_time_since_boot = cpu.core[pos].idle + cpu.core[pos].iowait;
        usage_time_since_boot = time_since_boot - idle_time_since_boot;
        printf("\n\ttime since boot: %.0f", time_since_boot);
        printf("\n\tidle since boot: %.0f", idle_time_since_boot);
        printf("\n\tusage time since boot: %.0f", usage_time_since_boot);
        printf("\n\tUsage: %.1f%%", cpu.core[pos].usage);
    }
}


/*------------------------------------------------------------------------------
|   FUNCTION:
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:
|-----------------------------------------------------------------------------*/
void print_cpu_info_graph(struct CPU cpu)
{
    for(int pos = 0; pos < SIZE; pos++)
    {
        if(cpu.core[pos].empty)
            break;

        if(pos != 0)
        {
            printf("\n%s|", cpu.core[pos].core_num);
            if(cpu.core[pos].usage < 20)
                printf("%s  %s %.1f%%\n", LO, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 20 && cpu.core[pos].usage < 30)
                printf(" %s  %s  %s %.1f%%\n", LO, LO_1, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 30 && cpu.core[pos].usage < 40)
                printf(" %s  %s  %s  %s %.1f%%\n", LO, LO_1, LO_2, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 40 && cpu.core[pos].usage < 50)
                printf(" %s  %s  %s  %s  %s %.1f%%\n", LO, LO_1, LO_2, LO_3, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 50 && cpu.core[pos].usage < 60)
                printf(" %s  %s  %s  %s  %s  %s %.1f%%\n", LO, LO_1, LO_2, LO_3, MID_LO, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 60 && cpu.core[pos].usage < 70)
                printf(" %s  %s  %s  %s  %s  %s  %s %.1f%%\n", LO, LO_1, LO_2, LO_3, MID_LO, MID_HI, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 70 && cpu.core[pos].usage < 80)
                printf(" %s  %s  %s  %s  %s  %s  %s  %s %.1f%%\n", LO, LO_1, LO_2, LO_3, MID_LO, MID_HI, HI_1, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 80 && cpu.core[pos].usage < 86)
                printf(" %s  %s  %s  %s  %s  %s  %s  %s  %s %.1f%%\n", LO, LO_1, LO_2, LO_3, MID_LO, MID_HI, HI_1, HI_2, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 86 && cpu.core[pos].usage < 90)
                printf(" %s  %s  %s  %s  %s  %s  %s  %s  %s  %s %.1f%%\n", LO, LO_1, LO_2, LO_3, MID_LO, MID_HI, HI_1, HI_2, HI_3, cpu.core[pos].usage, FLUSH);
            else if(cpu.core[pos].usage >= 90 && cpu.core[pos].usage <=100)
                printf(" %s  %s  %s  %s  %s  %s  %s  %s  %s  %s  %s %.1f%%\n", LO, LO_1, LO_2, LO_3, MID_LO, MID_HI, HI_1, HI_2, HI_3, HI, cpu.core[pos].usage, FLUSH);
        }
    }
}


/*------------------------------------------------------------------------------
|   FUNCTION:
|
|   DATE:       Sept 8, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:
|-----------------------------------------------------------------------------*/
void calc_cpu_usage(struct CPU *cpu)
{
    float time_since_boot;
    float idle_time_since_boot;
    float usage_time_since_boot;

    // calculate
    for(int pos = 0; pos < SIZE; pos++)
    {
        if(cpu->core[pos].empty)
            break;

        time_since_boot = cpu->core[pos].user + cpu->core[pos].nice + cpu->core[pos].sys + cpu->core[pos].idle +
                          cpu->core[pos].iowait + cpu->core[pos].irq + cpu->core[pos].soft_irq + cpu->core[pos].steal;
        idle_time_since_boot = cpu->core[pos].idle + cpu->core[pos].iowait;
        usage_time_since_boot = time_since_boot - idle_time_since_boot;
        cpu->core[pos].usage = (usage_time_since_boot/time_since_boot) * 100;
    }
}
