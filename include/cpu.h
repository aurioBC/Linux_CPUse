// cpu.h

/* INCLUDES */
#include <stdio.h>

/* MACROS */
#define SIZE 16
#define STATFILE "/proc/stat"

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
    int num_of_cores;
};


/* FUNCTION PROTOTYPES */
void zero(struct CPU *cpu);
void get_info(FILE *fp, struct CPU *cpu);
void add_info(char *line, struct CPU *cpu);
void add(char *line, struct CPU *cpu, int pos);
void print_cpu_info(struct CPU cpu);
void copy_cpu_info(struct CPU src, struct CPU *dst);
void calc(struct CPU prev, struct CPU *curr);
