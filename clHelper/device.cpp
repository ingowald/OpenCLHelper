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

    char buffer[CLH_BUFFER_SIZE];
    
    // -------------------------------------------------------
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_GLOBAL_MEM_SIZE,
                            sizeof(device->globalMemSize),&device->globalMemSize,NULL));

    // -------------------------------------------------------
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
                            sizeof(device->globalCacheSize),&device->globalCacheSize,NULL));

    // -------------------------------------------------------
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_LOCAL_MEM_SIZE,
                            sizeof(device->localMemSize),&device->localMemSize,NULL));

    // -------------------------------------------------------
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                            sizeof(device->maxMemAllocSize),&device->maxMemAllocSize,NULL));

    // -------------------------------------------------------
    cl_uint maxComputeUnits;
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_MAX_COMPUTE_UNITS,
                            sizeof(maxComputeUnits),&maxComputeUnits,NULL));
    device->maxComputeUnits = maxComputeUnits;

    // -------------------------------------------------------
    cl_uint maxClockFrequency;
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_MAX_CLOCK_FREQUENCY,
                            sizeof(maxClockFrequency),&maxClockFrequency,NULL));
    device->maxClockFrequency = maxClockFrequency * (1024. * 1024.);

    // -------------------------------------------------------
    cl_uint vectorWidthInt;
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
                          sizeof(vectorWidthInt),&vectorWidthInt,NULL));
    device->vectorWidthInt = vectorWidthInt;

    // -------------------------------------------------------
    cl_uint cacheLineSize;
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
                            sizeof(cacheLineSize),&cacheLineSize,NULL));
    device->cacheLineSize = cacheLineSize;

    // -------------------------------------------------------
    size_t maxWorkGroupSize;
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_MAX_WORK_GROUP_SIZE,
                            sizeof(maxWorkGroupSize),&maxWorkGroupSize,NULL));
    device->maxWorkGroupSize = maxWorkGroupSize;

    // -------------------------------------------------------
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_NAME,
                          CLH_BUFFER_SIZE,buffer,NULL));
    device->name = buffer;

    // -------------------------------------------------------
    return device;
  }

  void Device::print(const std::string &indent, std::ostream &out)
  {
    out << indent << "Device '" << name << "' (on platform #" << platformID << ")" << std::endl;
    // out << indent << "  name       : " << name << std::endl;
    out << indent << "  memory     : "
        << prettyNumber(globalMemSize) << "B global, "
        << prettyNumber(localMemSize) << "B local." << std::endl;
    out << indent << "  cache      : "
        << prettyNumber(globalCacheSize) << "B global, in cache lines of "
        << prettyNumber(cacheLineSize) << "Bs" << std::endl;
    out << indent << "  max malloc : "
        << prettyNumber(maxMemAllocSize) << "B" << std::endl;
    out << indent << "  max freq   : " << prettyNumber(maxClockFrequency) << "Hz" << std::endl;
    out << indent << "  compute    : " << maxComputeUnits << " cores @" << prettyNumber(maxClockFrequency) << "Hz (max), vector width of " << vectorWidthInt << " ints" << std::endl;
  }
  
  
} // ::clHelper

