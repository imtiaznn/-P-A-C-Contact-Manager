#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "utils.h"


void saveContact(const char* name, const char* phoneNum, const char* email) {
    //Open the files contacts.csv to append contact

    printf("%s\n", "-- Saving Contact... --");

    FILE *fPtr = NULL;
    fPtr = fopen("contacts.csv", "a");
    if((fPtr = fopen("contacts.csv","a")) == NULL) return;

    fprintf(fPtr, "%s,%s,%s\n", name, phoneNum, email);

    fclose(fPtr);
}

void loadCSV(TreeNode** root, int* count) {

    char line[1024];
    FILE* fPtr = NULL;
    fPtr = fopen("contacts.csv", "r");

    //Returns the function if file cant be opened
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
        (*count)++;
        if (c == NULL) continue;

        //Creates a node with c as the contact field        
        TreeNode* newNode = createNode(c);
        if (newNode == NULL) {
            free(c);
            continue;
        }

        //Inserts the newNode at its respective position based on the root
        *root = insertNode(*root, newNode);

    }

    //Closes file
    fclose(fPtr);
}

void updateCSVLoop(TreeNode* root, FILE* fPtr) {
    if (root == NULL) return;

    // Write left subtree
    if (root->leftPtr != NULL) {
        updateCSVLoop(root->leftPtr, fPtr);
    }

    // Write the current node's contact information to the file
    fprintf(fPtr, "%s,%s,%s", root->contact->name, root->contact->phoneNum, root->contact->email);

    // Write right subtree
    if (root->rightPtr != NULL) {
        updateCSVLoop(root->rightPtr, fPtr);
    }
}

void updateCSV(TreeNode* root) {
    //Uses In-Order traversal to write the values in BST to CSV
    //Used to update the file after changes are made

    // Open the file for writing
    FILE* fPtr = fopen("contacts.csv", "w");
    if (fPtr == NULL) return;

    // Call the helper function to perform the in-order traversal
    updateCSVLoop(root, fPtr);

    // Close the file after traversal is complete
    fclose(fPtr);
}

void displayCSV(TreeNode* root, int* count, int contactPerPage, int currentPage) {
    // Base case
    if (root == NULL) {
        return;  
    }

    // Recursively visit the left subtree
    displayCSV(root->leftPtr, count, contactPerPage, currentPage);

    // Calculate the range of contacts for the current page
    int start = currentPage * contactPerPage;
    int end = start + contactPerPage;

    // Print only if the current count is within the range for this page
    if (*count >= start && *count < end) {
        printf("%d - %-39s%-21s%s",
               *count + 1,  // Adjusted to display as 1-based index
               root->contact->name,
               root->contact->phoneNum,
               root->contact->email);
    }

    // Increment the count after visiting a node
    (*count)++;

    // Recursively visit the right subtree
    displayCSV(root->rightPtr, count, contactPerPage, currentPage);
}


// Function to update indices of all contacts in a single traversal
int refreshIndex(TreeNode* root, int currentIndex) {
    if (root == NULL) {
        return currentIndex;  // Return the current index when tree node is null
    }

    // Recursively update the left subtree and get the next index
    currentIndex = refreshIndex(root->leftPtr, currentIndex);

    // Assign the current index to the contact
    root->contact->index = currentIndex;
    currentIndex++;  // Increment the index for the next contact

    // Recursively update the right subtree and return the final index
    return refreshIndex(root->rightPtr, currentIndex);
}


int getOption(int currentOption, char input[10]) {

    printf("%s\n", "------ Available Operations -----");
    printf("\033[36m%-9s\t%-9s\n%-9s\t%-9s\033[0m\n\n%-9s\n", "1.Save", "2.Edit", "3.Delete", "4.Search", "\033[33mPress '0' to Exit\033[0m");
    printf("%s\n? ", "---------------------------------");

    fgets(input, 10, stdin);
    currentOption = atoi(input);
}