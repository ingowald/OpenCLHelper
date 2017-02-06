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
#include <memory>

namespace clHelper {

  std::string clErrorString(cl_int error);

  struct DeviceBuffer;
  
  /*! C++ wrapper for an OpenCL device object */
  struct Device : public std::enable_shared_from_this<Device> {

    Device(const cl_device_id clDeviceID,
           const size_t platformID);

    /*! c++ wrappers for opencl buffer objects - not yet implemented */
    // std::shared_ptr<DeviceBuffer> createRawBuffer(size_t size) { return nullptr; };
    
    void print(const std::string &indent="", std::ostream &out = std::cout);
    
    /*! platform that this device is on, in our linear ordering. */
    const size_t platformID;
    
    /*! opencl device id */
    const cl_device_id clDeviceID;

    std::string name;
    size_t globalMemSize     { (size_t)-1 };
    size_t globalCacheSize   { (size_t)-1 };
    size_t cacheLineSize     { (size_t)-1 };
    size_t localMemSize      { (size_t)-1 };
    size_t maxMemAllocSize   { (size_t)-1 };
    size_t maxComputeUnits   { (size_t)-1 };
    size_t maxWorkGroupSize  { (size_t)-1 };
    size_t vectorWidthInt    { (size_t)-1 };

    double maxClockFrequency { -1. };
  };

  std::shared_ptr<Device> getDeviceInfo(/*! opencl device ID */
                                        cl_device_id clDeviceID,
                                        /*! _our_ platform ID (not necessarily same as cl_platform_id */
                                        size_t platformID);

  /*! get a list of all devices found in the system */
  std::vector<std::shared_ptr<Device>> getAllDevices();
} // ::clHelper

