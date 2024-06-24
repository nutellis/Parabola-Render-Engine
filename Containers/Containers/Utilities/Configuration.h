#pragma once

#include <fstream>

#include <string>

#include <iostream>

#include <Containers/Array.h>


class Value {
public:
	
	Value() : Key(""), Content("") {}

	Value(const Value & CopyValue) : Key(CopyValue.Key), Content(CopyValue.Content) {}

	Value(Value && MoveValue) : Key(MoveValue.Key), Content(MoveValue.Content) {}

	Value(std::string Key, std::string Content) : Key(Key), Content(Content) {}

	~Value() {
		std::cout << "deleting Value\n";
	}

	Value & operator=(const Value & CopyValue)
	{
		if (this != &CopyValue) {

			Key = CopyValue.Key;
			Content = CopyValue.Content;
		}
		return *this;
	}

	Value & operator=(Value && MoveValue)
	{
		if (this != &MoveValue) {

			this->Key = MoveValue.Key;
			this->Content = MoveValue.Content;
		}
		return *this;
	}


public:
	std::string Key;
	std::string Content;
};

class Section {
public:

	std::string SectionName;

	Section() : SectionName() , Values(){

	}

	Section(const Section & CopySection) : SectionName(CopySection.SectionName), Values(CopySection.Values) {
		
		using std::copy;

		copy(&CopySection, &CopySection + sizeof(CopySection), this);
	}

	Section(Section && MoveSection) : SectionName(MoveSection.SectionName), Values(MoveSection.Values) {
	}

	Section(std::string Name) :
		SectionName(Name.substr(1, Name.length() - 2)), Values() {
	}

	~Section() {

		std::cout << "deleting Section " << SectionName <<"\n";
	}

	void AddValue(std::string inValue) {
		size_t DelimeterPos = inValue.find("=");

		std::string Key = inValue.substr(0, DelimeterPos);

		std::string Content = inValue.substr(DelimeterPos + 1, inValue.length());

		Values.PushBack(Value(Key, Content));
	 std::cout << Values.Back().Key << "\n";

	}

	Section & operator=(const Section & Copy)	{
		SectionName = Copy.SectionName;
		Values = Copy.Values;

		return *this;
	}

	Section & operator=(Section && Move)	{

		SectionName = Move.SectionName;
		Values = Move.Values;

		return *this;
	}

public:
	 TArray <Value> Values;
	// std::vector<Value> Values;
};

class Configuration {
public:
	Configuration();
	Configuration(const Configuration & CopyConfiguration) : Sections(CopyConfiguration.Sections){

	}
	Configuration(Configuration && MoveConfiguration): Sections(MoveConfiguration.Sections) {

	}
	~Configuration();

	void AddSection(Section newSection) {
	//	Sections.push_back(newSection);
		Sections.PushBack(newSection);
	}
	void Edit(const char * filename);

	void Load(const char * filename);
	void Save(const char * filename);

	int32 GetInt(const char * Key,
		const char * Section);

	float GetFloat(const char * Key,
		const char * Section);

	const char * GetString(const char * Key,
		const char * Section);

	void GetSection(std::string SectionName, Section & ToSection) {
		//for (int32 i = 0; i < Sections.size(); i++) {
		//	if (Sections.at(i).SectionName == SectionName) {
		//		ToSection = Section(Sections.at(i));
		//	}
		//}

		for (int32 i = 0; i < Sections.Size(); i++) {
			if (Sections.At(i).SectionName == SectionName) {
				ToSection = Section(Sections.At(i));
			}
		}
		ToSection = Section();
	}

	Configuration & operator=(const Configuration & Copy) {
		if (this == &Copy) {
			return *this;
		}
	//	Sections = std::vector<Section>(); 
		Sections = TArray<Section>();
		Sections = Copy.Sections;
	}

	Configuration & operator=(Configuration && Move) {
		if (this == &Move) {
			return *this;
		}
		 TArray<Section> Temp = Sections;
		// std::vector<Section> Temp = Sections;

		Sections = Move.Sections;

		Move.Sections = Temp;
	}

public:

	TArray <Section> Sections;
//	std::vector<Section> Sections;
};

Configuration::Configuration() : Sections(){
}

inline Configuration::~Configuration() {
	 Sections.Clear();
	// Sections.clear();
}

void Configuration::Edit(const char * filename) {

}

void Configuration::Load(const char * filename) {
	std::ifstream INIFILE;

	INIFILE.open(filename, std::ios::in);

	std::string ParsingLine;

	if (INIFILE) {
		std::string SectionName;
		//parse sections
		while (std::getline(INIFILE, ParsingLine)) {
			// Comment
			switch (ParsingLine[0]) {
			case '#':
				break;
			case '[':
				 Sections.PushBack(ParsingLine);
			//	Sections.push_back(ParsingLine);
			case '\0':
				break;
			case '\n':
				break;
			default:
			 Sections.Back().AddValue(ParsingLine);
			//	Sections.back().AddValue(ParsingLine);
			}			
		}

	}
}


void Configuration::Save(const char * filename) {}
