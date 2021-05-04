#pragma once

#include <string>


namespace BAScloud {

/** 
 * A Result structure representing API paging information.
 * 
 * Paging is structured so that there are two pointers, one for the next page when available, and 
 * one for the previous page when available, otherwise empty.
 * 
 */
struct PagingResult {

	/**
	 * Textual identifier for the next page of the request when available. 
	 * If not available, the value is empty ({}).
	 */
	std::string nextPagePointer = {};

   /**
	 * Textual identifier for the previous page of the request when available. 
	 * If not available, the value is empty ({}).
	 */
	std::string previousPagePointer = {};

   /**
	 * Page size describing the maximal number of data entries per page.
	 * Number of entries may be lower if less than page size entries are available.
	 */
	size_t pageSize = 1000;

	/**
	 * Current page number ranging from 1 to totalPages.
	 */
    size_t currentPage = 1;

   /**
	 * The total number of pages in the request result.
	 * For navigating between the result pages nextPagePointer, previousPagePointer are used.
	 */
    size_t totalPages = 1;

	/**
	 * The number of results remaining in the request result.
	 * At currentPage==1, count describes the total number of entries of the result over all pages. When
	 * navigating to following pages, the count decreases to the remaining entries excluding the already seen.
	 */
	size_t count = 1;
};

/** 
 * A User entity represents a API user of the BAScloud.
 * 
 * A User can authenticate itself against the BAScloud API using an email and password combination.
 * 
 * New User can be created using the static User method User::createUser().
 * 
 */
class PagingOption {

	public:

	/**
	 * Direction of the paging pointer.
	 * 
	 * When requesting paginated results one can navigate back and forth between the result pages.
	 * 
	 * Value NONE describes a no direction e.g. the first page request or non-paginated.
	 * 
	 */
	enum Direction {
		NEXT,
		PREVIOUS,
		NONE
	};

   /**
	 * Page size describing the maximal number of data entries per page.
	 * Number of entries may be less if less than page size are available.
	 */
	size_t page_size;

   /**
	 * Direction of the paging request, either to the next page, the previous or None.
	 */
	Direction direction;

	/**
	 * Page pointer, textual identifier to the next or previous page.
	 * Value returned by the API in a previous request result as nextPagePointer and previousPagePointer.
	 */
	std::string page_pointer;

	/**
	 * Default constructor.
	 * 
	 * Creates a default PagingOption object. Per default no paging is requested with direction NONE.
	 * 
	 */
	PagingOption() : 
		page_size(-1), direction(Direction::NONE), page_pointer({}) {
	}

	/**
	 * PagingOption constructor.
	 *
	 * Creates a PagingOption object representing a pagination request.
	 *
	 * @param pageSize Page size for the requested results e.g. maximal number of entries a result page should contain.
	 * @param direction Direction of the paging request, either to next or previous page (or None for no direction e.g. the first page request or non-paginated).
	 * @param pagePointer Textual identifier for the request page.
	 */
	PagingOption(size_t pageSize, Direction direction=Direction::NONE, std::string pagePointer={}) : 
		page_size(pageSize), direction(direction), page_pointer(pagePointer) {
	}

};

	
}