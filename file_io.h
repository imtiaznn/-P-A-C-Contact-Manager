#ifndef FILE_IO_H
#define FILE_IO_H
#include "structs.h"

void saveContact(const char* name, const char* phoneNum, const char* email);

int loadCSV(TreeNode** root, int* count);

int updateCSVLoop(TreeNode* root, FILE* fPtr);

int updateCSV(TreeNode* root);

void displayContacts(TreeNode* root, int currentPage, const char query[100]);

int refreshIndex(TreeNode* root, int currentIndex);

int getOption(int currentOption, char input[10]);

void getInput(char* input, const char* message);

#endif