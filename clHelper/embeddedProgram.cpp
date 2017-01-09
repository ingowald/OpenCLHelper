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

#include "embeddedProgram.h"
#include <dlfcn.h>
#include <sstream>
#include <stdio.h>

namespace clHelper {

  extern "C" char *clhGetEmbeddedOpenCLCode(const char *fileName, size_t *kernelLength)
  {
    char kernel_ptr_symbol_name[10000];
    sprintf(kernel_ptr_symbol_name,"_expanded_opencl__%s",fileName);
  
    char kernel_len_symbol_name[10000];
    sprintf(kernel_len_symbol_name,"_expanded_opencl__%s_len",fileName);
  
    for (char *s = kernel_ptr_symbol_name; *s; s++) {
      if (*s == '.') *s = '_';
      if (*s == '/') *s = '_';
    }
    for (char *s = kernel_len_symbol_name; *s; s++) {
      if (*s == '.') *s = '_';
      if (*s == '/') *s = '_';
    }

    void *kernel_ptr_symbol = dlsym(NULL,kernel_ptr_symbol_name);
    printf("symbol ptr for symbol name %s is %ld\n",kernel_ptr_symbol_name,(size_t)kernel_ptr_symbol);
    if (!kernel_ptr_symbol) return NULL;
    
    void *kernel_len_symbol = dlsym(NULL,kernel_len_symbol_name);
    if (!kernel_len_symbol) return NULL;
    
    *kernelLength = *(unsigned int *)kernel_len_symbol;
    
    printf("(begin sanity check)\n");
    printf("source size: %li\n",*kernelLength);
    
    char *kernel_src = (char *)kernel_ptr_symbol;
    
    for (int i=0;i<*kernelLength;i++)
      printf("%c",kernel_src[i]);
    /* printf("%i: %c\n",i,source_str[i]); */
    printf("(end sanity)\n");
    
    return kernel_src;
  }

  /*! get a std::string that contains the embedded OpenCL code. If the
    given file name couldn't be found among the embedded opencl codes
    a std::runtime_error will be thrown */
  std::string getEmbeddedProgram(const std::string &clFileName)
  {
    size_t len;
    const char *programSrc = clhGetEmbeddedOpenCLCode(clFileName.c_str(),&len);
    if (!programSrc)
      throw std::runtime_error("could not find embedded opencl code for '"+clFileName+"'");
    std::stringstream ss;
    for (int i=0;i<len;i++)
      ss << programSrc[i];
    return ss.str();
  }


} // ::clHelper

