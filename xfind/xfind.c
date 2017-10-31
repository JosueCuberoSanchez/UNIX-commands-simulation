#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>




int main(int argc, char *argv[]) {
    xfind(".",argv[1]); //argv[1] es lo que entra
    return 0;
}

void xfind(char *path,char *str){
    DIR *dir = opendir(path); //abre el directorio
    struct dirent *pent = NULL;  //struct de cada elemento que hay en el
    if(dir == NULL){
        printf("An error has ocurred with dir");
    } else {
        while (pent = readdir (dir)){
            if(pent == NULL){
                printf("Error in while");
            } else {
                //printf("%s",pent->d_name);
                char* name[256];
                if(pent->d_type == DT_REG){ //si es archivo
                    strncpy(name, pent->d_name, 256); //mete el nombre del elemento en name
                    if(strstr(name, str) != NULL){
                        printf ("%s", name);
                        printf(" encontrado en el directorio ");
                        printf("%s\n",path);
                    }
                } else if((pent->d_type==DT_DIR)&&(strcmp(pent->d_name,".")!=0)&&(strcmp(pent->d_name,"..")!=0)){ //directorio,evita el mismo y el anterior
                    strcpy (name, path); //concatenamos para crear la ruta
                    strcat (name, "/");
                    strcat (name, pent->d_name);
                    xfind(name,str);
                }
            }
        }
    }
     close(dir);
}
