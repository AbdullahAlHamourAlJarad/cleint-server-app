#include "thread.h"
#include "socket.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

bool done = false;
using namespace Sync;

//re-define clienthread
class ClientThread: public Thread
{

private:
    Socket& socket;
    ByteArray data;
    std::string string_data;

public:
    ClientThread(Socket& socket)
    :socket(socket){}

    ~ClientThread()
    {}

    virtual long ThreadMain(){
        int open = socket.Open();

		 data = ByteArray(string_data);
            //write to server
            socket.Write(data);
            // read data
            socket.Read(data);
            string_data = data.ToString();

            std::cout << "server response: " << string_data << std::endl;

			
        while(true){
            std::cout << "Input data (enter 'done' to exit): ";
            std::cout.flush(); // sanitize the input
            std::getline(std::cin, string_data); // gets data from stream
            if(string_data == "done"){
                std::cout << "Exiting" << std::endl;
                done = true;
                break;
            }

           

        }
        return 0;

    }
};

int main(void)
{

	std::cout << "Client" << std::endl; // message 

	Socket socket("127.0.0.1", 3000); // creating the socket
	ClientThread clientThread(socket);
	while(done == false){
	    sleep(1);
	}
	socket.Close();
	return 0;

	socket.Close();
	return 0;
}

