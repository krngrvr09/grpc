/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include "helper.h"

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#ifdef BAZEL_BUILD
#include "examples/protos/distsys.grpc.pb.h"
#else
#include "distsys.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using distsys::Request;
using distsys::Response;
using std::chrono::system_clock;
using distsys::Distsys;


class DistsysImpl final : public Distsys::Service {
 public:
  explicit DistsysImpl(const std::string& db) {
    
  }

  Status GetString(ServerContext* context, const Request* request,
                    Response* response) override {
    
	std::cout<<"inside getstring"<<std::endl;
	response->set_response_code(200);
    response->set_response_message("ok");
    return Status::OK;
  }

  Status GetStringStream(ServerContext* context,
                      const Request* request,
                      ServerWriter<Response>* writer) override {
    Response response;
    response.set_response_code(200);
    response.set_response_message("ok");
    for (int i=0;i<10;i++) {
        writer->Write(response);
    }
    return Status::OK;
  }

  Status SendStringStream(ServerContext* context, ServerReader<Request>* reader,
                     Response* response) override {
    Request request;
    int test_int=0;
    system_clock::time_point start_time = system_clock::now();
    while (reader->Read(&request)) {
      test_int+=request.intarg();
      }
    system_clock::time_point end_time = system_clock::now();
    response->set_response_code(200);
    response->set_response_message("ok");
    auto secs =
        std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

    return Status::OK;
  }

  Status ExchangeStringStream(ServerContext* context,
                   ServerReaderWriter<Response, Request>* stream) override {
    Request request;
    //int test_int=0;
    while (stream->Read(&request)) {
      std::unique_lock<std::mutex> lock(mu_);
      for (const Request& r : received_requests_) {
        Response response;
	response.set_response_code(200);
	response.set_response_message("ok");
	stream->Write(response);
        }
      }
      received_requests_.push_back(request);
    

    return Status::OK;
  }

 private:
  std::mutex mu_;
  std::vector<Request> received_requests_;
};

void RunServer(const std::string& db_path) {
  std::string server_address("0.0.0.0:50051");
  DistsysImpl service(db_path);

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  // Expect only arg: --db_path=path/to/route_guide_db.json.
  std::string db = "";//distsys::GetDbFileContent(argc, argv);
  RunServer(db);

  return 0;
}
