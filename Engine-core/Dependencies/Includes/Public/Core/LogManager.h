#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>    

#include <Typedefs.h>

#include <Core/SingletonBase.h>
#include <Core/Managers/ManagerBase.h>


#define FORMAT(msg, ...) GLogManager::getInstance().ArgParser(msg, ##__VA_ARGS__);
#define WRITE(sev) GLogManager::getInstance().WriteLog(sev)
#define LOG(sev,msg,...) FORMAT(msg, ##__VA_ARGS__) WRITE(sev)

enum ImportanceEnum
{
	CMD, DEBUG, INFO, NOTICE, WARNING, ERROR, FATAL
};

class GLogManager : SingletonBase<GLogManager> , ManagerBase
{
public:

	//GLogManager(GLogManager const&) = delete;
	//void operator=(GLogManager const&) = delete;

	GLogManager();

	~GLogManager();



	static GLogManager& getInstance();
	static GLogManager * getInstancePtr();

	//ONLY LOG FOR NOW
	void WriteLog(ImportanceEnum lvl);

	const char * ArgParser(const char * msg);


	//templates must be defined inside the header
	template<typename T, typename ...Args>
	const char * ArgParser(char * msg, T&& value, Args... args) 
	{
		while (msg && *msg)
		{
			if (*msg == '%' && *++msg != '%')
			{
				if (*msg == 'd' || *msg == 'f' || *msg == 'c')
				{
					Helper.LogMessage += Helper.To_String(std::forward<T>(value));
				}
				else if (*msg == 's')
				{
					Helper.LogMessage += Helper.To_String(std::forward<T>(value));
				}
				return ArgParser(++msg, args...);
			}
			Helper.LogMessage += *msg++;
		}
		//Error! Extra arguments provided!
		return "!!ERROR!! Extra arguments provided!\n";
	}

	virtual void Init() override;
	virtual void Terminate() override;

private:
	struct LogHelper
	{
		std::string LogMessage;

		uint8 boolIsEmpty : 1;

		LogHelper() {}

		void flush()
		{
			LogMessage.clear();
		}

		template<typename T>
		std::string To_String(const T& value)
		{
			return std::to_string(value);
		}

		std::string To_String(const char* value)
		{
			return std::string(value);
		}
		std::string To_String(char* value)
		{
			return std::string(value);
		}

		std::string To_String(const uint8* value)
		{
			return std::string(reinterpret_cast<const char*>(value));
		}


		//cat cpy etc.

	}Helper;

	std::ofstream ERRORFILE;
	std::ofstream LOGFILE;

	uint8 bErrFileIsOpen : 1;
	uint8 bLogFileIsOpen : 1;


	const std::string SeverityString[7] = { "CMD","DEBUG","INFO", "NOTICE", "WARNING","ERROR","FATAL" };

	
};






/*
std::string ArgPrint(std::string FinalString, const char* s)
{
	while (s && *s) {
		if (*s == '%' && *++s != '%')
		{// make sure that there wasn't meant to be more arguments
						// %% represents plain % in a format string
			throw std::runtime_error("invalid format: missing arguments");
		}
		FinalString.append(*s++);
	}
}


template<typename T, typename... Args>		// note the "..."
std::string ArgPrint(std::string FinalString, const char* s, T value, Args... args)	// note the "..."
{
	while (s && *s) {
		if (*s == '%' && *++s != '%')
		{	// a format specifier (ignore which one it is)
			FinalString.append(value);		// use first non-format argument
			return ArgPrint(FinalString, ++s, args...); 	// ``peel off'' first argument
		}
		FinalString.append(*s++);
	}
	throw std::runtime error("extra arguments provided to printf");
}

*/
