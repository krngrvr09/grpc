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

#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include "helper.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#ifdef BAZEL_BUILD
#include "examples/protos/distsys.grpc.pb.h"
#else
#include "distsys.grpc.pb.h"
#endif
#include<unistd.h>
#include<time.h>
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using distsys::Request;
using distsys::Response;
using distsys::Distsys;
/*
int intarg = 1;
long longarg=2;
std::string stringarg = "123";
char *bytesarg = {0};
*/

timespec diff(timespec start, timespec end)
{
        timespec temp;
        if ((end.tv_nsec-start.tv_nsec)<0) {
                temp.tv_sec = end.tv_sec-start.tv_sec-1;
                temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
        } else {
                temp.tv_sec = end.tv_sec-start.tv_sec;
                temp.tv_nsec = end.tv_nsec-start.tv_nsec;
        }
        return temp;
}


Request makeRequestForGetString(){
	int32_t intarg = 1;
	int64_t longarg=2;
	std::string stringarg = "123";
	//char *bytesarg = {0};
	Request request;
	
	struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
		request.set_intarg(intarg);
        clock_gettime(CLOCK_MONOTONIC, &end);
        std::cout<<diff(start, end).tv_nsec<<std::endl;


      	clock_gettime(CLOCK_MONOTONIC, &start);
                request.set_intarg(intarg);
        clock_gettime(CLOCK_MONOTONIC, &end);
        std::cout<<diff(start, end).tv_nsec<<std::endl;
 	clock_gettime(CLOCK_MONOTONIC, &start);
	request.set_longarg(longarg);
	clock_gettime(CLOCK_MONOTONIC, &end);
	std::cout<<diff(start, end).tv_nsec<<std::endl;
	
	clock_gettime(CLOCK_MONOTONIC, &start);
	request.set_stringarg(stringarg);
	clock_gettime(CLOCK_MONOTONIC, &end);
	std::cout<<diff(start, end).tv_nsec<<std::endl;
	//request.set_bytesarg(bytesarg);
	return request;
}

Request makeRequest(){
        int32_t intarg = 1;
        int64_t longarg=2;
        std::string stringarg = "123";
        //char *bytesarg = {0};
        Request request;

        //struct timespec start, end;
        //clock_gettime(CLOCK_MONOTONIC, &start);
                request.set_intarg(intarg);
        //clock_gettime(CLOCK_MONOTONIC, &end);
        //std::cout<<diff(start, end).tv_nsec<<std::endl;

        request.set_longarg(longarg);
        request.set_stringarg(stringarg);
        //request.set_bytesarg(bytesarg);
        return request;
}

class DistsysClient {
 public:
  DistsysClient(std::shared_ptr<Channel> channel, const std::string& db)
      : stub_(Distsys::NewStub(channel)) {
	      //commenting this out because we are not using it. Do we need the constructor then?
    //routeguide::ParseDb(db, &feature_list_);
  }

  void GetString() {
    Request request;
    request = makeRequestForGetString();
    Response response;
    GetOneString(request, &response);
  }

  void GetStringStream() {
    ClientContext context;

    Request request = makeRequest();
    Response response;
    std::unique_ptr<ClientReader<Response> > reader(
        stub_->GetStringStream(&context, request));
    while (reader->Read(&response)) {
      std::cout << "Found response string: " << response.response_message() << std::endl;
    }
    Status status = reader->Finish();
    if (status.ok()) {
      std::cout << "GetStringStream rpc succeeded." << std::endl;
    } else {
      std::cout << "GetStringStream rpc failed." << std::endl;
    }
  }

  void SendStringStream() {
    ClientContext context;
    const int kPoints = 10;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    Request request = makeRequest();
    std::default_random_engine generator(seed);
    //std::uniform_int_distribution<int> feature_distribution(
    //    0, feature_list_.size() - 1);
    std::uniform_int_distribution<int> delay_distribution(500, 1500);
    Response response;
    std::unique_ptr<ClientWriter<Request> > writer(
        stub_->SendStringStream(&context, &response));
    for (int i = 0; i < kPoints; i++) {
      //const Feature& f = feature_list_[feature_distribution(generator)];
      //std::cout << "Visiting point " << f.location().latitude() / kCoordFactor_
      //          << ", " << f.location().longitude() / kCoordFactor_
      //          << std::endl;
      if (!writer->Write(request)) {
        // Broken stream.
        break;
      }
      std::this_thread::sleep_for(
          std::chrono::milliseconds(delay_distribution(generator)));
    }
    writer->WritesDone();
    Status status = writer->Finish();
    if (status.ok()) {
      std::cout << "Finished trip with " << response.response_message() << " string as response\n"
                << std::endl;
    } else {
      std::cout << "SendStringStream rpc failed." << std::endl;
    }
  }

  void ExchangeStringStream() {
    ClientContext context;

    std::shared_ptr<ClientReaderWriter<Request, Response> > stream(
        stub_->ExchangeStringStream(&context));

    std::thread writer([stream]() {
      std::vector<Request> requests{makeRequest(),makeRequest(), makeRequest(), makeRequest()
				   };
      int request_idx=0;
      for (const Request& request : requests) {
        std::cout << "Sending request " << request_idx << std::endl;
        stream->Write(request);
	request_idx++;
      }
      stream->WritesDone();
    });

    Response response;
    while (stream->Read(&response)) {
      std::cout << "Got message " << response.response_message() << std::endl;
    }
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
      std::cout << "ExchangeStringStream rpc failed." << std::endl;
    }
  }

 private:
  bool GetOneString(const Request& request, Response* response) {
	  //std::cout<<"Inside GetOneString"<<std::endl;
    ClientContext context;
    Status status = stub_->GetString(&context, request, response);
    if (!status.ok()) {
      std::cout << "GetFeature rpc failed." << std::endl;
      std::cout<<status.error_message()<<std::endl;
      std::cout<<status.error_details()<<std::endl;
      std::cout<<status.error_code()<<std::endl;
      return false;
    }
    /*
    if (!feature->has_location()) {
      std::cout << "Server returns incomplete feature." << std::endl;
      return false;
    }
    if (feature->name().empty()) {
      std::cout << "Found no feature at "
                << feature->location().latitude() / kCoordFactor_ << ", "
                << feature->location().longitude() / kCoordFactor_ << std::endl;
    } else {
      std::cout << "Found feature called " << feature->name() << " at "
                << feature->location().latitude() / kCoordFactor_ << ", "
                << feature->location().longitude() / kCoordFactor_ << std::endl;
    }
    */
    return true;
  }

  //const float kCoordFactor_ = 10000000.0;
  std::unique_ptr<Distsys::Stub> stub_;
  //std::vector<Feature> feature_list_;
};

int main(int argc, char** argv) {
	std::cout<<"argc: "<<argc<<std::endl;
  if(argc<2){
	  std::cout<<"usage: ./distsys_client <1|2|3|4|all>"<<std::endl;
	  return 0;
	// Expect only arg: --db_path=path/to/route_guide_db.json.
  }
  std::string db = "";//routeguide::GetDbFileContent(argc, argv);
  DistsysClient guide(
      grpc::CreateChannel("localhost:50051",
                          grpc::InsecureChannelCredentials()),
      db);

  std::string option(argv[1]);
  if(option=="1"||option=="all"){
  	std::cout << "-------------- GetString --------------" << std::endl;
  	guide.GetString();
  }
  if(option=="2"||option=="all"){  
	std::cout << "-------------- GetStringStream --------------" << std::endl;
  	guide.GetStringStream();
  }
  if(option=="3"||option=="all"){
	std::cout << "-------------- SendStringStream --------------" << std::endl;
  	guide.SendStringStream();
  }
  if(option=="4"||option=="all"){

  	std::cout << "-------------- ExchangeStringStream --------------" << std::endl;
  	guide.ExchangeStringStream();
  }
  return 0;
}
