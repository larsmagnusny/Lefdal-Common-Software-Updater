#include <iostream>
#include <stdio.h>
#include <string>
#include <Poco/Net/DNS.h>
#ifndef HP
#include "HTML_PARSER.cpp"
#endif

using namespace std;
using Poco::Net::DNS;
using Poco::Net::HostEntry;

int main(){
	const HostEntry& entry = DNS::hostByName("www.appinf.com");
	cout << "Canonical Name: " << entry.name() << endl;

	const HostEntry::AliasList& aliases = entry.aliases();
	HostEntry::AliasList::const_iterator it = aliases.begin();

	for(; it != aliases.end(); ++it){
		cout << "Alias: " << *it << endl;
	}

	const HostEntry::AddressList& addrs = entry.addresses();
	HostEntry::AddressList::const_iterator its = addrs.begin();

	for(; its != addrs.end(); ++its){
		cout << "Address: " << its->toString() << endl;
	}

	return 0;
}
