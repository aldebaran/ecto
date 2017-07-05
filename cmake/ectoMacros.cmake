#
# Copyright (c) 2011, Willow Garage, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Willow Garage, Inc. nor the names of its
#       contributors may be used to endorse or promote products derived from
#       this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
option(ECTO_LOG_STATS "Generate logs containing fine-grained per-cell execution timing information.  You probably don't want this."
  OFF)
mark_as_advanced(ECTO_LOG_STATS)

if(ECTO_LOG_STATS)
  add_definitions(-DECTO_LOG_STATS=1)
endif()

include(CMakeParseArguments)
set(ECTO_PYTHON_BUILD_PATH ${CMAKE_BINARY_DIR}/sdk/lib/python2.7/dist-packages/ecto/../)
set(ECTO_PYTHON_INSTALL_PATH lib/python2.7/dist-packages/ecto/../)
 

# 
# :param NAME: the name of your ecto module
# :param INSTALL: if given, it will also install the ecto module
#                 you might not want to install test modules.
# :param DESTINATION: the relative path where you want to install your ecto
#                     module (it will be built/install in the right place but
#                     you can specify submodules). e.g: ${PROJECT_NAME}/ecto_cells
macro(ectomodule NAME)
  cmake_parse_arguments(ARGS "INSTALL" "DESTINATION" "" ${ARGN})

  # #these are required includes for every ecto module
  # include_directories(${ecto_INCLUDE_DIRS}
  #                     ${PYTHON_INCLUDE_PATH}
  #                     ${BOOST_INCLUDE_DIRS}
  # )

  qi_create_lib(${NAME}_ectomodule SHARED
    ${ARGS_UNPARSED_ARGUMENTS}
    DEPENDS PYTHON BOOST ECTO
    NO_RPATH
  )
  qi_use_lib(${NAME}_ectomodule
    BOOST_PYTHON
    PYTHON
    ECTO
  )
  if(UNIX)
    set_target_properties(${NAME}_ectomodule
      PROPERTIES
      OUTPUT_NAME ${NAME}
      COMPILE_FLAGS "${FASTIDIOUS_FLAGS}"
      LINK_FLAGS -shared-libgcc
      PREFIX ""
      )
  elseif(WIN32)
    execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import distutils.sysconfig; print distutils.sysconfig.get_config_var('SO')"
      RESULT_VARIABLE PYTHON_PY_PROCESS
      OUTPUT_VARIABLE PY_SUFFIX
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    set_target_properties(${NAME}_ectomodule
      PROPERTIES
      COMPILE_FLAGS "${FASTIDIOUS_FLAGS}"
      OUTPUT_NAME ${NAME}
      PREFIX ""
      SUFFIX ${PY_SUFFIX}
      )
    message(STATUS "Using PY_SUFFIX = ${PY_SUFFIX}")
  endif()
  if(APPLE)
    set_target_properties(${NAME}_ectomodule
      PROPERTIES
      SUFFIX ".so"
      )
  endif()
  if(UNIX AND NOT APPLE)
    set_target_properties(${NAME}_ectomodule
        PROPERTIES
          INSTALL_RPATH "\$ORIGIN/lib;\$ORIGIN/../lib"
      )
  endif()

  if (ARGS_INSTALL)
    qi_install_target(${NAME}_ectomodule
      SUBFOLDER python2.7/dist-packages/${ARGS_DESTINATION}
    )
  endif()
endmacro()

# ==============================================================================

macro(link_ecto NAME)
  qi_use_lib(${NAME}_ectomodule
    ${ARGN}
  )
endmacro()
