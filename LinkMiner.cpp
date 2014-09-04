#ifndef HP
#include "HTML_PARSER.cpp"
#endif
#include <vector>
#include <iostream>

class LinkMiner {
private:
	std::vector<Element> links;
public:
	LinkMiner(HTMLParser html){
		links = html.getElementsByTag("A");

		for(unsigned int i = 0; i < links.size(); ++i){
			std::cout << links[i].getProperty("href") << std::endl;
		}
	}


};
