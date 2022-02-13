# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake build file for C++ distsys example.
# Assumes protobuf and gRPC have been installed using cmake.
# See cmake_externalproject/CMakeLists.txt for all-in-one cmake build
# that automatically builds all the dependencies before building distsys.

cmake_minimum_required(VERSION 3.5.1)

project(DistSys C CXX)

include(../cmake/common.cmake)

# Proto file
get_filename_component(ds_proto "../../protos/distsys.proto" ABSOLUTE)
get_filename_component(ds_proto_path "${ds_proto}" PATH)

# Generated sources
set(ds_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/distsys.pb.cc")
set(ds_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/distsys.pb.h")
set(ds_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/distsys.grpc.pb.cc")
set(ds_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/distsys.grpc.pb.h")
add_custom_command(
      OUTPUT "${ds_proto_srcs}" "${ds_proto_hdrs}" "${ds_grpc_srcs}" "${ds_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${ds_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${ds_proto}"
      DEPENDS "${ds_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# ds_grpc_proto
add_library(ds_grpc_proto
  ${ds_grpc_srcs}
  ${ds_grpc_hdrs}
  ${ds_proto_srcs}
  ${ds_proto_hdrs})
target_link_libraries(ds_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# distsys_helper
add_library(distsys_helper
  "helper.h"
  "helper.cc")
target_link_libraries(distsys_helper
  ds_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# Targets distsys_(client|server)
foreach(_target
  distsys_client distsys_server)
  add_executable(${_target}
    "${_target}.cc")
  target_link_libraries(${_target}
    ds_grpc_proto
    distsys_helper
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()
