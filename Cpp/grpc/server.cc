
#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>

#include <grpcpp/server_context.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "counter.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using counter::CounterReply;
using counter::CounterRequest;
using counter::CounterService;

class CounterServiceImpl final : public CounterService::Service
{
    Status GetCounter(ServerContext *ctx, const CounterRequest *request, CounterReply *reply)
    {
        char buffer[256] = {'\0'};
        int value = rand();
        snprintf((char *)&buffer,256, " this is %ld message", value);
        reply->set_value(value);
        reply->set_message((char *)&buffer);
        return Status::OK;
    }
};
void RunCounterServer()
{
    std::string server_address("0.0.0.0:50051");
    CounterServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}
int main(int argc, char **argv)
{
    RunCounterServer();
    return 0;
}
