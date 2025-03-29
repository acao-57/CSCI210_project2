/*
The program prompt is output to stderr. So, if you want to redirect input from a file and redirect the output to a file without the prompts appearing as part of the output, you can run it like below:

./main < test_cases/test01.in 2>/dev/null > output.txt
You can use this apprach to compare your output to the expected outputs provided in the test_cases directory. Example: Run your implementation as:

./main < test_cases/test05.in 2>/dev/null > mytest5.out
diff mytest5.out test_cases/test05.out
*/

#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//arrays char arr[] = "Hello Wolrd"; char *ptr = arr
//static array int array [10] = { 0, 3, 5, 0, 0, 1, 0, 0, 2, 0}
// \0 is null terminator

//make directory
void mkdir(char pathName[]){

    //get baseName
    if (pathName == NULL || strcmp(pathName, "/") == 0) //if user does not specify any input to mkdir command, pathName string = "/"
    {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64];   //max name is 64 bytes, req.
    char dirName[500];   //allocated up to 500 bytes for directory path

    struct NODE* parent = splitPath(pathName, baseName, dirName); //uses the splitPath function to split baseName, dirName and return the parent directory of the target dir
    if (parent == NULL) //splitPath will return the error message
    {
        return;
    }

    struct NODE* child = parent->childPtr; //looks at each file/subdirectory in the local directory
    while (child != NULL)
    {
        if (strcmp(child->name, baseName) == 0 && child->fileType == 'D') //checks if the target directory already exists in current directory
        {
            printf("MKDIR ERROR: directory %s already exists\n", dirName);
            return;
        }
        child = child->siblingPtr;  //cycles thru all siblings
    }

    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    
    //initializes node with all fields set correctly
    newDir->fileType = 'D';
    newDir->parentPtr = parent;
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    strcpy(newDir->name, baseName);

    if (parent->childPtr == NULL) //if it is the first child, the childPtr of the parent should be set
    {
        parent->childPtr = newDir;
    }
    else
    {
        struct NODE* fin_sibling = parent->childPtr;
        while (fin_sibling->siblingPtr != NULL) //cycles through all siblings until the last node
        {
            fin_sibling = fin_sibling->siblingPtr;
        }
        fin_sibling->siblingPtr = newDir; //sets the next sibling (originally NULL) to the newDir
    }
    //success
    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);

    /*
struct NODE {
    char name [64];
    char fileType;
    struct NODE* childPtr; INITIALIZE TO NULL
    struct NODE* siblingPtr; INITIALIZE TO NULL
    struct NODE* parentPtr;
};
*/
    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    char* fin_slash = strrchr(pathName, '/'); //finds the last occurence of / in pathName

    if (fin_slash == NULL) //must be current directory or file if we can't find the slash
    {   
        strcpy(dirName, ""); //dirName is empty string
        //dirName = "";
        strcpy(baseName, pathName); //pathName copied to baseName
    }
    else if (fin_slash == pathName && *(fin_slash + 1) == '\0') //if the path name is just / (string terminator \0 to indicate end of string)
    {
        strcpy(dirName, "/");
        strcpy(baseName, "");
       //dirName = "/";
       //baseName = "";
    }
    else 
    {
        if (fin_slash == pathName) //if we have just have a root directory like /root
        {
            strcpy(dirName, "/"); //the dir name is the root slash
        }
        else //otherwise we know that there are multiple /.../...
        {
            strncpy(dirName, pathName, fin_slash - pathName); // /a/b/c/f1.txt gets first 6 bytes /a/b/c
            dirName[fin_slash-pathName] = '\0'; //index of the final / (end of dirName) becomes string terminator, strncpy doesn't automatically terminate
        }
        strcpy(baseName, fin_slash+1); //copies the entire remaining portion after the last /
    }

    struct NODE* parent = NULL;

    if (strcmp(dirName, "") == 0) //0 means the two are equal, and empty string means we are in current dir
    {
        return cwd;
    }
    if (strcmp(dirName, "/") == 0)// if it's just the /, then it must be the root
    {
        return root;
    }

    if(pathName[0] == '/') //starting from the root directory
    {
        parent = root;
    }
    else //otherwise local directory
    {
        parent = cwd;
    }

    char temp[500]; //initialize array, 500 bytes
    strcpy(temp, dirName);   //cpoies dirName into tempDirName, /0 in dirName will mark end of string, may not use all 500 bytes
    char* token = strtok(temp, "/"); //looks for / as the delimiter

    while (token != NULL) //for each directory that exists
    {
        struct NODE* child = parent->childPtr; //parent points to the child node
        struct NODE* found = NULL; //set found=NULL
        while (child != NULL) //look through all entries in current directory
        {
            if(strcmp(child->name, token) == 0 && child->fileType == 'D') //looking for directories that match current child with token directory
            {
                found = child;
                break;
            }
            child = child-> siblingPtr;
        }
        if (found == NULL) //if we don't have a directory matching token directory, then directory does not exist, error is NULL
        {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }
        parent = found; //directory to be the new parent, token traversal
        token = strtok(NULL, "/"); //gets the next directory
    }

/*
struct NODE {
    char name [64];
    char fileType;
    struct NODE* childPtr; INITIALIZE TO NULL
    struct NODE* siblingPtr; INITIALIZE TO NULL
    struct NODE* parentPtr;
};
*/
    return parent; //since returning null is in the while loops, returns target directory
}
