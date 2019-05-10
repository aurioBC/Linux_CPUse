/*==============================================================================
|   SOURCE:    fileio.c
|
|   DATE:       Sept 9, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Module for interacting with files.
|=============================================================================*/
#include "../include/fileio.h"


/*------------------------------------------------------------------------------
|   FUNCTION:   int open_file(char *fn, FILE **fp)
|                   *fn : file directory path
|                   **fp : file descriptor
|
|   DATE:       Sept 4, 2018
|
|   AUTHOR:     Alex Zielinski
|
|   DESC:       Opens file
|-----------------------------------------------------------------------------*/
int open_file(char *fn, FILE **fp)
{
    // validate open operation
    if((*fp = fopen(fn, "r")) == NULL)
    {
        printf("Failure to open\n\n");
        return -1;
    }

    return 0;
}
