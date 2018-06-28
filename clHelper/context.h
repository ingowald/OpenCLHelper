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
#include "program.h"

/*! c++ wrappers for opencl buffer objects - not yet implemented */
namespace clHelper {

  /*! abstracts a memory region on the device */
  struct Context : std::enable_shared_from_this<Context> {
    
    Context(const std::shared_ptr<Device> &device);
    
    ~Context();
    
    static std::shared_ptr<Context> create(const std::shared_ptr<Device> &device);
    
    std::shared_ptr<Program> createProgram(const std::string &code);

    std::shared_ptr<Device> device;
    cl_context              handle       { 0 };
    cl_command_queue        commandQueue { 0 };
    cl_command_queue        deviceCommandQueue { 0 };
  };
  
}
