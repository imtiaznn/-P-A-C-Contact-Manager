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

    //Variables currentOption for submenus during operations
    int currentOption, 
        count = 0;

    //Variables for handling app pagination
    int contactPerPage = 5,
        currentPage = 0,
        maxPage = 0,
        needRefreshIndex = 0;

    //Buffers for receiving integer input option and string input options
    char input[10];
    char *buffer = malloc(100);

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
        displayContacts(root, &count, contactPerPage, currentPage);

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
                printf("%s\n", "(1/3) - Enter the name of the contact to be saved");
                fgets(name, 100, stdin);
                name[strcspn(name, "\n")] = '\0';

                printf("%s\n", "(2/3) - Enter the phone number of the contact to be saved");
                fgets(phoneNum, 100, stdin);
                phoneNum[strcspn(phoneNum, "\n")] = '\0';

                printf("%s\n", "(3/3) - Enter the email of the contact to be saved");
                fgets(email, 100, stdin);
                email[strcspn(email, "\n")] = '\0';

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
                printf("%s", "(1/3) - Enter the name of the contact to be edited\n");
                fgets(buffer, 100, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
    
                //Gets the node that needs to be edited
                targetNode = searchNode(root, buffer);
                if (targetNode == NULL) break;

                printf("\nEditing contact %s...\n", targetNode->contact->name);

                //Receives input from the user, removing any newline characters
                printf("%s", "(2/3) - Which part of the contact should be edited:\n(1) - Name\n(2) - Phone Number\n(3) - Email\n");
                fgets(buffer, 100, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';

                //Gets choice from user
                int currentChoice = atoi(buffer);

                //Receives input from the user, removing any newline characters
                printf("%s", "(3/3) - Enter the value to replace the existing value\n");
                fgets(buffer, 100, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';

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
                printf("%s\n", "(1/2) - Enter the name of the contact to be deleted");
                fgets(buffer, 100, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';

                //Deletes the contact that the user has selected
                root = deleteContact(root, buffer);

                needRefreshIndex = 1;

                printf("%s\n", "-- Contact deleted succesfully --");

                break;

            case OPTION_SEARCH:

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