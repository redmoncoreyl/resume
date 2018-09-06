//very similar to pageset

#include <iostream>
#include <cstring>
#include <assert.h>
#include "wordset.h"
#include "pageset.h"

using namespace std;

/* Allocate a table of pointers to nodes, all initialized to NULL */
sNode **allocate_sNodetable(int size) {
	sNode **table = new sNode *[size];
	for (int i=0; i<size; i++)
		table[i] = NULL;
	return table;
}

Wordset::Wordset() {
	size = 4; // initial size of table
	table = allocate_sNodetable(size);
	num_elems = 0; 
}

Wordset::Wordset(int numEl) {
	num_elems = numEl;
	size = 2*numEl;	
	table = allocate_sNodetable(size);
}

Wordset::~Wordset() {
	for (int i=0; i<size; i++) {
		while (table[i] != NULL) {
			sNode *temp = table[i];
			table[i] = table[i]->next;
			delete temp;
		}
	}
	delete[] table;
}

/* Return true if key is in the set */
sNode *Wordset::find(string key) {
	int h = hash(key, size);
	sNode *n = table[h];
	while (n != NULL) {
		if (n->word == key)
			return n;
		n = n->next;
	}
	return n;
}

/* Inserts a new key. It is an error if key is already in the set. */
sNode *Wordset::insert(string key) {
	assert (!find(key));
	int hashVal = hash(key, size);
	sNode *newsNode = new sNode(key, table[hashVal]);
	table[hashVal] = newsNode;
	return newsNode;
}

/* Removes a key. It is an error if key isn't in the set */
void Wordset::remove(string key) {
	assert (find(key));
	num_elems--;

	int hashVal = hash(key, size);
	sNode *cursNode = table[hashVal];
	sNode *tempsNode;

	if (cursNode->word != key) {
		// go through the list to find the key
		while (cursNode->next->word != key) {
			cursNode = cursNode->next;
		}
		// delete key
		tempsNode = cursNode->next;
		cursNode->next = tempsNode->next;
		delete tempsNode;
	} else {	// special case if key is first element
		table[hashVal] = cursNode->next;
		delete cursNode;
	}	
}

void Wordset::print(void) {
	int i;
	sNode *cursNode = table[0];
	
	// for each list in hash table
	for (i = 0; i < size; i++) {
		cursNode = table[i];
		// print each element in list
		while (cursNode != NULL) {
			cout << cursNode->word << endl;
			cursNode = cursNode->next;
		}
	}
}
