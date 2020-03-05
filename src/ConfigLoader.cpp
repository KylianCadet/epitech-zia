/*
** EPITECH PROJECT, 2020
** test
** File description:
** ConfigLoader
*/

#include "ConfigLoader.hpp"
#include "filesystem.hpp"
#include <fstream>
#include <rapidjson/document.h>
#include <sstream>
#include <vector>

#ifdef unix
#include <getopt.h>
#endif

std::vector<std::string> splitString(const std::string &str, const char &delimiter, int max_token = -1) noexcept;

std::string get_help()
{
	std::stringstream help;

	help << "./zia [OPTION]" << std::endl;
	help << "\t-c, --config\tthe relative path to the configuration file, DEFAULT: ./config.json" << std::endl;
	help << "\t-h, --help\tdisplay this message and exit";
	return help.str();
}

void Zia::ConfigLoader::_getOpt(int ac, char **av)
{
#ifdef unix
	char c = 0;
	static struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"config", required_argument, 0, 'c'}};

	while (c != -1) {
		c = getopt_long(ac, av, "hc:", long_options, nullptr);
		if (c == -1)
			return;
		switch (c) {
			case 'c':
				this->_configFile = optarg;
				break;
			case 'h':
				this->_help = true;
				break;
			default:
				this->_error = true;
				break;
		}
	}
#endif
}

Zia::ConfigLoader::ConfigLoader(int ac, char **av, char **env) :
	_env(env)
{
	this->_getOpt(ac, av);
	if (this->_error)
		return;
	if (this->_help) {
		std::cout << get_help() << std::endl;
		return;
	}
	try {
		this->_parseConfigFile();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		this->_error = true;
		return;
	}
	if (!fs::is_directory(this->_moduleDir)) {
		std::cerr << "ConfigLoader: moduleDir: " << this->_moduleDir << ": not a directory" << std::endl;
		this->_error = true;
		return;
	}
}

std::string Zia::ConfigLoader::getEnv(const std::string &key) noexcept
{
	std::string res = "";

	for (int i = 0; this->_env[i] != nullptr; i++) {
		std::vector<std::string> tokens = splitString(this->_env[i], '=', 1);
		if (tokens[0] == key) {
			res = tokens[1];
		}
	}
	return res;
}

char const *const *Zia::ConfigLoader::getEnv() const noexcept
{
	return this->_env;
}

Zia::ConfigLoader::~ConfigLoader()
{
}

void has_member_or_throw(const rapidjson::GenericValue<rapidjson::UTF8<>> &document, const std::string &member)
{
	if (!document.HasMember(member.c_str()))
		throw std::invalid_argument("JSON parser : document has no Member \"" + member + "\"");
}

void is_good_type_or_throw(const bool &isGood, const std::string &member, const std::string &type)
{
	if (!isGood)
		throw std::invalid_argument("JSON parser : Member \"" + member + "\" must be of type " + type);
}

void Zia::ConfigLoader::_parseConfigFile()
{
	std::string configFilePath(this->_configFile);

	// Check if exists
	if (!fs::exists(configFilePath))
		throw std::invalid_argument(configFilePath + ": doesn't exist");

	// Check if regular file
	if (!fs::is_regular_file(configFilePath))
		throw std::invalid_argument(configFilePath + ": is not a file");

	std::ifstream ifs(configFilePath);
	std::string configFileContent((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	rapidjson::Document document;
	document.Parse(configFileContent.c_str());

	// Check if good json document
	if (!document.IsObject())
		throw std::invalid_argument(configFilePath + ": bad formated json file");

	// Get "moduleDir"
	try {
		has_member_or_throw(document, "moduleDir");
		is_good_type_or_throw(document["moduleDir"].IsString(), "moduleDir", "String");
		this->_moduleDir = document["moduleDir"].GetString();
	} catch (std::exception &e) {
		std::cerr << e.what() << ": switching to default value: " << this->_moduleDir << std::endl;
	}

	// Get "port"
	try {
		has_member_or_throw(document, "port");
		is_good_type_or_throw(document["port"].IsInt(), "port", "Int");
		this->_port = document["port"].GetInt();
	} catch (std::exception &e) {
		std::cerr << e.what() << ": switching to default value: " << this->_port << std::endl;
	}

	// Get "hasEncryption"
	try {
		has_member_or_throw(document, "hasEncryption");
		is_good_type_or_throw(document["hasEncryption"].IsBool(), "hasEncryption", "Bool");
		this->_hasEncryption = document["hasEncryption"].GetBool();
	} catch (std::exception &e) {
		std::cerr << e.what() << ": switching to default value: " << this->_hasEncryption << std::endl;
	}
}

void Zia::ConfigLoader::hotReload() noexcept
{
	try {
		this->_parseConfigFile();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
