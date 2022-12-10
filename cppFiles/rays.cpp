#define __RAYS__

#ifndef __HELPERFILE__
#include "helperFile.h"
#endif

class Rays {
	public:

		Rays() {
			RAY= vector(8, vector<U64>(64));
			calc();
		}

		int square(int file, int rank) {
			return (file-1 + 8*(rank-1));
		}
		vector<vector<U64>> RAY;


		void calc() {
			int rank, file, f, r;
			U64 temp1, temp2;
			const U64 rDiag = (U64) 0x8040201008040201; 
			const U64 lDiag = (U64) 0x102040810204080;

			for (rank = 1; rank <= 8; rank++)
				for (file = 1; file<=8; file++) {
					RAY[EAST][square(file,rank)] = (rank_i(rank) << file) & (rank_i(rank));
				}
			
			for (rank = 1; rank <= 8; rank++)
				for (file = 1; file<=8; file++) {
					RAY[WEST][square(file,rank)] = (rank_i(rank) >> (8- file + 1)) & (rank_i(rank));
				}
			
			for (rank = 1; rank <= 8; rank++)
				for (file = 1; file<=8; file++) {
					RAY[NORTH][square(file,rank)] = (file_i(file) << 8*rank) & (file_i(file));
				}
			
			for (rank = 1; rank <= 8; rank++)
				for (file = 1; file<=8; file++) {
					RAY[SOUTH][square(file,rank)] = (file_i(file) >> 8*(8-rank + 1)) & (file_i(file));
				}
			
			for (rank = 1; rank <= 8; rank++) 
				for (file = 1; file <=8; file++) {
					temp1 = temp2 = 0;
					for (f = file; f<=8; f++) 
						temp1 |= file_i(f);
					for (r = rank; r<=8;r++)
						temp2 |=rank_i(r);
					RAY[NORTHEAST][square(file,rank)] = (rDiag << square(file+1,rank+1)) & (temp1 & temp2); 
				}
			
			for (rank = 1; rank <= 8; rank++) 
				for (file = 1; file <=8; file++) {
					temp1 = temp2 = 0;
					for (f = 1; f<file; f++) 
						temp1 |= file_i(f);
					for (r = rank; r<=8;r++)
						temp2 |=rank_i(r);
					RAY[NORTHWEST][square(file,rank)] = (lDiag << square(file,rank)) & (temp1 & temp2); 
				}
			
			for (rank = 1; rank <= 8; rank++) 
				for (file = 1; file <=8; file++) {
					temp1 = temp2 = 0;
					for (f = 1; f<file; f++) 
						temp1 |= file_i(f);
					for (r = 1; r<rank;r++)
						temp2 |=rank_i(r);
					RAY[SOUTHWEST][square(file,rank)] = (rDiag >> square(9-file,9-rank)) & (temp1 & temp2); 
				}
			for (rank = 1; rank <= 8; rank++) 
				for (file = 1; file <=8; file++) {
					temp1 = temp2 = 0;
					for (f = file+1; f<=8; f++) 
						temp1 |= file_i(f);
					for (r = 1; r<rank;r++)
						temp2 |=rank_i(r);
					RAY[SOUTHEAST][square(file,rank)] = (lDiag >> square(9-file,9-rank)) & (temp1 & temp2); 
				}
		}
};
