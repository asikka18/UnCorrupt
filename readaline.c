/*
 *     readaline.c
 *     by: Armaan Sikka & Nate Pfeffer
 *     utln: asikka01 & npfeff01
 *     date: 9/6/24
 *     assignment: filesofpix
 *
 *     summary: Reads a line from the given file and stores each character into
 *              a char array. Updates a pointer given as an argument (datapp)
 *              to point to the start of the char array and returns the size of
 *              the line that was read.
 */

#include "readaline.h"
#include <stdlib.h>
int INITIAL_SIZE = 1000;
int capacity = 0;

/********** readaline ********
 *
 *      Reads a line from a file, storing it in a char array pointed to by
 *      datapp and returns the number of bytes in the line.  Each invocation of
 *      readaline retrieves the next unread line in the file.
 *
 *      Parameters:
 *              FILE *inputfd: the stream of the file to read from
 *              char **datapp: the address of the current char array of line
 *
 *      Return: number of bytes in the line
 *
 *      Expects:
 *              
 *              
 *      Notes:
 *              *scoresUnderLimit is set to the number of scores under limit
 *              Will CRE if scores or scoresUnderLimit is NULL
 *              May CRE if malloc fails.
 ************************/
size_t readaline(FILE *inputfd, char **datapp)
{       
        if (inputfd == NULL || datapp == NULL) {
                exit(EXIT_FAILURE);
        }

        /* grab first char from line */
        char c = fgetc(inputfd);
        size_t size = 0;

        if (c == EOF) {
                if (feof(inputfd)) {
                        *datapp = NULL;
                        return size;

                } else if (ferror(inputfd)) {
                        exit(EXIT_FAILURE);
                }    
        }

        /* malloc a 10000 long array of chars */
        /* set *datapp to point to address of first byte */
        *datapp = (char *)malloc(INITIAL_SIZE * sizeof(char));
        capacity = INITIAL_SIZE;

        if (*datapp == NULL) {
                exit(EXIT_FAILURE);
        }

        /* Check if at EOF */
        while (c != EOF) {
                /* continue for all chars, stopping at '\n' */
                if (c == '\n') {
                        return size;
                }

                size++;

                if ((int)size >= capacity) {
                        char *newDatapp = (char *)realloc(*datapp, 
                                        (capacity * 2) * sizeof(char));

                        if (newDatapp == NULL) {
                                free(*datapp);
                                exit(EXIT_FAILURE);
                        }
                        capacity *= 2;
                        *datapp = newDatapp;
                }

                /* insert current char into the array */
                (*datapp)[size - 1] = c;
                c = fgetc(inputfd);
        }

        if (ferror(inputfd)) {
                exit(EXIT_FAILURE);
        }

        return size;
}
