#include <stdio.h>
#include <string.h>
#define SIZE 5

void sortCharArray(char a[][20]) {
    char temp[20];

    //loops over the amount of passes to sort array
    for(size_t i = 1; i < SIZE; ++i) {
        //loops over every index of the array and the index after it
        for(size_t j = 0; j < SIZE-1; ++j) {
            if(strcmp(a[j],a[j+1]) == 1) {
                //swaps the position of an index in an array and the index after it
                strcpy(temp, a[j]);
                strcpy(a[j], a[j+1]);
                strcpy(a[j+1], temp);
            }
        }
    }
}

int strBinSearch(const char a[SIZE][20], char key[20], size_t low, size_t high) {
    while(low <= high) {
        int mid = (low + high) / 2;

        printf("\nComparing: %s, %s", a[mid], key);   

        if (strcmp(a[mid], key) == 0) {
            return mid;
        } else if (strcmp(a[mid], key) > 0) {  // a[mid] is greater than key
            high = mid - 1;
        } else {  // a[mid] is less than key
            low = mid + 1;
        }
    }

    return -1;
}