#include "helperFile.h"
using namespace std;
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

void bit_reverse(U64 * bb) {
	U64 b = *bb; 
	U64 c = b & 1;
	int s = 63;
	while(s != 0) {
		b>>=1;
		c<<=1;
		c|= (b&1);
		s--;
	}
	*bb = c;
}

int pop_1st_bit(U64 *bb) {
	U64 b = *bb ^ (*bb - 1);
	unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
	*bb &= (*bb - 1);
	return BitTable[(fold * 0x783a9b23) >> 26];
}

vector<U64> permutBit(U64 bb) {
	if (bb == (U64) 0) 
		return {(U64)0};
	vector<U64> result, temp;
	U64 t = ((U64)1 << (pop_1st_bit(&bb)));
	temp = permutBit(bb);
	
	result.clear();
	result.insert(result.end(), temp.begin(), temp.end());

	for(U64 r: temp) {
		result.push_back(t | r);
	}
	return result;

}
