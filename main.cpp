#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

static std::string filename = "md5_rainbow_table_part_";
static std::string extension = ".csv";

static std::string hash = "\"b8599f2472cf69aef71545fef1c1b9e4\"";

std::string closestHash(std::ifstream& file, size_t pos) {
	file.clear();
	file.seekg(pos, std::ios_base::beg);
	
	char c;
	while ((c = file.get()) != '\n');

	std::string hash;
	file >> hash;
	return hash;
}

std::string find(const std::string& hash, bool& found) {
	std::string res;
	found = false;
	
	size_t fileL = 1, fileR = 12, fileN;
	while (fileL != fileR) {
		fileN= (fileL+fileR)/2;
		std::string line;
		std::ifstream file(filename + std::to_string(fileN) + extension);
		file >> line;
		if (hash < line) {
			fileR = fileN-1;
			continue;
		}
		if (fileN != 12) {
			file = std::ifstream(filename + std::to_string(fileN+1) + extension);
			file >> line;
			if (hash >= line) {
				fileL = fileN+1;
				continue;
			}
		}
		fileL = fileN;
		break;
	}

	std::ifstream file(filename + std::to_string(fileL) + extension);
	std::cout << "File found: " << fileL << std::endl;
	size_t left = 0;
	file.seekg(0, std::ios_base::end);
	size_t right = file.tellg();

	while (left != right) {
		size_t middle = (left+right)/2;
		std::string closest = closestHash(file, middle);
		if (hash < closest) {
			right = middle;
			continue;
		}
		if (hash > closest) {
			left = middle;
			continue;
		}
		found = true;
		file >> res;
		return res;
	}
	found = closestHash(file, left) == hash;
	if (found) file >> res;
	return res;
}

int main() {
	bool found = false;
	auto start = std::chrono::steady_clock::now();
	std::string res = find(hash, found);
	auto end = std::chrono::steady_clock::now();
	
	std::cout << "Hash: " << hash << std::endl;
	
	if (found) {
		std::cout << "Hash found: " << res << std::endl;
	} else {
		std::cout << "Hash not found" << std::endl;
	}
	
	std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << "us" << std::endl;
}
