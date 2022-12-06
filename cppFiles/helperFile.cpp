#include "helperFile.h"

vector<string> splitString(string str, string delim = " ") {
	vector<string> ret;
	int start = 0;
	int end = str.find(delim);
	while (end != -1) {
		ret.push_back(str.substr(start, end - start));
		start = end + delim.size();
		end = str.find(delim, start);
	}
	ret.push_back(str.substr(start, end-start));
	return ret;
}

U64 rank_i(int i) {
	return (U64) ((U64) 0xFF << (8*(i-1)));
}

U64 file_i(int i) {
	i = i+1;
	return (U64) ((U64) (file_mask << (i-1)) + (1 << (i-2)));
}

