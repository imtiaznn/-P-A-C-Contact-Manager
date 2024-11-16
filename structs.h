#ifndef STRUCTS_H
#define STRUCTS_H

//Creates a struct to store contact info
typedef struct contact {
    int index;
    char *name;
    char *phoneNum;
    char *email;
} Contact;

//Creates a struct to store a node for binary search trees
typedef struct treeNode {
    Contact *contact;
    struct treeNode *leftPtr;
    struct treeNode *rightPtr;
} TreeNode;

#endif