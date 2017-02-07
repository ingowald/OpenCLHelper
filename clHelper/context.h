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

#pragma once

#include "device.h"

/*! c++ wrappers for opencl buffer objects - not yet implemented */
namespace clHelper {

  struct Context;
  struct Program;
  struct KernelArgs;

  struct Kernel : std::enable_shared_from_this<Kernel>  {
    Kernel(const std::shared_ptr<Program> &program, const char *name);
    ~Kernel() {
      throw std::runtime_error("not yet releasing kernel ... ");
    }
    void run(const KernelArgs &args, size_t numThreads=1);

    std::shared_ptr<Program> program;
    cl_kernel handle { 0 };
  };
  
  struct Program : std::enable_shared_from_this<Program>  {
    Program(const std::shared_ptr<Context> &context, const std::string &code);
    
    ~Program() {
      throw std::runtime_error("not yet releasing kernel ... ");
    }

    cl_program handle { 0 };
    
    std::shared_ptr<Kernel> createKernel(const char *name)
    {
      return std::make_shared<Kernel>(shared_from_this(),name);
    }

    std::shared_ptr<Context> context;
  };
  
  /*! abstracts a memory region on the device */
  struct Context : std::enable_shared_from_this<Context> {
    
    Context(const std::shared_ptr<Device> &device)
    {
      cl_int ret = 0;
      assert(device);
      this->device = device;
      this->handle = clCreateContext(NULL, 1, &device->clDeviceID, NULL, NULL, &ret);
      if (ret != CL_SUCCESS)
        throw std::runtime_error("error in clHelper::Context (from clCreateContext)");

      commandQueue = clCreateCommandQueue(this->handle, device->clDeviceID, 0, &ret); 
      if (ret != CL_SUCCESS)
        throw std::runtime_error("error in clHelper::Context (from clCreateCommandQueue)");
    }
    
    ~Context() { throw std::runtime_error("releasing cl contexts not yet implemneted"); }
    
    static std::shared_ptr<Context> create(const std::shared_ptr<Device> &device)
    { return std::make_shared<Context>(device); }
    
    std::shared_ptr<Program> createProgram(const std::string &code)
    {
      return std::make_shared<Program>(shared_from_this(),code);
    };

    std::shared_ptr<Device> device;
    cl_context       handle { 0 };
    cl_command_queue commandQueue { 0 };
  };
  



  struct KernelArgs {
    KernelArgs() {};
    KernelArgs &add(const std::shared_ptr<DeviceBuffer> buffer);
    KernelArgs &add(const void *ptr, size_t size)
    {
      const uint8_t *in = (const uint8_t *)ptr;
      for (int i=0;i<size;i++)
        argMem.push_back(*in++);
      argSize.push_back(size);
      return *this;
    }
    
    std::vector<size_t> argSize;
    std::vector<uint8_t> argMem;
  };

}
