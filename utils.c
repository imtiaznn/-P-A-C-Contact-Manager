#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#define SIZE 5

//Creates a node on a binary tree with contact as stored data
TreeNode* createNode(Contact *c) {
    TreeNode* newNode = (TreeNode*) malloc(sizeof(TreeNode));

    newNode->contact = (Contact*) malloc(sizeof(Contact));

    strcpy(newNode->contact->name, c->name);
    strcpy(newNode->contact->phoneNum, c->phoneNum);
    strcpy(newNode->contact->email, c->email);

    newNode->leftPtr = newNode->rightPtr = NULL;

    return newNode;
}

//Searches for a node based on name strating with the root
TreeNode* searchNode(TreeNode* root, const char* name) {

    //Returns the root if theres no tree or if node is found
    if (root == NULL || root->contact->name == name) {
        return root;
    }

    //Continues searching towards node to the left or right of the root
    if (strcmp(root->contact->name, name) > 0) {
        return searchNode(root->leftPtr, name);
    } else {
        return searchNode(root->rightPtr, name);
    }
}

//Inserts a node at its appropiate position based on the name
TreeNode* insertNode(TreeNode* root, const char* name){
    //Create a tree if no tree is present
    if (root == NULL) {
        return createNode(root->contact);
    }

    //Returns the contact if its already present in the tree
    if (root->contact->name == name) {
        return root;
    }

    if (strcmp(root->contact->name, name) < 0) {
        //If the key to be inserted is greater than the name, insert it to the right subtree
        root->rightPtr = insertNode(root->rightPtr, name);
    } else {
        //If the key to be inserted is lesser than the name, insert it to the left subtree
        root->leftPtr = insertNode(root->leftPtr, name);
    }

    return root;
}