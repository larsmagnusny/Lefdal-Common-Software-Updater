#define DL_CLASS

#include <string>
#include <iostream>
#include <fstream>
#include "ZipOperations.cpp"

class DOWNLOADED_FILE {
private:
	std::string binary_data;
	std::string uncompressed;
	std::string type;
	bool compressed;
public:
	DOWNLOADED_FILE(){
		type = "";
	}

	DOWNLOADED_FILE(std::string str){
		binary_data = str;
	}

	void print_file(){
		std::cout << uncompressed << std::endl;
	}

	void save_file(std::string loc){
		std::ofstream myFile;
		myFile.open(loc.c_str(), std::ios::out | std::ios::binary);

		myFile.write(uncompressed.c_str(), uncompressed.length());
		myFile.close();
	}

	void SetType(std::string str){
		type = str;
	}

	std::string GetType(){
		return type;
	}

	std::string GetUncompressedData(){
		return uncompressed;
	}

	void SetBinaryData(std::string s){
		binary_data = s;

		ZipOperations z;
		if(z.gzipInflate(binary_data, uncompressed)){
			compressed = false;
		} else {
			std::cout << "Could not inflate data" << std::endl;
		}
	}

	void SetUncompressed(std::string s){
		uncompressed = s;
	}

	void SetCompressed(bool v){
		compressed = v;
	}

	bool GetCompressed(){
		return compressed;
	}
};
