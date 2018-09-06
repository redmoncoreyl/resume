#include <cstdio>
#include "pageset.h"
#include "wordset.h"
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

// read file to create web hash table
Pageset *loadWeb() {
	//determine number of pages
	int webSize = 0;
	ifstream fin;
	string s;
	fin.open("webpages.txt");
	while (fin >> s) {
		if (s == "NEWPAGE") {
			webSize++;
		}
	}
	fin.close();
	
	// insert all pages
	Pageset *Web = new Pageset(webSize);
	Page *curPage = NULL;
	sNode *curWord = NULL;
	sNode *curLink = NULL;
	Page *linkPage = NULL;
	
	fin.open("webpages.txt");
	while (fin >> s) {
		if (s == "NEWPAGE") {
			fin >> s;
			Web->insert(s);
		}
	}
	fin.close();
	
	// insert all words and links
		//this must be done separately from inserting all pages so that
		//it will be able to insert links that are NEWPAGE forward references
	fin.open("webpages.txt");
	while (fin >> s) {
		if (s == "NEWPAGE") {
			fin >> s;
			curPage = Web->find(s);
		} else {
			if (s.find("http://") != string::npos) {
				if (linkPage = Web->find(s)) {
					curLink = new sNode(s, curPage->links);
					curLink->linkPage = linkPage;
					curPage->nLinks++;
					
					curPage->links = curLink;
				}
			} else {
				curWord = new sNode(s, curPage->words);
				curPage->words = curWord;
			}
		}
	}
	fin.close();
	
	return Web;
}

Wordset *loadInvertedWeb(Pageset *theWeb) {
	// count all unique words
	int wordSize = 0;
	ifstream fin;
	string s;
	fin.open("webpages.txt");
	while (fin >> s) {
		if (s != "NEWPAGE") {
			if(s.find("http://") == string::npos) {
				wordSize++;
			}
		}
	}
	fin.close();
	
	// create a list of copy of Pages from which each word comes from
	Wordset *invertedWeb = new Wordset(wordSize);
	Page *curPage = NULL;
	sNode *curWord = NULL;
	Page *pageCopy = NULL;
	
	fin.open("webpages.txt");
	while (fin >> s) {
		if (s == "NEWPAGE") {
			fin >> s;
			curPage = theWeb->find(s);
		} else {
			if (s.find("http://") == string::npos) {
				if (curWord = invertedWeb->find(s)) {
					pageCopy = new Page(curPage->URL, curWord->pages, curPage->weight);
					curWord->pages = pageCopy;
				} else {
					curWord = invertedWeb->insert(s);
					curWord->pages = NULL;
					pageCopy = new Page(curPage->URL, curWord->pages, curPage->weight);
					curWord->pages = pageCopy;
				}
			}
		}
	}
	fin.close();
	
	return invertedWeb;
}


// The following functions sort page lists and are slightly modified from class
Page *merge(Page *pList1, Page *pList2)
{
  if (pList1 == NULL) return pList2;
  if (pList2 == NULL) return pList1;
  if (pList1->weight > pList2->weight) {
    pList1->next = merge(pList1->next, pList2);
    return pList1;
  } else {
    pList2->next = merge(pList2->next, pList1);
    return pList2;
  }
}

Page *evens(Page *pList);
Page *odds(Page *pList);

Page *evens(Page *pList) 
{
  if (pList == NULL) return NULL;
  return new Page(pList->URL, pList->weight, odds(pList->next));
}

Page *odds(Page *pList) 
{
  if (pList == NULL) return NULL;
  return evens(pList->next);
}

Page *merge_sort(Page *pList)
{
  if (pList == NULL) return NULL;     // empty
  if (pList->next == NULL) return pList;  // 1 element
  return merge(merge_sort(evens(pList)), merge_sort(odds(pList)));
}


//----------MAIN-----------//
int main(void) {
	// load Web hash table class from file
	Pageset *theWeb = loadWeb();
	cout << "\nSuccessfully loaded web.\n\n";
	
	// page rank
	theWeb->weighPages(50);
	cout << "Successfully ranked pages.\n\n";
	
	// create word hash table from file
	Wordset *invertedWeb = loadInvertedWeb(theWeb);
	cout << "Successfully loaded inverted web.\n";
	
	// recieve input from user
	string inp;
	int curWeight;
	sNode *foundWord = NULL;
	Page *curPage = NULL;
	cout << "\nPlease input word (QUITNOW to stop): ";
	while (cin >> inp && inp != "QUITNOW") {
		if (foundWord = invertedWeb->find(inp)) {
			curPage = merge_sort(foundWord->pages);
			while (curPage != NULL) {
				curWeight = curPage->weight * 7500000;
				cout << curWeight << " " << curPage->URL << endl;
				curPage = curPage->next;
			}
		} else {
			cout << "\nCould not find word" << endl;
		}
		cout << "\nPlease input word (QUITNOW to stop): ";
	}
	cout << "\nThank you for your searches.\n\n";

	return(0);
}
