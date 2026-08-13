#pragma once

namespace rage
{
	class atString
	{
	public:
		operator const char* () const {
			return m_data ? m_data : "";
		}

	private:
		char* m_data;
		uint32_t m_length;
		uint32_t m_allocated;
	};
}