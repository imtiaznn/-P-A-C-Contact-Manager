#ifndef UTILS_H
#define UTILS_H

#include "structs.h"

Contact* createContact(int count, const char* name, const char* phoneNum, const char* email);

TreeNode* createNode(Contact *c);

TreeNode* insertNode(TreeNode* root, TreeNode* newNode);

TreeNode* searchNode(TreeNode* root, const char* name);

void updateCSVLoop(TreeNode* root, FILE* fPtr);

void updateCSV(TreeNode* root);

void inorderTrav(TreeNode* root);

TreeNode* deleteContact(TreeNode* root, const char* name);

void freeContact(Contact* c);

#endif