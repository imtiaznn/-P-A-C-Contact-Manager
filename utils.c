#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    // Allocate memory for each field in the Contact struct
    c->index = count;
    c->name = malloc(strlen(name) + 1);
    c->phoneNum = malloc(strlen(phoneNum) + 1);
    c->email = malloc(strlen(email) + 1);

    // Check for memory allocation failure
    if (c->name == NULL || c->phoneNum == NULL || c->email == NULL) {
        free(c->name);
        free(c->phoneNum);
        free(c->email);
        free(c);
        return NULL;
    }

    // Copy the strings into the struct fields
    strcpy(c->name, name);
    strcpy(c->phoneNum, phoneNum);
    strcpy(c->email, email);

    return c;
}

//Creates a node on a binary tree with contact as stored data
TreeNode* createNode(Contact *c) {
    //Allocates memory for the new node
    TreeNode* newNode = (TreeNode*) malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        return NULL;
    }

    //Stores the contact and sets the value of the pointers to NULL
    newNode->contact = c;
    newNode->leftPtr = newNode->rightPtr = NULL;

    return newNode;
}

//Inserts a node at its appropiate position based on the name
TreeNode* insertNode(TreeNode* root, TreeNode* newNode) {

    //If root is NULL, then the newNode will be inserted at the position
    //This is as NULL implies a children of a leaf node
    if (root == NULL) {
        // Insert the new node at the appropriate place
        return newNode;
    }

    // Compare names to determine where to insert the new node - left or right pointer
    if (strcmp(root->contact->name, newNode->contact->name) > 0) {

        root->leftPtr = insertNode(root->leftPtr, newNode);
    
    } else if (strcmp(root->contact->name, newNode->contact->name) < 0) {
    
        root->rightPtr = insertNode(root->rightPtr, newNode);
    
    }

    return root;
}

//Searches for a node based on name strating with the root
TreeNode* searchNode(TreeNode* root, const char* name) {

    //Returns the root if theres no tree or if node is found
    if (root == NULL || strcmp(root->contact->name, name) == 0) {
        return root;
    }

    //Continues searching towards node to the left or right of the root
    if (strcmp(root->contact->name, name) > 0) {
        return searchNode(root->leftPtr, name);
    } else {
        return searchNode(root->rightPtr, name);
    }
    
}

//DEBUGGING PURPOSES
// Function to visualize the contents of the binary tree
void inorderTrav(TreeNode* root) {
    if (root == NULL) {
        return; // Base case: if the tree is empty or we've reached a leaf
    }

    // Recursively visit the left subtree
    inorderTrav(root->leftPtr);

    // Print the current node's contact information
    printf("Name: %s\nPhone Number: %s\nEmail: %s\n", 
           root->contact->name, 
           root->contact->phoneNum, 
           root->contact->email);

    // Recursively visit the right subtree
    inorderTrav(root->rightPtr);
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
