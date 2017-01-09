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

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <sys/types.h>

#include <string>
#include <iostream>
#include <memory>
#include <vector>

/* debug printing macros */
#define STRING(x) #x
#define TOSTRING(x) STRING(x)
#define PING std::cout << __FILE__ << " (" << __LINE__ << "): " << __FUNCTION__ << std::endl
#define PRINT(x) std::cout << STRING(x) << " = " << (x) << std::endl
#define PRINT2(x,y) std::cout << STRING(x) << " = " << (x) << ", " << STRING(y) << " = " << (y) << std::endl
#define PRINT3(x,y,z) std::cout << STRING(x) << " = " << (x) << ", " << STRING(y) << " = " << (y) << ", " << STRING(z) << " = " << (z) << std::endl
#define PRINT4(x,y,z,w) std::cout << STRING(x) << " = " << (x) << ", " << STRING(y) << " = " << (y) << ", " << STRING(z) << " = " << (z) << ", " << STRING(w) << " = " << (w) << std::endl

#define THROW_RUNTIME_ERROR(str) \
  throw std::runtime_error(std::string(__FILE__) + " (" + std::to_string((long long)__LINE__) + "): " + std::string(str));

