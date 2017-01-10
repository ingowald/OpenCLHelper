// ======================================================================== //
// Copyright 2017 Ingo Wald                                                 //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //
/*
  Copyright (c) 2010-2011, Intel Corporation
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  * Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  * Neither the name of Intel Corporation nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.


  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#pragma warning (disable: 4244)
#pragma warning (disable: 4305)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef __linux__
#include <malloc.h>
#endif
#include <math.h>
#include <map>
#include <string>
#include <algorithm>
#include <sys/types.h>
// clhelper
#include "clHelper/buffer.h"
#include "clHelper/embeddedProgram.h"

#define NSUBSAMPLES        2

extern void ao_serial(int w, int h, int nsubsamples, float image[]);

static unsigned int test_iterations[] = {3, 7, 1};
static unsigned int width, height;
static unsigned char *img;
static float *fimg;


static unsigned char
clamp(float f)
{
  int i = (int)(f * 255.5);

  if (i < 0) i = 0;
  if (i > 255) i = 255;

  return (unsigned char)i;
}


static void
savePPM(const char *fname, int w, int h)
{
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++)  {
      img[3 * (y * w + x) + 0] = clamp(fimg[3 *(y * w + x) + 0]);
      img[3 * (y * w + x) + 1] = clamp(fimg[3 *(y * w + x) + 1]);
      img[3 * (y * w + x) + 2] = clamp(fimg[3 *(y * w + x) + 2]);
    }
  }

  FILE *fp = fopen(fname, "wb");
  if (!fp) {
    perror(fname);
    exit(1);
  }

  fprintf(fp, "P6\n");
  fprintf(fp, "%d %d\n", w, h);
  fprintf(fp, "255\n");
  fwrite(img, w * h * 3, 1, fp);
  fclose(fp);
  printf("Wrote image file %s\n", fname);
}


int main(int argc, char **argv)
{
  if (argc < 3) {
    printf ("%s\n", argv[0]);
    printf ("Usage: ao [width] [height] [ispc iterations] [tasks iterations] [serial iterations]\n");
    getchar();
    exit(-1);
  }
  else {
    if (argc == 6) {
      for (int i = 0; i < 3; i++) {
        test_iterations[i] = atoi(argv[3 + i]);
      }
    }
    width = atoi (argv[1]);
    height = atoi (argv[2]);
  }

  const std::string source = clHelper::getEmbeddedProgram("aoBench.cl");
  
  // Allocate space for output images
  img  = new unsigned char[width * height * 3];
  fimg = new float[width * height * 3];

  std::vector<std::shared_ptr<clHelper::Device>> devices
    = clHelper::getAllDevices();

  assert(!devices.empty());
  std::shared_ptr<clHelper::Device> device = devices[0];

  
  // std::shared_ptr<clHelper::DeviceBuffer> deviceImage
  //   = device->createRawBuffer(width*height*3*sizeof(float));
  /* Create OpenCL context */
  cl_int ret;
  cl_device_id device_id = device->clDeviceID;
  cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
     
  /* Create Command Queue */
  cl_command_queue command_queue = clCreateCommandQueue(context, device_id
                                                        , 0, &ret); 
  /* command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret); */
     
  /* Create Memory Buffer */
  size_t memSize = width*height*3*sizeof(float);
  cl_mem memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,
                                 memSize, NULL, &ret);
     
  /* printf("(begin sanity check)\n"); */
  /* printf("source size: %i\n",source_size); */
  /* for (int i=0;i<source_size;i++) */
  /*   printf("%c",source_str[i]); */
  /*   /\* printf("%i: %c\n",i,source_str[i]); *\/ */
  /* printf("(end sanity)\n"); */
  /* Create Kernel Program from the source */
  const char *source_str = source.c_str();
  size_t source_size = source.size();
  cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
                                                 (const size_t *)&source_size, &ret);

  /* Build Kernel Program */
  ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
     
  /* Create OpenCL Kernel */
  cl_kernel kernel = clCreateKernel(program, "aoBench", &ret);
     
  /* Set OpenCL Kernel Parameters */
  int numSubSamples = 4;
  ret = clSetKernelArg(kernel, 0, sizeof(width),  (void *)&width);
  ret = clSetKernelArg(kernel, 1, sizeof(height), (void *)&height);
  ret = clSetKernelArg(kernel, 2, sizeof(numSubSamples), (void *)&numSubSamples);
  ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&memobj);
     
  /* Execute OpenCL Kernel */
  // ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);
  // size_t global_work_offset[2] = { 0, 0 };
  size_t global_work_size[2] = { width, height };
  // size_t local_work_size[2] = { 8, 8 };
  ret = clEnqueueNDRangeKernel(command_queue, kernel, 2,
                               NULL, global_work_size, NULL,
                               0, NULL, NULL);
     
  /* Copy results from the memory buffer */
  ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,
                            memSize, fimg, 0, NULL, NULL);

  
  // //
  // // Run the ispc path, test_iterations times, and report the minimum
  // // time for any of them.
  // //
  double minTimeISPC = 1e30;
  // for (unsigned int i = 0; i < test_iterations[0]; i++) {
  //   memset((void *)fimg, 0, sizeof(float) * width * height * 3);
  //   assert(NSUBSAMPLES == 2);

  //   // reset_and_start_timer();
  //   // ao_ispc(width, height, NSUBSAMPLES, fimg);
  //   double t = 0.f; //get_elapsed_mcycles();
  //   printf("@time of ISPC run:\t\t\t[%.3f] million cycles\n", t);
  //   minTimeISPC = std::min(minTimeISPC, t);
  // }

  // Report results and save image
  printf("[aobench cl]:\t\t\t[%.3f] million cycles (%d x %d image)\n", 
         minTimeISPC, width, height);
  savePPM("ao-cl.ppm", width, height); 

  return 0;
}
