#ifndef FILE_IO_H
#define FILE_IO_H
#include "structs.h"

void saveContact(const char *name, const char *phoneNum, const char *email);

Contact* createContact(const char *name, const char *phoneNum, const char *email);

void freeContact(Contact *c);

#endif