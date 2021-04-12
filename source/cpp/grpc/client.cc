// greeter_client.cc
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "counter.grpc.pb.h"

using counter::CounterReply;
using counter::CounterRequest;
using counter::CounterService;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class CounterClient
{
public:
    CounterClient(std::shared_ptr<Channel> channel)
        : stub_(CounterService::NewStub(channel)) {}

    std::string GetCounter(const std::string &user)
    {
        // Data we are sending to the server.
        CounterRequest request;
        request.set_key(user);

        // Container for the data we expect from the server.
        CounterReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->GetCounter(&context, request, &reply);

        // Act upon its status.
        if (status.ok())
        {
            return reply.message();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<CounterService::Stub> stub_;
};

int main(int argc, char **argv)
{

    CounterClient counter(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));
    std::string key("first-key");
    std::string reply = counter.GetCounter(key);
    std::cout << "Greeter received: " << reply << std::endl;

    return 0;
}