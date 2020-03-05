/*
** EPITECH PROJECT, 2020
** test
** File description:
** ConfigLoader
*/

#pragma once

#include <iostream>
#include <map>

namespace Zia {
	class ConfigLoader {
	public:
		ConfigLoader(int ac, char **av, char **env);
		~ConfigLoader();

	public:
		char const *const *getEnv() const noexcept;
		std::string getEnv(const std::string &key) noexcept;
		bool hasError() const noexcept { return this->_error; };
		int getPort() const noexcept { return this->_port; };
		std::string getModuleDir() const noexcept { return this->_moduleDir; };
		std::string getConfigFile() const noexcept { return this->_configFile; };
		bool getEncryption() const noexcept { return this->_hasEncryption; };
		void hotReload() noexcept;

	private:
		void _getOpt(int ac, char **av);
		void _parseConfigFile();

	protected:
	private:
		char **_env;
		std::string _configFile = "./config.json";
// Define the API moduleDir
#ifdef unix
		std::string _moduleDir = "./module";
#elif _WIN32
		std::string _moduleDir = "./";
#endif
		// Define the Server port
		int _port = 8000;
		// Define the API hadEncryption value
		bool _hasEncryption = false;
		bool _help = false;
		bool _error = false;
	};
};
