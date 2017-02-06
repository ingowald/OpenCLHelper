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
#include "platform.h"

namespace clHelper {

  std::string clErrorString(cl_int error)
  {
    switch(error){
      // run-time and JIT compiler errors
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

      // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

      // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
    }
  }

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
  
  /*! get a list of all devices found in the system */
  std::vector<std::shared_ptr<Device>> getAllDevices()
  {
    std::vector<std::shared_ptr<Device>> ret;
    size_t numPlatforms = getNumPlatforms();
    for (int pID=0;pID<numPlatforms;pID++) {
      std::shared_ptr<Platform> platform = getPlatformInfo(pID);
      for (auto device : platform->devices)
        ret.push_back(device);
    }
    return ret;
  }

} // ::clHelper

