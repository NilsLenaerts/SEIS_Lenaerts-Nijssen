#include "Lexer.h"
#include "Character.h"


Lexer::Lexer(std::string path) {
	bytestream = new Bytestream{ path };
	lexBytestream();
}

void Lexer::lexBytestream()
{
	tokens = std::vector<Token>();	//We start off by making an empty vector which we will fill up.
	
	//well if we are over our bytestream length then we don't need to continue as we just well won't achieve anything with it.
	while (!bytestream->atEnd()) {

		//First we check if what we have is a white space (and thus skipped for our tokens)
		while (Character::isWhiteSpace(bytestream->checkByte())) {
			bytestream->skipByte();	//we skip this byte
		}
		char character = bytestream->checkByte();
		//we use the if else since we already defined functions to do some checks for us (check character class)
		//and well these functions don't really work right off the bat in a switch even though it's a lot of if else
		//we check numbers prior to identifiers since numbers are a part of identifiers and we need numbers seperate aswell
		if (Character::isNumber(character)) {
			tokens.push_back(extractNumber());
		}
		else if (Character::isWATIdentifier(character)) {
			tokens.push_back(extractIdentifier());
		}
		else if (character == '"') {
			tokens.push_back(extractString());
		}
		else if (character == '(') {
			bytestream->skipByte();

			//here we need to know if it is a comment or not.
			if (bytestream->checkByte() == ';') {
				removeComment(true);
			}
			else {
				tokens.push_back(Token(TypeOfToken::Bracket_Open, std::string(1, character)));
			}
		}
		else if (character == ')') {
			tokens.push_back(Token(TypeOfToken::Bracket_Close, std::string(1, character)));
		}
		else if (character == ';') {
			bytestream->skipByte();
			if (bytestream->checkByte() == ';') {
				removeComment(false);
			}
			else {

				tokens.push_back(Token(TypeOfToken::String, std::string(1, character)));
			}
		}

	}

}

Token Lexer::extractNumber()
{
	std::string result{};
	while (Character::isNumber(bytestream->checkByte())) {
		result.append(1, bytestream->readByte());
	}
	return Token(TypeOfToken::Number, result);

}

Token Lexer::extractIdentifier()
{
	std::string result{};
	while (Character::isWATIdentifier(bytestream->checkByte())) {
		result.append(1, bytestream->readByte());
	}
	return Token(TypeOfToken::Identifier, result);
}

Token Lexer::extractString()
{
	std::string result{};
	bytestream->skipByte();	//we skip a byte since we start off with our "
	while (bytestream->checkByte() != '"') {
		result.append(1, bytestream->readByte());
	}
	bytestream->skipByte();	//same at the end of our string
	return Token(TypeOfToken::String, result);
}

/*https://webassembly.github.io/spec/core/text/lexical.html#comments
* Since comments are unneccesary code when it comes to lexing/compiling we just remove it 
* Comments in wat come in two types:
* singleLine comments and multiline comments
* singleLine comments always start on ;; and will end on a new line or '\n'
* 
* Multiline comments start with (; and end on ;)
* so they look a little like this:
  (; bla
  vlafcjisn bla bla ;)

* Well this ;) indicates the end of our multiline comment and thus we should search it in that event
*/
void Lexer::removeComment(bool isMultiline)	
{
	//we start off by skipping a byte since it will always be a ; which is part of the comment operation
	bytestream->skipByte();
	if (isMultiline) {
	
		while (true) {
			// if we got ; we need to check if the next char is a ) in all other cases we can just skip the ; as it's part of the comment
			// and we just keep looping as long as there is no ;) found
			if (bytestream->checkByte() == ';') {
				bytestream->skipByte();
				if (bytestream->checkByte() == ')') {
					return;
				}
			}



			bytestream->skipByte();
		}



	}
	else {
		//single line comment --> just break on \n
	
		while (bytestream->checkByte() != '\n') {
			bytestream->skipByte();
		}
	
	}

}
