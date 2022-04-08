#include "Token.h"
#include "Token.h"

std::string Token::getStringValue()
{
    if (auto result = std::get_if<std::string>(&value)) {
        return std::get<std::string>(value);
    }
    return nullptr;

}

uint32_t Token::getI32Value()
{
    if (auto result = std::get_if<uint32_t>(&value)) {
        return std::get<uint32_t>(value);
    }
    return nullptr;
}

TypeOfToken Token::getTokenType()
{
    return type;
}
