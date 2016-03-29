#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


char *matches[10];
int matchNum = 0;

char* getComponent(char component[], char* path){
    int iterator;
    char* pathPointer = path;
    for(iterator = 0; *pathPointer != '/' && *pathPointer; iterator++, pathPointer++);
    memcpy(component, path, iterator);
    component[iterator] = '\0';
    return (*pathPointer == '\0') ? pathPointer : ++pathPointer;
}


int doesMatch(char component[], char* name){
    return strncmp(component, name, strlen(component));
}

//char* getDir(char component[], char* name){
void getDir(char component[], char* name){

    DIR *directory;
    struct dirent *de;

    if (!(directory = opendir(name))){
        write(STDERR_FILENO, "TP error: cannot open directory\n", sizeof("TP error: cannot open directory\n"));
        exit(1);
    }

//    int matchNum = 0;
    while ( (de = readdir(directory)) ){
        if (de->d_type == DT_DIR) {
            //TODO: Add support for multiple matching folders
            if (doesMatch(component, de->d_name) == 0) {
                matches[matchNum] = de->d_name;
                matchNum++;
            }
        }
    }
    closedir(directory);
//    fputs("TP error: cannot match directory path\n", stderr);
//    for (int x = 0; x < 2; ++x){
//        printf("Match: %s\n", matches[x]);
//        fflush(0);
//    }

//    return NULL;
//    return (matchNum > 0) ? matches : NULL;
}

char* pickDir(char* realPath){

    while (1) {
        int counter = 1;
        char buffer[30];
        for (int x = 0; x < matchNum; ++x) {
            sprintf(buffer, "%d. %s", counter++, matches[x]);
            puts(buffer);
            fflush(0);
        }
        write(STDOUT_FILENO, "> ", 3);
        read(STDIN_FILENO, buffer, 2);
        int chosenInt = atoi(buffer);

        if (chosenInt <= matchNum) {
            matchNum = 0;
            return matches[chosenInt - 1];
        }
    }

}

int main(int argc, char **argv, char **envp){

    printf("Debug");
    if (argc == 1){
        puts(getenv("HOME"));
    } else {
        char component[30];
        char* path = *(++argv);

        char realPath[70];
        bzero(realPath, sizeof(realPath));
        if (*path == '/'){
            strcat(realPath, "/.");
            path++;
        } else {
            strcat(realPath, ".");
        }

        while (*path) {
            path = getComponent(component, path);
            strcat(realPath, "/");
            getDir(component, realPath);


            if (matchNum > 1){
                char* chosen = pickDir(realPath);
                strcat(realPath, chosen);
            } else if (matchNum == 1){
                strcat(realPath, matches[0]);
            } else {
                char errorMessage[50] = "TP error - cannot match path: \"";
                strcat(errorMessage, component);
                strcat(errorMessage, "\"");
                write(STDERR_FILENO, errorMessage, sizeof(errorMessage));
                return 1;
            }
        }

        //TODO: Add history mechanism
        puts(realPath);
    }
    return 0;
}
