#define HD_CLASS

#include <string>
#include <iostream>
#include <vector>
#include "HTTP_STATUS.h"

class Header {
private:
	std::vector<std::string> property;
	std::vector<std::string> value;
	std::vector<std::string> cookie_name;
	std::vector<std::string> cookie_value;
	std::vector<std::string> cookie_domain;
	int status;
public:
	Header(std::string str){
		std::string status_buf = "";
		int index = str.find("\r\n");

		status_buf = str.substr(0, index);

		str = str.substr(index+2, str.length()-(index+2));

		index = status_buf.find(" ");

		status = atoi(status_buf.substr(index+1, status_buf.length()-1).c_str());

		while(index != -1 && (unsigned int)index < str.length()){
			index = str.find("\r\n");

			if(index != -1){
				std::string line = str.substr(0, index);
				str = str.substr(index+2, str.length()-index-2);

				index = line.find(": ");

				property.push_back(line.substr(0, index));
				value.push_back(line.substr(index+2, line.length()-(index+2)));
			}
		}

		SaveCookies();
	}

	Header(){

	}

	void new_h(std::string str){
			property.clear();
			value.clear();

			status = 0;

			std::string status_buf = "";
			int index = str.find("\r\n");

			status_buf = str.substr(0, index);

			str = str.substr(index+2, str.length()-(index+2));

			index = status_buf.find(" ");

			status = atoi(status_buf.substr(index+1, status_buf.length()-1).c_str());

			while(index != -1 && (unsigned int)index < str.length()){
				index = str.find("\r\n");

				if(index != -1){
					std::string line = str.substr(0, index);
					str = str.substr(index+2, str.length()-index-2);

					index = line.find(": ");

					property.push_back(line.substr(0, index));
					value.push_back(line.substr(index+2, line.length()-(index+2)));
				}
			}

			SaveCookies();
		}

	void SaveCookies(){
		for(unsigned int i = 0; i < property.size(); ++i){
			if(property[i].compare("Set-Cookie") == 0 || property[i].compare("Set-cookie") == 0){
				std::string cookie1 = value[i];
				std::string cookie = "";

				for(unsigned int i = 0; i < cookie1.length(); i++){
					if(cookie1[i] != ' '){
						cookie += cookie1[i];
					}
				}

				std::vector<std::string> things = split_string(cookie, ";");


				int find = things[0].find("=");

				if(find != -1){
					std::string cookname = things[0].substr(0, find);
					std::string cookval = things[0].substr(find+1, things[0].length()-1);
					if(!ChangeCookie(cookname, cookval)){
						cookie_name.push_back(cookname);
						cookie_value.push_back(cookval);
					}
					std::cout << things[0].substr(0, find) << std::endl;
					std::cout << things[0].substr(find+1, things[0].length()-1) << std::endl;
				} else {
					std::cout << "STATEMENT" << std::endl;
				}
			}
		}
	}

	bool ChangeCookie(std::string name, std::string value){
		for(unsigned int i = 0; i < cookie_name.size(); ++i){
			if(cookie_name[i].compare(name) == 0){
				cookie_value[i] = value;
				return true;
			}
		}

		return false;
	}

	std::string GetCookies(){
		std::string buffer = " ";

		for(unsigned int i = 0; i < cookie_name.size(); ++i){
			buffer += cookie_name[i];
			buffer += "=";
			buffer += cookie_value[i];
			buffer += "; ";
		}

		return buffer;
	}

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

	std::string GetValue(std::string pro){
		for(unsigned int i = 0; i < property.size(); ++i){
			if(property[i].compare(pro) == 0){
				return value[i];
			}
		}
		return "";
	}

	int GetStatus(){
		return status;
	}

	void print_all(){
		for(unsigned int i = 0; i < property.size(); ++i){
			std::cout << "Property: " << property[i] << " Value: " << value[i] << std::endl;
		}
	}
};
