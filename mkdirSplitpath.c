#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

void mkdir(char pathName[]) {
    if (strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    struct NODE* current = (pathName[0] == '/') ? root : cwd;
    char temp[128];
    strcpy(temp, pathName);
    char* token = strtok(temp, "/");

    while (token != NULL) {
        struct NODE* child = current->childPtr;
        struct NODE* found = NULL;

        // Search for existing directory
        while (child != NULL) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                found = child;
                break;
            }
            child = child->siblingPtr;
        }

        if (found) {
            current = found; // Move deeper
        } else {
            // Create new directory
            struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
            strcpy(newNode->name, token);
            newNode->fileType = 'D';
            newNode->parentPtr = current;
            newNode->childPtr = NULL;
            newNode->siblingPtr = NULL;

            // Insert into child list
            if (current->childPtr == NULL) {
                current->childPtr = newNode;
            } else {
                struct NODE* last = current->childPtr;
                while (last->siblingPtr != NULL) {
                    last = last->siblingPtr;
                }
                last->siblingPtr = newNode;
            }

            printf("MKDIR: created directory %s\n", token);
            current = newNode;
        }

        token = strtok(NULL, "/");
    }

    printf("MKDIR SUCCESS: full path %s created\n", pathName);
}



/*
//make directory
void mkdir(char pathName[]) {
    if (strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64], dirName[128];
    struct NODE* parent = splitPath(pathName, baseName, dirName);
    if (parent == NULL) {
        // Error already printed by splitPath
        return;
    }

    // Check if baseName already exists under parent
    struct NODE* child = parent->childPtr;
while (child != NULL) {
    if (strcmp(child->name, baseName) == 0 && child->fileType == 'D') {
        printf("MKDIR ERROR: directory %s already exists\n", baseName);
        return;
    }
    child = child->siblingPtr;
}


    // Create new directory node
    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->parentPtr = parent;
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;

    // Insert into parent's child list
    if (parent->childPtr == NULL) {
        parent->childPtr = newNode;
    } else {
        struct NODE* last = parent->childPtr;
        while (last->siblingPtr != NULL) {
            last = last->siblingPtr;
        }
        last->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", baseName);
}
*/

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    // Handle root case
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    // Find last slash to split baseName and dirName
    char* lastSlash = strrchr(pathName, '/');
if (lastSlash == NULL) {
    strcpy(dirName, "");
    strcpy(baseName, pathName);
    return cwd;
}

    // Extract baseName and dirName
    strcpy(baseName, lastSlash + 1);
    size_t dirLen = lastSlash - pathName;
    strncpy(dirName, pathName, dirLen);
    dirName[dirLen] = '\0'; // null-terminate

    // Start traversal
    struct NODE* current;
    if (pathName[0] == '/') {
        current = root;
    } else {
        current = cwd;
    }

    // Tokenize dirName and traverse
    if (strlen(dirName) == 0) {
        return cwd;
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
