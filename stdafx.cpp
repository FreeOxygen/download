#include <regex>
#include "stdafx.h"
using namespace std;
//------------------ global ---------------
Config g_config;//全局配置信息

//------------------ functions ------------
/*
判断URL是否表达式
修改info的信息
*/
bool is_url_valid(url_info & info)
{
	regex http_Mold(g_config.http_mold);//config.url_mold_1);//"^(http://).*(\\.\\w+)+");//http协议下载
	regex https_Mold(g_config.https_mold);
	regex ftp_Mold(g_config.ftp_mold);//"ftp://[^\\s]*");//ftp协议下载
	regex magnet_Mold(g_config.magnet_mold);//"magnet:\\?xt=urn:btih:[^\\s]*");//磁力链接下载
	regex ED2K_Mold(g_config.ED2K_mold);//"(ed2k://\\|file\\|){1}.*");//电驴格式下载
	regex thunder_Mold(g_config.thunder_mold);
	switch (1)//进行判断url是否满足允许的协议
	{
	case 1:
		if (regex_match(info.url, http_Mold))
		{
			info.protocol = http;
			return true;
		}
	case 2:
		if (regex_match(info.url, https_Mold))
		{
			info.protocol = https;
			return true;
		}
	case 3:
		if (regex_match(info.url, ftp_Mold))
		{
			info.protocol = ftp;
			return true;
		}
	case 4:
		if (regex_match(info.url, magnet_Mold))
		{
			info.protocol = magnet;
			return true;
		}
	case 5:
		if (regex_match(info.url, ED2K_Mold))
		{
			info.protocol = ED2K;
			return true;
		}
	case 6:
		if (regex_match(info.url, thunder_Mold))
		{
			info.protocol = thunder;
			return true;
		}
	default:
		info.protocol = Err;
		return false;
	}
}

bool is_file_valid(char ex[])
{
	if (NULL != g_config.file_rule)
	{
		char tmp[1024] = "";
		char *rule;
		strcpy(tmp, g_config.file_rule);
		rule = strtok(tmp, ";");
		while (rule != NULL)
		{
			if (strcmp(ex, rule) == 0)
			{
				return true;
			}
			rule = strtok(NULL, ";");
		}
		return false;
	}
	return true;
}