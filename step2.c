#include <stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <string.h>

int main(char *argv[], int argc)
{
	pid_t pid;
	FILE *fp;
	char b[100];
	int cnt=0;
	char c;
	fp=fopen("new.txt","r+");
	fseek(fp, 0, SEEK_SET);
	fscanf(fp,"%[^\n]", b);
	c=getc(fp);
	printf("%s\n",b);
	fclose(fp);
	char* args={strtok(b, " "),NULL}; 
	int pos=0;
	while (args != NULL) {
	args[pos++] = tok;
	tok = strtok(NULL, " ");
	}
	//char* tok = strtok(buffer[i], " ");
        execvp(args[0],args); 




}
