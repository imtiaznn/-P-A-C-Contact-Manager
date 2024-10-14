#include <stdio.h>

void saveContact(char name[30], char phoneNum[20], char email[20]) {
    FILE *cfPtr = NULL;

    //Opens the file contacts.csv to write a data
    if((cfPtr = fopen("contacts.csv", "a")) == NULL) {
        //Error handling in case file can not be opened
        puts("Error: File can not be accessed");
    } else {
        fprintf(cfPtr, "%29s,%19s,%19s\n", name, phoneNum, email);
    }

    fclose(cfPtr);
}