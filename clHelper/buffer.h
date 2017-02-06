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
#include "context.h"

/*! c++ wrappers for opencl buffer objects - not yet implemented */
namespace clHelper {

  /*! abstracts a memory region on the device */
  struct DeviceBuffer {
    /*! create a read/write memory buffer of given size (in given
        context), or throw exception of not possible */
    DeviceBuffer(const std::shared_ptr<Context> &context,
                 const size_t size);

    /*! destroy the device-size buffer */
    ~DeviceBuffer() { throw std::runtime_error("releasing buffers not yet implemneted"); }

    /*! write given data into the buffer - size must match buffer size */
    void write(const void *data, size_t size);

    /*! read buffer content into given hist-side array. buffer and array sizes must match */
    void read(void *data, size_t size);
    

    static std::shared_ptr<DeviceBuffer> create(const std::shared_ptr<Context> &context,
                                                const size_t size);

    //! the context we were created with - need this to access the command queue etc
    std::shared_ptr<Context> context;
    cl_mem handle { 0 };
  };
  

}

