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
	}

	std::string GetLinkByClass(std::string s){
		for(unsigned int i = 0; i < links.size(); ++i){
			std::string PropertyValue = links[i].getProperty("class");

			if(PropertyValue.compare(s) == 0){
				return links[i].getProperty("href");
			}
		}

		return "";
	}

};
