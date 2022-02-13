# Copyright 2020 the gRPC authors.
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

licenses(["notice"])

cc_library(
    name = "distsys_helper",
    srcs = [
        "helper.cc",
        "helper.h",
    ],
    defines = ["BAZEL_BUILD"],
    deps = [
        "//:grpc++",
        "//examples/protos:distsys",
    ],
)

cc_binary(
    name = "distsys_client",
    srcs = [
        "distsys_client.cc",
    ],
    data = ["distsys_db.json"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":distsys_helper",
        "//:grpc++",
        "//examples/protos:distsys",
    ],
)

cc_binary(
    name = "distsys_server",
    srcs = [
        "distsys_server.cc",
    ],
    data = ["distsys_db.json"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":distsys_helper",
        "//:grpc++",
        "//examples/protos:distsys",
    ],
)

cc_binary(
    name = "distsys_callback_client",
    srcs = [
        "distsys_callback_client.cc",
    ],
    data = ["distsys_db.json"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":distsys_helper",
        "//:grpc++",
        "//examples/protos:distsys",
    ],
)

cc_binary(
    name = "distsys_callback_server",
    srcs = [
        "distsys_callback_server.cc",
    ],
    data = ["distsys_db.json"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":distsys_helper",
        "//:grpc++",
        "//examples/protos:distsys",
    ],
)
