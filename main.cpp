#include <bits/stdc++.h>

using namespace std;
using ll = long long;

#define forn(i, n) for(ll i = 0; i < n; i++)

struct Trie {
	struct Trie *nextLetters[256];
	// Store all of the possible next letters
	bool isLastCharacter;
	Trie(){
		isLastCharacter = false;
		forn(i, 26){
			nextLetters[i] = NULL;
		}
	}
};

// https://www.geeksforgeeks.org/trie-delete/?ref=lbp

void add(Trie *rootNode, string query){
	Trie *cur = rootNode;
	forn(i, query.length()){
		// Add a new child node to the current one if not already created
		if(!(cur -> nextLetters[ll(query[i])])){
			cur -> nextLetters[ll(query[i])] = new Trie();
		}
		// Go down one node in the tree and continue inserting the characters
		cur = cur -> nextLetters[ll(query[i])];
	}
	// We reached the last character
	cur -> isLastCharacter = true;
}

bool found(Trie *rootNode, string query){
	Trie *cur = rootNode;
	forn(i, query.length()){
		// If character i hasn't been found return false
		if(!(cur -> nextLetters[ll(query[i])])){
			return false;
		}
		// Go down one node in the tree and continue searching the characters
		cur = cur -> nextLetters[ll(query[i])];
	}
	// Make sure the final character is also in the tree
	return (cur -> isLastCharacter);
}

ll countChildren(Trie *rootNode){
	ll cnt = 0;
	forn(i, 256){
		if(rootNode -> nextLetters[i]) ++cnt;
	}
	return cnt;
}

// Bottom up deletions

Trie* deleteQuery(Trie *rootNode, string query, ll len){
	if(rootNode == NULL) return NULL;
	if(len == query.length()){
		// No more characters after this
		rootNode -> isLastCharacter = false;
		if(!countChildren(rootNode)){
			delete(rootNode);
			rootNode = NULL;
		}
	} else {
		rootNode -> nextLetters[ll(query[len])] = 
		deleteQuery(rootNode -> nextLetters[ll(query[len])], query, len + 1);
	
		if(!countChildren(rootNode) && !(rootNode -> isLastCharacter)){
			delete(rootNode);
			rootNode = NULL;
		}
	}
	return rootNode;
}

void display(Trie *rootNode, string query){
	if(rootNode -> isLastCharacter){
		cout << query << endl;
	}
	forn(i, 256){
		if(rootNode -> nextLetters[i]){
			display(rootNode -> nextLetters[i], query + char(i));
		}
	}
}

void autoComplete(Trie *rootNode, string query){
	Trie *cur = rootNode;
	ll cnt = 0;
	forn(i, query.length()){
		if(!(cur -> nextLetters[ll(query[i])])){
			cout << "Keyword not found." << endl;
			return;
		}
		cnt++;
		cur = cur -> nextLetters[ll(query[i])];
	}
	if(!countChildren(cur)){
		cout << query << "Found" << endl;
		return;
	}
	display(cur, query);
}

int main(){
	Trie *cur = new Trie();
	cout << "Welcome to the keyword search system with ASCII character supported." << endl;
	cout << "Here is a list of different queries you can type" << endl;
	cout << "1 query -> This adds the keyword 'query' to the database." << endl;
	cout << "2 query -> This deletes the keyword 'query from the database" << endl;
	cout << "3 query -> This searches for the keyword 'query' in the database." << endl;
	cout << "4 query -> This prints a list of similar keywords to 'query' in the database" << endl;
	cout << "5 -> This prints out all the words in the database" << endl;
	cout << "-1 -> This exits out from the keyword search system" << endl;
	while(true){
		cout << "Enter any of the queries from the supported operations." << endl;
		ll queryType; cin >> queryType;
		if(queryType == -1){
			break;
		}
		if(queryType == 5){
			display(cur, "");
			continue;
		}
		string query; cin >> query;
		if(queryType == 1){
			cout << "Adding " << query << " to the database..." << endl;
			add(cur, query);
			cout << "Added " << query << " to the database." << endl; 
		} else if(queryType == 2){
			cout << "Deleting " << query << " to the database..." << endl;
			deleteQuery(cur, query, 0);
			cout << "Deleted " << query << " from the database." << endl; 
		} else if(queryType == 3){
			cout << "Finding " << query << " in the database..." << endl;
			if(found(cur, query)){
				cout << "Found " << query << " in the database." << endl; 
			} else {
				cout << query << " not found." << endl;
			}
		} else {
			cout << "Did you mean any of these words?" << endl;
			autoComplete(cur, query);
		}
	}
}