#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW

    if (strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64], dirName[128];
    struct NODE* parent = splitPath(pathName, baseName, dirName);
    if (parent == NULL) {
        return;
    }

    struct NODE* child = parent->childPtr;
    while (child != NULL) {
        if (strcmp(child->name, baseName) == 0 && child->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
            return;
        }
        child = child->siblingPtr;
    }

    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newNode->name, BaseName);
    newNode->fileType = 'D';
    newNode->parentPTR = parent;
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;

    if (parent->childPtr == NULL) {
        parent->childPtr = newNode;
    } else {
        struct NODE* last = parent->childPtr;
        while (last->siblingPtr != NULL) {
            last->siblingPtr;
        }
        last->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", baseName);
}

    
    //printf("TO BE IMPLEMENTED\n");

    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED SOLUTION EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    // YOUR CODE HERE
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root:
    }
    
    char* lastSlash = strrchr(pathName, '/');
    if (lastSlash == NULL) {
        strcpy(dirName, "");
        strcpy(baseName, pathName);
        return cwd;
    }
    
    strcpy(baseName, lastSlash + 1);
    size_t dirLen = lastSlash - pathName;
    strncpy(dirName, pathName, dirLen);
    dirName[dirLen] = '\0';

    struct NODE* current;
    if (pathName[0] == '/') {
        current = root;
    } else {
        current = cwd;
    }

    char temp[128];
    strcpy(temp, dirName);
    char* token = strtok(temp, "/");
    while (token != NULL) {
        struct NODE* child = current->childPtr;
        int found = 0;
        while (child != NULL) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                current = child;
                found = 1;
                break;
            }
            child = child->siblingPtr;
        }
        if (!found) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }
        token = strtok(NULL, "/");
    }

    return current;
}
    //
    return NULL;
}
