#pragma once


namespace Vague
{
	class INonCopyable
	{
	public:

		INonCopyable()	= default;
		~INonCopyable()	= default;

	private:

		INonCopyable(const INonCopyable&)				= delete;
		INonCopyable& operator = (const INonCopyable&)	= delete;
	};

	class IDefaultConstructorCallNotAllowed
	{
	protected:

		IDefaultConstructorCallNotAllowed() = default;
	};
}
