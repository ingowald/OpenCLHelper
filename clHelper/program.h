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
#include "kernel.h"

namespace clHelper {

  struct Context;
  
  struct Program : std::enable_shared_from_this<Program>  {

    /*! constructor - create a new cl program handle in given context
      by building given program code */
    Program(const std::shared_ptr<Context> &context, const std::string &code);
    
    /*! destructor - release the opencl handle and clean up */
    ~Program();

    /*! create a new clHelper::Kernel from this program. may throw an
      exception if the program doesn't contain this kernel, of if
      there was a problem while building the program */
    std::shared_ptr<Kernel> createKernel(const char *name);

    /*! context this program was created in */
    std::shared_ptr<Context> context;

    /* the opencl handle to this program */
    cl_program handle { 0 };
  };
  
} // ::clHelper
