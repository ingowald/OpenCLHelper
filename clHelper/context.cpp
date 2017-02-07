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

#include "context.h"
#include "buffer.h"

/*! c++ wrappers for opencl buffer objects - not yet implemented */
namespace clHelper {

  KernelArgs &KernelArgs::add(const std::shared_ptr<DeviceBuffer> buffer)
  {
    return add(&buffer->handle,sizeof(buffer->handle));
  }

    Kernel::Kernel(const std::shared_ptr<Program> &program, const char *name)
    : program(program)
  {
    cl_int ret;
    this->handle = clCreateKernel(program->handle, name, &ret);
    if (ret != CL_SUCCESS) {
      throw std::runtime_error("error in clHelper::Kernel (clCreateKernel) : "+clErrorString(ret));
    }
  }

  

  Program::Program(const std::shared_ptr<Context> &context, const std::string &code)
    : context(context)
  {
    const char  *source_str  = code.c_str();
    const size_t source_size = code.size();
    cl_int ret;
    this->handle
      = clCreateProgramWithSource(context->handle, 1, (const char **)&source_str,
                                  (const size_t *)&source_size, &ret);
    if (ret != CL_SUCCESS)
      throw std::runtime_error("error in clHelper::Program (from clCreateProgramWithSource) : "+clErrorString(ret));
    /* Build Kernel Program */
    ret = clBuildProgram(this->handle, 1, &context->device->clDeviceID, NULL, NULL, NULL);
    if (ret != CL_SUCCESS) {
      
      size_t len;
      char *buffer;
      clGetProgramBuildInfo(this->handle, context->device->clDeviceID, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
      buffer = (char *)malloc(len+1);
      clGetProgramBuildInfo(this->handle, context->device->clDeviceID, CL_PROGRAM_BUILD_LOG, len, buffer, NULL);
      printf("%s\n", buffer);

      throw std::runtime_error("error in clHelper::Program (from clBuildProgram) : "+clErrorString(ret));
    }
  }


  
  void Kernel::run(const KernelArgs &args, size_t numThreads)
  {
    cl_int ret;
    const unsigned char *in = args.argMem.data();
    for (int i=0;i<args.argSize.size();i++) {
      size_t sz_i = args.argSize[i];
      ret = clSetKernelArg(handle, i, sz_i,  (void *)in);
      if (ret != CL_SUCCESS)
        throw std::runtime_error("error in clHelper::Kernel::run (clSetKernelArgs) : "+clErrorString(ret));
      in += sz_i;
    };

    size_t width = numThreads;
    size_t height = 1;
    size_t global_work_size[2] = { width, height };
    // size_t local_work_size[2] = { 8, 8 };
    ret = clEnqueueNDRangeKernel(program->context->commandQueue, this->handle, 2,
                                 NULL, global_work_size, NULL,
                                 0, NULL, NULL);
    if (ret != CL_SUCCESS)
        throw std::runtime_error("error in clHelper::Kernel::run (clEnqueueNDRangeKernel)");
  }
  

}


