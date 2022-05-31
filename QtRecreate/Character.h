#pragma once

//this class is used to see what kind of char we are dealing with.
//and that's it we don't have any data members in here so yea
//we alse are not going to add a constructor since this litterally is for some little checks so we make all methods static


//the methods in this class are based on desciptions of https://webassembly.github.io/spec/core/text/lexical.html#

class Character {
public:
	/*
	* following characters are considered white spaces
	* horizontal tabulation, line feed, carriage return and off course the literal space character.
	* These are also known as "\t", "\n", "\r" and " " respectively
	* and well these 4 characters should return true all others should return false
	*/
	static bool isWhiteSpace(char c) {
		return (c == '\t' || c =='\n' || c=='\r' || c== ' ');
	}
	/*
	* In the next function we are looking to what is possibly considered for identifiers in WAT 
	* in this format we allow any symbol found at the bottom table of the page: https://webassembly.github.io/spec/core/text/values.html
	* Since numbers and characters might be usefull for other functions we will devide them to have their own seperate functions aswell
	*/
	static bool isWATIdentifier(char c) {
		return 
			(  Character::isAlphabeticalChar(c)
			|| Character::isNumber(c)
			//Here we have the other identifiers. some are in order that's why we sometimes use ranges and sometimes single chars
			|| c == '!' 
			|| c == '|'
			|| c == '~'
			|| c == '*'
			|| c == '+'
			|| c == ':'
			|| c == '\\'	/*is just \ */ 
			|| (c >= '^' && c <= '`')
			|| (c >= '#' && c <= '&')
			|| (c >= '-' && c <= '/')
			|| (c >= '<' && c <= '@')
			);
	}

	// Since we use ascii we know a-> z are all in order we can assume that every value between the ASCII correspond for a and z will be a character that is also a part
	// of the alphabet and thus also a option that should return true.
	//same goes for capital letters. for those not familiar with ASCii here is a table https://en.wikipedia.org/wiki/ASCII#/media/File:USASCII_code_chart.png
	//according to that table we can see that capital and normal letters don't follow up on eachother but they have their own kind of seperate groups.
	//so we need to divide those up a little hence the "or" statement
	static bool isAlphabeticalChar(char c) {
		return((c >= 'a' && c <= 'z') ||(c >= 'A' && c <= 'Z'));
	}
	//Same train of thought as the chars
	static bool isNumber(char c) {
		return (c >= '0' && c <= '9');
	}
	

};