#include <Managers/LogManager.h>
#include <GL/glew.h>


template<> GLogManager* SingletonManagerBase<GLogManager>::instance = 0;
GLogManager & GLogManager::getInstance()
{
	//assert?
	return (*instance);
}
GLogManager * GLogManager::getInstancePtr()
{
	if (instance == nullptr)
	{
		instance = new GLogManager();
	}
	return instance;
}

GLogManager::GLogManager()
{
	bInitSuccessful = false;
	bErrFileIsOpen = 0;
	bLogFileIsOpen = 0;
}

GLogManager::~GLogManager()
{}


void GLogManager::WriteLog(ImportanceEnum lvl)

{
	auto CurrentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	//Print to screen.
	if(lvl != CMD)
	{
		std::cout << "[" << std::strtok(std::ctime(&CurrentTime), "\n") << "] ::[" << SeverityString[lvl] << "]:: " << Helper.LogMessage << "\n";

	}
	
	if (lvl == INFO || lvl == WARNING) {

		LOGFILE << "[" << std::strtok(std::ctime(&CurrentTime), "\n") << "] ::[" << SeverityString[lvl] << "]:: " << Helper.LogMessage << "\n";

	}
	else if (lvl == ERROR || lvl == FATAL)
	{
		ERRORFILE << "[" << std::strtok(std::ctime(&CurrentTime), "\n") << "] ::[" << SeverityString[lvl] << "]:: " << Helper.LogMessage << "\n";
	}

#if WITH_EDITOR
		EditorLog.AddLog("[%s] ::[%s]:: %s\n", std::strtok(std::ctime(&CurrentTime), "\n"), SeverityString[lvl].c_str(), Helper.LogMessage.c_str());
#endif

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

bool GLogManager::CheckGLError(const char* file, int line)
{

	bool wasError = false;

	for (GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError())
	{
		wasError = true;
		const GLubyte* sError = glewGetErrorString(glErr);

		if (!sError)
		{
			sError = reinterpret_cast<const GLubyte*>(" (no message available)");
		}
		LOG(ERROR, "GL Error #%d (%s) in File %s at line: %d\n", glErr, sError, file, line);

	}
	return wasError;
}


void GLogManager::Init()
{
	std::ios_base::sync_with_stdio(false);
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
		bInitSuccessful = true;
	}
	if (bInitSuccessful)
	{
#if WITH_EDITOR
		EditorLog = GuiLog();
#endif
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