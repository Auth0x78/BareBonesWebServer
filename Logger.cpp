#include "logger.h"

//Set highdef value of m_level
LogLevel Logger::m_level = LogLevel::Info;

//Constructor
Logger::Logger() {}

//Public Functions 
void Logger::Log(const std::string& message) /*Assume the log level to be none*/
{
	std::cout << GREEN_COLOR << "[] " << RESET_COLOR << message;
}
void Logger::Log(LogLevel level, const std::string& message)
{
	if (level < m_level)
		return;
	switch (level)
	{
	case Info:
		LogInfo(message);
		break;
	case Warning:
		LogWarning(message);
		break;
	case Error:
		LogError(message);
		break;
	case HighDef:
		std::cout << RESET_COLOR << message;
		break;
	highdef:
		break;
	}
}

void Logger::LogError(const std::string& message)
{
	std::cout << RED_COLOR << "[ERROR]: " << RESET_COLOR << message << std::endl;
}
void Logger::LogWarning(const std::string& message)
{
	std::cout << YELLOW_COLOR << "[WARN]: " << RESET_COLOR << message << std::endl;
}
void Logger::LogInfo(const std::string& message)
{
	std::cout << BLUE_COLOR << "[INFO]: " << RESET_COLOR << message << std::endl;
}

void Logger::fmtLog(const char* message, ...) /* Assume the log level to be none */
{
	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);

	printf("\n");
}
void Logger::fmtLog(LogLevel level, const char* const message, ...)
{
	if (level < m_level)
		return;

	va_list args;
	va_start(args, message);
	switch (level)
	{
	case Info:
		std::cout << BLUE_COLOR << "[INFO]: " << RESET_COLOR;
		vprintf(message, args);
		break;
	case Warning:
		std::cout << YELLOW_COLOR << "[WARN]: " << RESET_COLOR;
		vprintf(message, args);
		break;
	case Error:
		std::cout << RED_COLOR << "[ERROR]: " << RESET_COLOR;
		vprintf(message, args);
		break;
	case HighDef:
		vprintf(message, args);
		break;
	}
	va_end(args);
	printf("\n");
}

void Logger::fmtLogError(const char* message, ...)
{
	if (Error < m_level)
		return;

	va_list args;
	va_start(args, message);
	
	std::cout << RED_COLOR << "[ERROR]: " << RESET_COLOR;
	vprintf(message, args);
	
	va_end(args);
	
	printf("\n");
}
void Logger::fmtLogWarning(const char* message, ...)
{
	if (Warning < m_level)
		return;

	va_list args;
	va_start(args, message);
	
	std::cout << YELLOW_COLOR << "[WARN]: " << RESET_COLOR;
	vprintf(message, args);
	
	va_end(args);

	printf("\n");
}
void Logger::fmtLogInfo(const char* message, ...)
{
	if (Info < m_level)
		return;

	va_list args;
	va_start(args, message);

	std::cout << BLUE_COLOR << "[INFO]: " << RESET_COLOR;
	vprintf(message, args);

	va_end(args);

	printf("\n");
}

void Logger::SetLogLevel(LogLevel level)
{
	m_level = level;
}

LogLevel Logger::GetLogLevel()
{
	return m_level;
}