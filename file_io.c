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
        char* email = strtok(NULL, ",");

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
int updateCSVLoop(TreeNode* root, FILE* fPtr) {
    if (root == NULL) return -1;

    //Recursively visits the left subtree
    updateCSVLoop(root->leftPtr, fPtr);

    // Write the current node's contact information to the file
    fprintf(fPtr, "%s,%s,%s", root->contact->name, root->contact->phoneNum, root->contact->email);

    //Recursively visit the right subtree
    updateCSVLoop(root->rightPtr, fPtr);

    return 0;
}

//Uses In-Order traversal to write the values in BST to CSV
//Update the file after changes are made
int updateCSV(TreeNode* root) {

    // Open the file for writing
    FILE* fPtr = fopen("contacts.csv", "w");
    if (fPtr == NULL) return -1;

    // Call the helper function to perform the in-order traversal
    if (updateCSVLoop(root, fPtr) == -1) {
        return -1;
    }

    fclose(fPtr);
    return 0;
}

//Display the contacts in the BST using inorder traversal
void displayContacts(TreeNode* root, int currentPage, const char query[100]) {
    if (root == NULL) return;

    static int matchesFound = 0;

    //Recursively visit the left subtree
    displayContacts(root->leftPtr, currentPage, query);

    //Calculate the range of contacts for the current page
    int start = currentPage * CONTACTPERPAGE;
    int end = start + CONTACTPERPAGE;

    //Flag for matches when searching contacts

    //Removes the newline char from the email
    root->contact->email[strcspn(root->contact->email, "\n")] = '\0';

    //Checks if any query searches are being made beofehand
    if (strlen(query) != 0) {

        //Flag for matches in a contact
        int match = 0; 

        if (strcmp(root->contact->name, query) == 0) match = 1;

        //Indicates that a contact matches the query and will be printed
        if (match) {
            printf("%d - %-39s%-21s%s",
            root->contact->index + 1,
            root->contact->name,
            root->contact->phoneNum,
            root->contact->email);

            matchesFound = 1;
        }


    } else {

        //Displays the all the contacts if there is no search query        
        if (root->contact->index >= start && root->contact->index < end) {
            printf("%-2d - %-39s%-21s%s\n",
                root->contact->index + 1,
                root->contact->name,
                root->contact->phoneNum,
                root->contact->email);
        }

    }

    
    //Adds the newline back to the contact
    root->contact->email[strcspn(root->contact->email, "\0")] = '\n';

    //Recursively visit the right subtree
    displayContacts(root->rightPtr, currentPage, query);

    if (root->rightPtr == NULL && root->leftPtr == NULL && strlen(query) != 0 && matchesFound == 0) {
        printf("No contact found matching the query.\n");
        matchesFound = 0;  
    }

    matchesFound = 0;
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