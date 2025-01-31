// ISourceQueryConfig.h
#ifndef ISOURCE_QUERY_CONFIG_H
#define ISOURCE_QUERY_CONFIG_H

#include <string>

struct ISourceQueryConfig {
    std::string object_view_id; // could be a JSON object, a string, or a custom class
    std::string object_data;    // Replace this with the actual C++ type
};

#endif // ISOURCE_QUERY_CONFIG_H 