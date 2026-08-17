#pragma once

#include <filesystem>

#include "inicpp.h"

class config
{
public:
	static ini::IniFile& get_ini()
	{
		static ini::IniFile iniFile;
		return iniFile;
	}

	static void load()
	{
		if (std::filesystem::exists("RageOpenRDR2.ini"))
		{
			get_ini().load("RageOpenRDR2.ini");
		}
		else
		{	//defaults
			get_ini()["config"]["console"] = false;
			get_ini()["log"]["info"] = false;
			get_ini()["log"]["mods"] = false;
			get_ini()["log"]["rpf"] = false;
			get_ini()["log"]["device"] = false;
			save();
		}
	}

	static void save()
	{
		get_ini().save("RageOpenRDR2.ini");
	}

	template<typename T>
	static T get_config(const char* key)
	{
		return get_ini()["config"][key].as<T>();
	}

	static bool get_log(const char* key)
	{
		return get_ini()["log"][key].as<bool>();
	}
};