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
#include <assert.h>

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

#define CLH_BUFFER_SIZE 10000
  
  /*! helper macro that checks the return value of all MPI_xxx(...)
    calls via MPI_CALL(xxx(...)).  */
#define CL_CALL(a) { cl_int rc = cl##a; if (rc != CL_SUCCESS) throw std::runtime_error("opencl call returned error in " STRING(a)); }
  
namespace clHelper {
  
  // /*! added pretty-print function for large numbers, printing 10000000 as "10M" instead */
  // inline std::string prettyNumber(const size_t s) {
  //   const double val = s;
  //   char result[100];
    
  //   if      (val >= 1e+18f) sprintf(result,"%.1f%c",val/1e18f,'E');
  //   else if (val >= 1e+15f) sprintf(result,"%.1f%c",val/1e15f,'P');
  //   else if (val >= 1e+12f) sprintf(result,"%.1f%c",val/1e12f,'T');
  //   else if (val >= 1e+09f) sprintf(result,"%.1f%c",val/1e09f,'G');
  //   else if (val >= 1e+06f) sprintf(result,"%.1f%c",val/1e06f,'M');
  //   else if (val >= 1e+03f) sprintf(result,"%.1f%c",val/1e03f,'k');
  //   else sprintf(result,"%lu",s);
  //   return result;
  // }

  /*! added pretty-print function for large numbers, printing 10000000 as "10M" instead */
  inline std::string prettyMetric(const double val) {
    char result[100];
    
    if      (val >= 1e+18f) sprintf(result,"%.1f%c",val/1e18f,'E');
    else if (val >= 1e+15f) sprintf(result,"%.1f%c",val/1e15f,'P');
    else if (val >= 1e+12f) sprintf(result,"%.1f%c",val/1e12f,'T');
    else if (val >= 1e+09f) sprintf(result,"%.1f%c",val/1e09f,'G');
    else if (val >= 1e+06f) sprintf(result,"%.1f%c",val/1e06f,'M');
    else if (val >= 1e+03f) sprintf(result,"%.1f%c",val/1e03f,'k');
    else sprintf(result,"%lu",(size_t)val);
    return result;
  }

  /*! added pretty-print function for large numbers, printing 10000000 as "10M" instead */
  inline std::string prettyNumber(const double val) {
    char result[100];

    const double kilo = 1024.;
    const double mega = 1024.*kilo;
    const double giga = 1024.*mega;
    const double tera = 1024.*giga;
    const double peta = 1024.*tera;
    const double exa  = 1024.*peta;

    if      (val >= exa) sprintf(result,"%.1f%c",val/exa,'E');
    else if (val >= peta) sprintf(result,"%.1f%c",val/peta,'P');
    else if (val >= tera) sprintf(result,"%.1f%c",val/tera,'T');
    else if (val >= giga) sprintf(result,"%.1f%c",val/giga,'G');
    else if (val >= mega) sprintf(result,"%.1f%c",val/mega,'M');
    else if (val >= kilo) sprintf(result,"%.1f%c",val/kilo,'k');
    else sprintf(result,"%lu",(size_t)val);
    return result;
  }
  
} // ::clHelper
