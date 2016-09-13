#ifndef NLOG_H_
#define NLOG_H_

#include <iostream>
#include <sstream>
#include <ctime>

#include "fileIface.h"
#include "colorize.h"

enum logLevel {
	DEBUG,
	INFO,
	WARNING,
	ERROR
};

enum logType {
	_stdout,
	_file
};


class Log
{
public:
	Log() {	}

	~Log() { }


	Log &logNow(logLevel newLevel);

	template<class T>
	void operator<<(const T &msg);

	void setLoggingLevel(logLevel level);
	void setLoggingProfile(std::string profileName);
	void setLoggingType(logType newType);
	void setLoggingFile(std::string filename);
	void setColorLogging(bool isColor);

private:
	std::ostringstream os;
	FileInterface fileIface;

	logLevel msgLevel = INFO;
	logLevel defaultLevel = INFO;
	logType curLogType= _stdout;

	bool revertLogLevel = false;

	bool colorize = true;
	std::string logProfile = "default";

	std::string getProfile();
	std::string timeStamp();
	std::string getLabel(logLevel lvl);

	std::string getColorMessage(logLevel level, std::string msg);
	std::string getColorLabel(logLevel lvl);


};


/* Public functions */
void setColorLogging(bool isColor)
{
	colorize = isColor;
}

void Log::setLoggingLevel(logLevel level)
{
	msgLevel = level;
	defaultLevel = level;
}

Log& Log::logNow(logLevel newLevel)
{
	if(newLevel >= msgLevel)
	{
		msgLevel = newLevel;
		revertLogLevel = true;
	}

	return *this;
	
}

template<class T>
void Log::operator<<(const T &msg)
{
	/*
	 * Main log event -- write to stream
	 */
	if(colorize)
	{
		os << getColorLabel(msgLevel) << " " << msg << std::endl;
	}
	else
		os << getLabel(msgLevel) << " " << msg << std::endl;

	if(curLogType == _stdout)
	{
		if(msgLevel == ERROR || msgLevel == WARNING)
			std::cerr << os.str();
		else
			std::cout << os.str();
	}
	else
	{
		fileIface.WriteToFile(os.str().c_str());
	}

	/*
	 * Change logging level back if needed
	 */
	if(revertLogLevel)
	{
		msgLevel = defaultLevel;
		revertLogLevel = false;
	}

	/*
	 * Clear the stream
	 */
	os.str("");
	os.clear();
}

void Log::setLoggingProfile(std::string profileName)
{
	logProfile = profileName;
}

void Log::setLoggingType(logType newType)
{
	curLogType = newType;

	if(newType == _file)
	{
		if(fileIface.getFileName() != "")
		{
			if(fileIface.opened == false)
			{
				fileIface.OpenStream();
			}
		}
		else
		{
			std::string errorFileName = logProfile + '_' + timeStamp() + ".log";

			std::cerr << "[ERROR] Logger: no file name" << std::endl;
			std::cerr << "[INFO] Logger: using " << errorFileName << " as log file" << std::endl; 

			setLoggingFile(errorFileName);
			fileIface.OpenStream();
		}
	}
}

void Log::setLoggingFile(std::string filename)
{
	fileIface.setFileName(filename);
}

/* Private functions */
std::string Log::getProfile()
{
    if(logProfile != "default")
   	 return logProfile;
    else return " - ";
}

std::string Log::timeStamp()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);

	const char* type1 = "%Y-%m-%d.%X";
	const char* type2 = "%X";

	strftime(buf, sizeof(buf), type1, &tstruct);

	return buf;	
}

std::string Log::getLabel(logLevel lvl)
{
	std::string label;
    switch(lvl)
    {
     case DEBUG:   label =  "[DEBUG]";     break; 
   	 case INFO:    label =  "[INFO]";      break; 
   	 case WARNING: label =  "[WARNING]";   break; 
   	 case ERROR:   label =  "[ERROR]";     break; 
   	 default:      label =  "[UNDEFINED]"; break;
    }

	return "[" + timeStamp() + "] " + "[" + getProfile() + "] " + label;

}

std::string Log::getColorMessage(logLevel level, std::string msg)
{
}
std::string Log::getColorLabel(logLevel lvl)
{
	
	std::string label;
	std::string _time;
	std::string _profile;

    switch(lvl)
    {
     case DEBUG:   label =  applyColor("[DEBUG]", GREEN, DIM);     break; 
   	 case INFO:    label =  applyColor("[INFO]", LCYAN);           break; 
   	 case WARNING: label =  applyColor("[WARNING]", RED);          break; 
   	 case ERROR:   label =  applyColor("[ERROR]", LRED, BOLD);     break; 
   	 default:      label =  applyColor("[UNDEFINED]", BLACK, DIM); break;
    }

	_time = applyColor("[" + timeStamp() + "]", DGRAY, DIM);
	_profile = applyColor("[" + getProfile() + "]", LGRAY);

	return _time + " " + _profile + " " + label;

}
#endif
