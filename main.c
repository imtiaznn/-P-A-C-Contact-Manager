#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "structs.h"
#include "file_io.h"
#include "utils.h"

typedef enum  {
    OPTION_EXIT,
    OPTION_SAVE,
    OPTION_EDIT,
    OPTION_DELETE,
    OPTION_SEARCH,
    OPTION_PREV,
    OPTION_NEXT
} Option;

typedef enum {
    ERROR_ALLOCATION_FAILED
} Error;

const char* getErrorMsg[] = {
    "(main) Could not allocate memory",
    "(main) Could not load contacts",
    "(main) Could not update data into CSV",
    "(main) Could not refresh index",
    "(Error) Empty searches are not allowed",
    "(Error) Invalid operation. Please try again",
    "(Error) Duplicate values are not allowed",
    "(Error) Invalid phone number, please try again",
    "(Error) Invalid email address, please try again"
};

int main() {

    //Variables root to represent the binary search tree
    //Variables targetNode for current node being accessed
    TreeNode* root = NULL;
    TreeNode* targetNode = NULL;

    //Buffers for receiving integer input option and string input options
    char input[10];
    char *buffer = malloc(100);
    if (buffer == NULL){
        printf(getErrorMsg[0]);
        exit(1);
    }
        

    //Variables currentOption for menu options, count for counting contacts
    int currentOption,
        currentChoice,
        count = 0;

    //Variables for handling app pagination and indexing
    int contactPerPage = 5,
        currentPage = 0,
        maxPage = 0,
        needRefreshIndex = 0;

    //Handles queries and searches
    char query[100] = "";

    //Load in data value from CSV into BST
    if(loadCSV(&root, &count) == -1) {
        printf(getErrorMsg[1]);
        exit(1);
    }

    //Sets the maximum number of pages
    maxPage = ceil((count-1)/contactPerPage);

    while(1) {

        refreshIndex(root, 0);

        //Prints the application's header
        printf("%s\n", "----------------------------- Contact List ---------------------------------");
        printf("%-43s%-21s%-21s\n", "NAME","PHONE NUMBER","EMAIL");
        printf("%s\n", "----------------------------------------------------------------------------");
        
        //Display the contact list
        displayContacts(root, currentPage, query);

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
                
                printf("%s\n", "-- Contact information has been saved successfully --");

                //Frees allocated memory
                free(name);
                free(phoneNum);
                free(email);

                break;

            case OPTION_EDIT:
                
                //Receives input from the user, removing any newline characters
                getInput(buffer, "(1/3) Enter the name of the contact to be edited\n?");
    
                //Gets the node that needs to be edited
                targetNode = searchNode(root, buffer);
                if (targetNode == NULL) break;

                printf("\nEditing contact %s...\n", targetNode->contact->name);

                //Receives input from the user, removing any newline characters
                getInput(buffer, "(2/3) Which value would you like to edit? Enter the index of the selected option:\n1 - Name\n2 - Phone Number\n3 - Email\n");

                //Gets choice from user
                currentChoice = atoi(buffer);

                //Receives input from the user, removing any newline characters
                getInput(buffer, "(3/3) Enter the value to replace the existing value\n");

                //Replaces the selected value with new value
                //Only replaces in BST and not CSV
                    if(currentChoice==1) {
                        strcpy(targetNode->contact->name, buffer);
                    }
                    if(currentChoice==2) {
                        strcpy(targetNode->contact->phoneNum, buffer);
                    }  
                    if(currentChoice==3) {
                        strcpy(targetNode->contact->email, buffer);
                    }

                //Sets the refresh flag
                needRefreshIndex = 1;

                break;
            case OPTION_DELETE:

                //Receives input from the user, removing any newline characters
                getInput(buffer, "(1/2) Enter the name of the contact to be deleted");

                //Asks the user if they want to confirm deletion

                //Deletes the contact that the user has selected
                root = deleteContact(root, buffer);

                needRefreshIndex = 1;

                printf("%s\n", "-- Contact deleted succesfully --");

                break;

            case OPTION_SEARCH:

                getInput(query, "Enter search query:\n? ");

                if(strlen(query) == 0) {
                    printf(getErrorMsg[4]);
                    break;
                }

                break;
            
            case OPTION_PREV:

                if(currentPage > 0) {
                    //Decrements the current page
                    currentPage--;
                }
                break;
            case OPTION_NEXT:

                if (currentPage < maxPage) {
                    //Increments the current page
                    currentPage++;
                }
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