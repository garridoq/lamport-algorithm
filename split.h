#ifndef SPLIT_H
#define SPLIT_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int count_occurences(char* texte, const char sep);
char** split(char* texte,int count, const char* sep);

#endif
