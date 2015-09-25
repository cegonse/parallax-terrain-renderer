#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <iostream>

class Debug
{
    public:
        static void Log(const std::string & message, const std::string & file, const int line)
        {
            std::cout << "[" << file << ":" << line << "] " << message << std::endl;
        }

		static void Log(const std::string & message)
		{
			std::cout << message << std::endl;
		}
};

#endif // DEBUG_H
