#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <string.h>
#define SIZE 5

void sortCharArray(char a[][20]);

int strBinSearch(const char a[SIZE][20], char key[20], size_t low, size_t high);

#endif