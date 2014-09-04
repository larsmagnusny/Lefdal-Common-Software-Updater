#include <string>
#include <iostream>
#include <vector>
#include "HTTP_STATUS.h"

class Header {
private:
	std::vector<std::string> property;
	std::vector<std::string> value;
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
