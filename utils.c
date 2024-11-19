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
