#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <string.h>
#include "structs.h"

TreeNode* createNode(Contact *c);

TreeNode* searchNode(const TreeNode* root, const char* name);

TreeNode* insertNode(TreeNode* node, const char* name);

#endif