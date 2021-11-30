#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <list>
#include <vector>
#include <algorithm>
#include <time.h>


using namespace Sync; // create threads to handle clients

class ClientThread : public Thread
{
    
private:
    ByteArray data; // used to store the data
    Socket& socket;

public:
    ClientThread(Socket& socket) //constructor to instantiate the socket
    : socket(socket)
    {}

    ~ClientThread()
    {}


    virtual  long ThreadMain(){
       while(1){
           try{

               socket.Read(data);
               std::string string_data = data.ToString();

               std::reverse(string_data.begin(), string_data.end());
               data = ByteArray(string_data);

               socket.Write(data);
           }
           catch (...){
           }
       }
       return 0;
    }

};


// This thread handles the server operations
class ServerThread : public Thread
{
private:
    //ptr to the server socket *******
    SocketServer& server; // wil accept incoming request for stream sockets from any ip

    // create flag for termination
    bool flag = false;

public:

virtual long ThreadMain()
    {
        //wrap in try catch
        while(!flag){
            try{
                // waits for the client socket connection
                Socket* newConnection = new Socket(server.Accept());
                Socket& socketReference = *newConnection;
                ClientThread* clientThread = new ClientThread(socketReference);
            } catch (...){
                std::cout << " Server Shutdown" << std::endl;
            }
        }

	return 1;
    }

    ServerThread(SocketServer & server)
    : server(server)
    {}

    ~ServerThread()
    {
        flag = true;
        std::cout <<"Threads have gracefully exited" << std::endl;

    }
    
};


int main(void)
{
    std::cout << "This is the server." << std::endl;
	
    SocketServer server(3000);    // creating the server 

    // thread needed in order to perform server operations
    ServerThread serverThread(server);

	
    // wait for input to shutdown server
    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait(); // wait function
    std::cin.get();
    server.Shutdown(); // shuts down the server

}