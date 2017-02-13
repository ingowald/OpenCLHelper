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

#include "kernel.h"
#include "program.h"
#include "context.h"
#include "buffer.h"
// std
#include <sstream>

namespace clHelper {

  Kernel::Kernel(const std::shared_ptr<Program> &program, const char *name)
    : program(program)
  {
    cl_int ret;
    this->handle = clCreateKernel(program->handle, name, &ret);
    if (ret != CL_SUCCESS) {
      throw std::runtime_error("error in clHelper::Kernel (clCreateKernel) : "+clErrorString(ret));
    }
  }
  
  
  Kernel::~Kernel()
  {
    throw std::runtime_error("not yet releasing kernel ... ");
  }

  
  /*! return kernel info (such as local mem size, private mem size, preferrred warp size, etc, as a string */
  std::string Kernel::getWorkGroupInfoString()
  {
    std::stringstream ss;
    ss << "Kernel Work Group Information:" << std::endl;

    size_t workGroupSize;
    clGetKernelWorkGroupInfo(handle,program->context->device->clDeviceID,
                             CL_KERNEL_WORK_GROUP_SIZE,
                             sizeof(workGroupSize),&workGroupSize,NULL);

    ss << " - work group size        : " << workGroupSize << std::endl;

    cl_ulong privateMemSize;
    clGetKernelWorkGroupInfo(handle,program->context->device->clDeviceID,
                             CL_KERNEL_PRIVATE_MEM_SIZE,
                             sizeof(privateMemSize),&privateMemSize,NULL);
    ss << " - private mem size       : " << privateMemSize << std::endl;

        cl_ulong localMemSize;
    clGetKernelWorkGroupInfo(handle,program->context->device->clDeviceID,
                             CL_KERNEL_LOCAL_MEM_SIZE,
                             sizeof(localMemSize),&localMemSize,NULL);
    ss << " - local mem size         : " << localMemSize << std::endl;

    size_t compileWorkGroupSize[3];
    clGetKernelWorkGroupInfo(handle,program->context->device->clDeviceID,
                             CL_KERNEL_COMPILE_WORK_GROUP_SIZE,
                             sizeof(compileWorkGroupSize),&compileWorkGroupSize,NULL);
    ss << " - compile wkgrp size     : " << compileWorkGroupSize[0] << "," << compileWorkGroupSize[1] << "," << compileWorkGroupSize[2] << std::endl;

    size_t preferredWorkGroupSizeMultiple;
    clGetKernelWorkGroupInfo(handle,program->context->device->clDeviceID,
                             CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
                             sizeof(preferredWorkGroupSizeMultiple),&preferredWorkGroupSizeMultiple,NULL);
    ss << " - pref wokgrp (warp) size: " << preferredWorkGroupSizeMultiple << std::endl;
    return ss.str();
  }


  void Kernel::run(const Kernel::Args &args, size_t globalSize, size_t localSize)
  {
    cl_int ret;
    const unsigned char *in = args.argMem.data();
    for (int i=0;i<args.argSize.size();i++) {
      size_t sz_i = args.argSize[i];
      ret = clSetKernelArg(handle, i, sz_i,  (void *)in);
      if (ret != CL_SUCCESS)
        throw std::runtime_error("error in clHelper::Kernel::run (clSetKernelArgs) : "+clErrorString(ret));
      in += sz_i;
    }

    size_t global_work_size[2] = { globalSize,0 };
    size_t local_work_size[2]  = { localSize,0 };
    // size_t local_work_size[2] = { 8, 8 };
    ret = clEnqueueNDRangeKernel(program->context->commandQueue, this->handle, 1,
                                 NULL, global_work_size, local_work_size,
                                 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
      const std::string err = "error in clHelper::Kernel::run (clEnqueueNDRangeKernel)"+clErrorString(ret);
      PING;
      PRINT(err);
      throw std::runtime_error(err);
    }

    ret = clFinish(program->context->commandQueue);
    if (ret != CL_SUCCESS) {
      const std::string err = "error in clHelper::Kernel::run (clFinish) "+clErrorString(ret);
      PING;
      PRINT(err);
      throw std::runtime_error(err);
    }
  }

  
  Kernel::Args &Kernel::Args::add(const void *ptr, size_t size)
  {
    const uint8_t *in = (const uint8_t *)ptr;
    for (int i=0;i<size;i++)
      argMem.push_back(*in++);
    argSize.push_back(size);
    return *this;
  }

  
  Kernel::Args &Kernel::Args::add(const std::shared_ptr<DeviceBuffer> &buffer)
  {
    return add(&buffer->handle,sizeof(buffer->handle));
  }




} // ::clHelper
