#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

// The root and cwd nodes are defined in main.c as extern
extern struct NODE *root, *cwd;

// Helper: find a child node by name under a given directory
struct NODE* findChild(struct NODE* parent, const char* name) {
    struct NODE* cur = parent->childPtr;
    while (cur) {
        if (strcmp(cur->name, name) == 0)
            return cur;
        cur = cur->siblingPtr;
    }
    return NULL;
}

/*
 * splitPath()
 * Given pathName, separate it into dirName (the directory path)
 * and baseName (the final name in the path).
 * Return a pointer to the NODE representing the parent directory of baseName.
 * Print error if a directory in the path doesn't exist.
 */
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    // Handle root edge case
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        baseName[0] = '\0';
        return root;
    }

    // Find the last '/' in the path
    char *lastSlash = strrchr(pathName, '/');

    if (lastSlash == NULL) {
        // No '/' means it's a relative name in the current directory
        dirName[0] = '\0';
        strcpy(baseName, pathName);
        return cwd;
    }

    // There is a slash somewhere
    if (lastSlash == pathName) {
        // Path starts with '/', like "/a"
        strcpy(dirName, "/");
        strcpy(baseName, lastSlash + 1);
    } else {
        // Split into dirName and baseName
        strncpy(dirName, pathName, lastSlash - pathName);
        dirName[lastSlash - pathName] = '\0';
        strcpy(baseName, lastSlash + 1);
    }

    // Traverse the dirName path from root
    struct NODE* current;
    if (pathName[0] == '/')
        current = root;
    else
        current = cwd;

    if (strcmp(dirName, "/") == 0 || strlen(dirName) == 0)
        return current;

    char dirCopy[128];
    strcpy(dirCopy, dirName);

    char *token = strtok(dirCopy, "/");
    while (token != NULL) {
        struct NODE* child = findChild(current, token);
        if (!child || child->fileType != 'D') {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }
        current = child;
        token = strtok(NULL, "/");
    }

    return current;
}

/*
 * mkdir()
 * Create a directory node at the given pathName.
 */
void mkdir(char* pathName) {
    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char dirName[128], baseName[64];
    struct NODE* parent = splitPath(pathName, baseName, dirName);

    // If splitPath failed, stop here
    if (parent == NULL)
        return;

    // If baseName is empty (e.g., mkdir /), error
    if (strlen(baseName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    // Check if directory already exists
    struct NODE* existing = findChild(parent, baseName);
    if (existing != NULL) {
        printf("MKDIR ERROR: directory %s already exists\n", baseName);
        return;
    }

    // Create new node
    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;
    newNode->parentPtr = parent;

    // Attach newNode to parent's child list
    if (parent->childPtr == NULL) {
        parent->childPtr = newNode;
    } else {
        struct NODE* cur = parent->childPtr;
        while (cur->siblingPtr != NULL)
            cur = cur->siblingPtr;
        cur->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}
