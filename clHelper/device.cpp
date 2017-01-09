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

#include "device.h"

namespace clHelper {

  Device::Device(const cl_device_id clDeviceID,
                 const size_t platformID)
    : clDeviceID(clDeviceID),
      platformID(platformID)
  {
  }
  
  std::shared_ptr<Device> getDeviceInfo(/*! opencl device ID */
                                        cl_device_id clDeviceID,
                                        /*! _our_ platform ID (not necessarily same as cl_platform_id */
                                        size_t platformID)
  {
    std::shared_ptr<Device> device
      = std::make_shared<Device>(clDeviceID,platformID);
    PRINT(clDeviceID);

    char buffer[CLH_BUFFER_SIZE];
    
    // -------------------------------------------------------
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_GLOBAL_MEM_SIZE,
                            sizeof(device->globalMemSize),&device->globalMemSize,NULL));
    PRINT(prettyNumber(device->globalMemSize));

    // -------------------------------------------------------
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_LOCAL_MEM_SIZE,
                            sizeof(device->localMemSize),&device->localMemSize,NULL));
    PRINT(prettyNumber(device->localMemSize));

    // -------------------------------------------------------
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                            sizeof(device->maxMemAllocSize),&device->maxMemAllocSize,NULL));
    PRINT(prettyNumber(device->maxMemAllocSize));

    // -------------------------------------------------------
    cl_uint maxComputeUnits;
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_MAX_COMPUTE_UNITS,
                            sizeof(maxComputeUnits),&maxComputeUnits,NULL));
    device->maxComputeUnits = maxComputeUnits;
    PRINT(device->maxComputeUnits);
    PRINT(prettyNumber(device->maxComputeUnits));

    // -------------------------------------------------------
    cl_uint maxClockFrequency;
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_MAX_CLOCK_FREQUENCY,
                            sizeof(maxClockFrequency),&maxClockFrequency,NULL));
    device->maxClockFrequency = maxClockFrequency * 1024. * 1024.;
    PRINT(device->maxClockFrequency);
    PRINT(prettyNumber(device->maxClockFrequency));

    // -------------------------------------------------------
    size_t maxWorkGroupSize;
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_MAX_WORK_GROUP_SIZE,
                            sizeof(maxWorkGroupSize),&maxWorkGroupSize,NULL));
    device->maxWorkGroupSize = maxWorkGroupSize;
    PRINT(device->maxWorkGroupSize);
    PRINT(prettyNumber(device->maxWorkGroupSize));

    // -------------------------------------------------------
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_NAME,
                          CLH_BUFFER_SIZE,buffer,NULL));
    device->name = buffer;
    PRINT(device->name);

    // -------------------------------------------------------
    return device;
  }
  
} // ::clHelper

