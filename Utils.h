#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
# include <algorithm>
class Utils
{
public:
	//unsigned int split 
	//split a string <txt> everywhere a certain <ch> is found
	//store the resulting substrings in vector <strs>
	//returns the size of the vector
	static unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch) {
		int pos = txt.find(ch);	//Searches for the first instance of a character
		int initialPos = 0;
		strs.clear();
		while (pos != std::string::npos) {
			strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
			initialPos = pos + 1;

			pos = txt.find(ch, initialPos);
		}
		strs.push_back(txt.substr(initialPos, std::min<int>(pos, txt.size() - (initialPos + 1))));
		return strs.size();
	}

};

#endif // !UTILS_H