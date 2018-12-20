#pragma once

#include <Core.hpp>
#include <Types.hpp>


namespace Vague
{
	namespace Tests
	{
		void	TestFailLog(const char* const _szFilePath, const int _iLine, const char* const _szFunction);
		void	TestEndLog(const char* const _szFunction, const u32 _iTestCount, const u32 _iSuccessful);
	}
}

#define VAGUE_UTEST_INIT        Vague::u32 iTestCount__ = 0, iSuccessfulTests__ = 0

#define VAGUE_UTEST(expr)			    \
{									    \
	const bool bUTest__ = (expr);	    \
	++iTestCount__;					    \
	VAGUE_ASSERT(bUTest__);	            \
	if (bUTest__)					    \
	{								    \
		++iSuccessfulTests__;		    \
	}								    \
	else							    \
	{								    \
		Vague::Tests::TestFailLog(__FILE__, __LINE__, __FUNCTION__);\
	}								    \
}

#define VAGUE_UTEST_END				    \
{									    \
	Vague::Tests::TestEndLog(__FUNCTION__, iTestCount__, iSuccessfulTests__);\
	return (iTestCount__ - iSuccessfulTests__);\
}
