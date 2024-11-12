/*
 *     restoration.c
 *     by: Armaan Sikka & Nate Pfeffer
 *     utln: asikka01 & npfeff01
 *     date: 9/8/24
 *     assignment: filesofpix
 *
 *     summary: 
 */

#include "readaline.h"
#include "atom.h"
#include <stdbool.h>

typedef struct Table_T *T;

int width = 0;
int height = 0;
const char MAGIC_NUM[3] = "P5\0";
const int MAX_VAL = 255;
char *raw = NULL;
int raster_l = 0;
int raw_l = 0;
const char *corr = NULL;
int raster_cap = 0;

void restoration(FILE *fp);
bool processLine(FILE *fp, T dict);
const char *split(char *ints, char **datapp, size_t read_length, int *ints_l);
void limbo(const char *atom, int ints_l, char *line_one, char *line_two);
void add_to_raster(char *line);
bool processLine_pt2(FILE *fp);
void print_raw();
static void vfree(const void *key, void **value, void *cl);
