#include <iostream>
#include <stdio.h>
#include <string>
#include "Sockets.h"
#include "HTML_PARSER.cpp"
#include "Downloaded_File.cpp"

using namespace std;

int main(){
	Sockets socket_def;

	// char * ip = socket_def.nslookup("");

	int socket = socket_def.sock_connect(socket_def.nslookup("download.macromedia.com"), 80);


	DOWNLOADED_FILE f(socket_def.send_and_recieve(socket, "GET /get/flashplayer/current/support/uninstall_flash_player.exe HTTP/1.1\nHost: download.macromedia.com\r\n\r\n"));
	f.print_file();

	return 1;
}
