#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <regex.h> 

int main(int argc, char *argv[]) {
    xgrep(".", argv[1]);
    return 0;
}

void xgrep(char *path,char *str){
    DIR *dir = opendir(path);
    struct dirent *pent = NULL;
    if(dir == NULL){
        printf("An error has ocurred with dir");
    } else {
        while (pent = readdir (dir)){
            if(pent == NULL){
                printf("Error in while");
            } else {
                char* name[256];
                char* file[256];
                if(pent->d_type == DT_REG){ //arch
                    strncpy(name, pent->d_name, 256);
                    strncpy(file, pent->d_name, 256);
                    strcpy (name, path);
                    strcat (name, "/");
                    strcat (name, pent->d_name);
                    //empieza el regex
                    regex_t regex;
                    int reti = 0;
                    reti = regcomp(&regex, str, 0);
                    if (reti) {
                        fprintf(stderr, "Could not compile regex\n");
                        exit(1);
                    }
                    //listo regex
                    FILE *fp = fopen(name,"r");
                    if(fp!=NULL){
                        //tomado de http://stackoverflow.com/questions/38998407/strip-new-line-character-from-end-from-char
                        const size_t line_size = 1024;
                        char* line = malloc(line_size);
                        int counter = 0;
                        while (fgets(line, line_size, fp) != NULL)  {
                            reti = regexec(&regex, line, 0, NULL, 0);
                            if (!reti) {
                                printf("Match in file named: ");
                                printf("%s",file);
                                printf(" in line ");
                                printf("%d",counter);
                                printf("%s",line);
                                printf("\n");
                            }
                            counter++;
                        }
                        free(line); 
                        fclose(fp);
                    } else {
                        printf("%sError opening",name);
                    }
                } else if((pent->d_type==DT_DIR)&&(strcmp(pent->d_name,".")!=0)&&(strcmp(pent->d_name,"..")!=0)){ //== DT_DIR
                    strcpy (name, path);
                    strcat (name, "/");
                    strcat (name, pent->d_name);
                    xgrep(name,str);
                }
            }
        }
    }
     close(dir);
}