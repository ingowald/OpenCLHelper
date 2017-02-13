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

namespace clHelper {

  struct Program;

  /*! a sigle opencl kernel in a opencl program */
  struct Kernel : std::enable_shared_from_this<Kernel>  {

    /*! helper function to set up kernel arguments. Use in the form of
      kernel.run(Kernel::Args().add(...).add(...).add(...) */
    struct Args {
      Args() {};
      
      /*! add a buffer object to the list of kernel args */
      Args &add(const std::shared_ptr<DeviceBuffer> &buffer);
      
      /*! add raw memory region to list of kernel args */
      Args &add(const void *ptr, size_t size);
    private:
      friend class clHelper::Kernel;
      
      std::vector<size_t> argSize;
      std::vector<uint8_t> argMem;
    };
    
    Kernel(const std::shared_ptr<Program> &program, const char *name);
    ~Kernel();
    void run(const Kernel::Args &args, size_t globalSize=1, size_t localSize=1);
    
    /*! return kernel info (such as local mem size, private mem size, preferrred warp size, etc, as a string */
    std::string getWorkGroupInfoString();
  private:
    std::shared_ptr<Program> program;
    cl_kernel handle { 0 };
  };
  
} // ::clHelper
