/*
** EPITECH PROJECT, 2020
** test
** File description:
** Server
*/

#pragma once

#include "network.h"
#include <openZia/Pipeline.hpp>

typedef struct socket
{
	oZ::FileDescriptor fd;
	oZ::Endpoint _endpoint;
} socket_t;

#include "ConfigLoader.hpp"
#include "filesystem.hpp"
#include <iostream>
#include <thread>
#include <vector>

namespace Zia {
	class Server {
	public:
		Server(Zia::ConfigLoader &config);
		~Server();

	public:
		virtual void start();
		virtual void stop() noexcept;

	private:
		void _launchPipeline(oZ::Context &context);
		void _bind(int const &port);
		void _handleClient(socket_t sock, const Zia::ConfigLoader config);
		void _closeSocket(socket_t const &fd);
		oZ::ByteArray _generateByteArray(std::int8_t const *buffer, int const &n) const noexcept;
		oZ::ByteArray _recv(socket_t const &sock) noexcept;
		void _send(const oZ::FileDescriptor &fd, oZ::Context &context) noexcept;
		void __IO() noexcept;

	protected:
	private:
		oZ::Pipeline _pipeline;
		Zia::ConfigLoader &_config;

		socket_t _sock;
		socket_t _new_sock;
		struct sockaddr_in _sockaddr_in;
		struct sockaddr_in _new_sockaddr_in;
		int _new_sock_len;

		std::vector<std::thread> _threadPool;

		bool _running = true;

#ifdef WIN32
		WSADATA wsaData;
#endif
	};
};

std::vector<std::string> splitString(const std::string &str, const char &delimiter, int max_token = -1) noexcept;