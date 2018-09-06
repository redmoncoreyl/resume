// very similar to wordset

#ifndef PAGESET_H /* Prevent multiple inclusion... */
#define PAGESET_H

#include <cstring>
#include <iostream>
#include <cstdio>

using namespace std;

int hash(string s, int table_size);

struct Page;

struct sNode {
	string word;
	sNode *next;
	struct Page *pages; // for word hash table only
	Page *linkPage; // only for links (for reducing hash look ups)

	sNode(string k, sNode *n) {word = k; next = n;}
	sNode(string k, sNode *n, struct Page *p) {word = k; next = n; pages = p;}
};

struct Page {
	string URL;
	sNode *words;
	sNode *links;	
	int nLinks;
	Page *next;
	double weight;
	double newWeight;
	
	Page() {URL = ""; words = NULL; links = NULL; next = NULL; nLinks = 0;}
	Page(string u, Page *n) {URL = u; next = n; links = NULL; words = NULL; nLinks = 0;}
	Page(string u, Page *n, double w) {URL = u; next = n; weight = w;}
	Page(string u, double w, Page *n) {URL = u; weight = w; next = n;}
};

class Pageset {
	private:
		Page **table;
		int size;      // size of table, as currently allocated
		int num_elems; // number of elements stored in the table

	public:
		Pageset();
		Pageset(int numEl);
		~Pageset();
		Page *find(string key);
		void insert(string key);
		void remove(string key);
		void print(void);
		void weighPages(int nIter);
};

#endif
