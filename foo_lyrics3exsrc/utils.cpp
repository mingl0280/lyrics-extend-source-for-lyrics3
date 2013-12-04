#include "stdafx.h"
#include "utils.h"

namespace utils{

	void remove_chars( const char* p_src , pfc::string_base& p_out , const char* p_char)
	{
		const char* p  = p_char;
		pfc::string8 temp(p_src);
		const char* p2 = temp.get_ptr();
		for (;*p;)
		{
			unsigned test;
			t_size delta = pfc::utf8_decode_char(p,test);
			if(delta == 0 || test == 0)break;
			for (;*p2;)
			{
				unsigned test2;
				t_size delta2 = pfc::utf8_decode_char(p2,test2);
				if(delta2 == 0 || test2 == 0)break;
				if( test == test2 ){
					temp.remove_chars(p2-temp.get_ptr(),delta2);
				}
				else{
					p2 += delta2;
				}
			}
			p2 = temp.get_ptr();
			p += delta;
		}
		p_out.set_string(temp);
	}

	void chars_to_lower( const char* p_src , pfc::string_base& p_out )
	{
		p_out.reset();
		const char* p = p_src;
		for (;*p;)
		{
			unsigned test;
			t_size delta = pfc::utf8_decode_char(p,test);
			if( delta == 0 || test == 0 ) break;
			p_out.add_char(pfc::charLower(test));
			p+=delta;
		}
	}

}