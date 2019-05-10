#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

int main()
{
	if(has_colors())
        printf("\nYESSS\n\n");
    else
        printf("\nNOOOO\n\n");

	return 0;
}
