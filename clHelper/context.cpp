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
  
void Kernel::run(const KernelArgs &args)
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

    size_t width = 1;
    size_t height = 1;
    size_t global_work_size[2] = { width, height };
    // size_t local_work_size[2] = { 8, 8 };
    ret = clEnqueueNDRangeKernel(program->context->commandQueue, this->handle, 2,
                                 NULL, global_work_size, NULL,
                                 0, NULL, NULL);
    if (ret != CL_SUCCESS)
        throw std::runtime_error("error in clHelper::Kernel::run (clEnqueueNDRangeKernel)");
    PING;
    PRINT(args.argSize.size());
  }
  

}


