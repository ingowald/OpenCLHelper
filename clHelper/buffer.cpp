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

#include "buffer.h"

/*! c++ wrappers for opencl buffer objects - not yet implemented */
namespace clHelper {

  /*! create a read/write memory buffer of given size (in given
    context), or throw exception of not possible */
  std::shared_ptr<DeviceBuffer> DeviceBuffer::create(const std::shared_ptr<Context> &context,
                                                     const size_t size)
  {
    return std::make_shared<DeviceBuffer>(context,size);
  }

    /*! create a read/write memory buffer of given size (in given
        context), or throw exception of not possible */
  DeviceBuffer::DeviceBuffer(const std::shared_ptr<Context> &context,
                             const size_t size)
  {
    cl_int ret;

    this->context = context;
    assert(context);
    this->handle = clCreateBuffer(context->handle, CL_MEM_READ_WRITE,
                                  size, NULL, &ret);
    if (ret != CL_SUCCESS)
      throw std::runtime_error("error in clHelper::DeviceBuffer (clCreateBuffer)");
  };

  /*! write given data into the buffer - size must match buffer size */
  void DeviceBuffer::write(const void *data, size_t size)
  {
    cl_int ret
      = clEnqueueWriteBuffer(context->commandQueue, this->handle, CL_TRUE, 0,
                             size, data, 0, NULL, NULL);
    if (ret != CL_SUCCESS)
      throw std::runtime_error("error in clHelper::DeviceBuffer (clEnqueueWriteBuffer)");
  }
  
  /*! read buffer content into given hist-side array. buffer and array sizes must match */
  void DeviceBuffer::read(void *data, size_t size)
  {
    cl_int ret
      = clEnqueueReadBuffer(context->commandQueue, this->handle, CL_TRUE, 0,
                            size, data, 0, NULL, NULL);
    if (ret != CL_SUCCESS)
      throw std::runtime_error("error in clHelper::DeviceBuffer (clEnqueueReadBuffer)");
  }
    

}

