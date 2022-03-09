#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUFSIZE 256 

void setFileName(char *fileName,int fileSize)
{
	memset(fileName,0,sizeof(char)*BUFSIZE);
	char buf[256];
	memset(buf,0,sizeof(buf));
	if(fileSize < 64000)
	{
		sprintf(buf,"./inputfiles/input_%dk.txt",fileSize/64);
	}else{
		sprintf(buf,"./inputfiles/input_%dm.txt",fileSize/64000);
	}
	
	sprintf(fileName,"%s",buf);
}
int main(int argc,char **argv)
{
	FILE *fp;
	
	char buffer[16] = "1234567890abcdef";
	char fileName[256];
	int kbyte = 64;

	setFileName(fileName,kbyte);
	if((fp = fopen(fileName,"w")) == NULL) 
	{
		if(mkdir("inputfiles",0776) == -1){
			fprintf(stderr,"mkdier e");
			exit(1);
		}
		else{
			if((fp = fopen(filename,"w")) == NULL){
				fprintf(stderr,"file error");
				exit(1);
			}
		}

		fprintf(stderr,"fileName e");
		exit(1);
	}
	
	for(int j = 0; j < kbyte; j++)
	{
			fwrite(buffer,1,sizeof(buffer),fp);
	}

	for(int i = 1; i < 12; i++)
	{	
		kbyte = i%2 ? kbyte*5 : kbyte*2;
		printf("%d %d %s\n",kbyte,kbyte/64,fileName);
		setFileName(fileName,kbyte);
		if((fp = fopen(fileName,"w")) == NULL) 
		{
			fprintf(stderr,"fileName e");
			exit(1);
		}
		for(int j = 0; j < kbyte; j++)
		{
			fwrite(buffer,1,sizeof(buffer),fp);
		}
	}
}

