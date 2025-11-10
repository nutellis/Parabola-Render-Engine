#pragma once

#ifndef JSON_PARSER
#define JSON_PARSER

#include <string>
#include <fstream>


enum class JsonTokensEnum {
	None = 0,
	ObjectBegin = '{',
	ObjectEnd = '}',
	ArrayBegin = '[',
	ArrayEnd = ']',
	String = '\"',
	Number = '0',
	True = 't',
	False = 'f',
	Null = 'n',
	Colon = ':',
	Comma = ','
};

struct JsonToken {
	JsonTokensEnum Type = JsonTokensEnum::None;
	std::string Value = "";

	std::string ToString();
};


class Tokenizer {
public:
	Tokenizer();
	Tokenizer(std::string InputFile);
	~Tokenizer();

	JsonToken GetNextToken();


private:
	std::fstream InputStream;
	size_t prevPos = 0;
};


class JsonParser {


};


#endif // !JSON_PARSER

