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

  Context::Context(const std::shared_ptr<Device> &device)
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
  
  Context::~Context()
  {
    throw std::runtime_error("releasing cl contexts not yet implemneted");
  }

  std::shared_ptr<Context> Context::create(const std::shared_ptr<Device> &device)
  {
    return std::make_shared<Context>(device);
  }
  
  std::shared_ptr<Program> Context::createProgram(const std::string &code)
  {
    return std::make_shared<Program>(shared_from_this(),code);
  };
  

}


