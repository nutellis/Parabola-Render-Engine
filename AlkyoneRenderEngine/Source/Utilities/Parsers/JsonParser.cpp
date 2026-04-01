#include <Utilities/Parsers/JsonParser.h>

#include <fstream>

Tokenizer::Tokenizer()
{
}

Tokenizer::Tokenizer(std::string InputFile)
{
}

Tokenizer::~Tokenizer()
{
}

JsonToken Tokenizer::GetNextToken()
{
	return JsonToken();
}
