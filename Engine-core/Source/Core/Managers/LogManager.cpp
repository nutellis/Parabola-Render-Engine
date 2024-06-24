#include <Core\LogManager.h>


template<> GLogManager* SingletonBase<GLogManager>::instance = 0;
GLogManager & GLogManager::getInstance()
{
	//assert?
	return (*instance);
}
GLogManager * GLogManager::getInstancePtr()
{
	return instance;
}

GLogManager::GLogManager()
{
	bInitSuccessful = 0;
	bErrFileIsOpen = 0;
	bLogFileIsOpen = 0;
}

GLogManager::~GLogManager()
{}


void GLogManager::WriteLog(ImportanceEnum lvl)

{
	auto CurrentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	//Print to screen.
	std::cout << "[" << std::strtok(std::ctime(&CurrentTime), "\n") << "] ::[" << SeverityString[lvl] << "]:: " << Helper.LogMessage << "\n";

	if (lvl == INFO || lvl == WARNING) {

		LOGFILE << "[" << std::strtok(std::ctime(&CurrentTime), "\n") << "] ::[" << SeverityString[lvl] << "]:: " << Helper.LogMessage << "\n";

	}
	else if (lvl == ERROR || lvl == FATAL)
	{
		ERRORFILE << "[" << std::strtok(std::ctime(&CurrentTime), "\n") << "] ::[" << SeverityString[lvl] << "]:: " << Helper.LogMessage << "\n";
	}

	Helper.flush();
}

const char * GLogManager::ArgParser(const char * msg)
{
	while (msg && *msg)
	{
		if (*msg == '%' && *++msg != '%')
		{
			return "!!ERROR!! Missing arguments!\n";
		}
		Helper.LogMessage += *msg++;
	}

	return Helper.LogMessage.c_str();
}


void GLogManager::Init()
{

	Helper.LogMessage = "Initiating LOG_MANAGER\n";
	WriteLog(INFO);

	LOGFILE.open("log.logfile", std::ios::out);
	if (!LOGFILE)
	{
		Helper.LogMessage = "Cannot open file\n";
		WriteLog(CMD);
	}
	else
	{
		bInitSuccessful = 1;
	}
	if (bInitSuccessful == INIT_OK)
	{

		Helper.LogMessage = "Log is open for bussiness\n";
		WriteLog(INFO);
	}
	else
	{
		Helper.LogMessage = "It's fine, really. Log is not working though so... something will probably fail. \
 Who am I to stop you, I'm just a warning :) ";
		WriteLog(WARNING);

		exit(EXIT_FAILURE);

	}
}

void GLogManager::Terminate()
{
	LOGFILE.close();
	ERRORFILE.close();

	LOG(DEBUG, "LOG_MANAGER Terminated");
}