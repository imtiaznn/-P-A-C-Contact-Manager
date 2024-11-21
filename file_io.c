#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "utils.h"
#define CONTACTPERPAGE 5

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
void loadCSV(TreeNode** root, int* count) {

    char line[1024];
    FILE* fPtr = NULL;
    fPtr = fopen("contacts.csv", "r");

    if (fPtr == NULL) return;

    //Runs the while loop until fgets doesnt detect any more lines in the file
    while(fgets(line, sizeof(line), fPtr)) {

        //Parses CSV file using commas as delimiters
        char* name = strtok(line, ",");
        char* phoneNum = strtok(NULL, ",");
        char* email = strtok(NULL, ",");

        //Checks for any NULL cases while parsing csv file
        if (name == NULL || phoneNum == NULL || email == NULL) {
            continue;
        }

        //Stores the index, name, phoneNum, email into a contact struct
        Contact* c = createContact(*count, name, phoneNum, email);
        if (c == NULL) continue;

        //Counts the value of contacts in the list for indexing
        (*count)++;

        //Creates a node with c as the contact field        
        TreeNode* newNode = createNode(c);
        if (newNode == NULL) {
            free(c);
            continue;
        }

        //Inserts the newNode at its respective position based on the root
        *root = insertNode(*root, newNode);

    }

    fclose(fPtr);
}

//Loop used in the updateCSV function, separated to prevent opening file multiple times during recursion
void updateCSVLoop(TreeNode* root, FILE* fPtr) {
    if (root == NULL) return;

    //Recursively visits the left subtree
    updateCSVLoop(root->leftPtr, fPtr);

    // Write the current node's contact information to the file
    fprintf(fPtr, "%s,%s,%s", root->contact->name, root->contact->phoneNum, root->contact->email);

    //Recursively visit the right subtree
    updateCSVLoop(root->rightPtr, fPtr);

}

//Uses In-Order traversal to write the values in BST to CSV
//Update the file after changes are made
void updateCSV(TreeNode* root) {

    // Open the file for writing
    FILE* fPtr = fopen("contacts.csv", "w");
    if (fPtr == NULL) return;

    // Call the helper function to perform the in-order traversal
    updateCSVLoop(root, fPtr);

    fclose(fPtr);
}

//Display the contacts in the BST using inorder traversal
void displayContacts(TreeNode* root, int* count, int currentPage) {
    if (root == NULL) return;

    //Recursively visit the left subtree
    displayContacts(root->leftPtr, count, currentPage);

    //Calculate the range of contacts for the current page
    int start = currentPage * CONTACTPERPAGE;
    int end = start + CONTACTPERPAGE;

    //Removes the newline char from the email
    root->contact->email[strcspn(root->contact->email, "\n")] = '\0';

    //Print only if the current count is within the range for this page
    if (*count >= start && *count < end) {
        printf("%d - %-39s%-21s%s\n",
               *count + 1,
               root->contact->name,
               root->contact->phoneNum,
               root->contact->email);
    }

    root->contact->email[strcspn(root->contact->email, "\0")] = '\n';

    //Counts the number of contacts in the list for indexing
    (*count)++;

    //Recursively visit the right subtree
    displayContacts(root->rightPtr, count, currentPage);
}

// Function to update indices of all contacts in a single traversal
int refreshIndex(TreeNode* root, int currentIndex) {
    if (root == NULL) return currentIndex;
    
    // Recursively update the left subtree and get the next index
    currentIndex = refreshIndex(root->leftPtr, currentIndex);

    // Assign the current index to the contact
    root->contact->index = currentIndex;
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
    currentOption = atoi(input);
}

void getInput(char* buffer, const char* message) {
    printf("%s", message);
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}