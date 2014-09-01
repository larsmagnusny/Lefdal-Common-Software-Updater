#include <iostream>
#include <stdio.h>
#include <string>
#include "Sockets.h"

using namespace std;

int main(){
	Sockets socket_def;

	// char * ip = socket_def.nslookup("");

	int socket = socket_def.sock_connect(socket_def.nslookup("www.hig.no"), 80);

	cout << socket_def.send_and_recieve(socket, "GET / HTTP/1.1\nHost: www.hig.no\r\n\r\n") << endl;

	return 0;
}
