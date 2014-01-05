#pragma once

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#ifdef _DEBUG
	#define LOG(level, msg) \
	do { \
		std::ostringstream oss; \
		oss << __FUNCTION__ <<"():"<<__LINE__<<":"<<msg<<std::endl; \
		BOOST_LOG_TRIVIAL(level) << oss.str(); \
	}while(0)

#else //can be strengthened in boost::log::trivial options.
	#define LOG(level, msg) \
	do { \
		if(level >= boost::log::trivial::severity_level::warning) { \
			std::ostringstream oss; \
			oss << msg<<std::endl; \
			BOOST_LOG_TRIVIAL(level) << oss.str(); \
		}\
    }while(0)
#endif

#define LOG_TRACE(msg)		LOG(trace, msg)
#define LOG_INFO(msg)		LOG(info, msg)
#define LOG_DEBUG(msg)		LOG(debug, msg)
#define LOG_WARNING(msg)	LOG(warning, msg)
#define LOG_ERROR(msg)		LOG(error, msg)
#define LOG_FATAL(msg)		LOG(fatal, msg)
