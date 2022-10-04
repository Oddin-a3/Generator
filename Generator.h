#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Generator
{
public:
	Generator() = default;

	~Generator() = default;

	Generator(const Generator& other) = delete;

	Generator& operator = (const Generator& other) = delete;

	Generator(const std::string& filename);

	void clear();


	void preProcessor();

	void generate();


private:
	std::ifstream file{};
	std::fstream outputFile{};
	int offset{};
	int quantityOfelem{};
	std::string str{};

	void helper_1(const std::size_t& i) const;
};

