# AES-OpenCL
check performance aes algorithm on gpu     
basicaly use private memeory in gpu for caching     

## AES Buffer    
input data is copied CPU memory to GPU memory    

## AES SVM 
use Virtual Shared Memeory    
Write data in shared memeory directly 

### generator.c 
Make input files 1kb ~ 500Mb size    

### autoTest.c 
It is helpful for testing AES openCL code    
It is possible to set the number of repetitions, input files to be put in, and executable files.    
The result will be written on file in 'log' dir at your working directory    

