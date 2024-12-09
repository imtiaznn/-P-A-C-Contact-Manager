#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "utils.h"
#define CONTACT_PER_PAGE 5

// Saves a contact into the CSV file, does not save it into the BST however
void saveContact(const char *name, const char *phoneNum, const char *email)
{
    // Encryption key and IV (Initialization Vector)
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901"; // 32 bytes key
    unsigned char *iv = (unsigned char *)"0123456789012345";                  // 16 bytes IV

    // Concatenate the contact information
    char plaintext[300];
    snprintf(plaintext, sizeof(plaintext), "%s,%s,%s\n", name, phoneNum, email);

    // Buffer for ciphertext
    unsigned char ciphertext[300];
    int ciphertext_len = encrypt((unsigned char *)plaintext, strlen(plaintext), key, iv, ciphertext);

    // Open the file for appending
    FILE *fPtr = fopen("contacts.csv", "a");
    if (fPtr == NULL)
        return;

    // Write the encrypted data to the file
    fwrite(ciphertext, 1, ciphertext_len, fPtr);

    fclose(fPtr);
}

// Loads the contacts form the CSV file into the BST
int loadCSV(TreeNode **root, int *count)
{
    // Encryption key and IV (Initialization Vector)
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901"; // 32 bytes key
    unsigned char *iv = (unsigned char *)"0123456789012345";                  // 16 bytes IV

    char line[1024];
    FILE *fPtr = fopen("contacts.csv", "r");
    if (fPtr == NULL)
        return -1;

    while (fgets(line, sizeof(line), fPtr))
    {
        // Buffer for decrypted text
        unsigned char decryptedtext[300];
        int decryptedtext_len = decrypt((unsigned char *)line, strlen(line), key, iv, decryptedtext);
        decryptedtext[decryptedtext_len] = '\0';

        // Parse the decrypted line
        char *name = strtok((char *)decryptedtext, ",");
        char *phoneNum = strtok(NULL, ",");
        char *email = strtok(NULL, "\n");

        if (name == NULL || phoneNum == NULL || email == NULL)
        {
            printf("(loadCSV) Malformed line in CSV: %s\n", line);
            continue;
        }

        Contact *c = createContact(*count, name, phoneNum, email);
        if (c == NULL)
        {
            printf("(loadCSV): Failed to allocate memory for contact");
            continue;
        }

        (*count)++;
        TreeNode *newNode = createNode(c);
        if (newNode == NULL)
        {
            printf("(loadCSV): Failed to create node for contact");
            free(c);
            continue;
        }

        *root = insertNode(*root, newNode);
    }

    fclose(fPtr);
    return 0;
}

// Loop used in the updateCSV function, separated to prevent opening file multiple times during recursion
int updateCSVHelper(TreeNode *root, FILE *fPtr)
{
    if (root == NULL)
        return 0;

    // Recursively visits the left subtree
    updateCSVHelper(root->leftPtr, fPtr);

    char email[strlen(root->contact->email) + 1];
    strcpy(email, root->contact->email);
    email[strcspn(email, "\n")] = '\0';

    // Write the current node's contact information to the file
    fprintf(fPtr, "%s,%s,%s\n", root->contact->name, root->contact->phoneNum, email);

    // Recursively visit the right subtree
    updateCSVHelper(root->rightPtr, fPtr);

    return 0;
}

// Uses In-Order traversal to write the values in BST to CSV
// Update the file after changes are made
int updateCSV(TreeNode *root)
{

    // Open the file for writing
    FILE *fPtr = fopen("contacts.csv", "w");
    if (fPtr == NULL)
        return -1;

    // Call the helper function to perform the in-order traversal
    if (updateCSVHelper(root, fPtr) == -1)
    {
        return -1;
    }

    fclose(fPtr);
    return 0;
}

// Helper functin to display the contacts in the BST
void displayContactsHelper(TreeNode *root, int currentPage, const char query[100], int *matched, int *count)
{
    if (root == NULL)
        return;

    // Recursively visit the left subtree
    displayContactsHelper(root->leftPtr, currentPage, query, matched, count);

    // Calculate the range of contacts for the current page
    int start = currentPage * CONTACT_PER_PAGE;
    int end = start + CONTACT_PER_PAGE;

    // Make a duplicate of the email to remove the newline character on display
    char email[strlen(root->contact->email) + 1];
    strcpy(email, root->contact->email);
    email[strcspn(email, "\n")] = '\0';

    // Perform partial matching if a query is provided
    if (strlen(query) != 0)
    {
        // Check if the query is a substring of the name, phone number, or email
        if (strstr(root->contact->name, query) != NULL ||
            strstr(root->contact->phoneNum, query) != NULL ||
            strstr(email, query) != NULL)
        {

            *matched = 1; // Mark that at least one match was found

            // Print if the contact falls within the range for the current page
            if (*count >= start && *count < end)
            {
                printf("%-2d - %-39s%-21s%s\n",
                       root->contact->index + 1,
                       root->contact->name,
                       root->contact->phoneNum,
                       email);
            }

            (*count)++;
        }
    }
    else
    {
        // Display all contacts if no query is provided
        if (*count >= start && *count < end)
        {
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

// Display the contacts in the BST using inorder traversal
void displayContacts(TreeNode *root, int currentPage, const char query[100], int *count, int mode)
{
    if (root == NULL)
    {
        printf("No contacts found\n");
        return;
    }

    // Reset flag and counter
    int matched = 0;
    *count = 0;

    if (mode > 1)
    {
        // Alternate sorting logic
        *count = countTreeNodes(root);
        int size = countTreeNodes(root);
        TreeNode **arr = alternateSort(root, mode);

        if (arr == NULL || size == 0)
        {
            printf("(displayContacts) Failed to sort contacts\n");
            return;
        }

        int start = currentPage * CONTACT_PER_PAGE;
        int end = start + CONTACT_PER_PAGE;

        for (int i = start; i < end && i < size; i++)
        {
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

        free(arr);
    }
    else
    {
        // Use in-order traversal
        displayContactsHelper(root, currentPage, query, &matched, count);
    }

    // Print a message if no contacts were found
    if (matched == 0 && strlen(query) != 0)
    {
        printf("\nNo contacts found matching '%s'...\n", query);
    }

    if (*count != 0)
    {
        printf("\nThere are %d contacts found.\n", *count);
    }
}

// Function to update indices of all contacts in a single traversal
int refreshIndex(TreeNode *root, int currentIndex)
{
    if (root == NULL)
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
    printf("\n\n%s\n", "------ Available Operations -----");
    printf("\033[36m%-9s\t%-9s\n%-9s\t%-9s\n\n%-9s\033[0m\n\n%-9s\n", "1.Save", "2.Edit", "3.Delete", "4.Search", "7.Sort by Choice", "\033[33mPress '0' to Exit\033[0m");
    printf("%s\n? ", "---------------------------------");

    // Gets the input from the user using input as buffer
    fgets(input, 10, stdin);

    if (!isInteger(input))
    {
        printf("(getInput) Invalid input, please enter an integer\n");
        return -1;
    }

    currentOption = atoi(input);
}

void getInput(char *input, const char *message)
{
    printf("%s", message);
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';
}
