#include<stdio.h>
#include <string.h>
#include <dirent.h>

int getSize(char* path){
    FILE* file;
    char comand[100]="";
    strcat(comand,"ls -l ");
    strcat(comand,path);
    char line[100];
    file=popen(comand,"r");
    fgets(line, sizeof(line),file);//get the line from file
    int i=0;
    char* sp=strtok(line," ");
    while(i<4){//find the size of the file
        sp=strtok(NULL," ");
        i++;
    }
    fclose(file);
    int sizeOfFile=atoi(sp);
    printf("%d \t %s \n",sizeOfFile,path  );
    return sizeOfFile;

    }

  xduWithFile( char* path[], char* ToFind[], int* finding){
    FILE *aux;
	DIR *pDIR;
	struct dirent *entry;
    if( pDIR=opendir(path) ){
        entry = readdir(pDIR);
		while( entry && *finding){
			if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){//cambiar
                char path2[100]="";
                char slash[]="/";
                strcat(path2,path);
                strcat(path2,slash);
                strcat(path2,entry->d_name);
                if( opendir(path2)!=NULL){
                     if(strcmp(entry->d_name,path) == 0){
                        getSize(path2);
                        *finding=0;
                     }else{
                        xduWithFile(path2,ToFind,finding);
                     }
                }else{
                    int result=strcmp(entry->d_name,path);
                    if(strcmp(entry->d_name,ToFind)==0 ){
                        getSize(path2);
                        *finding =0;
                     }
                }
			}
			entry = readdir(pDIR);
		}
	}
}


xduWithDirectory( char* path[],int* totalSize){

    FILE *aux;
	DIR *pDIR;
	struct dirent *entry;
    if( pDIR=opendir(path) ){
        entry = readdir(pDIR);
		while( entry){
			if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){//cambiar
                char path2[100]="";
                char slash[]="/";
                strcat(path2,path);
                strcat(path2,slash);
                strcat(path2,entry->d_name);
                //intento de abrir archivo
                if( opendir(path2)==NULL){
                    *totalSize+=getSize(path2);
                }else{
                    xduWithDirectory(path2,totalSize);
                }
			}
			entry = readdir(pDIR);
		}
	}
}

splitCommand(char* command){
    int i=0;
    char* sp=strtok(command," ");
    while(sp!=NULL){
        sp=strtok(NULL," ");// se vuelve a
        if(sp!=0){
            //printf(" %s \n",sp);
            int* finding=1;
            if( opendir(sp)==NULL){
                xduWithFile(".", sp,&finding);//un archivo
            }else{
                int* totalSize=0;
                xduWithDirectory(sp ,&totalSize);
                printf("%d \t %s", totalSize, sp);
            }
        }
    }
}

int main()
{
    char path[100];
    printf("Comando xdu, coloque el comando xdu, seguido de los archivos ejemplo\n  xdu holamundo.txt \n");
    fflush(stdin);
    scanf("%[^\n]s",&path);
    splitCommand(path);
    return 0;
}
