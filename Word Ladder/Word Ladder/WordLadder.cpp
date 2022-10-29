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
using namespace std;

//Prototype of function
Lexicon addWordsInLexicon();
Stack<string> wordLadder(Lexicon &lex, string inputStr, string finalStr);
Lexicon findAllWordsWithDifferentOneLetter(Lexicon &lex, string str);

int main() {
	Lexicon lex = addWordsInLexicon();
	Stack<string> ladder;
	string inputStr;
	string finalStr;

	while(true){
		inputStr = getLine("Enter start word: ");
		finalStr = getLine("Enter destination word: ");
	
		//checking inputed word
		if(!lex.contains(inputStr) || !lex.contains(finalStr) || inputStr.length() != finalStr.length()){
			cout << "It is not possible to get the second input word from the first input word" << endl;
		}else{ //If there is a chance to find a ladder
			ladder = wordLadder(lex, inputStr, finalStr); //searching word ladder
		
			//checking found ladder if not found
			if(ladder.size() > 1){
				cout << "Found ladder: " << ladder.toString() << endl;
			}else{
				cout << "No ladder found " << endl;
			}
		}

		cout << endl;
	}

    return 0;
}

Lexicon addWordsInLexicon(){ //this function gets all words from file and save in Lexicon data structure 
	Lexicon lex;
	lex.addWordsFromFile("EnglishWords.dat");
	
	return lex;
};

Stack<string> wordLadder(Lexicon &lex, string inputStr, string finalStr){ //main function
	Queue<Stack<string> > que; //Declaration queue
	Stack<string> ladder; ////Declaration main stack for trail
	
	//Declaration variables and objects, which we need in while loop
	Lexicon savedSearchedWords; //this lexicon is for saving words which already searched in english lexicon
	Stack<string> copyLadder;
	//for words which already searched in english lexicon, this words save temporary in this object but words save in savedSearchedWords
	Lexicon searchedWords; 
	string currentStr; //this variable is for peeked word from stack
	
	ladder.push(inputStr); //add inputed string in main stack
	que.enqueue(ladder);	// add main stack in queue
	savedSearchedWords.add(inputStr); //saved all searched words because Do not cycle while loop

	while(!que.isEmpty()){
		ladder = que.dequeue(); //get first stack from queue 
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
				savedSearchedWords.add(str); //add changed word in data structure in which are word whic is already searched
			}
		}

	}
	
	//This will happen when the corresponding ladder cannot be found and 
	//you must return to the ladder in which only the original input word will be inside
	ladder.clear();
	ladder.push(inputStr);
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