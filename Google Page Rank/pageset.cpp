#include <iostream>
#include <cstring>
#include <assert.h>
#include "pageset.h"

using namespace std;

/* Return a hash for the string s in the range 0..table_size-1 */
int hash(string s, int table_size) {
	unsigned int i, h = 0;
	for (i=0; i<s.length(); i++)
		h = (h * 2917 + (unsigned int)s[i]) % table_size;
	return h;
}

/* Allocate a table of pointers to nodes, all initialized to NULL */
Page **allocate_table(int size) {
	Page **table = new Page *[size];
	for (int i=0; i<size; i++)
		table[i] = NULL;
	return table;
}

Pageset::Pageset() {
	size = 4; // initial size of table
	table = allocate_table(size);
	num_elems = 0; 
}

Pageset::Pageset(int numEl) {
	num_elems = numEl;
	size = 2*numEl;	
	table = allocate_table(size);
}

Pageset::~Pageset() {
	for (int i=0; i<size; i++) {
		while (table[i] != NULL) {
			Page *temp = table[i];
			table[i] = table[i]->next;
			delete temp;
		}
	}
	delete[] table;
}

/* Return true if key is in the set */
Page *Pageset::find(string key) {
	int h = hash(key, size);
	Page *n = table[h];
	while (n != NULL) {
		if (n->URL == key)
			return n;
		n = n->next;
	}
	return n;
}

/* Inserts a new key. It is an error if key is already in the set. */
void Pageset::insert(string key) {
	assert (!find(key));
	int hashVal = hash(key, size);
	Page *newPage = new Page(key, table[hashVal]);
	table[hashVal] = newPage;
}

/* Removes a key. It is an error if key isn't in the set */
void Pageset::remove(string key) {
	assert (find(key));
	num_elems--;

	int hashVal = hash(key, size);
	Page *curPage = table[hashVal];
	Page *tempPage;

	if (curPage->URL != key) {
		// go through the list to find the key
		while (curPage->next->URL != key) {
			curPage = curPage->next;
		}
		// delete key
		tempPage = curPage->next;
		curPage->next = tempPage->next;
		delete tempPage;
	} else {	// special case if key is first element
		table[hashVal] = curPage->next;
		delete curPage;
	}	
}

void Pageset::print(void) {
	int i;
	Page *curPage = table[0];
	
	// for each list in hash table
	for (i = 0; i < size; i++) {
		curPage = table[i];
		// print each element in list
		while (curPage != NULL) {
			cout << curPage->URL << endl;
			curPage = curPage->next;
		}
	}
}

// uses the following page rank algorithm template:
// Give each page initial weight 1 / N
// Repeat 50 times:
//	 For each page i, set new_weight[i] = 0.1 / N
//	 For each page i,
//		 For each page j (of t total) to which i links,
//			 Increase new_weight[j] by 0.9 * weight[i] / t
//	 For each page i, set weight[i] = new_weight[i]
void Pageset::weighPages(int nIter) {
	int h, i;
	Page *curPage = table[0];
	sNode *curLink = NULL;
	Page *linkPage = NULL;
	
	for (i = 0; i < size; i++) {
		curPage = table[i];
		while (curPage != NULL) {
			curPage->weight = (double)1/num_elems;
			curPage = curPage->next;
		}
	}

	for (h = 0; h < nIter; h++) {
		for (i = 0; i < size; i++) {
			curPage = table[i];
			while (curPage != NULL) {
				curPage->newWeight = (double)0.1/num_elems;
				curPage = curPage->next;
			}
		}
		
		for (i = 0; i < size; i++) {
			curPage = table[i];
			while (curPage != NULL) {
				curLink = curPage->links;
				while (curLink != NULL) {
					linkPage = curLink->linkPage;
					linkPage->newWeight += 0.9 * curPage->weight / curPage->nLinks;
					curLink = curLink->next;
				}
				curPage = curPage->next;
			}
		}
		
		for (i = 0; i < size; i++) {
			curPage = table[i];
			while (curPage != NULL) {
				curPage->weight = curPage->newWeight;
				curPage = curPage->next;
			}
		}
	}
}
