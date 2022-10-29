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
#include "filelib.h"
#include "random.h"
using namespace std;

//This variable determines how many characters should be generated
//If we want to generate another quantity without changing the code, changing this variable will be easy
const int generateSymbolsNum = 2000; 

//Prototypes of functions
string enterFileName();
int enterMarkovModNum();
void getSymbolsFromFile(string fileName, Vector<char> &v);
string separateAndSaveEachKCharacter(int markovModNum, Vector<char> &charArr, Map<string, Vector<char> > &map);
void saveSeparatedStrInMap(int &markovModNum, Vector<char> &charArr, Map<string, Vector<char> > &map, string &eachStr, 
						   Vector<char> &nextCharsArr, string &mostCommonStr, int &maxVectorSize, int &i);
void generateText(string mostCommonStr, Map<string, Vector<char> > &map);
char getRandomSymbol(Vector<char> &v);

int main() {
	string fileName = enterFileName(); //entering file name
	int markovModNum = enterMarkovModNum(); //entering markov model number 

	cout << "Processing ..." << endl;

	Vector<char> charArr; //create variable for saving symbols in vector
	getSymbolsFromFile(fileName, charArr); //get symbols from file and save in vector(char array)
	
	Map<string, Vector<char> > map; //declaration of main data structure
	//Separate each k character from symbols array and find most common string
	string mostCommonStr = separateAndSaveEachKCharacter(markovModNum, charArr, map);
	
	//The reading and analysis of the text ends here
	//This is where text generation begins

	//cout << "end" << endl;

	generateText(mostCommonStr, map);
	
    return 0;
}

string enterFileName(){ //With this function, the user can enter the file name until they enter the correct file name
	string fileName;
	while(true){
		fileName = getLine("Enter source text: "); //entering file name

		if(fileExists(fileName)){ //checking file name
			return fileName;
		}else{
			cout << "Unable to open that file. Try again" << endl;;
		}
	}
};

int enterMarkovModNum(){//for entering markov model number
	int markovN;

	while(true){
		markovN = getInteger("Enter the Markov order [1-10]: "); //entering markov model number

		if(markovN > 0 && markovN < 11){ //chekcin markov model number range
			return markovN;
		}else{
			cout << "That value is out of range. Try again" << endl;
		}
	}
};

void getSymbolsFromFile(string fileName, Vector<char> &v){ //this function get symbols from file and save in vector(char array)
	ifstream file(fileName.c_str());
	
	char symbol;
	while(file.get(symbol)){
		v.add(symbol);
	}

	file.close();
};

//Separate each k character from char vector and save as string as key in map, value is frequency also
//this function return The most common string, The string refers to the most common sequence 
string separateAndSaveEachKCharacter(int markovModNum, Vector<char> &charArr, Map<string, Vector<char> > &map){ 
	//I have created this variables outside of the cycle because no time is lost on creation
	string eachStr = ""; //key of map
	Vector<char> nextCharsArr; //value of map 

	string mostCommonStr; //this variable will be string which has most symbols in vector
	int maxVectorSize = 0; //this variable define which string is most common and keeps value of max nextCharArr.size()

	for(int i = 0; i < charArr.size() - markovModNum - 1; i++){//Separate each k character from symbols array
		for(int j = i; j < i + markovModNum; j++){
			eachStr += charArr[j]; 
		}
		
		saveSeparatedStrInMap(markovModNum, charArr, map, eachStr, nextCharsArr, mostCommonStr, maxVectorSize, i);
	}

	return mostCommonStr;
};

//this function saves separated string in map and all variable is trasfered by referens because
// time should not be wasted on copying and creating variables
void saveSeparatedStrInMap(int &markovModNum, Vector<char> &charArr, Map<string, Vector<char> > &map, string &eachStr, 
						   Vector<char> &nextCharsArr, string &mostCommonStr, int &maxVectorSize, int &i){
	
	if(map.containsKey(eachStr)){ //add key and value in map if they already exist
		nextCharsArr = map.get(eachStr); //get existed array of next symbols
		nextCharsArr.add(charArr[i + markovModNum]); //add new symbols in array of next symbols
		map.put(eachStr, nextCharsArr); //add array of next symbols in map
	}else{ //Add keys and values in map if they do not exist	
		nextCharsArr.clear(); //clear because vector may have information about next symbols of other strings
		nextCharsArr.add(charArr[i + markovModNum]); //add new symbols in array of next symbols
		map.put(eachStr, nextCharsArr); //add vector in map 
	}
	
	//checking most common str
	if(maxVectorSize < nextCharsArr.size()){ //update most frequent sequence
		mostCommonStr = eachStr;
		maxVectorSize = nextCharsArr.size();
	}

	eachStr = "";
};

void generateText(string mostCommonStr, Map<string, Vector<char> > &map){
	Vector<char> symbolsArr;
	char randSymbol;
	string currentStr = mostCommonStr;
	string result = mostCommonStr; //this variable is for generated text

	for(int i = 0; i < (generateSymbolsNum - mostCommonStr.size()); i++){
		symbolsArr = map.get(currentStr); //get symbols from array
		randSymbol = getRandomSymbol(symbolsArr); //get random char
		result += randSymbol; //result is variable for whole genereted text
		
		currentStr = currentStr.substr(1, currentStr.length() - 1) + randSymbol; //update current string

		if(!map.containsKey(currentStr)){
			break;
		}
	}

	cout << result; //print generated text in console 
	cout << endl << endl << "End generate" << endl;
};

char getRandomSymbol(Vector<char> &v){//random simulation
	int randomSymbolIndex = randomInteger(0, (v.size() - 1));

	//return random symbol
	return v[randomSymbolIndex];
};