#pragma once
#include <string>
#include <variant>
enum class TypeOfToken {
	Identifier,
	Bracket_Open,
	Bracket_Close,
	String,
	Number
};


class Token {
private:
	//https://docs.microsoft.com/en-us/cpp/cpp/unions?view=msvc-170
	

	TypeOfToken type;

	//https://en.cppreference.com/w/cpp/utility/variant	--> this needs C++ 17 or higher and it will allow us to use the same variable for multiple datatypes.
	//https://www.youtube.com/watch?v=qCc_Vqg3hJk here is another video on std::variant
	
	//Changed back to int val and string val since emscripten doesn't allow it
	uint32_t intVal{};
	std::string stringVal{};

public:
	Token(TypeOfToken type, std::string value) : type{ type }, stringVal{ value } {}
	Token(TypeOfToken type, uint32_t value) : type{ type }, intVal{ value } {}
	std::string getStringValue()const;
	uint32_t getI32Value()const;
	TypeOfToken getTokenType()const;
};