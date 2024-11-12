/*
 *      restoration.c
 *      by: Armaan Sikka & Nate Pfeffer
 *      utln: asikka01 & npfeff01
 *      date: 9/8/24
 *      assignment: filesofpix
 *
 *      summary:
 *      This program will take in a corrupted plain pgm and restore
 *      it back in a raw format. The program first iterates over lines from
 *      the file, splitting the non-digits and digits. It does this 
 *      until two non-digit strings are found to be the same, indicating
 *      that the two lines are original lines from the raster.
 *      The next part of the program continues iterating over the lines,
 *      adding them to the restored raster if the infused non-digit
 *      strings match up. Finally, the program outputs to stdout the
 *      restored raw pgm file.
 */

#include "restoration.h"
#include "readaline.h"
#include "table.h"
#include <stdlib.h>
#include <ctype.h>

/********** restoration ********
 *
 *      Restores the given corrupted images and outputs the 'raw' image to
 *      stdout
 *
 *      Parameters:
 *              FILE *fp: open file of corrupted image
 *
 *      Return: 
 *              nothing
 *
 *      Expects:
 *              pointer to open corrupted image file
 *              a working readaline function from readaline.h
 *
 *      Notes:
 *              Logic Flow: iterate through lines until the correct 
 *                      non-unique string of infused chars is found,
 *                      save this, continue iterating over the remaining lines
 *                      comparing infused string to the correct one, adding if
 *                      so, then outputting raw pgm.
 *      
 ************************/
void restoration(FILE *fp)
{
        bool done = false;
        T dict = Table_new(2, NULL, NULL);

        /* loop through lines, processing each */
        while (!done) {
                done = processLine(fp, dict);
        }

        done = false;
        /* loop through lines, processing each */
        while (!done) {
                done = processLine_pt2(fp);
        }

        print_raw();
}


/********** processLine ********
 *
 *      Takes the array of chars returned from readaline and separates the
 *      digits from the non-digit characters. The digits are stored in a Hanson
 *      list and the non-digit characters are converted into an Atom string for
 *      easier comparison.
 *
 *      Parameters:
 *              char **datapp: pointer to a pointer to the char array
 *              size_t length: the length of the char array
 *
 *      Return: 
 *              nothing
 *
 *      Expects:
 *              non-null datapp
 *              valid length
 *              non-null head
 *
 *      Notes:
 *              
 ************************/
bool processLine(FILE *fp, T dict)
{
        char c = ' ';
        char *c_ptr = &c;
        char **datapp = &c_ptr;

        /* read a line */
        size_t read_length = readaline(fp, datapp);

        if (*datapp == NULL) {
                free(*datapp);
                return true;
        }

        /* char array to hold the digit integers of the line */
        char *ints = malloc((int)read_length * sizeof(char));
        int ints_l = 0;

        /* splits the line and occupies ints and returns curr_atom */
        const char *curr_atom = split(ints, datapp, read_length, &ints_l);
        char *temp_val = Table_put(dict, curr_atom, ints);

        /* if there exists a value associated with */
        if (temp_val != NULL) {
                limbo(curr_atom, ints_l, temp_val, ints);
                /* free the malloced variables from part 1 */
                Table_map(dict, vfree, NULL);
                Table_free(&dict);
                free(temp_val);
                free(*datapp);
                return true;
        }

        free(*datapp);
        return false;
}

/********** processLine ********
 *
 *      Takes the array of chars returned from readaline and separates the
 *      digits from the non-digit characters. The digits are stored in a Hanson
 *      list and the non-digit characters are converted into an Atom string for
 *      easier comparison.
 *
 *      Parameters:
 *              char **datapp: pointer to a pointer to the char array
 *              size_t length: the length of the char array
 *              Line *head: pointer to the head of the linked list of Line nodes
 *
 *      Return: 
 *              nothing
 *
 *      Expects:
 *              non-null datapp
 *              valid length
 *              non-null head
 *
 *      Notes:
 *              
 ************************/
bool processLine_pt2(FILE *fp)
{
        char c = ' ';
        char *c_ptr = &c;
        char **datapp = &c_ptr;

        /* read a line */
        size_t read_length = readaline(fp, datapp);

        if (*datapp == NULL) {
                return true;
        }

        /* char array to hold the digit integers of the line */
        char *ints = malloc((int)read_length * sizeof(char));
        int ints_l = 0;
        const char *curr_atom = split(ints, datapp, read_length, &ints_l);

        /* adds to raster if line was part of original plain pgm */
        if (curr_atom == corr) {
                add_to_raster(ints);
        }

        free(ints);
        free(*datapp);
        return false;
}

/********** split ********
 *
 *      split will take a read line and divide out the digits and non-digits
 *
 *      Parameters:
 *              char *ints: the array of chars that will hold the digits
 *              int *ints_l: the length of the array 'ints'
 *              char **datapp: the pointer to the line to split
 *              size_t read_length: how long the line to split is
 *
 *      Return: 
 *              const char * to an atom that pertains to the string
 *              of infused non-digits chars 
 *
 *      Expects:
 *              a line of digit and non-digit chars
 *
 *      Notes:
 *              
 ************************/
const char *split(char *ints, char **datapp, size_t read_length, int *ints_l)
{
        /* creates char array to hold the non-digit chars from the line */
        char *infused = malloc((int)read_length * sizeof(char));
        int infused_l = 0;

        size_t i = 0;
        char c = (*datapp)[i];

        /* Makes sure to read single, double, or triple digit numbers */
        while (i < read_length) {
                if (isdigit(c)) {
                        int temp = 0;
                        
                        while (i < read_length && isdigit(c)) {
                                temp = (temp * 10) + (int)c - 48;
                                c = (*datapp)[++i];
                        }

                        ints[(*ints_l)++] = (char)temp;
                }
                else {
                        infused[infused_l++] = c;
                        c = (*datapp)[++i];
                }
        }

        /* creates an Atom of the infused non-digits */
        const char *atom = Atom_new(infused, infused_l);
        free(infused);
        return atom;
}

/********** limbo ********
 *
 *      limbo is the moment of passage from the first part of
 *      restoration, finding the non-unique string of chars infused into the 
 *      original raster lines, to the second part, adding the remaining 
 *      original lines
 *      
 *      Parameters:
 *              const char *atom: this is the atom representing the non-
 *              unique string of chars infused into the original raster lines
 *              int ints_l: this is the length of one original raster line 
 *              char *line_one: first original line
 *              char *line_two: second original line
 *
 *      Return: 
 *              nothing
 *
 *      Expects:
 *              - the correct atom representing the non-
 *              unique string of chars infused into the original raster lines
 *              - the correct first two lines in the correct order
 *
 *      Notes:
 *              limbo initializes corr to the correct infused
 *              atom string, initializes the width of the raster, and adds
 *              the first two lines to the raster.
 *              
 ************************/
void limbo(const char *atom, int ints_l, char *line_one, char *line_two)
{
        corr = atom;
        width = ints_l;

        raw = malloc(2 * width * sizeof(char));
        raster_cap = 2 * width * sizeof(char);
        
        add_to_raster(line_one);
        add_to_raster(line_two);
}


/********** add_to_raster ********
 *
 *      adds a processed, original line to the final 'raw' pgm raster
 *
 *      Parameters:
 *              char *line: the line to add in string form
 *
 *      Return: 
 *              nothing
 *
 *      Expects:
 *              a valid line of length 'width' with ASCII outsid of the numeric
 *              chars
 *
 *      Notes: 
 *              Reallocates space of raw char array depending on size of
 *              original raster
 *              
 ************************/
void add_to_raster(char *line)
{
        /* Reallocate space of raw depending on size of original raster */
        if (raster_l + width >= raster_cap) {
                char *new_raw = (char *)realloc(raw, 
                                (raster_cap * 2) * sizeof(char));

                if (new_raw == NULL) {
                        free(raw);
                        exit(EXIT_FAILURE);
                }
                raster_cap *= 2;
                raw = new_raw;
        }

        for (int i = raster_l; i < raster_l + (width + 1); i++) {
                /* ensures we don't divide by 0 when raster_l == 0 */
                if (raster_l == 0) {
                        (raw)[i] = line[i];
                } else {
                        (raw)[i] = line[(i % raster_l)];
                }
        }

        /* increments raster_l after adding the line of length 'width' */
        raster_l += width;
}


/********** print_raw ********
 *
 *      Calculates height, then prints the header of the converted raw pgm
 *      and finally the raster.
 *
 *      Parameters:
 *              none
 *           
 *      Return: 
 *              none
 *
 *      Expects:
 *              a correct raster length, width, MAGIC_NUM, and MAX_VAL
 *
 *      Notes:
 *              uses fwrite to account for possible null characters in 
 *              the raster.
 *              
 ************************/
void print_raw()
{
        /* Set height, print header, then print raster */
        height = raster_l / (width);
        printf("%s\n%d %d\n%d\n", MAGIC_NUM, width, height, MAX_VAL);
        fwrite(raw, sizeof(char), (size_t)raster_l, stdout);
}

/********** vfree ********
 *
 *      used with Table_map to free all values stored in table
 *
 *      Parameters:
 *              const void *key: key from table
 *              void **value: value from table
 *              void *cl: closure for mapping
 *           
 *      Return: 
 *              none
 *
 *      Expects:
 *              key to be an atom, thus not freeing
 *
 *      Notes:
 *              obtained from Hanson textbook
 *              
 ************************/
static void vfree(const void *key, void **value, void *cl) 
{
        (void) key;
        (void) cl;
        free(*value);
}

/********** open_or_abort ********
 *
 *      opens a file or aborts the program provided the can't open
 *
 *      Parameters:
 *              char *fname: name of file to open
 *              char *mode: mode to open file
 *
 *      Return: 
 *              FILE * to the attempted open file
 *
 *      Expects:
 *              a proper filename / path
 *              a proper mode to open the expected file
 *
 *      Notes:
 *              obtained from CS40 website
 *              
 ************************/
static FILE *open_or_abort(char *fname, char *mode)
{
        FILE *fp = fopen(fname, mode);

        if (fp == NULL) {
                fprintf(stderr, "Could not open file %s with mode %s\n", 
                        fname, mode);
        }
        return fp;
}


/********** main ********
 *
 *      opens a file then runs restoration program
 *
 *      Parameters:
 *              int argc: number of command line arguments
 *              char *argv[]: the actual command line arguments
 *
 *      Return: 
 *              int
 *
 *      Expects:
 *              a corrupt pgm file
 *
 *      Notes:  checks whether the number of arguments given is valid for
 *              our implementation of the program
 *              
 ************************/
int main(int argc, char *argv[])
{
        if (argc == 1) {
                fprintf(stderr, "Incorrect number of arguments given \n");
        } else {
                FILE *fp = open_or_abort(argv[1], "rb");

                restoration(fp);
                free(raw);
                fclose(fp);
        }

        exit(EXIT_SUCCESS);
}