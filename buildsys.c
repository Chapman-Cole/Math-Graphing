#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LEN 3000

//Compile an executable named build with the command below:
// gcc buildsys.c -o build

//Currently the build command system only supports gcc, but cl.exe support may be added at a later date

void getBuildInfo(char** profile, char** srcFiles, char** includeFolders, char** libFolders, char** libs, char** args);

bool strFindReplace(char** src, char* find, char* replace);

int main(void) {

    char* profile;
    getBuildInfo(&profile, NULL, NULL, NULL, NULL, NULL);
    printf("%s\n\n", profile);

    while(strFindReplace(&profile, "this", "thiss"));
    printf("%s\n\n", profile);
    /*
    //This is for the headers in you program that might lie in a different folder (which you specify here)
    const char* includeFolders[] = {
        "C:\\ProgrammingUtils\\OpenGL\\includes"
    };
    int includeFoldersLen = sizeof(includeFolders)/sizeof(char*);

    //This is where the you specify the folders in which those libraries live
    const char* libFolders[] = {
        "C:\\ProgrammingUtils\\OpenGL\\lib"
    };
    int libFoldersLen = sizeof(libFolders)/sizeof(char*);

    //The libraries your code is linking with. Make sure to right out the full name including the file extension
    const char* libs[] = {
        //Note: for some reason when compiling with glfw libglfw3.a has to come before libgdi32.a
        "libglfw3.a",
        "libgdi32.a"
    };
    int libsLen = sizeof(libs)/sizeof(char*);

    //The source files of your code (the .c files)
    const char* srcFiles[] = {
        "main.c",
        "C:\\ProgrammingUtils\\OpenGL\\lib\\glad.c"
    };
    int srcFilesLen = sizeof(srcFiles)/sizeof(char*);

    //Extra arguments, such as disabling certain warnings or suppressing errors. Basically anyting that doesn't fit into the prior
    //categories
    const char* arguments[] = {
    };
    int argumentsLen = sizeof(arguments)/sizeof(char*);

    int index;
    char command[MAX_COMMAND_LEN];
    command[0] = 'g';
    command[1] = 'c';
    command[2] = 'c';
    command[3] = ' ';
    index = 4;

    //This adds the source files first
    for (int i = 0; i < srcFilesLen; i++) {
        int len = strlen(srcFiles[i]);

        if (index < MAX_COMMAND_LEN) {
            command[index] = '"';
            index++;
        } else {
            printf("Length of command exceeded the max character length of the buffer\n");
            return -1;
        }

        for (int j = 0; j < len; j ++) {
            if (index < MAX_COMMAND_LEN) {
                command[index] = srcFiles[i][j];
                index++;
            } else {
                printf("Length of command exceeded the max character length of the buffer\n");
                return -1;
            }
        }

        //This handles the space between successive arguments
        if (index+1 < MAX_COMMAND_LEN) {
            command[index] = '"';
            command[index+1] = ' ';
            index = index + 2;
        } else {
            printf("Length of command exceeded the max character length of the buffer\n");
            return -1;
        }
    }

    //This adds the include folders next
    for (int i = 0; i < includeFoldersLen; i++) {
        int len = strlen(includeFolders[i]);

        if (index + 2 < MAX_COMMAND_LEN) {
            command[index] = '-';
            command[index+1] = 'I';
            command[index+2] = '"';
            index = index + 3;
        } else {
            printf("Length of command exceeded the max character length of the buffer\n");
            return -1;
        }

        for (int j = 0; j < len; j++) {
            if (index < MAX_COMMAND_LEN) {
                command[index] = includeFolders[i][j];
                index++;
            } else {
                printf("Length of command exceeded the max character length of the buffer\n");
                return -1;
            } 
        }

        //This handles the space between successive arguments
        if (index+1 < MAX_COMMAND_LEN) {
            command[index] = '"';
            command[index+1] = ' ';
            index = index + 2;
        } else {
            printf("Length of command exceeded the max character length of the buffer\n");
            return -1;
        }
    }

    //This adds the lib folders next
    for (int i = 0; i < libFoldersLen; i++) {
        int len = strlen(libFolders[i]);

        if (index + 2 < MAX_COMMAND_LEN) {
            command[index] = '-';
            command[index+1] = 'L';
            command[index+2] = '"';
            index = index + 3;
        } else {
            printf("Length of command exceeded the max character length of the buffer\n");
            return -1;
        }

        for (int j = 0; j < len; j++) {
            if (index < MAX_COMMAND_LEN) {
                command[index] = libFolders[i][j];
                index++;
            } else {
                printf("Length of command exceeded the max character length of the buffer\n");
                return -1;
            } 
        }

        //This handles the space between successive arguments
        if (index+1 < MAX_COMMAND_LEN) {
            command[index] = '"';
            command[index+1] = ' ';
            index = index + 2;
        } else {
            printf("Length of command exceeded the max character length of the buffer\n");
            return -1;
        }
    }

    //This adds the libs next
    for (int i = 0; i < libsLen; i++) {
        int len = strlen(libs[i]);

        if (index+2 < MAX_COMMAND_LEN) {
            command[index] = '-';
            command[index+1] = 'l';
            command[index+2] = ':';
            index = index + 3;
        } else {
            printf("Length of command exceeded the max character length of the buffer\n");
            return -1;
        }

        for (int j = 0; j < len; j++) {
            if (index < MAX_COMMAND_LEN) {
                command[index] = libs[i][j];
                index++;
            } else {
                printf("Length of command exceeded the max character length of the buffer\n");
                return -1;
            } 
        }

        //This handles the space between successive arguments
        if (index < MAX_COMMAND_LEN) {
            command[index] = ' ';
            index++;
        } else {
            printf("Length of command exceeded the max character length of the buffer\n");
            return -1;
        }
    }

    //This adds the extra arguments
    for (int i = 0; i < argumentsLen; i++) {
        int len = strlen(arguments[i]);
        for (int j = 0; j < len; j++) {
            if (index < MAX_COMMAND_LEN) {
                command[index] = arguments[i][j];
                index++;
            } else {
                printf("Length of command exceeded the max character length of the buffer\n");
                return -1;
            } 
        }

        //This handles the space between successive arguments
        if (index < MAX_COMMAND_LEN) {
            command[index] = ' ';
            index++;
        } else {
            printf("Length of command exceeded the max character length of the buffer\n");
            return -1;
        }
    }

    //Add the null terminator
    if (index < MAX_COMMAND_LEN) {
        command[index] = '\0';
        index++;
    } else {
        printf("Length of command exceeded the max character length of the buffer\n");
        return -1;
    }

    printf("%s\n\n", command);
    system(command);
    */
    return 0;
}

//Syntax:
//profile=**some name**
//src=main.c, glad.c, etc.
//include=C:\Users\colec\..., etc.
//libFolder=C:\Users\colec\...etc.
//libs=libglfw3.a, libgdi32.a, etc.
//args=-Werror, etc.
void getBuildInfo(char** profile, char** srcFiles, char** includeFolders, char** libFolders, char** libs, char** args) {
    FILE* fptr = fopen("build.set", "rb");
    if (fptr == NULL) {
        printf("There was an error opening the file build.set\nMake sure that the file exists with that specific name\nand that it is in the same directory as build.exe\n");
        exit(-1);
    }
    fseek(fptr, 0L, SEEK_END);
    int fsize = ftell(fptr) / sizeof(char);
    fseek(fptr, 0L, SEEK_SET);

    char* fbuffer = (char*)malloc(fsize * sizeof(char));
    if (fbuffer == NULL) {
        printf("Failed to allocate memory for string buffer\n");
        exit(-1);
    }

    fread(fbuffer, sizeof(char), fsize + 1, fptr);
    fbuffer[fsize] = '\0';
    fclose(fptr);

    *profile = fbuffer;
}

//This assumes null terminated strings
bool strFindReplace(char** src, char* find, char* replace) {
    int srcLen = strlen(*src);
    int findLen = strlen(find);
    int replaceLen = strlen(replace);

    for (int i = 0; i < srcLen-findLen; i++) {
        int count = 0;
        for (int j = 0; j < findLen; j++) {
            if (i+j < srcLen) {
                if ((*src)[i+j] == find[j]) {
                    count++;
                } else {
                    break;
                } 
            }
        }

        if (count == findLen) {
            if (findLen == replaceLen) {
                for (int j = 0; j < findLen; j++) {
                    (*src)[i+j] = replace[j];
                }
                return true;
            } else if (replaceLen > findLen) {
                char* tempstr = (char*)realloc((*src), (srcLen - findLen + replaceLen) * sizeof(char));
                if (tempstr == NULL) {
                    printf("Failed to allocate memory in strFindReplace\n");
                    exit(-1);
                }
                for (int c = 1; c <= replaceLen-findLen; c++) {
                    char prev = (*src)[i+findLen];
                    for (int j = i + findLen+c+1; j < srcLen; j++) {
                        char temp = (*src)[j];
                        (*src)[j] = prev;
                        prev = temp;
                    }
                }

                for (int j = 0; j < replaceLen; j++) {
                    (*src)[i+j] = replace[j];
                }

                return true;
            } else if (replaceLen < findLen) {
                for (int c = 1; c <= findLen-replaceLen; c++) {
                    for (int j = i + replaceLen; j < srcLen-1; j++) {
                        (*src)[j] = (*src)[j+1];
                    }
                }

                for (int j = 0; j < replaceLen; j++) {
                    (*src)[i+j] = replace[j];
                }

                (*src)[srcLen-findLen+replaceLen] = '\0';
                return true;
            }
        }
    }

    return false;
}