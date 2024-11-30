#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "utils.h"
#define CONTACT_PER_PAGE 5

//Saves a contact into the CSV file, does not save it into the BST however
void saveContact(const char* name, const char* phoneNum, const char* email) {
    //Open the files contacts.csv to append contact
    printf("%s\n", "-- Saving Contact... --");

    //Gets the file pointer
    FILE *fPtr = NULL;
    fPtr = fopen("contacts.csv", "a");

    if(fPtr == NULL) return;

    //Prints the contact values into the CSV
    fprintf(fPtr, "%s,%s,%s\n", name, phoneNum, email);

    fclose(fPtr);
}

//Loads the contacts form the CSV file into the BST
int loadCSV(TreeNode** root, int* count) {

    char line[1024];
    FILE* fPtr = NULL;
    fPtr = fopen("contacts.csv", "r");

    if (fPtr == NULL) return -1;

    //Runs the while loop until fgets doesnt detect any more lines in the file
    while(fgets(line, sizeof(line), fPtr)) {

        //Parses CSV file using commas as delimiters
        char* name = strtok(line, ",");
        char* phoneNum = strtok(NULL, ",");
        char* email = strtok(NULL, "\n");

        //Checks for any NULL cases while parsing csv file
        if (name == NULL || phoneNum == NULL || email == NULL) {
            printf("(loadCSV) Malformed line in CSV: %s\n", line);
            continue;
        }

        //Stores the index, name, phoneNum, email into a contact struct
        Contact* c = createContact(*count, name, phoneNum, email);
        if (c == NULL) {
            printf("(loadCSV): Failed to allocate memory for contact");
            continue;
        } 
            

        //Counts the value of contacts in the list for indexing
        (*count)++;

        //Creates a node with c as the contact field        
        TreeNode* newNode = createNode(c);
        if (newNode == NULL) {
            printf("(loadCSV): Failed to create node for contact");
            free(c);
            continue;
        }

        //Inserts the newNode at its respective position based on the root
        *root = insertNode(*root, newNode);

    }

    fclose(fPtr);
    return 0;
}

//Loop used in the updateCSV function, separated to prevent opening file multiple times during recursion
int updateCSVHelper(TreeNode* root, FILE* fPtr) {
    if (root == NULL) return 0;

    //Recursively visits the left subtree
    updateCSVHelper(root->leftPtr, fPtr);

    char email[strlen(root->contact->email) + 1];
    strcpy(email, root->contact->email);
    email[strcspn(email, "\n")] = '\0';

    // Write the current node's contact information to the file
    fprintf(fPtr, "%s,%s,%s\n", root->contact->name, root->contact->phoneNum, email);

    //Recursively visit the right subtree
    updateCSVHelper(root->rightPtr, fPtr);

    return 0;
}

//Uses In-Order traversal to write the values in BST to CSV
//Update the file after changes are made
int updateCSV(TreeNode* root) {

    // Open the file for writing
    FILE* fPtr = fopen("contacts.csv", "w");
    if (fPtr == NULL) return -1;

    // Call the helper function to perform the in-order traversal
    if (updateCSVHelper(root, fPtr) == -1) {
        return -1;
    }

    fclose(fPtr);
    return 0;
}

//Helper functin to display the contacts in the BST
void displayContactsHelper(TreeNode* root, int currentPage, const char query[100], int* matched, int* count) {
    if (root == NULL) return;

    // Recursively visit the left subtree
    displayContactsHelper(root->leftPtr, currentPage, query, matched, count);

    // Calculate the range of contacts for the current page
    int start = currentPage * CONTACT_PER_PAGE;
    int end = start + CONTACT_PER_PAGE;

    //Makes a duplicate of the email to remove the newline character on display
    char email[strlen(root->contact->email) + 1];
    strcpy(email, root->contact->email);
    email[strcspn(email, "\n")] = '\0';

    // Checks if any query searches are being made beforehand
    if (strlen(query) != 0) {

        if (strcmp(root->contact->name, query) != 0) {
            //Skip the current contact if it does not match the query
        } else {
            *matched = 1;

            // Indicates that a contact matches the query and will be printed
            if (*matched) {
                printf("%d - %-39s%-21s%s\n",
                root->contact->index + 1,
                root->contact->name,
                root->contact->phoneNum,
                email);

                (*count)++;
            }
        }

    } else {

        // Displays the all the contacts if there is no search query        
        if (root->contact->index >= start && root->contact->index < end) {
            printf("%-2d - %-39s%-21s%s\n",
                root->contact->index + 1,
                root->contact->name,
                root->contact->phoneNum,
                email);
        }

        (*count)++;
    }

    // Recursively visit the right subtree
    displayContactsHelper(root->rightPtr, currentPage, query, matched, count);  
}

//Display the contacts in the BST using inorder traversal
void displayContacts(TreeNode* root, int currentPage, const char query[100], int* count) {
    if (root == NULL) {
        printf("No contacts found\n");
        return;
    }

    int matched = 0;
    *count = 0;

    displayContactsHelper(root, currentPage, query, &matched, count);

    //Prints the message if no contact is found
    if (matched == 0 && strlen(query) != 0) {
        printf("\nNo contacts found matching %s...\n", query);
    }

    if (*count != 0) {
        printf("\nThere are %d contacts found", *count);
    }
}

// Function to update indices of all contacts in a single traversal
int refreshIndex(TreeNode* root, int currentIndex) {
    if (root == NULL) return currentIndex;
    
    // Recursively update the left subtree and get the next index
    currentIndex = refreshIndex(root->leftPtr, currentIndex);

    // Assign the current index to the contact
    root->contact->index = currentIndex;

    // printf("printing currentIndex: %d, contactIndex: %d\n", currentIndex, root->contact->index);

    currentIndex++;  // Increment the index for the next contact

    // Recursively update the right subtree and return the final index
    return refreshIndex(root->rightPtr, currentIndex);
}

//Displays the menu and gets the user option
int getOption(int currentOption, char input[10]) {

    //Prints the menu display
    printf("%s\n", "------ Available Operations -----");
    printf("\033[36m%-9s\t%-9s\n%-9s\t%-9s\033[0m\n\n%-9s\n", "1.Save", "2.Edit", "3.Delete", "4.Search", "\033[33mPress '0' to Exit\033[0m");
    printf("%s\n? ", "---------------------------------");

    //Gets the input from the user using input as buffer
    fgets(input, 10, stdin);
    if (!isInteger(input)) {
        printf("(getInput) Invalid input, please enter an integer\n");
        return -1;
    }

    currentOption = atoi(input);
}

void getInput(char* input, const char* message) {
    printf("%s", message);
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';
}