#ifndef FILE_IO_H
#define FILE_IO_H
#include "structs.h"


void saveContact(const char* name, const char* phoneNum, const char* email);

int loadCSV(TreeNode** root, int* count);

int updateCSVHelper(TreeNode* root, FILE* fPtr);

int updateCSV(TreeNode* root);

void displayContacts(TreeNode* root, int currentPage, const char query[100], int* count, int mode);

void displayContactsHelper(TreeNode* root, int currentPage, const char query[100], int* matched, int* count, int mode);

int refreshIndex(TreeNode* root, int currentIndex);

int getOption(int currentOption, char input[10]);

void getInput(char* input, const char* message);

void build_decryption_key();

char encrypt_single_character(char c);

char decrypt_single_character(char c);

void process_string(char *str, int encrypt);

void process_csv(const char *filename, int encrypt);
#endif