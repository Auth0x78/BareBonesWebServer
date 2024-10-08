#pragma once
#include <iostream>
#include <string>
#include <cstdarg>

// Define color escape sequences
#define RESET_COLOR		"\033[0m"
#define RED_COLOR		"\033[31m"
#define GREEN_COLOR		"\033[32m"
#define YELLOW_COLOR	"\033[33m"
#define BLUE_COLOR		"\033[34m"


enum LogLevel
{
	Info = 0, Warning = 1, Error = 2, HighDef = 3
};

class Logger
{
public:
	static	void		Log(const std::string& message); /* Assume the message to level: HighDef */
	static	void		Log(LogLevel level, const std::string& message);
	
	static	void		LogInfo(const std::string& message);
	static	void		LogWarning(const std::string& message);
	static	void		LogError(const std::string& message);
	
	static	void		fmtLog(const char* message, ...);
	static	void		fmtLog(LogLevel level, const char* message, ...);
	
	static	void		fmtLogError(const char* message, ...);
	static	void		fmtLogWarning(const char* message, ...);
	static	void		fmtLogInfo(const char* message, ...);
	static	void		SetLogLevel(LogLevel level);
	static	LogLevel	GetLogLevel();

private:
	Logger();
private:
	static LogLevel m_level;
};