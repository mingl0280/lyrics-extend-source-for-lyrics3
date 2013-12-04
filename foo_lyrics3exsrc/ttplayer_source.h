#pragma once


class ttplayer_lyric_source : public lyric_source_base
{
public:

	void GetName( pfc::string_base& out ) { out = "TTPlayer"; }

	const GUID GetGUID() { return g_ttplayer_source_guid; }

	bool PrepareSearch( const search_info* pQuery, lyric_result_client::ptr p_results, search_requirements::ptr& pRequirements );
	
	bool Search( const search_info* pQuery, search_requirements::ptr& pRequirements, lyric_result_client::ptr p_results  );
	
	void SetHttpClient( lyric_http_request::ptr p_client ) { m_pclient = p_client; };

private:
	t_int32 generate_code(const char* p_info , int p_id);
	void generate_url(pfc::string_base& p_out , const search_info* p_info);
	inline t_int64 conv(t_int32 i) {
		t_int64 r = i % 0x100000000;
		if (i >= 0 && r > 0x80000000)
			r = r - 0x100000000;
		if (i < 0 && r < 0x80000000)
			r = r + 0x100000000;
		return r;
	}
private:

	lyric_http_request::ptr m_pclient;

};