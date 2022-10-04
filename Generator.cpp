#include "Generator.h"

Generator::Generator(const std::string& filename)
	:file(filename, std::ios::binary)
{
	if (!file.is_open()) {
		std::cerr << "cannot open the file\n";
		exit(-1);
	}
	file.seekg(0, std::ios::end);
	int size = static_cast<int>(file.tellg());
	file.seekg(0, std::ios::beg);
	str.resize(size);
	file.read(str.data(), size);
	file.close();
	str.push_back(0x0a);
}


void Generator::clear() 
{

	for (auto& i : str) {
		switch (i)
		{
		case 0x09: 
			i = 0x20;
			break;
		case 0x2c:
			i = 0x2e;
			break;
		}
		if (i != '.' 
			and (i | 7) != '7'
			and (i | 9) != '9'
			and i != ' '
			and i != 0x0a)
			i = ' ';
	}
	
	for (auto i = 0; i < str.size(); ++i) {
		if ((i == 0 and str[i] == '.') or (i == str.size() - 2 and str[i] == '.')) str[i] = ' ';
		else if (i != 0 and i != str.size() - 1 and str[i] == '.') {
			if (str[i - 1] == ' '
				or str[i - 1] == '.'
				or str[i + 1] == ' '
				or str[i + 1] == '.') 
				str[i] = ' ';
		}
	}

	for (auto i = str.begin(); i != str.end(); ++i) {
		if (i != str.begin() and i != (str.end() - 1) and *i == ' ') {
			if (*(i - 1) == ' '
				or *(i + 1) == ' '
				or *(i-1) == '.'
				or *(i+1) == '.') {
				str.erase(i);
				--i;
			}
		}
	}
	if (str[0] == ' ') str.erase(str.begin());

	for (auto i = 0; i < str.size(); ++i) {
		if (i != 0 and i != str.size() - 1) {
			if ((str[i + 1] == 0x0a and str[i] == ' ')
				or (str[i + 1] == ' ' and str[i] == 0x0a)) {
				str[i] = 0x0a;
			}
		}
	}

	for (auto i = str.begin(); i != str.end(); ++i) {
		if (i != str.begin() and i != (str.end() - 1) and *i == 0x0a) {
			if (*(i - 1) == 0x0a or *(i + 1) == 0x0a) {
				str.erase(i);
				--i;
			}
		}
	}

	for (auto it = ++str.begin(); it != str.end(); ++it) {
		if (*(it - 1) == 0x0a and *it == ' ') str.erase(it);
	}

	int max{};
	int k{};
	for (auto i = 1; i < str.size(); ++i) {
		if (str[i] == '.') {
			[ss = str](int ii, int& kk) {
				while (ii < ss.size() and (ss[ii] != 0x20 and ss[ii] != 0x0a)) {
					++kk;
					++ii;
				}
			}(i + 1, k);
		}
		if (str[i] == 0x20 or str[i] == 0x0a) {
			max = (max > k) ? max : k;
			k = 0;
		}
	}

	int ctr{};
	std::string::iterator copy{};
	for (auto it = str.begin(); it != str.end(); ++it) {
		if (*it == '.') {
			auto copy = it;
			while (*it != ' ' and *it != 0x0a and it != str.end()) {
				++ctr;
				++it;
			}
			for (auto j = 0; j <= max - ctr; ++j) {
				str.insert(it, '0');
				++it;
			}
			it = copy;
			ctr = 0;

		}
	}

	// add 0 before the value
	// add 0 after the value


	std::cout << str << '\n';

}


void Generator::preProcessor() {
	int row{};
	for (auto i : str) {
		if (i == 0x0a) ++row;
	}
	std::vector<int> collumn(row, 0);
	
	auto counter = 0;
	for (auto i = 0; i < str.size(); ++i) {
		if (str[i] == 0x20) {
			collumn[counter] += 1;
		}
		if (str[i] == 0x0a) ++counter;
	}
	auto spaces_counter = collumn[0];

	for (auto i : collumn) {
		if (i != spaces_counter) {
			std::cerr << "Different values quantity\n";
			exit(-47);
		}
	}
	
	for (auto i = 0; i < str.size(); ++i) {
		str[i] += 22 * !(str[i] - 0x0a);
	}
	
	if (row != collumn.size()) {
		std::cerr << "MATRIX IS NOT KVADRAT\n";
		exit(-156);
	}


}
