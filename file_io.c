#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "utils.h"
#define CONTACT_PER_PAGE 5  // Number of contents displayed per page
#define ENCRYPTION_KEY 0xBAC781 // Key for XOR encryption

// Saves a contact into the CSV file, does not save it into the BST
void saveContact(const char *name, const char *phoneNum, const char *email)
{
    // Open the files contacts.csv to append contact
    printf("%s\n", "Saving Contact...");

    // Gets the file pointer
    FILE *fPtr = NULL;  // File pointer for the CSV file
    fPtr = fopen("contacts.csv", "a");  // Opens the file in append mode

    if (fPtr == NULL) {  // Check if file opening was succesful
        return;
    }

    fprintf(fPtr, "%s,%s,%s\n", name, phoneNum, email);     // Write contact details in CSV format

    fclose(fPtr);   // Close file after writing
}

// Loads the contacts form the CSV file into the BST
int loadCSV(TreeNode **root, int *count)
{

    char line[1024];    // Buffer to store a line from the file
    FILE *fPtr = NULL;
    fPtr = fopen("contacts.csv", "r");  // Open the file in read mode

    if (fPtr == NULL) {  //Check if file opening was succesful
        return -1; 
    } 

    while (fgets(line, sizeof(line), fPtr))    // Read the file line by line
    {
        // Parses CSV file using commas as delimiters
        char *name = strtok(line, ",");
        char *phoneNum = strtok(NULL, ",");
        char *email = strtok(NULL, "\n");

        // Validate parsing information
        if (name == NULL || phoneNum == NULL || email == NULL) {
            printf("(loadCSV) Malformed line in CSV: %s\n", line);
            continue;
        }

        // Stores the index, name, phoneNum, email into a contact struct
        Contact *c = createContact(*count, name, phoneNum, email);  // Create a contact struct
        if (c == NULL) {  // Check if memory allocation failed
            printf("(loadCSV): Failed to allocate memory for contact");
            continue;
        }

        (*count)++;     // Counts the value of contacts in the list for indexing

        // Creates a new BST node for the contact
        TreeNode *newNode = createNode(c);
        if (newNode == NULL) {  // Check if memory allocation failed
            printf("(loadCSV): Failed to create node for contact");
            free(c);
            continue;
        }

        *root = insertNode(*root, newNode);     // Inserts the newNode at its respective position based on the root
    }

    fclose(fPtr);   // Close the file
    return 0;
}

// Loop used in the updateCSV function, separated to prevent opening file multiple times during recursion
int updateCSVHelper(TreeNode *root, FILE *fPtr)
{
    if (root == NULL) {   // If node is NULL, return
        return 0;
    }

    // Recursively visits the left subtree
    updateCSVHelper(root->leftPtr, fPtr);

    char email[strlen(root->contact->email) + 1];   // Temporary buffer to store email
    strcpy(email, root->contact->email);    // Copy email to buffer
    email[strcspn(email, "\n")] = '\0';    // Remove newline character from email

    // Write the current node's contact information to the file
    fprintf(fPtr, "%s,%s,%s\n", root->contact->name, root->contact->phoneNum, email);

    // Recursively visit the right subtree
    updateCSVHelper(root->rightPtr, fPtr);

    return 0;
}

// Update the CSV file with the current state of BST
int updateCSV(TreeNode *root)
{
    FILE *fPtr = fopen("contacts.csv", "w");    // Open the file in write mode
    if (fPtr == NULL) {  // Check if file opening was succesful
        return -1;
    }

    // Call the helper function to perform in-order traversal and write contacts to the file
    if (updateCSVHelper(root, fPtr) == -1) {  // Check if the helper function failed
        return -1;
    }

    fclose(fPtr);   // Close the file
    return 0;
}

// Helper function to display the contacts in a paginated format with optional filtering
void displayContactsHelper(TreeNode *root, int currentPage, const char query[100], int *matched, int *count)
{
    if (root == NULL) {     // Check if the BST is empty
        return;
    }

    // Recursively visit the left subtree
    displayContactsHelper(root->leftPtr, currentPage, query, matched, count);

    int start = currentPage * CONTACT_PER_PAGE;   // Calculate the start index for the current page
    int end = start + CONTACT_PER_PAGE;     // Calculate the end index for the current page

    // Make a duplicate of the email to remove the newline character on display
    char email[strlen(root->contact->email) + 1];   // Temporary buffer to store email
    strcpy(email, root->contact->email);    // Copy email to buffer
    email[strcspn(email, "\n")] = '\0';   // Remove newline character from email

    // Perform partial matching if a query is provided
    if (strlen(query) != 0) {
        // Check if the query is a substring of the name, phone number, or email
        if (strstr(root->contact->name, query) != NULL ||
            strstr(root->contact->phoneNum, query) != NULL ||
            strstr(email, query) != NULL)
        {

            *matched = 1; // Mark that at least one match was found

            // Print if the contact falls within the range for the current page
            if (*count >= start && *count < end) {  // Check if the contact is within the current page
                printf("%-2d - %-39s%-21s%s\n",
                       root->contact->index + 1,
                       root->contact->name,
                       root->contact->phoneNum,
                       email);
            }

            (*count)++;     // Increment the contact counter
        }
    }
    
    else {  // Display all contacts if no query is provided
        if (*count >= start && *count < end) {  // Check if the contact is within the current page
            printf("%-3d- %-40s%-21s%s\n",
                   root->contact->index + 1,
                   root->contact->name,
                   root->contact->phoneNum,
                   email);
        }

        (*count)++;    // Increment the contact counter
    }

    // Recursively visit the right subtree
    displayContactsHelper(root->rightPtr, currentPage, query, matched, count);
}

// Display the contacts in the BST using inorder traversal
void displayContacts(TreeNode *root, int currentPage, const char query[100], int *count, int mode)
{
    if (root == NULL) {     // Check if the BST is empty
        printf("No contacts found\n");
        return;
    }

    // Reset matched flag and contact counter
    int matched = 0;
    *count = 0;

    if (mode > 1)
    {
        // Alternate sorting logic
        *count = countTreeNodes(root);  // Count the number of nodes in the BST
        int size = countTreeNodes(root);
        TreeNode **arr = alternateSort(root, mode); // Sort the contacts based on the mode

        if (arr == NULL || size == 0) { // Check if sorting failed
            printf("(displayContacts) Failed to sort contacts\n");
            return;
        }

        int start = currentPage * CONTACT_PER_PAGE;  // Calculate the start index for the current page
        int end = start + CONTACT_PER_PAGE;     // Calculate the end index for the current page

        for (int i = start; i < end && i < size; i++)   // Iterate through the contacts in the current page
        {
            // Check if the contact matches the query or if no query is provided
            if (strlen(query) == 0 || strstr(arr[i]->contact->name, query) != NULL ||
                strstr(arr[i]->contact->phoneNum, query) != NULL ||
                strstr(arr[i]->contact->email, query) != NULL)
            {
                printf("%-2d - %-39s%-21s%s\n",
                       i + 1,
                       arr[i]->contact->name,
                       arr[i]->contact->phoneNum,
                       arr[i]->contact->email);
                matched++;
            }
        }

        free(arr);  // Free the dynamically allocated array
    }

    else {
        // Use in-order traversal for dispalying contacts
        displayContactsHelper(root, currentPage, query, &matched, count);
    }

    // Print a message if no contacts were found
    if (matched == 0 && strlen(query) != 0) {
        printf("\nNo contacts found matching '%s'...\n", query);
    }

    if (*count != 0) {  // Print the number of contacts found
        printf("\nThere are %d contacts found.\n", *count);
    }
}

// Function to update indices of all contacts in the BST
int refreshIndex(TreeNode *root, int currentIndex)
{
    if (root == NULL)   // If the BST is empty, return the current index
        return currentIndex;

    // Recursively update the left subtree and get the next index
    currentIndex = refreshIndex(root->leftPtr, currentIndex);

    // Assign the current index to the contact
    root->contact->index = currentIndex;

    // printf("printing currentIndex: %d, contactIndex: %d\n", currentIndex, root->contact->index);

    currentIndex++; // Increment the index for the next contact

    // Recursively update the right subtree and return the final index
    return refreshIndex(root->rightPtr, currentIndex);
}

// Displays the menu and gets the user option
int getOption(int currentOption, char input[10])
{
    // Prints the menu display
    printf("\n%s\n", "------ Available Operations -----");
    printf("\033[36m%-9s\t%-9s\n%-9s\t%-9s\n\n%-9s\033[0m\n\n%-9s\n", "1.Save", "2.Edit", "3.Delete", "4.Search", "7.Sort by Choice", "\033[33mPress '0' to Exit\033[0m");
    printf("%s\n? ", "---------------------------------");

    // Gets the input from the user using input as buffer
    fgets(input, 10, stdin);

    if (!isInteger(input)) {    // Check if the input is an integer
        printf("(getInput) Invalid input, please enter an integer\n");
        return -1;
    }

    currentOption = atoi(input);    // Convert the input to an integer
}

// Get the user input for the contact details
void getInput(char *input, const char *message)
{
    printf("%s", message);  // Print the message to the user
    fgets(input, 100, stdin);   // Get the input from the user
    input[strcspn(input, "\n")] = '\0';     // Remove the newline character from the input
}

// Function to decrypt a file using XOR decryption
void decryptFile(const char *filename)
{
    FILE *fPtr = fopen(filename, "rb");     // Open the file in binary read mode
    if (fPtr == NULL) {     // Check if file opening was succesful
        perror("Error opening file for decryption");
        exit(EXIT_FAILURE);
    }

    // Determine the file size
    fseek(fPtr, 0, SEEK_END);   // Move the file pointer to the end of the file
    long fSize = ftell(fPtr);   // Get the current position of the file pointer, get the file size
    fseek(fPtr, 0, SEEK_SET);   // Move the file pointer back to the beginning of the file

    char *buffer = (char *)malloc(fSize);   // Allocate memory for the file contents
    if (buffer == NULL) {   // Check if memory allocation failed
        perror("Memory allocation failed");
        fclose(fPtr);
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, fSize, fPtr);  // Read the file into buffer
    fclose(fPtr);

    // Decrypt the buffer using XOR encryption
    for (long i = 0; i < fSize; i++) {
        buffer[i] ^= ENCRYPTION_KEY;
    }

    // Write the decrypted buffer back to the file
    fPtr = fopen(filename, "wb");   // Open the file in binary write mode
    if (fPtr == NULL) {    // Check if file opening was succesful
        perror("Error opening file for writing");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    fwrite(buffer, 1, fSize, fPtr); // Write the decrypted buffer back to the file
    fclose(fPtr);

    free(buffer);   // Free the buffer memory
}

// Function to decrypt a file using XOR encryption
void encryptFile(const char *filename)
{
    FILE *fPtr = fopen(filename, "rb"); // Open the file in binary read mode
    if (fPtr == NULL) {   // Check if file opening was succesful
        perror("Error opening file for encryption");
        exit(EXIT_FAILURE);
    }

    // Determine the file size
    fseek(fPtr, 0, SEEK_END);   // Move the file pointer to the end of the file   
    long fSize = ftell(fPtr);   // Get the current position of the file pointer, get the file size
    fseek(fPtr, 0, SEEK_SET);   // Move the file pointer back to the beginning of the file
    
    // Read the entire file into memory
    char *buffer = (char *)malloc(fSize);   // Allocate memory for the file contents
    if (buffer == NULL) {   // Check if memory allocation failed
        perror("Memory allocation failed");
        fclose(fPtr);
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, fSize, fPtr);  // Read the file into buffer
    fclose(fPtr);

    // Encrypt the buffer using XOR encryption
    for (long i = 0; i < fSize; i++) {
        buffer[i] ^= ENCRYPTION_KEY;
    }

    // Write the encrypted buffer back to the file
    fPtr = fopen(filename, "wb");   // Open the file in binary write mode
    if (fPtr == NULL) {    // Check if file opening was succesful
        perror("Error opening file for writing");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    fwrite(buffer, 1, fSize, fPtr); // Write the encrypted buffer back to the file
    fclose(fPtr);

    free(buffer);   // Free the buffer memory
}
