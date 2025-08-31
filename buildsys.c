#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Determines the max length of the command that could be output to gcc
#define MAX_COMMAND_LEN 3000
//Determines the maximum number of source files that could be compiled
#define SRC_INDICES_LEN 100

//Compile an executable named build with the command below:
// gcc buildsys.c -o build

//Currently the build command system only supports gcc, but cl.exe support may be added at a later date

//For profile, the user can specify a named profile if they want, and if they don't profile can be NULL, telling
//the function to simply look for the first profile it finds
void getBuildInfo(char* profile, char* file, char*** srcFiles, char*** includeFolders, char*** libFolders, char*** libs, char*** args, char** out);

bool strFindReplace(char** src, char* find, char* replace);

void prepFileString(char** str);

// The offset is for if you want to see if there is another index of the find string after the first had been found
int getIndexOf(char* src, char* find, int offset);

void getLineInfo(char*** twoDArray, char* identifier, int offset);

int main(void) {
    char* fstring = NULL;
    prepFileString(&fstring);

    char** includeFolders = NULL;
    char** libFolders = NULL;
    char** libs = NULL;
    char** srcFiles = NULL;
    char** arguments = NULL;
    char* out = NULL;

    getBuildInfo(NULL, fstring, &srcFiles, &includeFolders, &libFolders, &libs, &arguments, &out);
    free(fstring);

    for (int i = 0; srcFiles[i] != NULL; i++) {
        printf("%s\n", srcFiles[i]);
    }
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

void prepFileString(char** str) {
    FILE* fptr = fopen("build.set", "rb");
    if (fptr == NULL) {
        printf("There was an error opening the file build.set\nMake sure that the file exists with that specific name\nand that it is in the same directory as build.exe\n");
        exit(-1);
    }
    fseek(fptr, 0L, SEEK_END);
    int fsize = ftell(fptr) / sizeof(char);
    fseek(fptr, 0L, SEEK_SET);

    char* fbuffer = (char*)malloc((fsize+1) * sizeof(char));
    if (fbuffer == NULL) {
        printf("Failed to allocate memory for string buffer\n");
        exit(-1);
    }

    fread(fbuffer, sizeof(char), fsize, fptr);
    fbuffer[fsize] = '\0';
    fclose(fptr);

    //Clean up the file string by getting rid of spaces and carriage return characters
    while(strFindReplace(&fbuffer, " ", ""));
    while(strFindReplace(&fbuffer, "\r", ""));

    *str = fbuffer;
}

int getIndexOf(char* src, char* find, int offset) {
    int srcLen = strlen(src);
    int findLen = strlen(find);

    for (int i = offset; i < srcLen - findLen; i++) {
        int count = 0;
        for (int j = 0; j < findLen; j++) {
            if (src[i + j] == find[j]) {
                count++;
            } else {
                break;
            }
        }

        if (count == findLen) {
            return i;
        }
    }

    //-1 indicates that the string find was not found in the src string
    return -1;
}

//Syntax:
//profile : m1 = {
//    src = main.c, glad.c
//    include = C:\Users\colec\..
//    libFolder = C:\Users\colec\..
//    libs = libglfw3.a, libgdi32.a
//    args = -Werror
//    out = main
//}
void getBuildInfo(char* profile, char* file, char*** srcFiles, char*** includeFolders, char*** libFolders, char*** libs, char*** args, char** out) {
    if (profile == NULL) {
        //This will find the first instance of a profile, and knowing the name of the profile is not even necessary because
        //if no profile is named, then the first profile defined will be used to get the information. It does this by simply
        //looking for the first instances of each of the possible parameters

        //This will be used to make sure that if there are multiple profiles, the program will never search beyond this
        //for either the src files, includes, etc.
        int closedCurlyIndex = getIndexOf(file, "}", 0);
        if (closedCurlyIndex == -1) {
            printf("Failed to find closing curly brace (}). Please make sure all profiles are enclosed by curly braces\n");
            exit(-1);
        }

        //Find and parse the src files first
        int srcFileIndex = getIndexOf(file, "src=", 0);
        if (srcFileIndex == -1) {
            printf("Failed to find src files. Make sure that your profile includes 'src=main.c,file2.c' and so on depending on the number of source files\n");
            exit(-1);
        }
        int srcCount = 0;
        int srcIndices[SRC_INDICES_LEN];
        for (int i = srcFileIndex+4; i < closedCurlyIndex + 1; i++) {
            if (file[i] == ',' || file[i] == '\n' || file[i] == '}') {
                if (srcCount >= SRC_INDICES_LEN) {
                    printf("Program is unable to handle more than %d source files in one compilation.\n", SRC_INDICES_LEN);
                    exit(-1);
                }

                srcIndices[srcCount] = i;
                srcCount++;

                //Make sure the loop stops once a newline is reached or an ending curly brace in order to prevent it from
                //going onto lines other the one that defines the src files
                if (file[i] == '\n' || file[i] == '}') {
                    break;
                }
            }
        }
        if (srcCount == 0) {
            printf("Found 'src=', but no files were specified. Please enter at least one source file to be compiled.\n");
            exit(-1);
        }

        *srcFiles = (char**)malloc((srcCount+1) * sizeof(char*));
        (*srcFiles)[srcCount] = NULL;
        if (*srcFiles == NULL) {
            printf("Failed to allocate memory for source files 2D array. (location 1)\n");
            exit(-1);
        }

        for (int i = 0; i < srcCount; i++) {
            if (i == 0) {
                int strLen = srcIndices[0] - (srcFileIndex + 4);
                (*srcFiles)[0] = (char*)malloc((strLen + 1) * sizeof(char));
                if ((*srcFiles)[0] == NULL) {
                    printf("Failed to allocate memory for source files 2D array. (location 2)\n");
                    exit(-1);
                }
                for (int j = srcFileIndex+4; j < srcIndices[0]; j++) {
                    (*srcFiles)[0][j - srcFileIndex - 4] = file[j];
                }
                (*srcFiles)[0][strLen] = '\0';
            } else {
                int strLen = srcIndices[i] - (srcIndices[i-1] + 1);
                (*srcFiles)[i] = (char*)malloc((strLen + 1) * sizeof(char));
                if ((*srcFiles)[i] == NULL) {
                    printf("Failed to allocate memory for source files 2D array. (location 3)\n");
                    exit(-1);
                }
                for (int j = srcIndices[i-1]+1; j < srcIndices[i]; j++) {
                    (*srcFiles)[i][j - srcIndices[i-1] -1] = file[j];
                }
                (*srcFiles)[i][strLen] = '\0';
            }
        }

    } else {

    }
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
                memcpy(*src + i, replace, replaceLen);
                return true;
            } else if (replaceLen > findLen) {
                (*src) = (char*)realloc((*src), (srcLen - findLen + replaceLen + 1) * sizeof(char));
                if ((*src) == NULL) {
                    printf("Failed to allocate memory in strFindReplace\n");
                    exit(-1);
                }

                char* tempstr2 = (char*)malloc((srcLen - i - findLen + 1) * sizeof(char));
                memcpy(tempstr2, *src + i + findLen, srcLen - i - findLen);

                memcpy(*src + i, replace, replaceLen);
                srcLen = srcLen - findLen + replaceLen;
                memcpy(*src + i + replaceLen, tempstr2, srcLen - i - replaceLen);

                free(tempstr2);
                (*src)[srcLen] = '\0';
                return true;
            } else if (replaceLen < findLen) {
                char* tempstr2 = (char*)malloc((srcLen - i - findLen + 1) * sizeof(char));
                memcpy(tempstr2, *src + i + findLen, srcLen - i - findLen);

                memcpy(*src + i, replace, replaceLen);
                memcpy(*src + i + replaceLen, tempstr2, srcLen - i - findLen);
                free(tempstr2);
                (*src)[srcLen - findLen + replaceLen] = '\0';
                return true;
            }
        }
    }

    return false;
}

void getLineInfo(char*** twoDArray, char* identifier, int offset) {

}