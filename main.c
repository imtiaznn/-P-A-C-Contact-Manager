#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "structs.h"
#include "file_io.h"
#include "utils.h"

#define CONTACT_PER_PAGE 5
#define BUFFER_SIZE 100
#define QUERY_SIZE 100

typedef enum  {
    OPTION_EXIT,
    OPTION_SAVE,
    OPTION_EDIT,
    OPTION_DELETE,
    OPTION_SEARCH,
    OPTION_PREV,
    OPTION_NEXT,
    OPTION_TOGGLESORT
} Option;

typedef enum {
    ERROR_ALLOCATION_FAILED
} Error;

const char* getErrorMsg[] = {
    "\033[1;31m(main) Could not allocate memory\033[0m",
    "\033[1;31m(main) There are currently no contacts to be loaded\033[0m",
    "\033[1;31m(main) Could not update data into CSV\033[0m",
    "\033[1;31m(main) Could not refresh index\033[0m",
    "\033[1;31m(Error) Empty searches are not allowed\033[0m",
    "\033[1;31m(Error) Invalid option. Please try again\033[0m",
    "\033[1;31m(Error) Duplicate values are not allowed\033[0m",
    "\033[1;31m(Error) Invalid phone number, please try again\033[0m",
    "\033[1;31m(Error) Invalid email address, please try again\033[0m",
    "\033[1;31m(Error) Unable to find contact\033[0m"
};

int main() {

    //Variables root to represent the binary search tree
    TreeNode* root = NULL;

    //Buffers for receiving integer input option and string input options
    char input[10];
    char *buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL){
        free(buffer);
        printf("%s\n",getErrorMsg[0]);
        exit(1);
    }

    //Variables currentOption for menu options, count for counting contacts
    int currentOption = 0,
        currentChoice = 0,
        count = 0;

    //Variables for handling app pagination and indexing
    int currentPage = 0,
        maxPage = 0,
        needRefreshIndex = 0,
        mode = 0;

    //Handles queries and searches
    char query[QUERY_SIZE] = "";

    //Load in data value from CSV into BST
    if(loadCSV(&root, &count) == -1) {
        printf("%s\n", getErrorMsg[1]);
    }


    while(1) {

        //Prints the application's header
        printf("%s\n", "----------------------------- Contact List ---------------------------------");
        printf("%-43s%-21s%-21s\n", "NAME","PHONE NUMBER","EMAIL");
        printf("%s\n", "----------------------------------------------------------------------------");
        
        //Display the contact list
        displayContacts(root, currentPage, query, &count, mode);

        //Sets the maximum number of pages
        maxPage = (int) ceil( (double) count / CONTACT_PER_PAGE) - 1;

        printf("\nPAGE (%d/%d)\n", currentPage + 1, maxPage + 1);
        printf("\n%-61s%s\n\n", "<< (5) Previous Page", "(6) Next Page >>");

        //Get input after each iteration
        currentOption = getOption(currentOption,input);

        //terminates the loop when '0' is entered
        if (currentOption == OPTION_EXIT) break;

        //Program control 
        switch (currentOption) {
            case OPTION_SAVE:

                //Variables acting as buffers for string inputs
                char *name = malloc(100),
                     *phoneNum = malloc(100),   
                     *email = malloc(100);

                //Checks if malloc() is successfull to prevent crashes
                if (name == NULL || phoneNum == NULL || email == NULL) {
                    printf(getErrorMsg[0]);

                    free(name);
                    free(phoneNum);
                    free(email);
                    break;
                }

                //Receives input from the user, removing any newline characters
                getInput(name, "(1/3) Enter the name of the contact to be saved\n?");
                if(isDuplicate(name, root)) {
                    printf("%s\n", getErrorMsg[6]);
                    free(name);
                    break;
                }

                getInput(phoneNum, "(2/3) Enter the the Phone Number of the contact to be saved\n?");
                if(!isValidPhoneNumber(phoneNum)) {
                    printf("%s\n", getErrorMsg[7]);
                    free(name);
                    free(phoneNum);
                    break;
                }

                getInput(email, "(3/3) Enter the email of the contact to be saved\n?");
                if(!isValidEmailAddress(email)) {
                    printf("%s\n", getErrorMsg[8]);
                    free(name);
                    free(phoneNum);
                    free(email);
                    break;
                }

                //Saves the contact to CSV
                saveContact(name, phoneNum, email);

                //Sets the refresh flag
                needRefreshIndex = 1;

                //Saves the contact to BST
                Contact* c = createContact(count, name, phoneNum, email);
                TreeNode* cNode = createNode(c);
                root = insertNode(root, cNode);
                

                //Frees allocated memory
                free(name);
                free(phoneNum);
                free(email);

                break;

            case OPTION_EDIT:
                
                //Receives input from the user, removing any newline characters
                getInput(buffer, "(1/4) Enter the name of the contact to be edited\n? ");
    
                //Gets the node that needs to be edited
                TreeNode* editedNode = searchNode(root, buffer);
                if (editedNode == NULL) {
                    printf("%s\n", getErrorMsg[9]);
                    break;
                }

                printf("\nEditing contact %s...\n\n", editedNode->contact->name);

                printf("Contact Information\nName: %s\nPhone Number: %s\nEmail: %s\n\n", editedNode->contact->name, editedNode->contact->phoneNum, editedNode->contact->email);

                //Receives input from the user, removing any newline characters
                getInput(buffer, "(2/4) Which value would you like to edit? Enter the index of the selected option (Press 1, 2 or 3):\n1 - Name\n2 - Phone Number\n3 - Email\n? ");
                //Gets choice from user
                currentChoice = atoi(buffer);

                if (currentChoice < 1 || currentChoice > 3) {
                    printf("Invalid choice. Please select 1, 2, or 3.\n");
                    break;
                }

                //Receives input from the user, removing any newline characters
                getInput(buffer, "(3/4) Enter the new value\n? ");

                // Validate the input and set the new value
                int isValid = 1;
                if (currentChoice == 1 && isDuplicate(buffer, root)) {
                    printf("%s\n", getErrorMsg[6]);
                    isValid = 0;
                } else if (currentChoice == 2 && !isValidPhoneNumber(buffer)) {
                    printf("%s\n", getErrorMsg[7]);
                    isValid = 0;
                } else if (currentChoice == 3 && !isValidEmailAddress(buffer)) {
                    printf("%s\n", getErrorMsg[8]); 
                    isValid = 0;
                }

                if(!isValid) break;

                printf("\n(4/4) Are you sure you want to edit the contact with the following new information (Y/N)?\n");
                if (currentChoice == 1) printf("Name: %s\n", buffer);
                if (currentChoice == 2) printf("Phone Number: %s\n", buffer);
                if (currentChoice == 3) printf("Email: %s\n", buffer);

                char confirmBuffer[10];

                getInput(confirmBuffer, "This process is NOT reversible.\n? ");
                if (strcasecmp(confirmBuffer, "Y") == 0) {
                    // Perform the update
                    if (currentChoice == 1) editContact(&root, editedNode->contact->name, buffer, editedNode->contact->phoneNum, editedNode->contact->email);
                    else if (currentChoice == 2) editContact(&root, editedNode->contact->name, editedNode->contact->name, buffer, editedNode->contact->email);
                    else if (currentChoice == 3) editContact(&root, editedNode->contact->name, editedNode->contact->name, editedNode->contact->phoneNum, buffer);

                    printf("Contact updated successfully.\n");
                    needRefreshIndex = 1; // Flag for reloading index or CSV
                } else {
                    printf("Edit operation canceled.\n");
                }

                //Sets the refresh flag
                needRefreshIndex = 1;

                break;
            case OPTION_DELETE:

                //Receives input from the user, removing any newline characters
                getInput(buffer, "(1/2) Enter the name of the contact to be deleted\n? ");

                TreeNode* deletedNode = searchNode(root, buffer);
                if (deletedNode == NULL) {
                    printf("%s\n", getErrorMsg[9]);
                    break;
                }   

                //Asks the user if they want to confirm deletion
                printf("(2/2) Are you sure you want to delete the node (Y/N)\n\nName: %s\nPhone Number: %s\nEmail: %s\n\n", deletedNode->contact->name, deletedNode->contact->phoneNum, deletedNode->contact->email);
                getInput(buffer, "This process is NOT reversible\n? ");

                if (strcasecmp(buffer, "Y") == 0) { 
                    currentChoice = 1;
                } else if (strcasecmp(buffer, "N") == 0) {
                    currentChoice = 0;
                } else {
                    //error
                }

                if(currentChoice) {
                    //Deletes the contact that the user has selected
                    root = deleteContact(root, deletedNode->contact->name);
                }

                needRefreshIndex = 1;

                break;

            case OPTION_SEARCH:

                getInput(query, "Enter search query:\n? ");

                if(strlen(query) == 0) {
                    printf("%s\n", getErrorMsg[4]);
                    break;
                }

                break;

            case OPTION_PREV:

                if(currentPage > 0) {
                    //Decrements the current page
                    currentPage--;
                } else {
                    printf("You are already on the first page\n");
                }
                break;
            case OPTION_NEXT:

                if (currentPage < maxPage) {
                    //Increments the current page
                    currentPage++;
                } else {
                    printf("You are already on the last page\n");
                }
                break;

            case OPTION_TOGGLESORT:

                getInput(buffer, "Enter the index of the selected option (Press 0 or 1):\n0. Sort by Name\n1. Sort by Phone Number\n2. Sort by Email\n? ");

                if( !isInteger(buffer) ) {
                    printf("%s\n", getErrorMsg[5]);
                }

                mode = atoi(buffer);

                break;

            default:
                printf("%s\n", getErrorMsg[5]);
                break;
        }

        if (needRefreshIndex == 1) {
            //Refreshes the index of the contact list
            refreshIndex(root, 0);
            needRefreshIndex = 0; 
        }

    }

    //Updates the CSV everytime the program is closed
    if (updateCSV(root) == -1) {
        printf(getErrorMsg[2]);
    }

    free(buffer);
    //Make function to free entire tree
    free(root);

    return 0;
}