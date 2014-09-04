#include <iostream>
#include <stdio.h>
#include <string>
#include "Sockets.h"
#ifndef HP
#include "HTML_PARSER.cpp"
#endif
#ifndef DL_CLASS
#include "Downloaded_File.cpp"
#endif
#ifndef LM
#include "LinkMiner.cpp"
#endif

using namespace std;

int main(){
	Sockets socket_def;

	// char * ip = socket_def.nslookup("");

	int socket = socket_def.sock_connect(socket_def.nslookup("www.vg.no"), 80);
	DOWNLOADED_FILE f = socket_def.send_and_recieve(socket, socket_def.GenerateGet("http://www.vg.no/"));

	if(f.GetCompressed()){
		cout << "The File is Compressed" << endl;
		f.print_file();
	}

	if(f.GetType().find("text/html") != std::string::npos){
		HTMLParser html(f.GetUncompressedData());
		LinkMiner lm(html);
	}
	f.save_file("/home/lars/javafile.tar.gz");
	return 1;
}
