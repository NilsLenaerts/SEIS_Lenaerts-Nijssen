#pragma once
#include <vector>
#include "Token.h"
#include "ByteStream.h"

#include <iostream>
#include <vector>
#include <string>
class Lexer {

private:
	std::vector<Token> tokens{};
	Bytestream *bytestream;// We can reuse the bytestream for the WAT file
	void lexBytestream();
	void printTokens();
	Token extractNumber();
	Token extractIdentifier();
	Token extractString();
	void  removeComment(bool isMultiline);
public:
	std::vector<Token> getTokens() { return tokens; }
	Lexer(std::string path);

};