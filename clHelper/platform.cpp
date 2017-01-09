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

#include "platform.h"

namespace clHelper {

#define BUFFER_SIZE 10000
  
  /*! helper macro that checks the return value of all MPI_xxx(...)
    calls via MPI_CALL(xxx(...)).  */
#define CL_CALL(a) { cl_int rc = cl##a; if (rc != CL_SUCCESS) throw std::runtime_error("opencl call returned error in " STRING(a)); }
  

  Device::Device(const cl_device_id clDeviceID,
                 const size_t platformID,
                 const size_t globalMemSize
                 )
    : clDeviceID(clDeviceID),
      platformID(platformID),
      globalMemSize(globalMemSize)
  {
  }
  
  
  /*! query the number of opencl-capable platforms in the system */
  size_t getNumPlatforms()
  {
    cl_uint numPlatforms;
    CL_CALL(GetPlatformIDs(0,NULL,&numPlatforms));
    return numPlatforms;
  }

  Platform::Platform(cl_platform_id clPlatformID,
                     const std::string &vendor,
                     const std::string &name,
                     const std::string &version,
                     const std::string &profile,
                     const std::string &extensions,
                     const std::vector<std::shared_ptr<Device>> &devices)
    : clPlatformID(clPlatformID),
      vendor(vendor),
      name(name),
      version(version),
      profile(profile),
      extensions(extensions),
      devices(devices)
  {}

  std::shared_ptr<Device> getDeviceInfo(/*! opencl device ID */
                                        cl_device_id clDeviceID,
                                        /*! _our_ platform ID (not necessarily same as cl_platform_id */
                                        size_t platformID)
  {
    // -------------------------------------------------------
    cl_ulong globalMemSize;
    CL_CALL(GetDeviceInfo(clDeviceID,CL_DEVICE_GLOBAL_MEM_SIZE,
                            sizeof(globalMemSize),&globalMemSize,NULL));

    // -------------------------------------------------------
    std::shared_ptr<Device> device = std::make_shared<Device>(clDeviceID,platformID,globalMemSize);
    return device;
  }
  
  std::vector<std::shared_ptr<Device>> queryDevicesForPlatform(cl_platform_id clPlatformID,
                                                               size_t platformID)
  {
    std::vector<std::shared_ptr<Device>> devices;

    cl_uint numDevices;
    CL_CALL(GetDeviceIDs(clPlatformID,CL_DEVICE_TYPE_ALL,0,NULL,&numDevices));
    cl_device_id clDeviceIDs[numDevices];
    CL_CALL(GetDeviceIDs(clPlatformID,CL_DEVICE_TYPE_ALL,numDevices,clDeviceIDs,NULL));
    
    for (int i=0;i<numDevices;i++)
      devices.push_back(getDeviceInfo(clDeviceIDs[i],platformID));
    
    return devices;
  }
  
  std::shared_ptr<Platform> getPlatformInfo(size_t platformID)
  {
    size_t numPlatforms = getNumPlatforms();
    if (platformID >= numPlatforms)
      throw std::runtime_error("invalid platform ID");

    cl_platform_id clPlatformIDs[numPlatforms];
    CL_CALL(GetPlatformIDs(numPlatforms,clPlatformIDs,NULL));

    // -------------------------------------------------------
    char buffer[BUFFER_SIZE];
    cl_platform_id clPlatformID = clPlatformIDs[platformID];

    // -------------------------------------------------------
    CL_CALL(GetPlatformInfo(clPlatformID,CL_PLATFORM_PROFILE,BUFFER_SIZE,buffer,NULL));
    const std::string profile = buffer;
    
    // -------------------------------------------------------
    CL_CALL(GetPlatformInfo(clPlatformID,CL_PLATFORM_VERSION,BUFFER_SIZE,buffer,NULL));
    const std::string version = buffer;
    
    // -------------------------------------------------------
    CL_CALL(GetPlatformInfo(clPlatformID,CL_PLATFORM_VENDOR,BUFFER_SIZE,buffer,NULL));
    const std::string vendor = buffer;
    
    // -------------------------------------------------------
    CL_CALL(GetPlatformInfo(clPlatformID,CL_PLATFORM_NAME,BUFFER_SIZE,buffer,NULL));
    const std::string name = buffer;

    // -------------------------------------------------------
    CL_CALL(GetPlatformInfo(clPlatformID,CL_PLATFORM_EXTENSIONS,BUFFER_SIZE,buffer,NULL));
    const std::string extensions = buffer;

    // -------------------------------------------------------
    return std::make_shared<Platform>(clPlatformID,vendor,name,version,profile,extensions,
                                      queryDevicesForPlatform(clPlatformID,platformID));
  }
  
} // ::clHelper

