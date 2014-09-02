#include <iostream>
#include <stdio.h>
#include <string>
#include "Sockets.h"
#include "HTML_PARSER.cpp"

using namespace std;

int main(){
	Sockets socket_def;

	// char * ip = socket_def.nslookup("");

	int socket = socket_def.sock_connect(socket_def.nslookup("www.vg.no"), 80);


	HTMLParser h(socket_def.send_and_recieve(socket, "GET / HTTP/1.1\nHost: www.vg.no\r\n\r\n"));

	return 0;
}
