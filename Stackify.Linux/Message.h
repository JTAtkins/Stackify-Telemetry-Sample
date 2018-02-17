#pragma once
#include <string>
#include <map>
#include "Priority.h"


namespace DefenseAgainstTheDarkArts
{
	class Message
	{
	public:
		std::string text;
		Priority priority;
		std::string transaction_id;
		std::string thread_name;
		std::string source_method;
		int  source_line;
		std::map<std::string, std::string> data_map;
	};
}

