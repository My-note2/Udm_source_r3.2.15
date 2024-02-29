#ifndef UDM_CONFIG_H
#define UDM_CONFIG_H

#include <string> // STL

[!if META_LOADING_DYNAMIC && DYNAMIC_META_XML_BACKEND]
#define _DYNAMIC_META
#define _DYNAMIC_META_DOM
[!endif]

[!if META_LOADING_DYNAMIC && DYNAMIC_META_STATIC_DATA_NETWORK]
#define _DYNAMIC_META
#define _DYNAMIC_META_STATIC
#define META_PATH "[!output VALID_META_PATH]"
[!endif]

[!if VALID_HEADER_TO_INCLUDE]
#include "[!output HEADER_TO_INCLUDE]"
 
#define META_NAMESPACE ::[!output NAMESPACE_NAME]

[!endif]


struct _config
{
	// Global settings
	std::string metaPath;

};
extern _config config;

#endif /* UDM_CONFIG_H */
