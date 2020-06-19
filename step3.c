#include <stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <string.h>
#define DELIM " \t\r\n\a"

char histf[50][100];
char histb[50][100];
int i=0;
char j[10];

int execute(char* fn)
{
	pid_t pid;
	FILE *fp;
	char b[100];
	int cnt=0;
	char c;
	fp=fopen(fn,"r+");
	fseek(fp, 0, SEEK_SET);
	
	//fscanf(fp,"%[^\n]", b);
	//c=getc(fp);
	
	while(fscanf(fp,"%[^\n]", b)==1)
	{
	//strcpy(hist[i],b);
	//i++;
	
	pid=fork();
	char *args[10]; 
	if(pid<0)
	{
		return 0;
	}	
	if(pid==0)
	{
		
		//printf("%ld\n",ftell(fp));
		//wait(NULL);	
		char* tok=strtok(b,DELIM);
				
		
		int f=0;
		while(tok!=NULL)
		{
			args[f]=tok;
			f++;
			tok=strtok(NULL,DELIM);
		}
		args[f]=NULL;
        	execvp(args[0],args); 
		return 0;
		
	}
	else
	{
		
		//fscanf(fp,"%[^\n]", b);
		c=getc(fp);
		//printf("hello	");
		//sprintf(j,"%d",i+1);		
		//strcpy(histf[i],strcat(j,b));
		strcpy(histf[i],b);
		char* tok=strtok(b,DELIM);
		strcpy(histb[i],tok);
		
		
	
		i++;
		wait(NULL);
	}
	//strcpy(j,args[0]);
	}
	
	
	fclose(fp);
}

int main(int argc,char* argv[])
{
	//printf("%s",argv[1]);
	char com[100];
	char com1[100];
	for(int i=1;i<argc;i++)
	{
		execute(argv[i]);
	}
	printf("enter the command \n");
	//scanf("%s",com);
		
	gets(com);
	strcpy(com1,com);
	while(strncmp(com1,"STOP",strlen("stop"))!=0)
	//printf("%s \n",com1);	
	{
		if(strncmp(com1,"HISTORY BRIEF",strlen("HISTORY BRIEF"))==0)
		{
			for(int k=0;k<=i;k++)
			printf("%s \n",histb[k]);
		}
		if(strncmp(com1,"HISTORY FULL",strlen("HISTORY FULL"))==0)
		{
			for(int k=0;k<=i;k++)
			printf("%d %s \n",k+1,histf[k]);
		}
		if(strncmp(com1,"EXEC",4)==0)
		{
			char* tok1=strtok(com," ");
			tok1=strtok(NULL," ");
			printf("%s \n",tok1);
			int k=0;
			if(isdigit(tok1[0])!=0)
			{k=atoi(tok1);
			if(k>i)
			{printf("command not found \n");
			break;}
			}
			else
			{
			while (strncmp(tok1,histb[k],strlen(tok1))!=0 && k<=i)
			k++;
			if(strncmp(tok1,histb[k],strlen(tok1))!=0)
			{printf("command not found \n");
			break;}
			}
			
				pid_t pid;
				pid=fork();
				char *args[10];
				if (pid==0)
				{char* tok=strtok(histf[k]," ");
				int f=0;
				while(tok!=NULL)
				{
					args[f]=tok;
					f++;
					tok=strtok(NULL," ");
				}
				args[f]=NULL;
	        		execvp(args[0],args);
				return 0; }
				else if(pid>0)
				{
					wait(NULL);	
				}
			
			//printf("dumbo");
			//return 0;
			//strcpy(histb[i],tok);
		}
		printf("enter the command \n");
		gets(com);
		strcpy(com1,com);
	}
	if(strncmp(com1,"STOP",strlen("stop"))==0)
	{
		printf("exiting normally \n");
		return(0);
	}
	//return 0;	
	
}
