#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;


//make directory
void mkdir(char pathName[]) {
    if (strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    struct NODE* current = splitPath(pathName); // deepest existing node
    char temp[128];
    strcpy(temp, pathName);
    char* token = strtok(temp, "/");

    // Skip tokens that already exist
    struct NODE* walker = (pathName[0] == '/') ? root : cwd;
    while (token != NULL && walker != current) {
        struct NODE* child = walker->childPtr;
        while (child != NULL) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                walker = child;
                break;
            }
            child = child->siblingPtr;
        }
        token = strtok(NULL, "/");
    }

    // Create remaining tokens
    while (token != NULL) {
        struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
        strcpy(newNode->name, token);
        newNode->fileType = 'D';
        newNode->parentPtr = current;
        newNode->childPtr = NULL;
        newNode->siblingPtr = NULL;

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
        token = strtok(NULL, "/");
    }

    printf("MKDIR SUCCESS: full path %s created\n", pathName);
}


//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char pathName[]) {
    struct NODE* current = (pathName[0] == '/') ? root : cwd;
    char temp[128];
    strcpy(temp, pathName);
    char* token = strtok(temp, "/");

    while (token != NULL) {
        struct NODE* child = current->childPtr;
        struct NODE* found = NULL;

        while (child != NULL) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                found = child;
                break;
            }
            child = child->siblingPtr;
        }

        if (found) {
            current = found;
        } else {
            break; // Stop at first missing component
        }

        token = strtok(NULL, "/");
    }

    return current;
}

