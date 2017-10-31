#include<stdio.h>
#include <string.h>

#include <dirent.h>

int* totalBytes=0;
int* totalWords=0;
int* totalChangeLine=0;

viewFile(char* path, int asDirectory){
    FILE* file = fopen(path,"r");
    char caracter;
    int countChangeLine=0;
    int countWords=0;
    int countByte=0;
    int previusCharacter=0;
	//Space characters
    char var1="\t";
    char var2="\b";
    char var3="\f";
    char var4="\r";
    while(!feof(file)){
        caracter = getc(file);
        switch(caracter){ 

        case 10:
        countChangeLine++;

        case 8://b

        case 12://f

        case 13://r
         if(previusCharacter!=8 && previusCharacter!=10 && previusCharacter!=12 && previusCharacter!=13 && previusCharacter!=32){
            countWords++;
        }
        countByte++;

        break;

        case 32:
        if(previusCharacter!=32){
            countWords++;
        }
        countByte++;
        break;

        case -1:
        break;


        default:
            countByte++;
        break;

        }

        previusCharacter=caracter;

    }
        printf(" %d \t %d \t %d \t %s \n", countChangeLine ,countWords, countByte, path);
    if(asDirectory){
        totalBytes+=countByte;
        totalChangeLine+=countChangeLine;
        totalWords+=countWords;
    }
}

  xwcWithFile( char* path[], char* ToFind[], int* finding){
    FILE *aux;
    DIR *pDIR;
    struct dirent *entry;
    if( pDIR=opendir(path) ){
        entry = readdir(pDIR);
        while( entry && *finding){
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){//Defaults Directories 
                //To concatenate path
				char path2[100]="";
                char slash[]="/";
                strcat(path2,path);
                strcat(path2,slash);
                strcat(path2,entry->d_name);
                if( opendir(path2)!=NULL){
                     if(strcmp(entry->d_name,path) == 0){
                        viewFile(path2,0);
                        *finding=0;
                     }else{
                        xwcWithFile(path2,ToFind,finding);
                     }
                }else{
                    int result=strcmp(entry->d_name,path);
                    if(strcmp(entry->d_name,ToFind)==0 ){
                        viewFile(path2,0);
                        *finding =0;
                     }
                }
            }
            entry = readdir(pDIR);
        }
    }
}


xwcWithDirectory( char* path[]){

    FILE *aux;
    DIR *pDIR;
    struct dirent *entry;
    if( pDIR=opendir(path) ){
        entry = readdir(pDIR);
        while( entry){
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){//cambiar
                //To concatenate path
                char path2[100]="";
                char slash[]="/";
                strcat(path2,path);
                strcat(path2,slash);
                strcat(path2,entry->d_name);
                
                if( opendir(path2)==NULL){//case file 
                    int asDirectory=1;
                    viewFile(path2, asDirectory);
                }else{// case directory
                    xwcWithDirectory(path2);
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
        sp=strtok(NULL," ");
        if(sp!=0){
           
            int* finding=1;
            if( opendir(sp)==NULL){//case file
                xwcWithFile(".", sp,&finding);
            }else{//case directory
                xwcWithDirectory(sp);
                printf(" %d \t %d \t %d \t Total \n", totalChangeLine ,totalWords, totalBytes);
            }
        }
    }
}


int main()
{
    char path[100];
    printf("Coma WC, coloque el comando wc, seguido de los archivos ejemplo\n  wc holamundo.txt \n");
    fflush(stdin);
    scanf("%[^\n]s",&path);
    splitCommand(path);
    totalBytes=0;
    totalChangeLine=0;
    totalWords=0;
    return 0;
}
