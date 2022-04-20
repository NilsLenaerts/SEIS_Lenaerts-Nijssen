#include "Token.h"
#include "Token.h"

std::string Token::getStringValue()const
{
    if (auto result = std::get_if<std::string>(&value)) {
        return std::get<std::string>(value);
    }
    return "";

}

uint32_t Token::getI32Value()const
{
    if (auto result = std::get_if<uint32_t>(&value)) {
        return std::get<uint32_t>(value);
    }
    return 0;
}

TypeOfToken Token::getTokenType()const
{
    return type;
}
