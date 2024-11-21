#ifndef FILE_IO_H
#define FILE_IO_H
#include "structs.h"

void saveContact(const char* name, const char* phoneNum, const char* email);

void loadCSV(TreeNode** root, int* count);

void updateCSVLoop(TreeNode* root, FILE* fPtr);

void updateCSV(TreeNode* root);

void displayContacts(TreeNode* root, int* count, int currentPage);

int refreshIndex(TreeNode* root, int currentIndex);

int getOption(int currentOption, char input[10]);

void getInput(char* buffer, const char* message);

#endif