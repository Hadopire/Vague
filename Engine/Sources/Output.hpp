#pragma once


#include <Core.hpp>


#if defined (__VAGUE_WINDOWS__)

#	include <Output_Win.hpp>

#else

#	include <ostream>
#	define VAGUE_COUT	std::cout
	
#endif
