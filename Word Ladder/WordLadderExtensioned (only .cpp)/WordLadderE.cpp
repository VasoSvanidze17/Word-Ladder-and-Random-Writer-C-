/*
 * File: WordLadder.cpp
 * --------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Word Ladder problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include "console.h"
#include "simpio.h"
#include <string>
#include <cctype>
#include "strlib.h"
#include <fstream>
#include "tokenscanner.h"
#include "stack.h"
#include "vector.h"
#include "lexicon.h"
#include "set.h"
#include "map.h"
#include "queue.h"
#include <time.h> 
using namespace std;

struct Stats{
	long length;
	long fullLength;
};

//Prototype of function
Lexicon addWordsInLexiconFromFile();
void addWordsInLexiconFromConsole(Lexicon &lex, string inputStr, string finalStr);
bool getYesOrNo(string str = "");
Stack<string> wordLadder(Lexicon &lex, string inputStr, string finalStr, Stats &stats);
Lexicon findAllWordsWithDifferentOneLetter(Lexicon &lex, string str);
void printWordWithChangedSeparatelyChar(Stack<string> &ladder);
Stack<string> reverseStack(Stack<string> &ladder);

int main() {
	Stats stats; //for counting stats
	clock_t t; //for measuring time
	Lexicon lex = addWordsInLexiconFromFile();
	Stack<string> ladder;
	string inputStr;
	string finalStr;

	while(true){
		inputStr = getLine("Enter start word: ");
		finalStr = getLine("Enter destination word: ");
		cout << endl;
		t = clock(); //start measuring
	
		//checking inputed word
		if(inputStr.length() != finalStr.length()){
			cout << "Due to the different lengths of the words entered, the program cannot find the ladder" << endl;
		}else if(!lex.contains(inputStr) || !lex.contains(finalStr)){
			//this function add inputed words in Lexicon, words which aren't in Lexicon currently
			addWordsInLexiconFromConsole(lex, inputStr, finalStr); 
		}else{//If there is a chance to find a ladder
			ladder = wordLadder(lex, inputStr, finalStr, stats); //searching word ladder
		
			t = clock() - t; //End measuring
			
			//checking found ladder if not found
			if(ladder.size() > 1){
				cout << "Found ladder: " << ladder.toString() << endl;
			}else{
				cout << "Searchin result: no ladder found" << endl;
			}
			
			//print some stats
			printWordWithChangedSeparatelyChar(ladder);
			cout << "Width search length (word location length/full lenght): " << stats.length << "/" << stats.fullLength << endl; 
			cout << "Time of searching: " << ((float)t)/CLOCKS_PER_SEC << " second" << endl; //print time
		}

		cout << endl;
	}

    return 0;
}

Lexicon addWordsInLexiconFromFile(){ //this function gets all words from file and save in Lexicon data structure 
	Lexicon lex;
	lex.addWordsFromFile("EnglishWords.dat");

	return lex;
};

void addWordsInLexiconFromConsole(Lexicon &lex, string inputStr, string finalStr){
	//this function can add starting or final word in lexicon
	//this function add inputed words in Lexicon, words which aren't in Lexicon currently
	//create boolian variable for if else
	bool word1 = lex.contains(inputStr);
	bool word2 = lex.contains(finalStr);
	
	if(!word1 && !word2){
		cout << "Both words could not be found, do you want the program to add this words to the dictionary?" << endl;
		if(getYesOrNo("Please enter 'yes' or 'no' with lowercase: ")){
			if(inputStr != finalStr){
				//add words in Lexicon
				lex.add(inputStr);
				lex.add(finalStr);	
			}else{
				lex.add(inputStr); //add word in Lexicon
			}
			cout << "Words added in dictionary, ";
		}
	}else if(!word1){
		cout << inputStr << " - could not be found, do you want the program to add this word to the dictionary?" << endl;
		if(getYesOrNo("Please enter 'yes' or 'no' with lowercase: ")){
			lex.add(inputStr); //add word in Lexicon
			cout << "Word added in dictionary, ";
		}
	}else{
		cout << finalStr << " - could not be found, do you want the program to add this word to the dictionary?" << endl;
		if(getYesOrNo("Please enter 'yes' or 'no' with lowercase: ")){
			lex.add(finalStr); //add word in Lexicon
			cout << "Words added in dictionary, ";
		}
	}
	cout << "Please enter words again" << endl;
};

bool getYesOrNo(string str){
	string input = "yes";
	
	while(true){
		cout << str;
		input = getLine();
		if(input == "yes"){
			return true;
		}else if(input == "no"){
			return false;
		}
		cout << "Please enter correct command" << endl;
	}
};

Stack<string> wordLadder(Lexicon &lex, string inputStr, string finalStr, Stats &stats){ //main function
	Queue<Stack<string> > que; //Declaration queue for saving main stack
	Stack<string> ladder; ////Declaration main stack for trail
	
	//Declaration variables and objects, which we need in while loop 
	Lexicon savedSearchedWords; //this lexicon is for saving words which already searched in english lexicon
	Stack<string> copyLadder;
	Lexicon searchedWords; //for words which already searched in english lexicon, this words save temporary in this object but words save in savedSearchedWords
	string currentStr; //this variable is for peeked word from stack
	
	ladder.push(inputStr); //add inputed string in main stack
	que.enqueue(ladder);	// add main stack in queue
	savedSearchedWords.add(inputStr); //saved all searched words because Do not cycle while loop
	
	//for stats
	stats.length = 0;
	stats.fullLength = 0;

	while(!que.isEmpty()){
		ladder = que.dequeue(); //get first stack from queue 
		stats.length++;
		currentStr = ladder.peek(); //get first word from stack without remove

		if(currentStr == finalStr){
			return ladder;
		}
		
		searchedWords = findAllWordsWithDifferentOneLetter(lex, currentStr);
		foreach(string str in searchedWords){
			if(!savedSearchedWords.contains(str)){
				copyLadder = ladder; //copy main stack for adding in queue
				copyLadder.push(str); //add changed word in coped stack
				que.enqueue(copyLadder); //add coped stack in queue
				stats.fullLength++;
				savedSearchedWords.add(str); //add changed word in data structure in which are word whic is already searched
			}
		}

	}
	
	//This will happen when the corresponding ladder cannot be found and 
	//you must return to the ladder in which only the original input word will be inside
	ladder.clear();
	ladder.push(inputStr);
	stats.length = 0;
	stats.fullLength = 0;
	return ladder;
};

Lexicon findAllWordsWithDifferentOneLetter(Lexicon &lex, string str){
	Lexicon result; //Declaration Lexicon for changed word, which will return end of function 
	string changedStr = str;

	for(int i = 0; i < str.length(); i++){
		for(char ch = 'a'; ch <= 'z'; ch++){
			changedStr[i] = ch;
			if(lex.contains(changedStr)){
				result.add(changedStr); //save word changed word which also is in english word lexicon
			}
			changedStr = str; //update changed Str
		}
	}

	return result;
};

void printWordWithChangedSeparatelyChar(Stack<string> &ladder){//this function find which char is changed
	Stack<string> revStack = reverseStack(ladder); //copy inputed stack for reversing stack
	string word;
	
	cout << "Changed char: ";
	
	if(ladder.size() > 1){ //if changed char
		cout << "{";
		for(int i = 0; i < ladder.size() - 1; i++){
			cout << "\"" ;
			word = revStack.pop();
			for(int j = 0; j < word.length(); j++){
				if(word[j] != revStack.peek()[j]){
					cout << "[" << word[j] << "]";
				}else{
					cout << word[j];
				}
			}
			cout << "\", ";
		}
		cout << revStack.pop() << "}" << endl;
	}else{ //if char not changed
		cout << "No change char" << endl;
	}

	cout << "Number of changed char: " << (ladder.size() - 1) << endl;
};

Stack<string> reverseStack(Stack<string> &ladder){
	Stack<string> copyLadder = ladder;
	Stack<string> result;

	for(int i = 0; i < ladder.size(); i++){
		result.push(copyLadder.pop());
	}

	return result;
};