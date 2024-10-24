#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "structs.h"
#include "utils.h"

//Returns a pointer for a contact with dynamically allocated name, phone numer and email
Contact* createContact(const char *name, const char *phoneNum, const char *email) {
    Contact *c = (Contact*) malloc(sizeof(Contact));

    //Allocate memory with size of the entered name with null terminator
    c->name = (char*) malloc(strlen(name) + 1);
    strcpy(c->name,name); 

    //Allocate memory with size of the entered phone number with null terminator
    c->phoneNum = (char*) malloc(strlen(phoneNum) + 1);
    strcpy(c->phoneNum,phoneNum);

    //Allocate memory with size of the entered email with null terminator
    c->email = (char*) malloc(strlen(email) + 1);
    strcpy(c->email,email);

    return c;
}

//Frees the memory allocated for creating a contact
void freeContact(Contact *c) {
    free(c->name);
    free(c->phoneNum);
    free(c->email);
    free(c);
}

//Saves a contact based on name, phoneNum and email
void saveContact(const char *name, const char *phoneNum, const char *email) {

    FILE *cfPtr = NULL;

    //Open the files contacts.csv to append contact
    if((cfPtr = fopen("contacts.csv","a")) == NULL) {
        //File can not be opened
        puts("Error: file can not be accessed");
    } else {
        fprintf(cfPtr, "%s,%s,%s\n", name, phoneNum, email);
    }
}

//Construct a binary search tree based on retrieved names
Contact* loadContact(Contact* root) {
    //Take data from contacts.csv
    FILE *file = fopen("contacts.csv", "r");
    char line[1024];

    //Make sure to check if root already contains a binary tree
    TreeNode* rootNode = createNode(&root);

    if (file == NULL) {
        puts("Error: file can not be accessed");
        return NULL;
    }

    while(fgets(line, sizeof(line), file)) {
        char* name = strtok(line, ",");

        insertNode(rootNode, name);
    }

    fclose(file);
    return root;
}
