#include <stdio.h>
#include <stdlib.h>
     
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <dlfcn.h>

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

extern char _expanded_opencl__hello_cl[];
extern unsigned int _expanded_opencl__hello_cl_len;

char *getOpenCLCode(const char *fileName, size_t *kernelLength)
{
  char kernel_ptr_symbol_name[10000];
  sprintf(kernel_ptr_symbol_name,"_expanded_opencl__%s",fileName);
  
  char kernel_len_symbol_name[10000];
  sprintf(kernel_len_symbol_name,"_expanded_opencl__%s_len",fileName);
  
  for (char *s = kernel_ptr_symbol_name; *s; s++) {
    if (*s == '.') *s = '_';
    if (*s == '/') *s = '_';
  }
  for (char *s = kernel_len_symbol_name; *s; s++) {
    if (*s == '.') *s = '_';
    if (*s == '/') *s = '_';
  }

  
  void *kernel_ptr_symbol = dlsym(NULL,kernel_ptr_symbol_name);
  printf("symbol ptr for symbol name %s is %ld\n",kernel_ptr_symbol_name,(size_t)kernel_ptr_symbol);
  if (!kernel_ptr_symbol) return NULL;
  
  void *kernel_len_symbol = dlsym(NULL,kernel_len_symbol_name);
  if (!kernel_len_symbol) return NULL;

  *kernelLength = *(unsigned int *)kernel_len_symbol;

  printf("(begin sanity check)\n");
  printf("source size: %i\n",*kernelLength);

  char *kernel_src = (char *)kernel_ptr_symbol;
  
  for (int i=0;i<*kernelLength;i++)
    printf("%c",kernel_src[i]);
    /* printf("%i: %c\n",i,source_str[i]); */
  printf("(end sanity)\n");
  
  return kernel_src;
}


int main()
{
  cl_device_id device_id = NULL;
  cl_context context = NULL;
  cl_command_queue command_queue = NULL;
  cl_mem memobj = NULL;
  cl_program program = NULL;
  cl_kernel kernel = NULL;
  cl_platform_id platform_id = NULL;
  cl_uint ret_num_devices;
  cl_uint ret_num_platforms;
  cl_int ret;
     
  char string[MEM_SIZE];
     
  /* FILE *fp; */
  /* char fileName[] = "../hello.cl"; */
  const char *source_str = _expanded_opencl__hello_cl;
  const size_t source_size = _expanded_opencl__hello_cl_len;
     
  /* Load the source code containing the kernel*/
  /* fp = fopen(fileName, "r"); */
  /* if (!fp) { */
  /*   fprintf(stderr, "Failed to load kernel.\n"); */
  /*   exit(1); */
  /* } */
  /* source_str = (char*)malloc(MAX_SOURCE_SIZE); */
  /* source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp); */
  /* fclose(fp); */
     
  /* Get Platform and Device Info */



  ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
  ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
  printf("found num devices: %d\n",ret_num_devices);

  size_t kernelLen = 0;
  char *kernelSrc = getOpenCLCode("hello.cl", &kernelLen);

  /* Create OpenCL context */
  context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
     
  /* Create Command Queue */
  command_queue = clCreateCommandQueue(context, device_id, 0, &ret); 
  /* command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret); */
     
  /* Create Memory Buffer */
  memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,MEM_SIZE * sizeof(char), NULL, &ret);
     
  printf("createprogram\n");

  /* printf("(begin sanity check)\n"); */
  /* printf("source size: %i\n",source_size); */
  /* for (int i=0;i<source_size;i++) */
  /*   printf("%c",source_str[i]); */
  /*   /\* printf("%i: %c\n",i,source_str[i]); *\/ */
  /* printf("(end sanity)\n"); */
  /* Create Kernel Program from the source */
  program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
                                      (const size_t *)&source_size, &ret);
  printf("ret %i\n",ret);

  printf("building\n");
  /* Build Kernel Program */
  ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
     
  printf("creating\n");
  /* Create OpenCL Kernel */
  kernel = clCreateKernel(program, "hello", &ret);
     
  /* Set OpenCL Kernel Parameters */
  ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
     
  /* Execute OpenCL Kernel */
  ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);
     
  /* Copy results from the memory buffer */
  ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,
                            MEM_SIZE * sizeof(char),string, 0, NULL, NULL);
     
  /* Display Result */
  puts(string);
     
  /* Finalization */
  ret = clFlush(command_queue);
  ret = clFinish(command_queue);
  ret = clReleaseKernel(kernel);
  ret = clReleaseProgram(program);
  ret = clReleaseMemObject(memobj);
  ret = clReleaseCommandQueue(command_queue);
  ret = clReleaseContext(context);
     
  /* free(source_str); */
     
  return 0;
}
