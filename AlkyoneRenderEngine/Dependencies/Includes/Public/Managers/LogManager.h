#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>    

#include <Core/SingletonBase.h>
#include <Utilities/Containers/Array.h>	

#include <Vector3.h>
#include <ImGUI/imgui.h>

#define FORMAT(msg, ...) GLogManager::getInstance().ArgParser(msg, ##__VA_ARGS__);
#define WRITE(sev) GLogManager::getInstance().WriteLog(sev)
#define LOG(sev,msg,...) FORMAT(msg, ##__VA_ARGS__) WRITE(sev)
#define CHECK_GL_ERROR() GLogManager::getInstance().CheckGLError(__FILE__, __LINE__) && (__debugbreak(), 1);

#define CMD(msg,...) LOG(CMD,msg, ##__VA_ARGS__)
#define DEBUG(msg,...) LOG(DEBUG,msg, ##__VA_ARGS__)
#define INFO(msg,...) LOG(INFO,msg, ##__VA_ARGS__)
#define NOTICE(msg,...) LOG(NOTICE,msg, ##__VA_ARGS__)
#define WARNING(msg,...) LOG(WARNING,msg, ##__VA_ARGS__)
#define ERROR(msg,...) LOG(ERROR,msg, ##__VA_ARGS__)
#define FATAL(msg,...) LOG(FATAL,msg, ##__VA_ARGS__)

enum ImportanceEnum
{
	CMD, DEBUG, INFO, NOTICE, WARNING, ERROR, FATAL
};


struct GuiLog
{
	char                  InputBuf[256];
	TArray<std::string>   Items;
//	bool                  AutoScroll;

	static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
	static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
	static char* Strdup(const char* s) { size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
	static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }


	GuiLog()
	{
		
		Clear();
		memset(InputBuf, 0, sizeof(InputBuf));
		//AutoScroll = true;
	}

	~GuiLog()
	{
		//TODO: crasharei gia kapoio logo
	//	Clear();

	}

	void Clear()
	{
		Items.Destroy();
	}

	void AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		Items.PushBack(Strdup(buf));
	}



};


class GLogManager : SingletonManagerBase<GLogManager>
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
	bool CheckGLError(const char* file, int line);

	//templates must be defined inside the header
	template<typename T, typename ...Args>
	const char * ArgParser(const char * msg, T&& value, Args... args) 
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

			//std::string test;

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

		std::string To_String(const Vector3f& value)
		{
			return "X: " + std::to_string(value.X) + "\tY:" + std::to_string(value.Y) + "\tZ:" + std::to_string(value.Z);
		}


		//cat cpy etc.

	}Helper;

	std::ofstream ERRORFILE;
	std::ofstream LOGFILE;

	uint8 bErrFileIsOpen : 1;
	uint8 bLogFileIsOpen : 1;


	const std::string SeverityString[7] = { "CMD","DEBUG","INFO", "NOTICE", "WARNING","ERROR","FATAL" };
	

public:
		GuiLog EditorLog;
	
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
// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
