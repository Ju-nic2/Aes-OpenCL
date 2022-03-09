#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUFSIZE 256

void test(char *inputFileName, int maxRepeat,char *exeFile);
int main(int argc, char **argv)
{
	struct dirent *dentry;
	struct stat statbuf;
	DIR *dir;

	char dirName[BUFSIZE];
	char fileName[BUFSIZE*2];

	int maxRepeat;

	if(argc < 4)
	{
		fprintf(stderr,"please write input file directory, maxRepeat Num,exe files .. ");
		exit(1);
	}

	
	if(( dir = opendir(argv[1])) == NULL )
	{
		fprintf(stderr,"input file dir e\n");
		exit(1);
	}
	
	memset(dirName,0,sizeof(dirName));
	sprintf(dirName,"./%s",argv[1]);
	
	maxRepeat = atoi(argv[2]);

	for(int i = 3; i < argc; i++)
	{
		while((dentry = readdir(dir)) != NULL)
		{
			if(dentry->d_ino == 0)
				continue;

			memset(fileName,0,sizeof(fileName));
			sprintf(fileName,"%s/%s",dirName,dentry->d_name);
		
			if(stat(fileName,&statbuf) == -1)
			{
				fprintf(stderr,"fileName e");
				exit(1);
			}

			if((statbuf.st_mode & S_IFMT) == S_IFDIR)
				continue;
		
			test(fileName,maxRepeat,argv[i]);
			printf("%s %s done\n",fileName,argv[i]);
		}
	rewinddir(dir);
	}

}


void test(char *inputFileName, int maxRepeat,char *exeFile)
{
	pid_t pid;
	int status;
	char **argvs;
	argvs = (char**)malloc(sizeof(char*)*6);
	argvs[0] = (char*)malloc(sizeof(char) * strlen(exeFile)+1);

	argvs[1] = (char*)malloc(sizeof(char) * strlen(inputFileName)+ 1);
	argvs[2] = (char*)malloc(sizeof(char) * strlen("testKey.txt") + 1);
	argvs[3] = (char*)malloc(sizeof(char) * strlen("o.txt") + 1);
	argvs[4] = (char*)malloc(sizeof(char) * 10);
	argvs[5] = (char*)malloc(sizeof(char) * 1);
	strcpy(argvs[0],exeFile);
	strcpy(argvs[1],inputFileName);
	strcpy(argvs[2],"testKey.txt");
	strcpy(argvs[3],"11.txt");
	argvs[5][0] = '\0';

	for(int i = 1; i <= maxRepeat; i++)
	{
		memset(argvs[4],0,sizeof(char)*10);
		sprintf(argvs[4],"%d",i);
		if((pid = fork()) == 0)
		{

			if(execv(exeFile,argvs) < 0){
				fprintf(stderr,"exe e");
				exit(1);
			}
			exit(1);
		}
		else{
			waitpid(pid,&status,0);
		}
	}
}

	
