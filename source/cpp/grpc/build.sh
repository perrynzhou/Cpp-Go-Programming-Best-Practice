g++ -std=c++0x   server.cc  -I/usr/local/include -L /usr/local/lib -o test_server -pthread  -lrt  -lprotobuf -lgrpc++ 
# g++ -std=c++0x  counter.pb.cc counter.grpc.pb.cc  client.cc  -o test_client -pthread -lgrpc -lprotobuf -lgrpc++_reflection -ldl



