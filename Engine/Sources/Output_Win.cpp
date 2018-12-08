
#include <Output.hpp>


#define WIN32_LEAN_AND_MEAN
#include <windows.h>	// ::OutputDebugString()
#undef WIN32_LEAN_AND_MEAN


namespace Vague
{
	VisualConsoleOutputBuffer::int_type
	VisualConsoleOutputBuffer::overflow(const int_type _iChar)
	{
		static char_type sz[2] = { 0 };
		sz[0] = static_cast<const char_type>(_iChar);
		::OutputDebugStringA(sz);
		return _iChar;
	}

	std::streamsize
	VisualConsoleOutputBuffer::sputn(const char_type* const _sz, const std::streamsize _iSize)
	{
		::OutputDebugStringA(_sz);
		return _iSize;
	}

	VisualConsoleOutput::VisualConsoleOutput()
		: std::ostream(&m_oBuffer)
	{
	}

	VisualConsoleOutput s_VSCOut;
}
