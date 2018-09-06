#include <cstring>
#include <iostream>
#include <cstdio>
#include "pageset.h"

using namespace std;

class Wordset {
	private:
		sNode **table;
		int size;
		int num_elems;
	
	public:
		Wordset();
		Wordset(int numEl);
		~Wordset();
		sNode *find(string key);
		sNode *insert(string key);
		void remove(string key);
		void print(void);
};
