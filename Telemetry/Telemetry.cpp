#include "stdafx.h"
#include "Telemetry.h"

#include "Poco/Mutex.h"
#include "Poco/Timestamp.h"
#include "Poco/URI.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/KeyConsoleHandler.h"

#include <sstream>

namespace DefenseAgainstTheDarkArts {

	const std::string StackifyChannel::PROP_APIKEY = "apiKey";
	const std::string StackifyChannel::PROP_ENVIRONMENT = "environment";
	const std::string StackifyChannel::PROP_SERVERNAME = "serverName";
	const std::string StackifyChannel::PROP_APPNAME = "appName";
	const std::string StackifyChannel::PROP_APPLOC = "appLocation";
	const std::string StackifyChannel::PROP_LOGGER = "logger";
	const std::string StackifyChannel::PROP_PLATFORM = "platform";

	StackifyChannel::StackifyChannel() : str_(std::clog)
	{
		stackify_logging_save_log_url.append("https://api.stackify.com/log/save");

		logger_ = "stackify-log-pococpp1.0.0.0";
		platform_ = "CPP";

		Poco::Net::initializeSSL();
		p_console_handler_ = new Poco::Net::KeyConsoleHandler(false); //This is kind of silly, but required for proper initialization (otherwise warning - prompts for password when using client side ssl).
		p_certificate_handler_ = new Poco::Net::AcceptCertificateHandler(true);
		p_context_ = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"); //Todo: Review (Verify NONE is not doing any checking.  Ok for now) need cert details

		Poco::Net::SSLManager::instance().initializeClient(p_console_handler_, p_certificate_handler_, p_context_);
	}

	StackifyChannel::StackifyChannel(std::ostream& str) : str_(str)
	{
		stackify_logging_save_log_url.append("https://api.stackify.com/log/save");

		logger_ = "stackify-log-pococpp1.0.0.0";
		platform_ = "CPP";

		Poco::Net::initializeSSL();
		p_console_handler_ = new Poco::Net::KeyConsoleHandler(false); //This is kind of silly, but required for proper initialization (otherwise warning - prompts for password when using client side ssl).
		p_certificate_handler_ = new Poco::Net::AcceptCertificateHandler(true);
		p_context_ = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"); //Todo: Review (Verify NONE is not doing any checking.  Ok for now) need cert details

		Poco::Net::SSLManager::instance().initializeClient(p_console_handler_, p_certificate_handler_, p_context_);
	}

	StackifyChannel::~StackifyChannel()
	{
		Poco::Net::uninitializeSSL();
	}

	void StackifyChannel::log(const Poco::Message& msg)
	{
		Poco::FastMutex::ScopedLock lock(mutex_);

		try
		{
			const auto priority = msg.getPriority();
			if (priority == Poco::Message::PRIO_FATAL)
			{
				auto result = PostMessage(FormatException(msg));
			}
			else if (priority == Poco::Message::PRIO_CRITICAL)
			{
				auto result = PostMessage(FormatException(msg));
			}
			else if (priority == Poco::Message::PRIO_ERROR)
			{
				auto result = PostMessage(FormatException(msg));
			}
			else if (priority == Poco::Message::PRIO_WARNING)
			{
				auto result = PostMessage(FormatMessage(msg));
			}
			else if (priority == Poco::Message::PRIO_NOTICE)
			{
				auto result = PostMessage(FormatMessage(msg));
			}
			else if (priority == Poco::Message::PRIO_INFORMATION)
			{
				auto result = PostMessage(FormatMessage(msg));
			}
			else if (priority == Poco::Message::PRIO_DEBUG)
			{
				auto result = PostMessage(FormatMessage(msg));
			}
			else if (priority == Poco::Message::PRIO_TRACE)
			{
				auto result = PostMessage(FormatMessage(msg));
			}
		}
		catch(...)
		{
			//Swallow everything.  Remote logging exceptions cannot break the application.
		}
	}

	void StackifyChannel::setProperty(const std::string& name, const std::string& value)
	{
		Poco::FastMutex::ScopedLock lock(mutex_);

		if (name == PROP_APIKEY)
			api_key_ = value;
		else if (name == PROP_APPLOC)
			location_ = value;
		else if (name == PROP_APPNAME)
			app_name_ = value;
		else if (name == PROP_ENVIRONMENT)
			environment_ = value;
		else if (name == PROP_LOGGER)
			logger_ = value;
		else if (name == PROP_PLATFORM)
			platform_ = value;
		else if (name == PROP_SERVERNAME)
			server_name_ = value;
		else
			Channel::setProperty(name, value);
	}

	std::string StackifyChannel::getProperty(const std::string& name) const
	{
		if (name == PROP_APIKEY)
			return api_key_;
		if (name == PROP_APPLOC)
			return location_;
		if (name == PROP_APPNAME)
			return app_name_;
		if (name == PROP_ENVIRONMENT)
			return environment_;
		if (name == PROP_LOGGER)
			return logger_;
		if (name == PROP_PLATFORM)
			return platform_;
		if (name == PROP_SERVERNAME)
			return server_name_;

		return Channel::getProperty(name);
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	std::string StackifyChannel::PostMessage(Poco::JSON::Object::Ptr message)
	{

		try
		{
			//Create the HTTPSClientSession & initialize the request
			Poco::URI uri(stackify_logging_save_log_url);
			Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());
			Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, uri.getPath(), Poco::Net::HTTPMessage::HTTP_1_1);
			session.setProxy("localhost", 8888);  //Enable this to send to local fiddler proxy


			std::stringstream body_stream;
			message->stringify(body_stream); //Used to set content length
			request.setKeepAlive(true);
			request.setContentLength(body_stream.str().size());
			request.setContentType("application/json");
			request.add("Accept", "application/json");
			request.add("X-Stackify-PV", "V1");
			request.add("X-Stackify-Key", api_key_);

			std::ostream& request_stream = session.sendRequest(request);
			message->stringify(request_stream); //Write to request stream (Send)

			// Receive the response.
			Poco::Net::HTTPResponse response;
			std::istream& response_stream = session.receiveResponse(response);

			if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_NOT_FOUND)
				throw Poco::ApplicationException("The requested resource was not found.");

			//Parse JSON Response
			Poco::JSON::Parser parser;
			Poco::Dynamic::Var result = parser.parse(response_stream);

			if (_stricmp(response.getContentType().c_str(), "application/json; charset=utf-8") == 0)
			{
				if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
				{
					return result.toString();
				}

				throw Poco::ApplicationException(response.getReason(), response.getStatus());
			}

			const std::string error = "Unexpected content type returned from server.  Expected (application/json; charset=utf-8) and received " + response.getContentType();
			throw Poco::ApplicationException(error);
		}
		catch (Poco::JSON::JSONException e)
		{
			throw Poco::ApplicationException("Session::Post() - JSON Exception", e);
		}
		catch (Poco::Net::SSLException e)
		{
			throw Poco::ApplicationException("Session::Post() - SSL Exception", e);
		}
	}

	Poco::JSON::Object::Ptr StackifyChannel::FormatMessage(const Poco::Message& msg) const
	{
		//Build Json
		Poco::JSON::Object::Ptr root = new Poco::JSON::Object(true);

		//Set Environment
		root->set("Env", environment_);

		//Set Server Name
		root->set("ServerName", server_name_);

		//Set App Name
		root->set("AppName", app_name_);

		//Set App Loc
		root->set("AppLoc", location_);

		//Set Logger
		root->set("Logger", logger_);

		//Set Platform
		root->set("Platform", platform_);

		//Set Message Array.  We are only sending one message at a time, but array is required
		Poco::JSON::Array::Ptr messages = new Poco::JSON::Array();

		//Message
		Poco::JSON::Object::Ptr message = new Poco::JSON::Object();
		message->set("Msg", msg.getText());
		message->set("Th", msg.getThread());

		//Epoch
		Poco::Timestamp epoch_time = msg.getTime();
		const auto epoch_milliseconds = epoch_time.epochMicroseconds() / 1000;

		std::stringstream epoch;
		epoch << epoch_milliseconds;

		message->set("EpochMs", epoch.str());

		//Source Method
		//message->set("SrcMethod", msg.getSourceFile());

		//Source Line
		//message->set("SrcLine,", msg.getSourceLine());

		//Message Priority
		const auto priority = msg.getPriority();
		if (priority == Poco::Message::PRIO_FATAL)
		{
			message->set("Level", "fatal");
		}
		else if (priority == Poco::Message::PRIO_CRITICAL)
		{
			message->set("Level", "critical");
		}
		else if (priority == Poco::Message::PRIO_ERROR)
		{
			message->set("Level", "error");
		}
		else if (priority == Poco::Message::PRIO_WARNING)
		{
			message->set("Level", "warning");
		}
		else if (priority == Poco::Message::PRIO_NOTICE)
		{
			message->set("Level", "notice");
		}
		else if (priority == Poco::Message::PRIO_INFORMATION)
		{
			message->set("Level", "info");
		}
		else if (priority == Poco::Message::PRIO_DEBUG)
		{
			message->set("Level", "debug");
		}
		else if (priority == Poco::Message::PRIO_TRACE)
		{
			message->set("Level", "trace");
		}

		//Add message to messages array
		messages->set(0, message);

		//Add messages array to root
		root->set("Msgs", messages);

		return root;
	}

	// ReSharper disable once CppMemberFunctionMayBeStatic
	// ReSharper disable once CppMemberFunctionMayBeConst
	Poco::JSON::Object::Ptr StackifyChannel::FormatException(const Poco::Message & msg)
	{
		//Build Json
		Poco::JSON::Object::Ptr root = new Poco::JSON::Object(true);

		//Set Environment
		root->set("Env", environment_);

		//Set Server Name
		root->set("ServerName", server_name_);

		//Set App Name
		root->set("AppName", app_name_);

		//Set App Loc
		root->set("AppLoc", location_);

		//Set Logger
		root->set("Logger", logger_);

		//Set Platform
		root->set("Platform", platform_);

		//Set Message Array.  We are only sending one message at a time, but array is required
		Poco::JSON::Array::Ptr messages = new Poco::JSON::Array();

		//Message
		Poco::JSON::Object::Ptr message = new Poco::JSON::Object();
		message->set("Msg", msg.getText());
		message->set("Th", msg.getThread());
		
		//Exception
		Poco::JSON::Object::Ptr ex = new Poco::JSON::Object();
		Poco::JSON::Object::Ptr environment_detail = new Poco::JSON::Object();
		environment_detail->set("DeviceName", server_name_);
		environment_detail->set("AppLocation", location_);
		environment_detail->set("ConfiguredAppName", app_name_);
		environment_detail->set("ConfiguredEnvironmentName", environment_);
		
		//Add Environment Detail to Exception Object
		ex->set("Ex", environment_detail);

		//Epoch
		Poco::Timestamp epoch_time = msg.getTime();
		const auto epoch_milliseconds = epoch_time.epochMicroseconds() / 1000;

		std::stringstream epoch;
		epoch << epoch_milliseconds;

		//Set Exception EpochMilliseconds
		ex->set("OccurredEpochMillis", epoch.str());

		/*"Error": {
			"Message": "java.lang.NullPointerException: Something important was null that can't be null (Example error message with exception details)",
				"ErrorType" : "java.lang.RuntimeException",
				"SourceMethod" : "com.stackify.error.test.SomeSweetClass.doSomething",
				"StackTrace" : [
			{
				"CodeFileName": "SomeSweetClass.java",
					"LineNum" : 16,
					"Method" : "com.stackify.error.test.SomeSweetClass.doSomething"
			},
			{
				"CodeFileName": "StackifyErrorAppenderTest.java",
				"LineNum" : 44,
				"Method" : "com.stackify.error.test.StackifyErrorAppenderTest.main"
			}
				],*/
		Poco::JSON::Object::Ptr error = new Poco::JSON::Object();


		message->set("EpochMs", epoch.str());

		//Source Method
		message->set("SrcMethod", msg.getSourceFile());

		//Source Line
		message->set("SrcLine,", msg.getSourceLine());

		//Message Priority
		const auto priority = msg.getPriority();
		if (priority == Poco::Message::PRIO_CRITICAL)
		{
			message->set("Level", "critical");
		}
		else if (priority == Poco::Message::PRIO_DEBUG)
		{
			message->set("Level", "debug");
		}
		else if (priority == Poco::Message::PRIO_FATAL)
		{
			message->set("Level", "fatal");
		}
		else if (priority == Poco::Message::PRIO_INFORMATION)
		{
			message->set("Level", "info");
		}
		else if (priority == Poco::Message::PRIO_WARNING)
		{
			message->set("Level", "warning");
		}
		else if (priority == Poco::Message::PRIO_TRACE)
		{
			message->set("Level", "trace");
		}

		//Add message to messages array
		messages->set(0, message);

		//Add messages array to root
		root->set("Msgs", messages);

		return root;
	}

}
