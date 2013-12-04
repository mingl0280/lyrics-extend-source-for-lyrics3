#include "stdafx.h"
#include "component_defines.h"
#include "ttplayer_source.h"
#include "utils.h"
#include "tinyxml2/tinyxml2.h"

bool ttplayer_lyric_source::PrepareSearch( const search_info* pQuery, lyric_result_client::ptr p_results, search_requirements::ptr& pRequirements )
{
	return true;
}

bool ttplayer_lyric_source::Search( const search_info* pQuery, search_requirements::ptr& pRequirements, lyric_result_client::ptr p_results )
{
	pfc::string8 url,out;

	generate_url(url,pQuery);

	if(!m_pclient->download_page(out,"",url))return false;
	
	//parse the result
	tinyxml2::XMLDocument doc;
	if( tinyxml2::XML_NO_ERROR == doc.Parse(out)){
		
		tinyxml2::XMLElement* result = doc.RootElement();
		tinyxml2::XMLElement* lrc_node = result->FirstChildElement("lrc");
		while(lrc_node){

			int id             = lrc_node->IntAttribute("id");
			const char* artist = lrc_node->Attribute("artist");
			const char* title  = lrc_node->Attribute("title");
			pfc::string8 p_info(artist);
			p_info.add_string(title);
			
			//generate lrc download url.
			int code = generate_code(p_info,id);
			url.reset();
			url << "http://ttlrcct.qianqian.com/dll/lyricsvr.dll?dl?Id=" << id << "&Code=" << code;

			//download lyric
			pfc::string8 lrc_conent;
			if(m_pclient->download_page(lrc_conent,"",url)){

				//add to lyrics3
				lyric_container_base* new_lyric = p_results->AddResult();

				new_lyric->SetFoundInfo(artist,"",title);
				new_lyric->SetSources(pfc::format_int(id), "TTPLAYER Private Source", GetGUID(), ST_INTERNET );
				new_lyric->SetLyric(lrc_conent);

			}

			//next 
			lrc_node = lrc_node->NextSiblingElement();
		}
	}
	
	return false;
}

void ttplayer_lyric_source::generate_url( pfc::string_base& p_out , const search_info* p_info )
{
	p_out.reset();

    pfc::string8 title,artist;

	//required.
	utils::chars_to_lower(p_info->title,title);
	utils::remove_chars(title,title," ,.'");
	utils::chars_to_lower(p_info->artist,artist);
	utils::remove_chars(artist,artist," ,.'");
		
	//optional.
	//utils::remove_chars(title,title,"@");
	//utils::remove_chars(artist,artist,"@");
	

	//convert to unicode hex string
	pfc::stringcvt::string_wide_from_utf8 title2(title);
	pfc::stringcvt::string_wide_from_utf8 artist2(artist);
	pfc::format_hexdump dump1(title2,2*title2.length(),0);
	pfc::format_hexdump dump2(artist2,2*artist2.length(),0);

	p_out << "http://ttlrcct.qianqian.com/dll/lyricsvr.dll?sh?Artist=" << dump2 << "&Title=" << dump1 << "&Flags=0";
	
}

t_int32 ttplayer_lyric_source::generate_code( const char* p_info , int p_id )
{
	t_size len = pfc::strlen_t(p_info);
	t_int32 t1=0,t2=0,t3=0;

	t1 = ( p_id & 0xFF00 ) >> 8;
	if(!( p_id&0xFF0000 ))
		t3 = 0xFF & ~t1;
	else
		t3 = 0xFF & ((p_id & 0xFF0000) >> 16);
	t3 |= ((0xFF & p_id) << 8);
	t3 <<= 8;
	t3 |= (0xFF & t1);
	t3 <<= 8;

	if (!(p_id & 0xFF000000))
		t3 |= (0xFF & (~p_id));
	else
		t3 |= (0xFF & (p_id >> 24));

	for (t_size n = len; n > 0 ; n--)
	{
		t_int32 c = (t_int32)p_info[n-1];
		if( !(c < 0x80) ) c -= 0x100;
		t1 = c + t2;
		t2 <<= ((n-1)%2 + 4);
		t2 += t1;
	}
	t1 = 0;
	for (t_size n=0;n<len;n++)
	{
		t_int32 c = (t_int32)p_info[n];
		if( !( c < 0x80 )) c -= 0x100;
		t_int32 t4 = c + t1;
		t1 <<= ( n%2 + 3 );
		t1 += t4;
	}
	t_int32 t5 = (t_int32)conv(t2 ^ t3);
	t5 = (t_int32)conv(t5 + (t1 | p_id));
	t5 = (t_int32)conv(t5 * (t1 | t3));
	t5 = (t_int32)conv(t5 * (t2 ^ p_id));
	long t6 = (long)t5;
	if (t6 > 0x80000000)
		t5 = (int)(t6 - 0x100000000);
	return t5;
}


namespace{
	static const source_factory<ttplayer_lyric_source> g_ttplayer_source;
}
