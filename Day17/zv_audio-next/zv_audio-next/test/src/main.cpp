
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <map>

#include "zv_kernel.h"
#include "zv_audio.h"
#include "json.h"


const char* _cmd_start = "r";
const char* _cmd_start_explain = "start audio channel";
const char* _cmd_stop = "s";
const char* _cmd_stop_explain = "stop audio channel";
const char* _cmd_quit = "q";
const char* _cmd_quit_explain = "quit";
const char* _cmd_help = "h";
const char* _cmd_help_explain = "display the format of cmds";


void _zva_event_callback(const zva_event* evt, void*);
void _print_help();


typedef struct {
	bool gw_enabled;
	uint16_t gw_port;
	std::string gw_addr;
	std::string gw_license;
	std::string gw_transport;

	uint16_t port;
	uint8_t index;
	std::string addr;
	std::string transport;

	std::string log_level;
	std::string log_name;
} _ZVA_CONTEXT;


bool load_config(const char* file_name, _ZVA_CONTEXT& config)
{
	std::ifstream file(file_name, std::ios_base::in);
	if (!file.is_open()) { return false; }

	Json::Reader json_reader;
	Json::Value json_root;
	if (!json_reader.parse(file, json_root) || json_root.isNull())
	{
		printf("parse json string failed!\n");
		return false;
	}

	// check global config
	if (json_root["gw_enabled"].isNull() ||
		json_root["gw_port"].isNull() ||
		json_root["gw_addr"].isNull() ||
		json_root["gw_license"].isNull() ||
		json_root["gw_transport"].isNull() ||
		json_root["addr"].isNull() ||
		json_root["port"].isNull() ||
		json_root["index"].isNull() ||
		json_root["transport"].isNull() ||
		json_root["log_level"].isNull() ||
		json_root["log_name"].isNull())
	{
		printf("config file format Error!\n");
		return false;
	}

	config.gw_enabled = json_root["gw_enabled"].asBool();
	config.gw_port = json_root["gw_port"].asInt();
	config.gw_addr = json_root["gw_addr"].asCString();
	config.gw_license = json_root["gw_license"].asCString();
	config.gw_transport = json_root["gw_transport"].asCString();

	config.addr = json_root["addr"].asCString();
	config.port = json_root["port"].asInt();
	config.index = json_root["index"].asInt();
	config.transport = json_root["transport"].asCString();

	config.log_level = json_root["log_level"].asCString();
	config.log_name = json_root["log_name"].asCString();

	return true;
}


int main(int argc, char *argv[])
{
	//77.1
	if (argc < 2)
	{
		printf("config file is NULL\n");
		return 0;
	}

	_ZVA_CONTEXT config;
	if (!load_config(argv[1], config))
	{
		printf("load config file failed\n");
		return 0;
	}

	zva_init(config.log_level.c_str(), config.log_name.c_str());
	zva_set_callback(_zva_event_callback, NULL);

	_print_help();

	char input[128] = {0};
	while (1)
	{
		if (1 != scanf("%s", input))
		{
			_print_help();
			continue;
		}

		if (strcmp(input, _cmd_start) == 0)
		{
			if (config.gw_enabled)
			{
				zva_set_proxy(config.gw_addr.c_str(), config.gw_port, config.gw_license.c_str(), "");
			}
			else
			{
				zva_remove_proxy();
			}
			zva_start(config.addr.c_str(), config.port, "", "", config.index);
		}
		else if (strcmp(input, _cmd_stop) == 0)
		{
			zva_stop();
		}
		else if (strcmp(input, _cmd_quit) == 0)
		{
			break;
		}
		else
		{
			_print_help();
		}
	}

	zva_uninit();
	return 0;
}


void _zva_event_callback(const zva_event* evt, void*)
{
	switch (evt->_type)
	{

	default:
		break;
	}
}

void _print_help()
{
	printf("\n=======================================================\n");
	printf("	\"%s\" : %s\n", _cmd_start, _cmd_start_explain);
	printf("	\"%s\" : %s\n", _cmd_stop, _cmd_stop_explain);
	printf("	\"%s\" : %s\n", _cmd_quit, _cmd_quit_explain);
	printf("	\"%s\" : %s\n", _cmd_help, _cmd_help_explain);
	printf("=======================================================\n\n");
}
