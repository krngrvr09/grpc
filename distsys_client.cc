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

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using distsys::Feature;
using distsys::Point;
using distsys::Rectangle;
using distsys::RouteGuide;
using distsys::RouteNote;
using distsys::RouteSummary;
using distSys::Request;
using distsys::Response;

Point MakePoint(long latitude, long longitude) {
  Point p;
  p.set_latitude(latitude);
  p.set_longitude(longitude);
  return p;
}

int intarg = 1;
long longarg=2;
string stringarg = "123";
char *bytesarg = {0};
Request makeRequest(){
	Request request;
	request.set_intarg(intarg);
	request.set_longarg(longarg);
	request.set_stringarg(stringarg);
	request.set_bytesarg(bytesarg);
	return request;
}

Feature MakeFeature(const std::string& name, long latitude, long longitude) {
  Feature f;
  f.set_name(name);
  f.mutable_location()->CopyFrom(MakePoint(latitude, longitude));
  return f;
}

RouteNote MakeRouteNote(const std::string& message, long latitude,
                        long longitude) {
  RouteNote n;
  n.set_message(message);
  n.mutable_location()->CopyFrom(MakePoint(latitude, longitude));
  return n;
}

class DistsysClient {
 public:
  DistsysClient(std::shared_ptr<Channel> channel, const std::string& db)
      : stub_(Distsys::NewStub(channel)) {
	      //commenting this out because we are not using it. Do we need the constructor then?
    //routeguide::ParseDb(db, &feature_list_);
  }

  void GetString() {
    Point point;
    Feature feature;
    point = MakePoint(409146138, -746188906);
    //GetOneFeature(point, &feature);
    //point = MakePoint(0, 0);
    Request request;
    request = makeRequest();
    Response response;
    GetOneString(request, &response);
  }

  void ListStrings() {
    distsys::Rectangle rect;
    Feature feature;
    std::string response_string;
    ClientContext context;

    rect.mutable_lo()->set_latitude(400000000);
    rect.mutable_lo()->set_longitude(-750000000);
    rect.mutable_hi()->set_latitude(420000000);
    rect.mutable_hi()->set_longitude(-730000000);
    std::cout << "Looking for features between 40, -75 and 42, -73"
              << std::endl;
    Request request = makeRequest();
    Response response;
    std::unique_ptr<ClientReader<Response> > reader(
        stub_->ListStrings(&context, request));
    while (reader->Read(&response)) {
      std::cout << "Found response string: " << response.response_message() << std::endl;
    }
    Status status = reader->Finish();
    if (status.ok()) {
      std::cout << "ListStrings rpc succeeded." << std::endl;
    } else {
      std::cout << "ListStrings rpc failed." << std::endl;
    }
  }

  void RecordRouteString() {
    Point point;
    RouteSummary stats;
    std::string response_string;
    ClientContext context;
    const int kPoints = 10;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::default_random_engine generator(seed);
    //std::uniform_int_distribution<int> feature_distribution(
    //    0, feature_list_.size() - 1);
    std::uniform_int_distribution<int> delay_distribution(500, 1500);
    Response response;
    std::unique_ptr<ClientWriter<Request> > writer(
        stub_->RecordRouteString(&context, &response));
    for (int i = 0; i < kPoints; i++) {
      //const Feature& f = feature_list_[feature_distribution(generator)];
      //std::cout << "Visiting point " << f.location().latitude() / kCoordFactor_
      //          << ", " << f.location().longitude() / kCoordFactor_
      //          << std::endl;
      if (!writer->Write(f.location())) {
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
      std::cout << "RecordRoute rpc failed." << std::endl;
    }
  }

  void RouteChatString() {
    ClientContext context;

    std::shared_ptr<ClientReaderWriter<Request, Response> > stream(
        stub_->RouteChatString(&context));

    std::thread writer([stream]() {
      std::vector<Request> requests{makeRequest(),makeRequest(), makeRequest(), makeRequest()
				   };
      int request_idx=0;
      for (const Request& request : requests) {
        std::cout << "Sending request " << request_idx << std::endl;
        stream->Write(note);
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
      std::cout << "RouteChat rpc failed." << std::endl;
    }
  }

 private:
  bool GetOneString(const Request& request, Response* reponse) {
    ClientContext context;
    Status status = stub_->GetString(&context, request, response);
    if (!status.ok()) {
      std::cout << "GetFeature rpc failed." << std::endl;
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

  const float kCoordFactor_ = 10000000.0;
  std::unique_ptr<Distsys::Stub> stub_;
  //std::vector<Feature> feature_list_;
};

int main(int argc, char** argv) {
  // Expect only arg: --db_path=path/to/route_guide_db.json.
  std::string db = "";//routeguide::GetDbFileContent(argc, argv);
  RouteGuideClient guide(
      grpc::CreateChannel("localhost:50051",
                          grpc::InsecureChannelCredentials()),
      db);

  std::cout << "-------------- GetString --------------" << std::endl;
  guide.GetString();
  std::cout << "-------------- ListStrings --------------" << std::endl;
  guide.ListStrings();
  std::cout << "-------------- RecordRouteString --------------" << std::endl;
  guide.RecordRouteString();
  std::cout << "-------------- RouteChatString --------------" << std::endl;
  guide.RouteChatString();

  return 0;
}
