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

#include "common.h"

namespace clHelper {

  /*! C++ wrapper for an OpenCL device object */
  struct Device {

    Device(const cl_device_id clDeviceID,
           const size_t platformID,
           const size_t globalMemSize
           );
    
    /*! platform that this device is on, in our linear ordering. */
    const size_t platformID;
    
    /*! opencl device id */
    const cl_device_id clDeviceID;

    const size_t globalMemSize;
  };

  std::shared_ptr<Device> getDeviceInfo(/*! opencl device ID */
                                        cl_device_id clDeviceID,
                                        /*! _our_ platform ID (not necessarily same as cl_platform_id */
                                        size_t platformID);
    
} // ::clHelper

