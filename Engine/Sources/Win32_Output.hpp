#pragma once


#include <ostream>


namespace Vague
{
    class VisualConsoleOutputBuffer
        : public std::streambuf
    {
    protected:

        virtual int_type            overflow(const int_type _iChar) override;
        virtual std::streamsize     sputn(const char_type* const _sz, const std::streamsize _iSize);
    };

    class VisualConsoleOutput
        : public std::ostream
    {
    public:

        VisualConsoleOutput();

    private:

        VisualConsoleOutputBuffer   m_oBuffer;
    };

    extern VisualConsoleOutput      s_VSCOut;
    #define VAGUE_COUT              Vague::s_VSCOut
}
