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

int main() {

    //Variables root to represent the binary search tree
    //Variables targetNode for current node being accessed
    TreeNode* root = NULL;
    TreeNode* targetNode = NULL;

    //Buffers for receiving integer input option and string input options
    char input[10];
    char *buffer = malloc(100);

    //Variables currentOption for menu options, count for counting contacts
    int currentOption,
        currentChoice,
        count = 0;

    //Variables for handling app pagination and indexing
    int contactPerPage = 5,
        currentPage = 0,
        maxPage = 0,
        needRefreshIndex = 0;


    //Load in data value from CSV into BST
    loadCSV(&root, &count);

    //Sets the maximum number of pages
    maxPage = ceil((count-1)/contactPerPage);

    while(1) {

        //Prints the application's header
        printf("%s\n", "----------------------------- Contact List ---------------------------------");
        printf("%-43s%-21s%-21s\n", "NAME","PHONE NUMBER","EMAIL");
        printf("%s\n", "----------------------------------------------------------------------------");
        
        //Display the contact list
        //Resets count to 0 so displayContacts can refresh the value
        count = 0;
        displayContacts(root, currentPage);

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
                    free(name);
                    free(phoneNum);
                    free(email);
                    break;
                }

                //Receives input from the user, removing any newline characters
                getInput(name, "(1/3) Enter the name of the contact to be saved\n?");
                getInput(phoneNum, "(2/3) Enter the the Phone Number of the contact to be saved\n?");
                getInput(email, "(3/3) Enter the email of the contact to be saved\n?");

                //Saves the contact to CSV
                saveContact(name, phoneNum, email);

                //Sets the refresh flag
                needRefreshIndex = 1;

                //Creats a contact struct and TreeNode in order to update BST
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

                char* filter = malloc(100);

                getInput(buffer, "Which criteria will you be searching with:\n1.Name\n2.Phone Number\n3.Email");

                currentChoice = atoi(buffer);

                if (currentChoice == 1) {

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

                break;
        }

        if (needRefreshIndex) {
            //Refreshes the index of the contact list
            refreshIndex(root, 1);
            needRefreshIndex = 0; 
        }
    }

    //Updates the CSV everytime the program is closed
    updateCSV(root);

    return 0;
}