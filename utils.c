#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"

void freeContact(Contact* contact) {
    if (contact == NULL) return;

    //Free any dynamically allocated memory inside the Contact structure
    free(contact->name);
    free(contact->phoneNum);
    free(contact->email);

    //Free the Contact structure itself
    free(contact);
}

//Stores name, phoneNum and email into a Contact struct object
Contact* createContact(int count, const char* name, const char* phoneNum, const char* email) {
   
    Contact* c = malloc(sizeof(Contact));
    if (c == NULL) return NULL;

    //Allocates memory for each filed in the contact struct
    c->index = count;
    c->name = malloc(strlen(name) + 1);
    c->phoneNum = malloc(strlen(phoneNum) + 1);
    c->email = malloc(strlen(email) + 1);

    //Checks if malloc is succesfull
    if (c->name == NULL || c->phoneNum == NULL || c->email == NULL) {
        free(c->name);
        free(c->phoneNum);
        free(c->email);
        free(c);
        return NULL;
    }

    //Copy the strings into each fields
    strcpy(c->name, name);
    strcpy(c->phoneNum, phoneNum);
    strcpy(c->email, email);

    return c;
}

//Creates a node on a binary tree with contact as stored data
TreeNode* createNode(Contact *c) {

    TreeNode* newNode = (TreeNode*) malloc(sizeof(TreeNode));
    if (newNode == NULL) return NULL;

    //Stores the contact and sets the value of the left and right pointers to NULL
    newNode->contact = c;
    newNode->leftPtr = newNode->rightPtr = NULL;

    return newNode;
}

//Inserts a node at its appropiate position based on the name
TreeNode* insertNode(TreeNode* root, TreeNode* newNode) {

    //If root is NULL, then the newNode will be inserted at the current position
    //This is as NULL implies a children of a leaf node
    if (root == NULL) return newNode;

    //Compares the name to determine whether the contact will inserted to the left or right node
    if (strcmp(root->contact->name, newNode->contact->name) > 0) {

        root->leftPtr = insertNode(root->leftPtr, newNode);
    
    } else if (strcmp(root->contact->name, newNode->contact->name) < 0) {
    
        root->rightPtr = insertNode(root->rightPtr, newNode);
    
    }

    return root;
}

//Searches for a node based on name starting with the root
TreeNode* searchNode(TreeNode* root, const char* name) {

    //Returns the root if theres no tree or if node is found
    if (root == NULL || strcmp(root->contact->name, name) == 0) return root;
    
    //Continues searching towards node to the left or right of the root
    if (strcmp(root->contact->name, name) > 0) {
        return searchNode(root->leftPtr, name);
    } else {
        return searchNode(root->rightPtr, name);
    }
    
}

//Deletes a node based on the name
TreeNode* deleteContact(TreeNode* root, const char* name) {
    if (root == NULL) {
        printf("Reached a NULL node; contact not found.\n");
        return NULL;
    }

    // Compare the name with the current node's contact name
    if (strcmp(name, root->contact->name) < 0) {
        root->leftPtr = deleteContact(root->leftPtr, name);
    } else if (strcmp(name, root->contact->name) > 0) {
        root->rightPtr = deleteContact(root->rightPtr, name);
    } else {
        // Node to be deleted found
        printf("Contact '%s' found. Performing selected operation...\n", name);

        if (root->leftPtr == NULL) {
            // If the node has only right child or no children
            TreeNode* temp = root->rightPtr;
            freeContact(root->contact);
            free(root);
            return temp;
        }

        else if (root->rightPtr == NULL) {
            // If the node has only left child
            TreeNode* temp = root->leftPtr;
            freeContact(root->contact);
            free(root);
            return temp;
        }

        // Find the in-order successor (smallest in the right subtree)
        TreeNode* temp = root->rightPtr;
        while (temp->leftPtr != NULL) {
            temp = temp->leftPtr;
        }

        // Copy the in-order successor's contact to the current node
        root->contact = temp->contact;

        // Delete the in-order successor node
        root->rightPtr = deleteContact(root->rightPtr, temp->contact->name);
    }

    return root;
}

//Edits a contact based on the keyName
void editContact(TreeNode** root, const char* keyName, const char* name, const char* phoneNum, const char* email) {
    //Finds the node to be edited
    TreeNode* oldNode = searchNode(*root, keyName);
    if (oldNode == NULL) {
        printf("(editContact) Contact not found.\n");
        return;
    }

    //Creates a new node with the updated contact information
    TreeNode* newNode = createNode(createContact(oldNode->contact->index, name, phoneNum, email));

    //Deletes the old node and inserts the new node
    *root = deleteContact(*root, keyName);

    //Inserts the new node into the BST
    insertNode(*root, newNode);

    printf("(editContact) Contact successfully updated.\n");
}

//Checks if the input is an integer
int isInteger(const char* input) {
    if (strlen(input) == 0) return 0;

    //Ensures all characters are digits
    for(int i = 0 ; i < strlen(input) - 1; i++) {
        if (!isdigit(input[i])) return 0;
    }

    return 1;
}

//Checks if the input is a valid Malaysian phone number
int isValidPhoneNumber(const char* input) {

    //Prefix for Malaysian phone numbers
    char prefix[] = "+601";

    //Checks if the input is within valid Malaysian phone length 
    if (strlen(input) < 11 || strlen(input) > 14) {
        return 0;
    }

    //Checks if the input has the same prefix as required
    if (strncmp(input, prefix, 4) != 0) {
        return 0;
    }

    //Ensures all characters are digits
    for(int i = 3; i < strlen(input); i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }

    return 1;

}

//Checks if the input is a valid email address
int isValidEmailAddress(const char* input) {
    //Separates the user part and domain part
    char *str1, *str2;

    char specialChar[] = {'!', '#', '$', '%', '&', '\'', '*', '+', '-', '/', '=', '?', '^', '_', '{', '|', '}', '~'};
    
    //Duplicate the string
    char *str = strdup(input);
    if (str == NULL) return 0;

    //Separate string into user part and domain part
    str1 = strtok(str, "@");
    str2 = strtok(NULL, "@");

    if (str1 == NULL || str2 == NULL) {
        free(str);
        return 0;
    }

    //Validate user part
    for (int i = 0; i < strlen(str1); i++) {
        //Check if character is alphanumeric
        if (!isalnum(str1[i]) && str1[i] != '.') {
            int valid = 0;

            //Check if character is a special character
            for (int j = 0; j < sizeof(specialChar) / sizeof(char); j++) {
                //If character is a special character, set valid flag
                if (str1[i] == specialChar[j]) {
                    valid = 1;
                    break;
                }
            }

            if (!valid) {
                free(str);
                return 0;
            }
        }

        //Check for consecutive dots or beginnings and endings with a dot
        if (str1[i] == '.') {
            if (i == 0 || i == strlen(str1) - 1 || str1[i - 1] == '.') {
                free(str);
                return 0;
            }
        }
    }

    //Validate the domain part
    //Check if domain part has a dot
    if (strchr(str2, '.') == NULL) {
        free(str);
        return 0;
    }

    //Check for consecutive dots or beginnings and endings with a dot
    for (int i = 0; i < strlen(str2); i++) {
        if (!isalnum(str2[i]) && str2[i] != '.' && str2[i] != '-') {
            free(str);
            return 0;
        }
    

        //Check for consecutive dots or hyphens and beginnings and endings with them
        if (str2[i] == '.' || str2[i] == '-') {
            //Check for beginnings and endings with dot or hyphen
            if (i == 0 || i == strlen(str2) - 1) {
                free(str);
                return 0;
            }

            //Check for consecutive dots or hyphen
            if (str2[i - 1] == '.' || str2[i - 1] == '-') {
                free(str);
                return 0;
            }
        }
    }


    free(str);
    return 1;
}

//Checks if the input is a duplicate key in the BST
int isDuplicate(const char* input, TreeNode* root) {

    if (root == NULL) return 0;

    //Checks the right subtree
    if (isDuplicate(input, root->rightPtr)) {
        return 1;
    }

    //Checks the current node
    if (strcmp(root->contact->name, input) == 0) {
        return 1; // Duplicate found at this node
    }

    //Checks the left subtree
    if (isDuplicate(input, root->leftPtr)) {
        return 1;
    }

    return 0;
}

//Compares the values of either phoneNum or email based on mode
int compareValue(const TreeNode* a, const TreeNode* b, int mode) {
    if (a == NULL || b == NULL) return 0;

    //Compares the values of either phoneNum or email
    if (mode == 2) {
        return strcmp(a->contact->phoneNum, b->contact->phoneNum);
    } else if (mode == 3) {
        return strcmp(a->contact->email, b->contact->email);
    }

    return 0; //return 0 if mode is invalid
}

//Merges the left and right arrays into a single array
void merge(TreeNode** arr, int left, int mid, int right, int mode) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    TreeNode** L = malloc(n1 * sizeof(TreeNode*)); 
    TreeNode** R = malloc(n2 * sizeof(TreeNode*)); 

    //Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    //Merge the temporary arrays back into arr[]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        //Compare the values of the left and right arrays
        if (compareValue(L[i], R[j], mode) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    //Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    //Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L); 
    free(R);
}

//Sorts the array using mergesort
void mergesort(TreeNode** arr, int left, int right, int mode) {
    if (left < right) {
        //Finds the middle point
        int mid = left + (right - left) / 2;

        //Sort first and second halves
        mergesort(arr, left, mid, mode);
        mergesort(arr, mid + 1, right, mode);

        //Merge the sorted halves
        merge(arr, left, mid, right, mode);
    }
}

//Counts the number of nodes in the BST
int countTreeNodes(TreeNode* root) {
    if (root == NULL) return 0;
    
    //Returns the count of all nodes in the BST
    return countTreeNodes(root->leftPtr) + countTreeNodes(root->rightPtr) + 1;
}

//Stores the nodes of the BST in an array
void alternateSortHelper(TreeNode* root, TreeNode** arr, int* i) {
    if (root == NULL) return;

    // Recursively visit the left subtree
    alternateSortHelper(root->leftPtr, arr, i);

    // Store the index of the contact in the array
    arr[(*i)++] = root;

    // Recursively visit the right subtree
    alternateSortHelper(root->rightPtr, arr, i);
}

//Sorts the BST in ascending order based on the mode
TreeNode** alternateSort(TreeNode* root, int mode) {
    int count = countTreeNodes(root);
    int i = 0;

    TreeNode** arr = malloc(count * sizeof(TreeNode*));

    if (arr == NULL) {
        printf("(alternateSort) Failed to allocate memory");
        return NULL;
    }

    alternateSortHelper(root, arr, &i);

    //Sort the array in ascending order using mergesort
    mergesort(arr, 0, count - 1, mode);

    return arr;
}
