/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** Core
*/

#pragma once

#include "ConfigLoader.hpp"
#include "Server.hpp"
#include <memory>

namespace Zia {
	class Core {
	public:
		Core(int ac, char **av, char **env);
		~Core() = default;

	public:
		void start();
		bool hasError() const noexcept { return this->_error; };

	private:
		bool _error = false;
		Zia::ConfigLoader _configLoader;
		std::shared_ptr<Zia::Server> _server;
	};
};