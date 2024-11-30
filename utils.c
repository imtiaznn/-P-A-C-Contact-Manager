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
        printf("Contact '%s' found. Deleting...\n", name);

        // Case 1: Node has no left child
        if (root->leftPtr == NULL) {
            TreeNode* temp = root->rightPtr;
            freeContact(root->contact);
            free(root);
            return temp;
        }
        // Case 2: Node has no right child
        else if (root->rightPtr == NULL) {
            TreeNode* temp = root->leftPtr;
            freeContact(root->contact);
            free(root);
            return temp;
        }

        // Case 3: Node has two children
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

void editContact(TreeNode* root, const char* keyName, const char* name, const char* phoneNum, const char* email) {
    TreeNode* oldNode = searchNode(root, keyName);
    if (oldNode == NULL) {
        printf("(editContact) Contact not found.\n");
        return;
    }

    TreeNode* newNode = createNode(createContact(oldNode->contact->index, name, phoneNum, email));

    deleteContact(root, keyName);

    insertNode(root, newNode);
}

int isInteger(const char* input) {
    if (strlen(input) == 0) return 0;

    for(int i = 0 ; i < strlen(input) - 1; i++) {
        if (!isdigit(input[i])) return 0;
    }

    return 1;
}

int isValidPhoneNumber(const char* input) {

    char prefix[] = "+60";

    //Checks if the input is within valid Malaysian phone length 
    if (strlen(input) < 11 || strlen(input) > 14) {
        return 0;
    }

    //Checks if the input has the same prefix as required
    if (strncmp(input, prefix, 3) != 0) {
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

int isValidEmailAddress(const char* input) {
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
        if (!isalnum(str1[i]) && str1[i] != '.') {
            int valid = 0;

            //Check if character is a special character
            for (int j = 0; j < sizeof(specialChar) / sizeof(char); j++) {
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
    if (strchr(str2, '.') == NULL) {
        free(str);
        return 0;
    }

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

int isDuplicate(const char* input, TreeNode* root) {

    //Base case for recursion
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
