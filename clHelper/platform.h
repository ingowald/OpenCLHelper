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

namespace clHelper {

  /*! C++ wrapper for an OpenCL platform object */
  struct Platform {

    /*! constructor */
    Platform(cl_platform_id clPlatformID,
             const std::string &vendor,
             const std::string &name,
             const std::string &version,
             const std::string &profile,
             const std::string &extensions,
             const std::vector<std::shared_ptr<Device>> &devices);

    /*! return name::version string */
    std::string nameAndVersion() const { return name+", "+version; }
    
    /*! devices found on this platform */
    const std::vector<std::shared_ptr<Device>> devices;

    /*! the opencl platform id to access this device through opencl */
    const cl_platform_id clPlatformID;
             
    /*! 
      CL_PLATFORM_PROFILE 	char[] 	
      
      OpenCL profile string. Returns the profile name supported by the
      implementation. The profile name returned can be one of the
      following strings:

      FULL_PROFILE - if the implementation supports the OpenCL
      specification (functionality defined as part of the core
      specification and does not require any extensions to be
      supported).

      EMBEDDED_PROFILE - if the implementation supports the OpenCL
      embedded profile. The embedded profile is defined to be a subset
      for each version of OpenCL.
    */
    const std::string profile;

    /*! 
      CL_PLATFORM_VERSION 	char[] 	
      
      OpenCL version string. Returns the OpenCL version supported by
      the implementation. This version string has the following
      format:
      
      OpenCL<space><major_version.minor_version><space><platform-specific
      information>
      
      The major_version.minor_version value returned will be 1.0.
    */
    const std::string version;

    /*!
      CL_PLATFORM_VENDOR 	char[] 	Platform vendor string.
    */
    const std::string vendor;
    
    /*!
      CL_PLATFORM_NAME 	char[] 	Platform name string.
    */
    const std::string name;

    /*!  CL_PLATFORM_EXTENSIONS char[] Returns a space-separated list
      of extension names (the extension names themselves do not
      contain any spaces) supported by the platform. Extensions
      defined here must be supported by all devices associated with
      this platform.
    */
    const std::string extensions;
  };

  /*! return the number of platforms in the system */
  size_t getNumPlatforms();
    
  std::shared_ptr<Platform> getPlatformInfo(size_t platformID);

} // ::clHelper

