#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>

int main(void)
{
    xps("/proc");
    return 0;
}

double getTotalMemorySize()
{
    FILE* file =fopen("/proc/meminfo","r");
    const size_t line_size = 50;
    char* line = malloc(line_size);
    fgets(line, line_size, file);
    char* data =strtok(line," ");
    data =strtok(NULL," ");
    double total= (double) atoi(data);
    return total;

}

double getDataFromStatM(char *path)
{
    FILE* file =fopen(path,"r");
    const size_t line_size = 300;
    char* line = malloc(line_size);
    fgets(line, line_size, file);
    char* data =strtok(line," ");
    double total=atoi(data);
    return total;

}
void xps(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *pent = NULL;
    double totalMemorySize=getTotalMemorySize();
    if(dir == NULL)
    {
        printf("An error has ocurred with dir");
    }
    else
    {
        while (pent = readdir (dir))
        {
            if(pent == NULL)
            {
                printf("Error in while");
            }
            else
            {
                char* name[256];
                if(pent->d_type == DT_REG)
                {
                    if(strcmp(path,"/proc")!=0)
                    {
                        char* newPath[256];
                        strncpy(name, pent->d_name,256);
                        if(strcmp(name,"stat") == 0)
                        {
                            strncpy(newPath,path,256);
                            strcat(newPath,"/");
                            strcat(newPath,name);
                            getDataFromStat(newPath);
                        }
                        else if(strcmp(name,"status") == 0)
                        {
                            strncpy(newPath,path,256);
                            strcat(newPath,"/");
                            strcat(newPath,name);
                            getDataFromStatus(newPath);
                        }
                        else if(strcmp(name,"cmdline")==0)
                        {
                            strncpy(newPath,path,256);
                            strcat(newPath,"/");
                            strcat(newPath,name);
                            getDataFromCmdline(newPath);
                        }
                        else if(strcmp(name,"statm")==0)
                        {
                            strncpy(newPath,path,256);
                            strcat(newPath,"/");
                            strcat(newPath,name);
                            double sizeFromProgram = getDataFromStatM(newPath);
                            double percent= (sizeFromProgram/totalMemorySize);
                            printf("Memory percent: %lf \n", percent);
                            printf("\n");
                        }
                    }
                }
                else if((pent->d_type==DT_DIR)&&(strcmp(pent->d_name,".")!=0)&&(strcmp(pent->d_name,"..")!=0))    //== DT_DIR
                {
                    char desiredFolder[256];
                    strncpy(desiredFolder,pent->d_name,256);
                    if (isdigit(desiredFolder[0]))  //es un folder de proceso
                    {

                        strcpy (name, path);
                        strcat (name, "/");
                        strcat (name, pent->d_name);
                        xps(name);
                    }
                }
            }
        }
    }
    close(dir);
}

void getDataFromStat(char* newPath)
{

    FILE *fp = fopen(newPath,"r");
    if(fp!=NULL)
    {
        const size_t line_size = 300;
        char* line = malloc(line_size);

        fgets(line, line_size, fp);
        char* data =strtok(line," ");
        int i=0;
        while (i<6)
        {
            data =strtok(NULL," ");
            i++;
        }

        int terminalNumber=atoi(data);
        printf("Process terminal: %d ", terminalNumber);
        printf("\n");

        while (i<13)
        {
            data =strtok(NULL," ");
            i++;
        }

        //punto 2
        char* data2=strtok(NULL," ");
        int userTime=atoi(data);
        int kernellTime= atoi(data2);
        int sum=(userTime + kernellTime)/100;
        printf("CPU time: %d ", sum);
        printf("\n");

    }
}

void getDataFromStatus(char* newPath)
{
    //printf("%s\n",newPath);
    FILE *fp = fopen(newPath,"r");
    if(fp!=NULL)
    {
        const size_t line_size = 300;
        char* line = malloc(line_size);
        int counter = 1;
        while (fgets(line, line_size, fp) != NULL)
        {
            if(strstr(line, "Name") != NULL)
            {
                printf("%s",line);
            }
            if(counter == 5)
            {
                printf("%s",line);
            }
            if(strstr(line, "VmSize") != NULL)
            {
                printf("%s",line);
            }
            if(strstr(line, "VmRSS") != NULL)
            {
                printf("%s",line);
            }
            counter++;
        }
    }
}

void getDataFromCmdline(char* newPath)
{
    FILE *fp = fopen(newPath,"r");
    if(fp!=NULL)
    {
        const size_t line_size = 300;
        char* line = malloc(line_size);
        int counter = 1;
        printf("First command: ");
        while (fgets(line, line_size, fp) != NULL)
        {
            printf("%s",line);
        }
        printf("\n");
    }
}