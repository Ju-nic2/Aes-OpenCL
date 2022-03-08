#include <CL/cl.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include "util.h"
using namespace std;
cl_device_id device ;
cl_context context ;
cl_command_queue cmdqueue ;
cl_platform_id platform ;
cl_program program;
cl_kernel kernel;

cl_int maxCoreNum = 0;
unsigned char RoundKey[240];
unsigned char  Key[32];
#define Nb 4
// The number of rounds in AES Cipher. It is simply initiated to zero. The actual value is recieved in the program.
int Nr = 14;
// The number of 32 bit words in the key. It is simply initiated to zero. The actual value is recieved in the program.
int Nk = 8;
char* TranslateOpenCLError(cl_int errorCode)
{
    switch (errorCode)
    {
    case CL_SUCCESS:                            return "CL_SUCCESS";
    case CL_DEVICE_NOT_FOUND:                   return "CL_DEVICE_NOT_FOUND";
    case CL_DEVICE_NOT_AVAILABLE:               return "CL_DEVICE_NOT_AVAILABLE";
    case CL_COMPILER_NOT_AVAILABLE:             return "CL_COMPILER_NOT_AVAILABLE";
    case CL_MEM_OBJECT_ALLOCATION_FAILURE:      return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case CL_OUT_OF_RESOURCES:                   return "CL_OUT_OF_RESOURCES";
    case CL_OUT_OF_HOST_MEMORY:                 return "CL_OUT_OF_HOST_MEMORY";
    case CL_PROFILING_INFO_NOT_AVAILABLE:       return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case CL_MEM_COPY_OVERLAP:                   return "CL_MEM_COPY_OVERLAP";
    case CL_IMAGE_FORMAT_MISMATCH:              return "CL_IMAGE_FORMAT_MISMATCH";
    case CL_IMAGE_FORMAT_NOT_SUPPORTED:         return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case CL_BUILD_PROGRAM_FAILURE:              return "CL_BUILD_PROGRAM_FAILURE";
    case CL_MAP_FAILURE:                        return "CL_MAP_FAILURE";
    case CL_MISALIGNED_SUB_BUFFER_OFFSET:       return "CL_MISALIGNED_SUB_BUFFER_OFFSET";                          //-13
    case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:    return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";   //-14
    case CL_COMPILE_PROGRAM_FAILURE:            return "CL_COMPILE_PROGRAM_FAILURE";                               //-15
    case CL_LINKER_NOT_AVAILABLE:               return "CL_LINKER_NOT_AVAILABLE";                                  //-16
    case CL_LINK_PROGRAM_FAILURE:               return "CL_LINK_PROGRAM_FAILURE";                                  //-17
    case CL_DEVICE_PARTITION_FAILED:            return "CL_DEVICE_PARTITION_FAILED";                               //-18
    case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:      return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";                         //-19
    case CL_INVALID_VALUE:                      return "CL_INVALID_VALUE";
    case CL_INVALID_DEVICE_TYPE:                return "CL_INVALID_DEVICE_TYPE";
    case CL_INVALID_PLATFORM:                   return "CL_INVALID_PLATFORM";
    case CL_INVALID_DEVICE:                     return "CL_INVALID_DEVICE";
    case CL_INVALID_CONTEXT:                    return "CL_INVALID_CONTEXT";
    case CL_INVALID_QUEUE_PROPERTIES:           return "CL_INVALID_QUEUE_PROPERTIES";
    case CL_INVALID_COMMAND_QUEUE:              return "CL_INVALID_COMMAND_QUEUE";
    case CL_INVALID_HOST_PTR:                   return "CL_INVALID_HOST_PTR";
    case CL_INVALID_MEM_OBJECT:                 return "CL_INVALID_MEM_OBJECT";
    case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:    return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case CL_INVALID_IMAGE_SIZE:                 return "CL_INVALID_IMAGE_SIZE";
    case CL_INVALID_SAMPLER:                    return "CL_INVALID_SAMPLER";
    case CL_INVALID_BINARY:                     return "CL_INVALID_BINARY";
    case CL_INVALID_BUILD_OPTIONS:              return "CL_INVALID_BUILD_OPTIONS";
    case CL_INVALID_PROGRAM:                    return "CL_INVALID_PROGRAM";
    case CL_INVALID_PROGRAM_EXECUTABLE:         return "CL_INVALID_PROGRAM_EXECUTABLE";
    case CL_INVALID_KERNEL_NAME:                return "CL_INVALID_KERNEL_NAME";
    case CL_INVALID_KERNEL_DEFINITION:          return "CL_INVALID_KERNEL_DEFINITION";
    case CL_INVALID_KERNEL:                     return "CL_INVALID_KERNEL";
    case CL_INVALID_ARG_INDEX:                  return "CL_INVALID_ARG_INDEX";
    case CL_INVALID_ARG_VALUE:                  return "CL_INVALID_ARG_VALUE";
    case CL_INVALID_ARG_SIZE:                   return "CL_INVALID_ARG_SIZE";
    case CL_INVALID_KERNEL_ARGS:                return "CL_INVALID_KERNEL_ARGS";
    case CL_INVALID_WORK_DIMENSION:             return "CL_INVALID_WORK_DIMENSION";
    case CL_INVALID_WORK_GROUP_SIZE:            return "CL_INVALID_WORK_GROUP_SIZE";
    case CL_INVALID_WORK_ITEM_SIZE:             return "CL_INVALID_WORK_ITEM_SIZE";
    case CL_INVALID_GLOBAL_OFFSET:              return "CL_INVALID_GLOBAL_OFFSET";
    case CL_INVALID_EVENT_WAIT_LIST:            return "CL_INVALID_EVENT_WAIT_LIST";
    case CL_INVALID_EVENT:                      return "CL_INVALID_EVENT";
    case CL_INVALID_OPERATION:                  return "CL_INVALID_OPERATION";
    case CL_INVALID_GL_OBJECT:                  return "CL_INVALID_GL_OBJECT";
    case CL_INVALID_BUFFER_SIZE:                return "CL_INVALID_BUFFER_SIZE";
    case CL_INVALID_MIP_LEVEL:                  return "CL_INVALID_MIP_LEVEL";
    case CL_INVALID_GLOBAL_WORK_SIZE:           return "CL_INVALID_GLOBAL_WORK_SIZE";                           //-63
    case CL_INVALID_PROPERTY:                   return "CL_INVALID_PROPERTY";                                   //-64
    case CL_INVALID_IMAGE_DESCRIPTOR:           return "CL_INVALID_IMAGE_DESCRIPTOR";                           //-65
    case CL_INVALID_COMPILER_OPTIONS:           return "CL_INVALID_COMPILER_OPTIONS";                           //-66
    case CL_INVALID_LINKER_OPTIONS:             return "CL_INVALID_LINKER_OPTIONS";                             //-67
    case CL_INVALID_DEVICE_PARTITION_COUNT:     return "CL_INVALID_DEVICE_PARTITION_COUNT";                     //-68
	//case CL_INVALID_PIPE_SIZE:                  return "CL_INVALID_PIPE_SIZE";                                  //-69
   // case CL_INVALID_DEVICE_QUEUE:               return "CL_INVALID_DEVICE_QUEUE";                               //-70    
    default:
        return "UNKNOWN ERROR CODE";
    }
}


int getSBoxValue(int num)
{
    int sbox[256] = {
        //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }; //F
    return sbox[num];
}


const int Rcon[255] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb 
};
#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))

void KeyExpansion()
{
    int i, j;
    unsigned char temp[4], k;
    // The first round key is the key itself.
    for (i = 0; i<Nk; i++)
    {
        RoundKey[i * 4] = Key[i * 4];
        RoundKey[i * 4 + 1] = Key[i * 4 + 1];
        RoundKey[i * 4 + 2] = Key[i * 4 + 2];
        RoundKey[i * 4 + 3] = Key[i * 4 + 3];
    /*    printf("i :: %d\n", i);
        printf("%2x %2x %2x %2x\n ", RoundKey[i * 4], RoundKey[i * 4 + 1], RoundKey[i * 4 + 2], RoundKey[i * 4 + 3]);
    */
	}
 
    // All other round keys are found from the previous round keys.
    while (i < (Nb * (Nr + 1)))
    {
        for (j = 0; j<4; j++)
        {
            temp[j] = RoundKey[(i - 1) * 4 + j];
        }
        if (i % Nk == 0)
        {
            // This function rotates the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]
            // Function RotWord()
            {
                k = temp[0];
                temp[0] = temp[1];
                temp[1] = temp[2];
                temp[2] = temp[3];
                temp[3] = k;
            }
            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.
            // Function Subword()
            {
                temp[0] = getSBoxValue(temp[0]);
                temp[1] = getSBoxValue(temp[1]);
                temp[2] = getSBoxValue(temp[2]);
                temp[3] = getSBoxValue(temp[3]);
            }
            temp[0] = temp[0] ^ Rcon[i / Nk];
        }
        else if (Nk > 6 && i % Nk == 4)
        {
            // Function Subword()
            {
                temp[0] = getSBoxValue(temp[0]);
                temp[1] = getSBoxValue(temp[1]);
                temp[2] = getSBoxValue(temp[2]);
                temp[3] = getSBoxValue(temp[3]);
            }
        }
        RoundKey[i * 4 + 0] = RoundKey[(i - Nk) * 4 + 0] ^ temp[0];
        RoundKey[i * 4 + 1] = RoundKey[(i - Nk) * 4 + 1] ^ temp[1];
        RoundKey[i * 4 + 2] = RoundKey[(i - Nk) * 4 + 2] ^ temp[2];
        RoundKey[i * 4 + 3] = RoundKey[(i - Nk) * 4 + 3] ^ temp[3];
		i++;
    }
}


void init(char *keyFileName)
{
	FILE *keyfile;

	if((keyfile = fopen(keyFileName, "rb")) == NULL)
	{
		fprintf(stderr,"keyFile error\n");
		exit(1);
	}
	for(int i = 0; i < 32; i++)
	{
		fscanf(keyfile,"%x",&Key[i]);
	}
	/*for(int i = 0; i < 32; i++)
	{
		printf("%x ",Key[i]);
	}*/
	KeyExpansion();
	//printf("\n");
	char buf[3];
	/*for(int i = 0; i < 240; i++)
	{
		sprintf(buf,"%x",RoundKey[i]);
		printf("%s ",buf);
	}*/
	std :: string base = "#define Nb 4\n#define Nr 14\n #define Nk 8\n  __constant unsigned char RoundKey[240] = {";
	for(int i = 0; i < 240; i++)
	{
		char buf[3];
		base.append("0x");
		sprintf(buf,"%x",RoundKey[i]);
		base.append(buf);
		if(i < 239)	base.append(",");
	}
	
	base.append("};\n");
	std::fstream kernelFile("aes.cl");
	std::string content((std::istreambuf_iterator<char>(kernelFile)), std::istreambuf_iterator<char>());
	base += content;
	const char* kernelCharArray = new char[base.size()];
	kernelCharArray = base.c_str();
   //printf("%s\n",kernelCharArray); 

    cl_int err;


    err = clGetPlatformIDs(1, &platform, NULL);
	//printf("1%s\n",TranslateOpenCLError(err));


    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	//printf("1%s\n",TranslateOpenCLError(err));

	clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(maxCoreNum), &maxCoreNum, NULL);
	//printf("%d\n",maxCoreNum);
    context = clCreateContext(0, 1, &device, NULL, NULL, &err);
	//printf("1%s\n",TranslateOpenCLError(err));

    cmdqueue = clCreateCommandQueue(context, device, 0, &err);
	//printf("1%s\n",TranslateOpenCLError(err));


	program = clCreateProgramWithSource(context, 1, (const char**)&kernelCharArray, NULL, &err);
	//printf("1%s\n",TranslateOpenCLError(err));

	err = clBuildProgram(program, 1, &device, "", NULL, NULL);
	//printf("1%s\n",TranslateOpenCLError(err));
if (err == CL_BUILD_PROGRAM_FAILURE) {
        printf("\nBuild Error = %i", err);
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char *log = (char *) malloc(log_size);

        // Get the log
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        printf("%s\n", log);
    }
	
	//printf("2%s\n",TranslateOpenCLError(err));


}
int main(int argc, char **argv)
{

	if(argc < 3){
		fprintf(stderr,"please write input file,key file, output file\n");
	}
	struct timeval start,end;
	struct timeval memStart,memEnd;
	struct timeval kernelStart,kernelEnd;
	struct timeval setStart, setEnd;
	struct timeval kernelRunStart, kernelRunEnd;

	double memTime = 0.0;
	double kerTime = 0.0;
	double setTime = 0.0;
	double totalTime = 0.0;
	double kernelRunTime = 0.0;


	gettimeofday(&start,NULL);
	gettimeofday(&setStart,NULL);
	init(argv[2]);
	gettimeofday(&setEnd,NULL);
	setTime +=  (double)((setEnd.tv_sec - setStart.tv_sec) * 1000 + (double)(setEnd.tv_usec - setStart.tv_usec) /1000);

	//printf("%s %s %s\n",argv[1],argv[2],argv[3]);
	FILE *inputFile;
	if((inputFile = fopen(argv[1],"rb")) == NULL)
	{
		fprintf(stderr,"input File Error");
		exit(1);
	}
	FILE *outputFile;
	if((outputFile = fopen(argv[3],"w")) == NULL)
	{
		fprintf(stderr,"output File Error");
		exit(1);
	}
	cl_int err;
	int cores = 0;
	unsigned char *data = (unsigned char*)malloc(sizeof(unsigned char) * 16 * maxCoreNum);
	cl_mem buffer = clCreateBuffer(context,CL_MEM_READ_WRITE, maxCoreNum*16*sizeof(unsigned char),NULL,&err);
	//printf("a%s\n",TranslateOpenCLError(err));

	bool flag = 1;
	while(flag){
	//allocate bufferSize 16 * Max compute unit
		cores = 0;
		for(int i = 0; i < maxCoreNum; i++)
		{
			cores++;
			for(int j = 0; j < 16; j++)
			{
				int ch = getc(inputFile);
				if(ch != EOF){data[i*16 +j] = ch;}
				else
				{
					if(j > 0){
						for(int k = j; k < 16; k++) data[i*16 + k] = 0x00;
					}else{
						cores--;
					}
					i = maxCoreNum+1;
					flag = 0;
					break;

				}
			}
		}
		//make state data
		cl_int err;	
		//printf("compute unsigned char : %d\n",cores);
		size_t globalws = cores;
		
		//printf("compute unsigned char : %d\n",globalws);
		gettimeofday(&memStart,NULL);
		err = clEnqueueWriteBuffer(cmdqueue,buffer,CL_TRUE,0,cores*16*sizeof(unsigned char),data,0,NULL,NULL);
		gettimeofday(&memEnd,NULL);
		memTime +=  (double)((memEnd.tv_sec - memStart.tv_sec) * 1000 + (double)(memEnd.tv_usec - memStart.tv_usec) /1000);
		gettimeofday(&kernelStart,NULL);
		//printf("1%s\n",TranslateOpenCLError(err));
		
		kernel = clCreateKernel(program, "Cipher", &err);
		
		//printf("3%s\n",TranslateOpenCLError(err));
		clSetKernelArg(kernel,0,sizeof(cl_mem),&buffer);
		
		gettimeofday(&kernelRunStart,NULL);
		err = clEnqueueNDRangeKernel(cmdqueue,kernel,1,NULL,&globalws,0,0,NULL,NULL);
		gettimeofday(&kernelRunEnd,NULL);
		kernelRunTime +=  (double)((kernelRunEnd.tv_sec - kernelRunStart.tv_sec) * 1000 + (double)(kernelRunEnd.tv_usec - kernelRunStart.tv_usec) /1000);
		
		//printf("2%s\n",TranslateOpenCLError(err));
		clFinish(cmdqueue);
		gettimeofday(&kernelEnd,NULL);
		kerTime +=  (double)((kernelEnd.tv_sec - kernelStart.tv_sec) * 1000 + (double)(kernelEnd.tv_usec - kernelStart.tv_usec) /1000);


		gettimeofday(&memStart,NULL);
		clEnqueueReadBuffer(cmdqueue,buffer,CL_TRUE,0,cores*16*sizeof(unsigned char),data,0,NULL,NULL);
		gettimeofday(&memEnd,NULL);
		memTime += (double)((memEnd.tv_sec - memStart.tv_sec) * 1000 + (double)(memEnd.tv_usec - memStart.tv_usec) /1000);
		
		unsigned char outBuffer[16];
		for(int i = 0; i < cores; i++)
		{
			for(int j = 0; j < 16; j++)
			{
				//printf("%x \n",data[i*16 +j]);
				outBuffer[j] = data[i*16 +j];
			}
			fwrite(outBuffer,1,sizeof(outBuffer),outputFile);
		}

	}
	gettimeofday(&end,NULL);
	
	totalTime =  (double)((end.tv_sec - start.tv_sec) * 1000 + (double)(end.tv_usec - start.tv_usec) /1000);
	printf("Time : %fms\n", totalTime);
	printf("setTime : %fms ratio : %f\n",setTime, setTime/totalTime*100);
	printf("memTime : %fms ratio : %f\n",memTime, memTime/totalTime*100);
	printf("kerenlTime : %fms ratio : %f\n",kerTime, kerTime/totalTime*100);
	printf("kerenlRunTime : %fms ratio : %f kernel Ratio : %f\n",kernelRunTime, kernelRunTime/totalTime*100,kernelRunTime/kerTime*100);

}
