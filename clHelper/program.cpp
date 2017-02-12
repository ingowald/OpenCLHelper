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

#include "program.h"
#include "context.h"

namespace clHelper {
  
  /*! constructor - create a new cl program handle in given context by
    building given program code */
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

  /*! destructor - release the opencl handle and clean up */
  Program::~Program()
  {
    throw std::runtime_error("not yet releasing kernel ... ");
  }
  

  /*! create a new clHelper::Kernel from this program. may throw an
      exception if the program doesn't contain this kernel, of if
      there was a problem while building the program */
  std::shared_ptr<Kernel> Program::createKernel(const char *name)
  {
    return std::make_shared<Kernel>(shared_from_this(),name);
  }


} // ::clHelper

