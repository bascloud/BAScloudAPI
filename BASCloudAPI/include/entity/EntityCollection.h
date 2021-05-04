#pragma once

#include <vector>
#include "Paging.h"


namespace BAScloud {

/** 
 * Type definition of a collection of entities returned by the API.
 * 
 * API endpoints of entity collections use paging for large collections. EntityCollection combines 
 * the collection result of a request (single page) in form of collection of entities and the paging information in form of a PagingResult.
 * 
 */
template<typename T>
using EntityCollection = std::pair<std::vector<T>, PagingResult>;

	
}