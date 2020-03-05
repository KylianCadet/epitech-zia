/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** Core
*/

#include "Core.hpp"
#include <iostream>

Zia::Core::Core(int ac, char **av, char **env) :
	_configLoader(ac, av, env)
{
	if (this->_configLoader.hasError()) {
		this->_error = true;
		return;
	}
}

void Zia::Core::start()
{
	std::cout << "Press ENTER to reset Modules and config file" << std::endl;
	while (std::cin) {
		std::cout << "Using Modules : " << std::endl;
		for (const auto &entry : fs::directory_iterator(this->_configLoader.getModuleDir())) {
			if (entry.path().extension() != ".so" && entry.path().extension() != ".dll")
				continue;
			std::string file_name(entry.path().filename().string());
			std::cout << file_name << std::endl;
		}
		try {
			this->_server = std::shared_ptr<Zia::Server>(new Zia::Server(this->_configLoader));
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
			return;
		}
		this->_server->start();
		if (!std::cin)
			return;
		this->_server.reset();
		this->_configLoader.hotReload();
	}
}