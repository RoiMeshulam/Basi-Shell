#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <netinet/in.h> //structure for storing address information
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>

#define TRUE 1
#define MAX_CMD_SIZE 1024

int flag = 0;

void copyFile(char *src, char *dst) {
    char c;
    FILE *srcFile = fopen(src, "rb");
    if (srcFile == NULL) {
        printf("Cannot open file %s\n", src);
        return;
    }
    FILE *dstFile = fopen(dst, "wb");
    if (dstFile == NULL) {
        printf("Cannot open file %s\n", dst);
        return;
    }
    while (fread(&c, 1, 1, srcFile) == 1) {
        fwrite(&c, 1, 1, dstFile);
    }
    fclose(srcFile);
    fclose(dstFile);
}

void toServer() {
    flag = 1;
    int sockD = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port= htons(9001); // use some unused port number
    servAddr.sin_addr.s_addr = INADDR_ANY;

    int connectStatus = connect(sockD, (struct sockaddr*)&servAddr,sizeof(servAddr));

    if (connectStatus == -1) {
        printf("Error...\n");
        return;
    }
    //Redirecting the STDOUT to the socket
    dup2(1,666);
    dup2(sockD,1);
}

int main() {
    char cmd[MAX_CMD_SIZE] = ""; //BUFFER
    while(TRUE){
        fgets(cmd,MAX_CMD_SIZE,stdin);
        cmd[strlen(cmd) - 1] = '\0';

        if (!strncmp(cmd, "EXIT",4)){
            printf("Exit command detected, Shell is closed\n");
            return 0;
        }

        else if (!strncmp(cmd, "getcwd",6)){
            char cwd[256];
            getcwd(cwd,sizeof (cwd)); //GETCWD FUNCTION
            printf("Curretly working on %s\n",cwd);
        }

        else if(!strncmp(cmd, "ECHO",4)){
            char* substring = cmd + 5;
            printf("%s\n",substring);
        }

        else if(!strncmp(cmd, "DIR",3)){
            DIR *d;
            struct dirent *dir;
            d = opendir(".");
            if (d) {
                while ((dir = readdir(d)) != NULL) {
                    if (strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0){
                        continue;
                    }
                    printf("%s\n", dir->d_name);
                }
                closedir(d);
            }
        }

        /*The chdir command is a system function (system call) which is used to change the current working directory */
        else if(!strncmp(cmd, "CD",2)){
            char* newDIR = cmd + 3;
            if (chdir(newDIR) == -1) {
                printf("%s: no such directory\n", newDIR);
            }
        }

        else if(!strncmp(cmd, "TCP PORT",8)){
            printf("Connecting to server!\n");
            toServer();
        }

        else if (!strncmp(cmd, "LOCAL",5)){
            //REDIRECTING BACK THE STDOUT
            dup2(666,1);
            printf("Back to Shell\n");
            continue;
        }

        /*This is a library function*/
        else if(!strncmp(cmd, "COPY",4)){
            //EXTRACT THE SRC AND DEST NAMES
            char src[MAX_CMD_SIZE];
            char dst[MAX_CMD_SIZE];
            int index = 5;
            int destIndex = 0;
            while (cmd[index] != ' ') {
                src[index-5] = cmd[index];
                index++;
            }
            src[index-5] = '\0';
            index++;
            for (int i = index; i < strlen(cmd); ++i) {
                dst[destIndex] = cmd[i];
                destIndex++;
            }
            dst[destIndex] = '\0';
            //COPYING FILE
            copyFile(src, dst);
        }

        else if(!strncmp(cmd, "DELETE",6)){
            char *fileDEL = cmd + 7;
            if(unlink(fileDEL) == -1){ //DELETE FUNCTION IF RETURNS -1 COULDNT DELETE
                printf("Wrong file input\n");
            }
        }

        /*This is an example of system function, the next "else" condition will be doing the same but without calling system() */
//        else{
//            system(cmd);
//        }
        else{
            //SPLITING THE STRING TO AN ARRAY
            char *token;
            token = strtok(cmd, " ");
            char* param[MAX_CMD_SIZE];
            int i=0;
            while( token != NULL ) {
                param[i++] = token;
                token = strtok(NULL, " ");
            }
            param[i] = NULL;
            //CREATING FORK
            pid_t pid = fork();
            if(pid == -1){
                printf("Error in forking\n");
            }
            //SON JOB
            else if (pid == 0){
                if(execvp(param[0],param) == -1){ //IF -1 THAN BASH COULDNT COMPLETE THE TASK
                    printf("Bad command\n");
                    break;
                }
            }
            //FATHER JOB
            else{
                waitpid(pid,NULL,0);
            }
        }

    }
    return 0;
}
