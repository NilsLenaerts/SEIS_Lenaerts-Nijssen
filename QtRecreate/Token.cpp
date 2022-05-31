#include "Token.h"


std::string Token::getStringValue()const
{
   
  return stringVal;
}

uint32_t Token::getI32Value()const
{
	return intVal;
}

TypeOfToken Token::getTokenType()const
{
    return type;
}
