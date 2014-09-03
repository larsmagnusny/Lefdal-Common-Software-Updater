#include <string>
#include <iostream>

class DOWNLOADED_FILE {
private:
	char* binary_data;
	unsigned long size;
public:
	DOWNLOADED_FILE(std::string str){
		binary_data = new char[str.length()+1];
		size = str.length();

		for(int i = 0; i < size; i++){
			binary_data[i] = str[i];
		}
	}

	void print_file(){
		for(int i = 0; i < size; i++){
			std::cout << binary_data[i];
		}
	}
};
