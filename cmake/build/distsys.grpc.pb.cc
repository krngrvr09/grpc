// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: distsys.proto

#include "distsys.pb.h"
#include "distsys.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace distsys {

static const char* Distsys_method_names[] = {
  "/distsys.Distsys/GetString",
  "/distsys.Distsys/GetStringStream",
  "/distsys.Distsys/SendStringStream",
  "/distsys.Distsys/ExchangeStringStream",
};

std::unique_ptr< Distsys::Stub> Distsys::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Distsys::Stub> stub(new Distsys::Stub(channel, options));
  return stub;
}

Distsys::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetString_(Distsys_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetStringStream_(Distsys_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_SendStringStream_(Distsys_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::CLIENT_STREAMING, channel)
  , rpcmethod_ExchangeStringStream_(Distsys_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  {}

::grpc::Status Distsys::Stub::GetString(::grpc::ClientContext* context, const ::distsys::Request& request, ::distsys::Response* response) {
  return ::grpc::internal::BlockingUnaryCall< ::distsys::Request, ::distsys::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetString_, context, request, response);
}

void Distsys::Stub::async::GetString(::grpc::ClientContext* context, const ::distsys::Request* request, ::distsys::Response* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::distsys::Request, ::distsys::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetString_, context, request, response, std::move(f));
}

void Distsys::Stub::async::GetString(::grpc::ClientContext* context, const ::distsys::Request* request, ::distsys::Response* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetString_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::distsys::Response>* Distsys::Stub::PrepareAsyncGetStringRaw(::grpc::ClientContext* context, const ::distsys::Request& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::distsys::Response, ::distsys::Request, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetString_, context, request);
}

::grpc::ClientAsyncResponseReader< ::distsys::Response>* Distsys::Stub::AsyncGetStringRaw(::grpc::ClientContext* context, const ::distsys::Request& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetStringRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::ClientReader< ::distsys::Response>* Distsys::Stub::GetStringStreamRaw(::grpc::ClientContext* context, const ::distsys::Request& request) {
  return ::grpc::internal::ClientReaderFactory< ::distsys::Response>::Create(channel_.get(), rpcmethod_GetStringStream_, context, request);
}

void Distsys::Stub::async::GetStringStream(::grpc::ClientContext* context, const ::distsys::Request* request, ::grpc::ClientReadReactor< ::distsys::Response>* reactor) {
  ::grpc::internal::ClientCallbackReaderFactory< ::distsys::Response>::Create(stub_->channel_.get(), stub_->rpcmethod_GetStringStream_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::distsys::Response>* Distsys::Stub::AsyncGetStringStreamRaw(::grpc::ClientContext* context, const ::distsys::Request& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::distsys::Response>::Create(channel_.get(), cq, rpcmethod_GetStringStream_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::distsys::Response>* Distsys::Stub::PrepareAsyncGetStringStreamRaw(::grpc::ClientContext* context, const ::distsys::Request& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::distsys::Response>::Create(channel_.get(), cq, rpcmethod_GetStringStream_, context, request, false, nullptr);
}

::grpc::ClientWriter< ::distsys::Request>* Distsys::Stub::SendStringStreamRaw(::grpc::ClientContext* context, ::distsys::Response* response) {
  return ::grpc::internal::ClientWriterFactory< ::distsys::Request>::Create(channel_.get(), rpcmethod_SendStringStream_, context, response);
}

void Distsys::Stub::async::SendStringStream(::grpc::ClientContext* context, ::distsys::Response* response, ::grpc::ClientWriteReactor< ::distsys::Request>* reactor) {
  ::grpc::internal::ClientCallbackWriterFactory< ::distsys::Request>::Create(stub_->channel_.get(), stub_->rpcmethod_SendStringStream_, context, response, reactor);
}

::grpc::ClientAsyncWriter< ::distsys::Request>* Distsys::Stub::AsyncSendStringStreamRaw(::grpc::ClientContext* context, ::distsys::Response* response, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::distsys::Request>::Create(channel_.get(), cq, rpcmethod_SendStringStream_, context, response, true, tag);
}

::grpc::ClientAsyncWriter< ::distsys::Request>* Distsys::Stub::PrepareAsyncSendStringStreamRaw(::grpc::ClientContext* context, ::distsys::Response* response, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::distsys::Request>::Create(channel_.get(), cq, rpcmethod_SendStringStream_, context, response, false, nullptr);
}

::grpc::ClientReaderWriter< ::distsys::Request, ::distsys::Response>* Distsys::Stub::ExchangeStringStreamRaw(::grpc::ClientContext* context) {
  return ::grpc::internal::ClientReaderWriterFactory< ::distsys::Request, ::distsys::Response>::Create(channel_.get(), rpcmethod_ExchangeStringStream_, context);
}

void Distsys::Stub::async::ExchangeStringStream(::grpc::ClientContext* context, ::grpc::ClientBidiReactor< ::distsys::Request,::distsys::Response>* reactor) {
  ::grpc::internal::ClientCallbackReaderWriterFactory< ::distsys::Request,::distsys::Response>::Create(stub_->channel_.get(), stub_->rpcmethod_ExchangeStringStream_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::distsys::Request, ::distsys::Response>* Distsys::Stub::AsyncExchangeStringStreamRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::distsys::Request, ::distsys::Response>::Create(channel_.get(), cq, rpcmethod_ExchangeStringStream_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::distsys::Request, ::distsys::Response>* Distsys::Stub::PrepareAsyncExchangeStringStreamRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::distsys::Request, ::distsys::Response>::Create(channel_.get(), cq, rpcmethod_ExchangeStringStream_, context, false, nullptr);
}

Distsys::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Distsys_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Distsys::Service, ::distsys::Request, ::distsys::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Distsys::Service* service,
             ::grpc::ServerContext* ctx,
             const ::distsys::Request* req,
             ::distsys::Response* resp) {
               return service->GetString(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Distsys_method_names[1],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< Distsys::Service, ::distsys::Request, ::distsys::Response>(
          [](Distsys::Service* service,
             ::grpc::ServerContext* ctx,
             const ::distsys::Request* req,
             ::grpc::ServerWriter<::distsys::Response>* writer) {
               return service->GetStringStream(ctx, req, writer);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Distsys_method_names[2],
      ::grpc::internal::RpcMethod::CLIENT_STREAMING,
      new ::grpc::internal::ClientStreamingHandler< Distsys::Service, ::distsys::Request, ::distsys::Response>(
          [](Distsys::Service* service,
             ::grpc::ServerContext* ctx,
             ::grpc::ServerReader<::distsys::Request>* reader,
             ::distsys::Response* resp) {
               return service->SendStringStream(ctx, reader, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Distsys_method_names[3],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< Distsys::Service, ::distsys::Request, ::distsys::Response>(
          [](Distsys::Service* service,
             ::grpc::ServerContext* ctx,
             ::grpc::ServerReaderWriter<::distsys::Response,
             ::distsys::Request>* stream) {
               return service->ExchangeStringStream(ctx, stream);
             }, this)));
}

Distsys::Service::~Service() {
}

::grpc::Status Distsys::Service::GetString(::grpc::ServerContext* context, const ::distsys::Request* request, ::distsys::Response* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Distsys::Service::GetStringStream(::grpc::ServerContext* context, const ::distsys::Request* request, ::grpc::ServerWriter< ::distsys::Response>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Distsys::Service::SendStringStream(::grpc::ServerContext* context, ::grpc::ServerReader< ::distsys::Request>* reader, ::distsys::Response* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Distsys::Service::ExchangeStringStream(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::distsys::Response, ::distsys::Request>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace distsys

