#ifndef CURL_DOWNLOADER_HPP
#define CURL_DOWNLOADER_HPP

#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>

class CURLDownloader
{
private:
	static CURLDownloader *instance;
	void *curl;

	CURLDownloader()
	{
		curl_global_init(CURL_GLOBAL_ALL); // extension of library loader
		curl = curl_easy_init();
	}
public:
	static CURLDownloader* get_instance()
	{
		if(!instance)
			instance = new CURLDownloader()

		return instance;
	}


};

CURLDownloader* CURLDownloader::instance = NULL;

#endif
