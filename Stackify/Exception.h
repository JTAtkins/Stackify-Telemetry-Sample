#pragma once
#include <string>
#include <map>


namespace DefenseAgainstTheDarkArts
{
	class Exception
	{
	public:
		Priority priority;
		std::wstring text;
		std::wstring error_message;
		std::wstring error_type;
		std::wstring error_type_code;
		std::wstring transaction_id;
		std::wstring thread_name;
		std::wstring source_method;
		std::wstring customer_name;
		std::wstring user_name;
		int  source_line;
		//boost::stacktrace::basic_stacktrace<> stack_trace;
		std::map<std::wstring, std::wstring> data_map;
		std::map<std::wstring, std::wstring> error_data_map;
	};
}

/*

Msgs[*]/Ex (Object)
Optional. Exception details.

Msgs[*]/Ex/EnvironmentDetail (Object)
Device, application and environment details.

Msgs[*]/Ex/EnvironmentDetail/DeviceName (String)
The name of the device.
Example: "PROD-RS-Debian-7"

Msgs[*]/Ex/EnvironmentDetail/AppName (String)
The name of the application.
Example: "stackify-agent"

Msgs[*]/Ex/EnvironmentDetail/AppLocation (String)
Optional. The full directory path for the application.
Example: "/usr/local/stackify/stackify-agent"

Msgs[*]/Ex/EnvironmentDetail/ConfiguredAppName (String)
Optional. The name of the application. This overrides the AppName field.
Example: "my-stackify-agent"

Msgs[*]/Ex/EnvironmentDetail/ConfiguredEnvironmentName (String)
Optional. The environment name. If the device is monitored by Stackify, we will use the environment associated to the device. If the device is not monitored by Stackify, this should be specified.
Example: "Prod"

Msgs[*]/Ex/OccurredEpochMillis (Long)
Unix/POSIX/Epoch time with millisecond precision.
Example: 1417535434194

Msgs[*]/Ex/Error (Object)
Exception details.

Msgs[*]/Ex/Error/Message (String)
Optional. The exception message.
Example: "java.lang.NullPointerException: Something important was null that can't be null"

Msgs[*]/Ex/Error/ErrorType (String)
The exception type.
Example: "java.lang.RuntimeException"

Msgs[*]/Ex/Error/ErrorTypeCode (String)
Optional. The exception type code.
Example: 40143

Msgs[*]/Ex/Error/Data (Object)
Optional. Additional data from the loggers diagnostic context.
Example: {"Key1": "Value1", "Key2":"Value2"}

Msgs[*]/Ex/Error/SourceMethod (String)
Optional. Fully qualified method from the first stack trace frame.
Example: "com.stackify.error.test.SomeSweetClass.doSomething"

Msgs[*]/Ex/Error/StackTrace (List(Object))
Lists of stack trace frames.

Msgs[*]/Ex/Error/StackTrace[*]/Method (String)
Fully qualified method name.
Example: "com.stackify.error.test.SomeSweetClass.doSomething"

Msgs[*]/Ex/Error/StackTrace[*]/CodeFileName (String)
Optional. File name.
Example: "SomeSweetClass.java"

Msgs[*]/Ex/Error/StackTrace[*]/LineNum (String)
Optional. Line number.
Example: 16

Msgs[*]/Ex/Error/InnerError (Object)
Optional. Same as Msgs[*]/Ex/Error object.

Msgs[*]/Ex/Error/WebRequestDetail (Object)
Optional. Web request details.

Msgs[*]/Ex/Error/WebRequestDetail/UserIPAddress (String)
Optional. IP address of the user.
Example: "127.0.0.1"

Msgs[*]/Ex/Error/WebRequestDetail/HttpMethod (String)
Optional. HTTP method (GET, POST, PUT, DELETE, etc.).
Example: "GET"

Msgs[*]/Ex/Error/WebRequestDetail/RequestProtocol (String)
Optional. HTTP request protocol.
Example: "HTTP/1.1"

Msgs[*]/Ex/Error/WebRequestDetail/RequestUrl (String)
Optional. HTTP request URL.
Example: "https://api.stackify.com/Metrics/IdentifyApp"

Msgs[*]/Ex/Error/WebRequestDetail/RequestUrlRoot (String)
Optional. Request URI.
Example: "/Metrics/IdentifyApp"

Msgs[*]/Ex/Error/WebRequestDetail/ReferralUrl (String)
Optional. Value of the "referer" header.
Example: "https://api.stackify.com/Metrics/IdentifyApp"

Msgs[*]/Ex/Error/WebRequestDetail/Headers (Object)
Optional. HTTP header name/value pairs.
Example: {"Content-Type": "application/json", "Content-Encoding":"gzip"}

Msgs[*]/Ex/Error/WebRequestDetail/Cookies (Object)
Optional. HTTP cookie name/value pairs. We mask all cookie values with "X-MASKED-X".
Example: {"name": "X-MASKED-X"}

Msgs[*]/Ex/Error/WebRequestDetail/QueryString (Object)
Optional. HTTP query string name/value pairs.
Example: {"name": "value"}

Msgs[*]/Ex/Error/WebRequestDetail/PostData (Object)
Optional. HTTP post data name/value pairs.
Example: {"name": "value"}

Msgs[*]/Ex/Error/WebRequestDetail/SessionData (Object)
Optional. HTTP session name/value pairs. We mask all session values with "X-MASKED-X".
Example: {"name": "X-MASKED-X"}

Msgs[*]/Ex/Error/WebRequestDetail/PostDataRaw (String)
Optional. HTTP raw post data.
Example: "name=value"

Msgs[*]/Ex/Error/WebRequestDetail/MVCArea (String)
Optional. MVC area.
Example: "API"

Msgs[*]/Ex/Error/WebRequestDetail/MVCController (String)
Optional. MVC controller.
Example: "Metrics"

Msgs[*]/Ex/Error/WebRequestDetail/MVCAction (String)
Optional. MVC action.
Example: "IdentifyApp"

Msgs[*]/Ex/Error/ServerVariables (Object)
Optional. Server data name/value pairs.
Example: {"java.runtime.name": "OpenJDK Runtime Environment", "os.name": "Linux", "user.timezone": "UTC", "java.vendor": "Oracle Corporation"}

Msgs[*]/Ex/Error/CustomerName (String)
Optional. Customer/client name.
Example: "Stackify"

Msgs[*]/Ex/Error/UserName (String)
Optional. User name.
Example: "test-user@stackify.com
 */