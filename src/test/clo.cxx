/*
 * This file was automaticly generated by clo++ version $Id$
 * http://pmade.org/pjones/software/clo++/
 */

/** @file
 * This file contains the implementation of the parser class.
 * You can control the name of this source file using the
 * cxx_source_file variable.
**/

#include "clo.h"
#include <cstring>
#include <cstdlib>
#include <cctype>

namespace {
    const char const_usage[] =
"  -d, --database string  Override default database of \"test\"\n"
"  -p, --password string  Specify the password.\n"
"  -u, --username string  Specify the username.\n";

    const char* expand_long_name (const std::string &name);
}

//#########################################################################
clo::parser::parser (void)
    : state_(state_option)
{
    std::memset(&locations_, 0, sizeof(locations_));
}
//#########################################################################
void clo::parser::parse (int argc, char *argv[], bool call_finalize) {
    for (int i=1; i<argc; ++i) parse_element(argv[i], i, source_cl);
    if (call_finalize) finalize();
}
//#########################################################################
void clo::parser::finalize (void) {
    if (state_ == state_value) {
	switch (openum_) {
	    case option_database:
		throw clo::exception("missing value for 'database' option");
	    case option_password:
		throw clo::exception("missing value for 'password' option");
	    case option_username:
		throw clo::exception("missing value for 'username' option");
	}

    }

}
//#########################################################################
void clo::parser::parse_element (const char *element, int position, opsource source) {
    std::size_t length = std::strlen(element);

    switch (state_) {
	case state_consume:
	    non_options_.push_back(element);
	    break;
	case state_option:
	    if (length >= 2 && element[0] == '-' && element[1] == '-') {
		if (length == 2) { state_ = state_consume; return; }
		element += 2;
		const char *value = element;
		while (*value != 0 && *value != '=') ++value;
		if (*value == '=') {
		    std::string selement(element, value - element), svalue(++value);

		    parse_long_option(selement.c_str(), position, source);
		    if (state_ != state_value) {
			std::string error("the '"); error += element; error += "' option does not take a value";
			throw clo::exception(error);
		    }

		    parse_value(svalue.c_str());
		    state_ = state_option;
		} else {
		    parse_long_option(element, position, source);
		}
	    } else if (length >= 2 && element[0] == '-') {
		++element;

		if (length > 2) {
		    while (*element != 0) {
			parse_short_option(*element, position, source);
			++element;
	    
			if (state_ == state_value && *element != 0) {
			    parse_value(element);
			    state_ = state_option;
			    break;
			}
		    }
		} else {
		    parse_short_option(*element, position, source);
		}
	    } else {
		non_options_.push_back(element);
	    }
	    break;
	case state_value:
	    parse_value(element);
	    state_ = state_option;
	    break;
    }
}
//#########################################################################
void clo::parser::parse_short_option (char option, int position, opsource source) {
    switch (option) {
    	case 'd':
    	    openum_ = option_database;
    	    state_ = state_value;
    	    locations_.database = position;
    	    return;
    	case 'p':
    	    openum_ = option_password;
    	    state_ = state_value;
    	    locations_.password = position;
    	    return;
    	case 'u':
    	    openum_ = option_username;
    	    state_ = state_value;
    	    locations_.username = position;
    	    return;
    	case 'h':
    	    throw clo::autoexcept(autothrow_help, const_usage);
    	case '?':
    	    throw clo::autoexcept(autothrow_help, const_usage);
    }

    std::string error("unknown option: '"); error += option; error += "'";
    throw clo::exception(error);
}
//#########################################################################
void clo::parser::parse_long_option (const char *option, int position, opsource source) {
    option = expand_long_name(option);

	    if (std::strcmp(option, "database") == 0) {
		openum_ = option_database;
		locations_.database = position;
		state_ = state_value;
		return;
	    } else if (std::strcmp(option, "password") == 0) {
		openum_ = option_password;
		locations_.password = position;
		state_ = state_value;
		return;
	    } else if (std::strcmp(option, "username") == 0) {
		openum_ = option_username;
		locations_.username = position;
		state_ = state_value;
		return;
	    } else if (std::strcmp(option, "help") == 0) {
		throw clo::autoexcept(autothrow_help, const_usage);
	    }

    std::string error("unknown option '"); error += option; error += "'";
    throw clo::exception(error);
}
//#########################################################################
void clo::parser::parse_value (const char *value) {
    switch (openum_) {
    	case option_database:
    	    {
    		options_.database = value;
    	    }
	    break;
    	case option_password:
    	    {
    		options_.password = value;
    	    }
	    break;
    	case option_username:
    	    {
    		options_.username = value;
    	    }
	    break;
    }
}
//#########################################################################
namespace {
    const char* expand_long_name (const std::string &name) {
	std::string::size_type name_size = name.size();
	std::vector<const char*> matches;

        if (name_size <= 8 && name.compare(0, name_size, "database", name_size) == 0)
        	matches.push_back("database");

        if (name_size <= 8 && name.compare(0, name_size, "password", name_size) == 0)
        	matches.push_back("password");

        if (name_size <= 8 && name.compare(0, name_size, "username", name_size) == 0)
        	matches.push_back("username");

        if (name_size <= 4 && name.compare(0, name_size, "help", name_size) == 0)
        	matches.push_back("help");


	if (matches.empty()) {
	    std::string error("unknown option '"); error += name; error += "'";
	    throw clo::exception(error);
	}

	if (matches.size() == 1) {
	    return matches[0];
	}

	std::string error("the option name '"); error += name; error += "' is ambiguous";
	throw clo::exception(error);
    }
} // end anonymous namespace