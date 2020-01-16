#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct file file;
struct file{
	int estampille;
	int sitePos;
	struct file* next;
};

void inserer(file** filei, int estampille, int sitePos);	
void retirer(file** filei, int estampille, int sitePos);
int verifierSiPremier(file* filei, int estampille,int sitePos);
void printFile(file* filei);

#endif
