#pragma once
#include <string>
#include <map>
#include "Priority.h"

namespace DefenseAgainstTheDarkArts
{
	class Message
	{
	public:
		Message();
		~Message();

		std::string source;
		std::string text;
		Priority    priority;
		time_t		time;
		int         tid;
		std::string thread;
		long        pid;
		int         line;
		std::map<std::string, std::string>*  property_map;
	};
}

