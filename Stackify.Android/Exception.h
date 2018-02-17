#include <string>
#include <map>
// #include <boost/stacktrace.hpp>

namespace DefenseAgainstTheDarkArts
{
	class Exception
	{
	public:
		Priority priority;
		std::string text;
		std::string error_message;
		std::string error_type;
		std::string error_type_code;
		std::string transaction_id;
		std::string thread_name;
		std::string source_method;
		std::string customer_name;
		std::string user_name;
		int  source_line;
		//boost::stacktrace::basic_stacktrace<> stack_trace;
		std::map<std::string, std::string> data_map;
		std::map<std::string, std::string> error_data_map;
	};
}
