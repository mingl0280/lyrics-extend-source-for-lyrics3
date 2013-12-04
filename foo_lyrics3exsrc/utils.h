#pragma once

namespace utils{

	void chars_to_lower(const char* p_src , pfc::string_base& p_out);
	void remove_chars(const char* p_src , pfc::string_base& p_out , const char* p_char);
}