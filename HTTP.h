#include <string>
#include <iostream>
#include <vector>

class Header {
private:
	std::vector<std::string> property;
	std::vector<std::string> value;
	std::string status;
public:
	Header(std::string str){

		int index = str.find("\r\n");

		status = str.substr(0, index);

		str = str.substr(index+2, str.length()-(index+2));

		index = status.find(" ");

		status = status.substr(index+1, status.length()-1);

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

	void print_all(){
		for(unsigned int i = 0; i < property.size(); ++i){
			std::cout << "Property: " << property[i] << " Value: " << value[i] << std::endl;
		}
	}
};
