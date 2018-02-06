#pragma once
#include <string>
#include <map>
#include "Priority.h"


namespace DefenseAgainstTheDarkArts
{
	class Message
	{
	public:
		std::wstring text;			
		Priority priority;
		std::wstring transaction_id;
		std::wstring thread_name;
		std::wstring source_method;
		int  source_line;
		std::map<std::wstring,std::wstring> data_map;
	};
}
