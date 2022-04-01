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
	std::variant<
		uint32_t,
		std::string> value;


public:
	Token(TypeOfToken type, std::string value) : type{ type }, value{ value } {}
	Token(TypeOfToken type, uint32_t value) : type{ type }, value{ value } {}




};