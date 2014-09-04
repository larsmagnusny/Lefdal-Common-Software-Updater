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
#ifndef DL_CLASS
#include "Downloaded_File.cpp"
#endif

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

	std::string GenerateGet(std::string location){
		std::string ret = "GET " + GetPathFromLocation(location) + " HTTP/1.1\r\n";
					ret += "Host: " + GetHostFromLocation(location) + "\r\n";
					ret += "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:32.0) Gecko/20100101 Firefox/32.0\r\n";
					ret += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
					ret += "Accept-Language: en-US,en;q=0.5\r\n";
					ret += "Accept-Encoding: gzip, deflate\r\n";
					ret += "Connection: keep-alive\r\n\r\n";

		return ret;
	}

	std::string GetHostFromLocation(std::string str){
		int find1 = str.find("http://");
		int find2 = str.find("/", find1+7);
		std::string host = str.substr(find1+7, find2-7);

		return host;
	}

	std::string GetPathFromLocation(std::string str){
		int find1 = str.find("http://");
		int find2 = str.find("/", find1+7);

		std::string location = str.substr(find2, str.length()-find2);

		return location;
	}

	DOWNLOADED_FILE send_and_recieve(int sock, std::string str){
		DOWNLOADED_FILE f;
		std::string ret;
		char *buffer = new char[2];
		unsigned int echolen;

		echolen = str.length();
		if(send(sock, str.c_str(), echolen, 0) != echolen){
			return DOWNLOADED_FILE();
		}

		std::cout << "Sendt Get Requiest";

		int bytes = 0;

		do{
			bytes = recv(sock, buffer, 1, 0);
			ret += buffer[0];
		} while(ret.find("\r\n\r\n") == -1);

		std::cout << "Got Answer" << std::endl;

		if(bytes > 0){
			//Read Header then determine how to read the rest
			std::string header = ret;

			std::cout << header << std::endl;

			Header h(header);

			std::string transfer_encoding = h.GetValue("Transfer-Encoding");

			unsigned int content_length = atoi(h.GetValue("Content-Length").c_str());
			h.print_all();
			f.SetType(h.GetValue("Content-Type"));

			std::cout << transfer_encoding << std::endl;

			unsigned int bytesread = 0;

			ret = "";

			std::cout << content_length << std::endl;

			if(h.GetValue("Content-Encoding").compare("gzip") == 0){
				f.SetCompressed(true);
			}

			if(content_length > 0 && h.GetStatus() == 200){      // Normal transfer
				int bytes_read = 0;
				int total_bytes_read = 0;
				char* buffer = new char[content_length+1];
				while(content_length > total_bytes_read){
					if((bytes_read = recv(sock, buffer, content_length-total_bytes_read, 0)) <= 0){
						break;
					} else {
						for(int i = 0; i < bytes_read; i++){
							ret += buffer[i];
						}
						total_bytes_read += bytes_read;
						std::cout << total_bytes_read << "/" << content_length << std::endl;
					}
				}
			} else if(transfer_encoding.compare("chunked") == 0 && h.GetStatus() == 200){
				int bytes_read = 0;
				int length = 0;
				do
				{
					std::string hex = "";
					char *byte = new char[2];
					while(hex.find("\r\n") == -1){
						if((bytes_read = recv(sock, byte, 1, 0)) < 0){
							break;
						} else {
							hex += byte[0];
							std::cout << byte[0];
						}
					}
					hex = hex.substr(0, hex.length()-2);
					length = hex_to_decimal(hex);

					std::cout << "Length: " << length << std::endl << std::endl;

					int total_bytes_read = 0;
					byte = new char[length+1];
					do {
						if((bytes_read = recv(sock, byte, length-total_bytes_read, 0)) < 0){
							break;
						} else {

							for(int i = 0; i < bytes_read; i++){
								ret += byte[i];
							}

							total_bytes_read += bytes_read;
							std::cout << length-total_bytes_read << std::endl;
						}
					} while(length > total_bytes_read);
					recv(sock, byte, 2, 0);
				} while(length != 0);
			} else if(h.GetStatus() == 302){
				close(sock);

				sock = sock_connect(nslookup(GetHostFromLocation(h.GetValue("Location")).c_str()), 80);
				f = send_and_recieve(sock, GenerateGet(h.GetValue("Location")));
			}
		}

		if(f.GetCompressed()){
			f.SetBinaryData(ret);
		} else {
			f.SetUncompressed(ret);
		}

		return f;
	}

	unsigned int hex_to_decimal(std::string hex){
		unsigned int x;
		std::stringstream ss;
		ss << std::hex << hex;
		ss >> x;
		return x;
	}

	const char* nslookup(const char * address){
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
