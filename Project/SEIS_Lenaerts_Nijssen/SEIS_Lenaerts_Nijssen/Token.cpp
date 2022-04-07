#include "Token.h"

std::string Token::getStringValue()
{
    if (auto result = std::get_if<std::string>(&value)) {
        return std::get<std::string>(value);
    }
    return nullptr;

}

TypeOfToken Token::getTokenType()
{
    return type;
}
