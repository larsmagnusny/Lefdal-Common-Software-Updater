#define HP

#ifndef _SSTREAM_
#include <sstream>
#endif

#ifndef _STRING_
#include <string>
#endif

#ifndef _VECTOR_
#include <vector>
#endif

#include <cctype>

class Element {
private:
	std::string tag;
	std::string text;
	std::vector<std::string> property;
	std::vector<std::string> value;

	std::vector<std::string> split_string(std::string str, const char* delim){
		std::vector<std::string> ret;
		int current_index = 0;
		int find = 0;
		while(find != -1){
			find = str.find(delim, current_index);

			if(find == -1){
				ret.push_back(str.substr(current_index, str.length()-current_index));
			} else {

			std::string temp = str.substr(current_index, find-current_index);

			ret.push_back(temp);
			}
			current_index = find+1;
		}

		return ret;
	}

public:
	Element(){
		tag = "";
	}

	Element(std::string html_element){
		int find1 = html_element.find('<');
		int find2 = html_element.find('>', find1+1);

		if(find1 != -1 && find2 != -1){
			std::string buf = html_element.substr(find1+1, find2-find1);

			int find4 = buf.find(' ', find1+1);

			if(find4 != -1){
				tag = buf.substr(0, find4);

				std::string rest = buf.substr(find4+1, buf.length()-1-(find4+1));

				std::vector<std::string> props = split_string(rest, "\" ");

				for(unsigned int i = 0; i < props.size(); ++i){
					props[i] = remove_space_untill_stop(props[i]);


					std::vector<std::string> props2 = split_string(props[i], "=\"");

					for(unsigned int i = 0; i < props2.size(); ++i){
						while(props2[i][props2[i].length()-1] == '"' || props2[i][props2[i].length()-1] == '/'){
							props2[i] = props2[i].substr(0, props2[i].length()-1);
						}
						while(props2[i][0] == '"'){
							props2[i] = props2[i].substr(1, props2[i].length()-1);
						}
					}

					if(props2.size() > 1){
						property.push_back(props2[0]);
						value.push_back(props2[1]);
					}

				}


				tag = toUpper(tag);

			} else {
				tag = toUpper(buf.substr(0, buf.length()-1));
			}

			std::string needle = "</";
			needle += toLower(tag);
			needle += ">";

			int find3 = html_element.find(needle);
			int find5 = html_element.find(toUpper(needle));

			if(find3 != -1){
				std::string rest = html_element.substr(find2+1, find3-find2-1);

				text = rest;
			} else if(find5 != -1){
				std::string rest = html_element.substr(find2+1, find5-find2-1);

				text = rest;
			} else {
				text = "";
			}
		}
	}

	std::string remove_space_untill_stop(std::string str){

		while(str[0] == ' '){
			str = str.substr(1, str.length()-1);
		}

		return str;
	}

	bool replace(std::string& str, const std::string& from, const std::string& to){
		size_t start_pos = str.find(from);
		if(start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}

	std::string getProperty(std::string name){
		for(unsigned int i = 0; i < property.size(); ++i){
			if(property[i].compare(name) == 0){
				return value[i];
			}
		}

		return "";
	}

	std::string getTag(){
		return tag;
	}

	std::string getText(){
		return text;
	}

	bool setProperty(std::string p, std::string v){
		for(unsigned int i = 0; i < property.size(); ++i){
			if(property[i].compare(p) == 0){
				value[i] = v;
				return true;
			}
		}

		return false;
	}

	bool addProperty(std::string p){
		property.push_back(p);
		value.push_back("");

		return true;
	}

	bool addProperty(std::string p, std::string v){
		property.push_back(p);
		value.push_back(v);

		return true;
	}

	std::string toUpper(std::string str){
		for (std::string::size_type i=0; i<str.length(); ++i){
			str[i] = std::toupper(str[i]);
		}
		return str;
	}

	std::string toLower(std::string str){
		for (std::string::size_type i=0; i<str.length(); ++i){
			str[i] = std::toupper(str[i]);
		}
		return str;
	}
};

class HTMLParser {
private:
	std::vector<Element> Elements;
	std::string page;
public:
	HTMLParser(std::string html){
		int find1 = 0, find2 = 0, cur_index = 0;

		while(find1 != -1 && find2 != -1){
			find1 = html.find('<', cur_index);
			find2 = html.find('>', find1+1);

			if((find1 != -1 && html[find1+1]!= '/') && find2 != -1){
				int find3 = html.find(' ', find1+1);

				if(find3 < find2){
					std::string needle = "</" + html.substr(find1+1, find3-find1-1) + ">";

					int find4 = html.find(needle);

					if(find4 != -1){
						std::string html2 = html.substr(find1, find4-find1+needle.length());
						Elements.push_back(Element(html2));
					} else {
						std::string html2 = html.substr(find1, find2-find1+1);
						Elements.push_back(Element(html2));
					}

					cur_index += find2-find1;
				} else {
					std::string needle = "</" + html.substr(find1+1, find2-find1-1) + ">";

					int find4 = html.find(needle);

					if(find4 != -1){
						std::string html2 = html.substr(find1, find4-find1+needle.length());
						Elements.push_back(Element(html2));
					} else {
						std::string html2 = html.substr(find1, find2-find1+1);
						Elements.push_back(Element(html2));
					}
					cur_index += find2-find1;
				}
			} else {
				cur_index++;
			}
		}
	}

	Element getElementByName(std::string str){
		for(unsigned int i = 0; i < Elements.size(); ++i){
			std::string name = Elements[i].getProperty("name");
			if(name.compare(str) == 0){
				return Elements[i];
			}
		}

		return Element();
	}

	Element getElementById(std::string str){
		for(unsigned int i = 0; i < Elements.size(); ++i){
			std::string id = Elements[i].getProperty("id");
			if(id.compare(str) == 0){
				return Elements[i];
			}
		}

		return Element();
	}

	std::vector<Element> getElementsByProperty(std::string prop){
		std::vector<Element> ret;
		for(unsigned int i = 0; i < Elements.size(); ++i){
			std::string proper = Elements[i].getProperty(prop);

			if(proper.compare("") != 0){
				ret.push_back(Elements[i]);
			}
		}

		return ret;
	}

	Element getElementsByPropertyValue(std::string str, std::string property){
		std::vector<Element> elmatch = getElementsByProperty(property);

		for(unsigned int a = 0; a < elmatch.size(); ++a){
			std::string value = elmatch[a].getProperty(property);

			if(value.compare(str) == 0){
				return elmatch[a];
			}
		}

		return Element();
	}

	std::vector<Element> getElementsByTag(std::string str){
		std::vector<Element> buf;

		for(unsigned int i = 0; i < Elements.size(); ++i){
			std::string tag = Elements[i].getTag();
			if(tag.compare(str) == 0){
				buf.push_back(Elements[i]);
			}
		}

		return buf;
	}

};
