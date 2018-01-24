#pragma once
#include <Poco/Channel.h>
#include <Poco/Message.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/PrivateKeyFactory.h>


namespace DefenseAgainstTheDarkArts {


	class StackifyChannel : public Poco::Channel
		/// A channel that writes to an ostream.
		///
		/// Only the message's text is written, followed
		/// by a newline.
		///
		/// Chain this channel to a FormattingChannel with an
		/// appropriate Formatter to control what is contained 
		/// in the text.
		///
		/// Similar to StreamChannel, except that a static
		/// mutex is used to protect against multiple
		/// console channels concurrently writing to the
		/// same stream.
	{
	public:
		StackifyChannel();
		/// Creates the channel and attaches std::clog.

		StackifyChannel(std::ostream& str);
		/// Creates the channel using the given stream.

		void log(const Poco::Message& msg) override;
		/// Logs the given message to the channel's stream.


		void setProperty(const std::string& name, const std::string& value) override;
		/// Sets the property with the given name. 
		/// 
		/// The following properties are supported:
		///   * apiKey:       Your Stackify API Key (Required)
		///
		///   * environment:  The Environment you are logging to (Required)
		///
		///   * serverName:   Server or Device Name (Required)
		///                   
		///   * appName:      The name of the application (Required)
		///                   
		///   * location      The directory path of the application (Optional)
		///
		///   * logger:   Name and version of the logger (example: stackify-log-log4j12-1.0.12)
		///				      Required by the api, but defaults to stackify-log-pococpp1.0.0.0 if not set.
		///
		///   * platform:     The logging language (development language) - Defaults to CPP


		std::string getProperty(const std::string& name) const override;
		/// Returns the value of the property with the given name.
		/// See setProperty() for a description of the supported
		/// properties.

		static const std::string PROP_APIKEY;
		static const std::string PROP_ENVIRONMENT;
		static const std::string PROP_SERVERNAME;
		static const std::string PROP_APPNAME;
		static const std::string PROP_APPLOC;
		static const std::string PROP_LOGGER;
		static const std::string PROP_PLATFORM;
		static const std::string PROP_OPERATINGSYSTEM;
		static const std::string PROP_OSMAJORVERSION;
		static const std::string PROP_OSMINORVERSION;
		static const std::string PROP_TIMEZONE;

	protected:
		~StackifyChannel();

	private:
		std::ostream& str_;
		Poco::FastMutex mutex_;

		std::string api_key_;
		std::string environment_;
		std::string server_name_;
		std::string app_name_;
		std::string location_;
		std::string logger_;
		std::string platform_;
		std::string operating_system_;
		std::string os_major_version_;
		std::string os_minor_version_;
		std::string timezone_;

		std::string stackify_logging_save_log_url;

		std::string base_uri_;
		Poco::SharedPtr<Poco::Net::PrivateKeyPassphraseHandler> p_console_handler_;
		Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> p_certificate_handler_;
		Poco::Net::Context::Ptr p_context_;

		std::string PostMessage(Poco::JSON::Object::Ptr message);
		Poco::JSON::Object::Ptr FormatMessage(const Poco::Message& msg) const;
		Poco::JSON::Object::Ptr FormatException(const Poco::Message& msg);
	};
}
