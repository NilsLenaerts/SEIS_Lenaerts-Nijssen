#pragma once
#include <vector>
#include "Token.h"
#include "ByteStream.h"
class Lexer {

private:
	std::vector<Token> tokens{};
	Bytestream *bytestream;// We can reuse the bytestream for the WAT file
	void lexBytestream();

	Token extractNumber();
	Token extractIdentifier();
	Token extractString();
	void  removeComment(bool isMultiline);
public:
	std::vector<Token> getTokens() { return tokens; }
	Lexer(std::string path);

	
	

};