#include <stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <string.h>
    


    
int main(int argc,char* argv[])    
   {
     pid_t pid;
	FILE *fp;
	char b[100];
	int cnt=0;
	char c;
	fp=fopen("22.txt","r+");
	fseek(fp, 0, SEEK_SET);
	//fscanf(fp,"%[^\n]", b);
	//c=getc(fp);
	if (fp == NULL)
    {
        printf("File does not exist\n");
        return 0;
    }
    //fputs("hello",fp);
	while(fscanf(fp,"%[^\n]", b)==1)
	{
	pid=fork();	
	if(pid==0)
	{
		
		//printf("%ld\n",ftell(fp));
		//wait(NULL);	
		char* tok=strtok(b," ");
				
		char *args[10]; 
		int i=0;
		while(tok!=NULL)
		{
			args[i]=tok;
			i++;
			tok=strtok(NULL," ");
		}
		args[i]=NULL;
        	execvp(args[0],args); 
		return 0;
		
	}
	else
	{
		
		fscanf(fp,"%[^\n]", b);
		c=getc(fp);
		wait(NULL);
	}
	}	
	
	fclose(fp);
   }
