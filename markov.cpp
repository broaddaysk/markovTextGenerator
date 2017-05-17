// markov.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <ctime>
#include <cstdlib>
using namespace std;

class Markov
{
public:
	void buildChain(unsigned int markovOrder);
	string generate(unsigned int wordLength);
private:
	map<string, vector<string>> chain;
	string getKey(deque<string> ngram, bool deleteLast);
	void extend(unsigned int len, vector<string>& sVec);
	unsigned int mOrder;
};

//joins deque elements together to form a key
string Markov::getKey(deque<string> ngram, bool deleteLast = false) {
	size_t size;
	deleteLast ? size = ngram.size() - 1 : size = ngram.size();
	string result;
	for (size_t i = 0; i < size; i++) {
		result += (ngram.front() + " ");
		ngram.pop_front();
	}
	return result.substr(0, result.length() - 1);
}

//build a markov chain of markovOrder order
void Markov::buildChain(unsigned int markovOrder) {
	mOrder = markovOrder;
	ifstream in("C:/Users/broaddaysk/Desktop/memes.txt");
	string word;
	deque<string> ngram; //a shifting deque spliced to get our keys and vals
	while (in >> word) {
		ngram.push_back(word);
		if (ngram.size() == mOrder + 1) {
			string val = ngram.back(), key = getKey(ngram, true);

			//if no key, then make empty vector and fill with val
			if (chain.find(key) == chain.end()) {
				chain[key] = { val };
			}
			else {
			//if key, then push_back val
				chain[key].push_back(val);
			}
			ngram.pop_front();
		}
	}
}

//extend a vector<string> via chain
void Markov::extend(unsigned int len, vector<string>& sVec) {
	deque<string> temp;
	for (unsigned int i = 0; i < mOrder; i++) {
		temp.push_back(sVec[i]);
	}
	for (unsigned int i = 0; i < len; i++) {
		string val, key = getKey(temp);
		if (chain.find(key) == chain.end()) {
			break;
		} else {
			//pick a random val, stored vals implicitly contain word distribution stats because repeats were allowed during build
			val = chain[key][rand() % chain[key].size()];
			temp.pop_front();
			temp.push_back(val);
			sVec.push_back(val);
		}
	}
}

//generate a string of wordLength
string Markov::generate(unsigned int wordLength) {
	//get random key to start the chain
	auto it = chain.begin();
	advance(it, rand() % chain.size());
	string word, result, randKey = it->first;
	istringstream iss(randKey);

	vector<string> strVec;
	while (iss >> word) {
		strVec.push_back(word);
	}
	extend(wordLength, strVec);
	for (size_t i = 0; i < strVec.size(); i++) {
		result += (strVec[i] + " ");
	}
	return result.substr(randKey.length(), result.length() - 1);
}

int main()
{
	Markov markov;
	unsigned int order;
	cout << "enter markov order: ";
	cin >> order;
	markov.buildChain(order); 
	srand((unsigned int)time(NULL));
	while (true) {
		cout << "\nenter word length: ";
		unsigned int len;
		cin >> len;
		cout << markov.generate(len) << "\n";
		ofstream out("C:/Users/broaddaysk/Desktop/output.txt");
		out << markov.generate(len);
	}
    return 0;
}