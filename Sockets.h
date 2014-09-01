#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
#include "HTTP.h"

#define BUFFSIZE 1024

class Sockets {
private:
public:
	int sock_connect(std::string server_ip, int port){
		int sock;
		struct sockaddr_in echoserver;

		// Create TCP-Socket
		if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
			std::cout << "Creating socket failed!" << std::endl;
			return -1;
		}

		memset(&echoserver, 0, sizeof(echoserver));
		echoserver.sin_family = AF_INET;
		echoserver.sin_addr.s_addr = inet_addr(server_ip.c_str());
		echoserver.sin_port = htons(port);

		// Establish connection

		if (connect(sock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
			std::cout << "Connection failed!" << std::endl;
			return -1;
		}

		return sock;
	}

	std::string send_and_recieve(int sock, std::string str){
		std::string ret;
		char *buffer = new char[2];
		unsigned int echolen;
		int recieved = 0;
		bool endsignature = false;

		echolen = str.length();
		if(send(sock, str.c_str(), echolen, 0) != echolen){
			return NULL;
		}

		int bytes = 0;

		do{
			bytes = recv(sock, buffer, 1, 0);
			ret += buffer[0];
		} while(ret.find("\r\n\r\n") == -1);

		if(bytes > 0){
			//Read Header then determine how to read the rest
			std::string header = ret;

			Header h(header);

			std::string transfer_encoding = h.GetValue("Transfer-Encoding");

			unsigned int content_length = atoi(h.GetValue("Content-Length").c_str());
			h.print_all();
			if(content_length > 0){
				char* buf = new char[content_length+1];
				recv(sock, buf, content_length,0);
				ret += buf;
			}

			std::cout << content_length << std::endl;

			unsigned int bytesread = 0;

			ret = "";

			if(transfer_encoding.compare("") == 0){      // Normal transfer

			} else if(transfer_encoding.compare("chunked") == 0){
					std::cout << "chunked" << std::endl; // Works most of the time?

					while(ret.find("\r\n\r\n") == -1){
					std::string hex;
					buffer = new char[2];
					do {
						recv(sock, buffer, 1, 0);
						buffer[1] = '\0';
						hex += buffer;
					} while(hex.find("\r\n") == -1);

					hex = hex.substr(0, hex.length()-2);
					std::cout << std::endl << hex << std::endl;
					unsigned int read_size = hex_to_decimal(hex);

					buffer = new char[read_size+1];

					int bytes_read = 0;

					while(bytes_read != read_size && bytes_read < read_size){
						if((bytes = recv(sock, buffer, read_size-bytes_read, 0)) < 0){
							read_size = bytes_read;
						}
						bytes_read += bytes;
						ret += buffer;

						std::cout << buffer << std::endl;
						std::cout << bytes_read << std::endl;
					}

					std::cout << ret << std::endl;
					std::cout << ret.length() << std::endl;
					}
			}
			// Create a handle for the incoming data
		}

		return "";
	}

	unsigned int hex_to_decimal(std::string hex){
		unsigned int x;
		std::stringstream ss;
		ss << std::hex << hex;
		ss >> x;
		return x;
	}

	char* nslookup(char * address){
		struct hostent *host;  // host information
		struct in_addr h_addr; // Internet address

		if((host = gethostbyname(address)) == NULL){
			fprintf(stderr, "(mini) nslookup failed on '%s'\n", address);
			return NULL;
		}
		h_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
		return inet_ntoa(h_addr);
	}
};
