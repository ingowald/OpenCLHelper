#include <stdio.h>
#include <stdlib.h>
     
#include "clHelper/embeddedProgram.h"
#include "clHelper/platform.h"

int main()
{
#if 1
  size_t numPlatforms = clHelper::getNumPlatforms();
  std::cout << "number of platforms: " << numPlatforms << std::endl;
  for (int i=0;i<numPlatforms;i++) {
    std::cout << "-------------------------------------------------------" << std::endl;
    std::shared_ptr<clHelper::Platform> platform = clHelper::getPlatformInfo(i);
    std::cout << "found platform " << platform->nameAndVersion() << std::endl;
    std::cout << "   num devices " << platform->devices.size() << std::endl;
    for (auto device : platform->devices) {
      device->print("   ");
    }
  }
  std::cout << "-------------------------------------------------------" << std::endl;
#else
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
  /* const char *source_str = _expanded_opencl__hello_cl; */
  /* const size_t source_size = _expanded_opencl__hello_cl_len; */
  size_t source_size;
  const char *source_str = clhGetEmbeddedProgram("hello.cl",&source_size);
  if (!source_str) {
    printf("could not find embedded program 'hello.cl'");
    exit(1);
  }
     
  ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
  ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
  printf("found num devices: %d\n",ret_num_devices);

  /* Create OpenCL context */
  context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
     
  /* Create Command Queue */
  command_queue = clCreateCommandQueue(context, device_id, 0, &ret); 
  /* command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret); */
     
  /* Create Memory Buffer */
  memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,MEM_SIZE * sizeof(char), NULL, &ret);
     
  /* printf("(begin sanity check)\n"); */
  /* printf("source size: %i\n",source_size); */
  /* for (int i=0;i<source_size;i++) */
  /*   printf("%c",source_str[i]); */
  /*   /\* printf("%i: %c\n",i,source_str[i]); *\/ */
  /* printf("(end sanity)\n"); */
  /* Create Kernel Program from the source */
  program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
                                      (const size_t *)&source_size, &ret);

  /* Build Kernel Program */
  ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
     
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
#endif     
  return 0;
}
 
