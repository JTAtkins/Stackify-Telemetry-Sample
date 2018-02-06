#pragma once

enum Priority
{
	FATAL = 1,   /// A fatal error. The application will most likely terminate. This is the highest priority.
	CRITICAL,    /// A critical error. The application might not be able to continue running successfully.
	ERROR,		 /// An error. An operation did not complete successfully, but the application as a whole is not affected.
	WARNING,     /// A warning. An operation completed with an unexpected result.
	NOTICE,      /// A notice, which is an information with just a higher priority.
	INFORMATION, /// An informational message, usually denoting the successful completion of an operation.
	DEBUG,       /// A debugging message.
	TRACE        /// A tracing message. This is the lowest priority.
};