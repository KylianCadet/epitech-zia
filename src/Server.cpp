/*
** EPITECH PROJECT, 2020
** test
** File description:
** Server
*/

#include "Server.hpp"
#include <memory>
#include <sstream>
#include <thread>

#ifdef unix
#include <unistd.h>
#endif

Zia::Server::Server(Zia::ConfigLoader &config) :
	_config(config),
	_pipeline(config.getModuleDir())
{
	this->_pipeline.loadModules();
#ifdef _WIN32
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
	this->_sock.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sock.fd < 0) {
		perror("socket");
		throw std::runtime_error("socket");
	}
	this->_new_sock_len = sizeof(this->_new_sockaddr_in);

	this->_bind(this->_config.getPort());
}

Zia::Server::~Server()
{
	std::cout << "Shutting down..." << std::endl;
	for (auto &Thread : this->_threadPool) {
		Thread.join();
	}
#ifdef _WIN32
	WSACleanup();
#endif
}

void Zia::Server::_bind(int const &port)
{
	this->_sockaddr_in.sin_family = AF_INET;
	this->_sockaddr_in.sin_port = htons(port);
	this->_sockaddr_in.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_sock.fd, reinterpret_cast<struct sockaddr *>(&this->_sockaddr_in), sizeof(this->_sockaddr_in)) < 0) {
		perror("bind");
		this->_sockaddr_in.sin_port = 0;
		bind(this->_sock.fd, reinterpret_cast<struct sockaddr *>(&this->_sockaddr_in), sizeof(this->_sockaddr_in));
	}

	if (listen(this->_sock.fd, 5) < 0) {
		perror("listen");
		throw std::runtime_error("listen");
	}

	struct sockaddr_in sin;
#ifdef unix
	unsigned int len = sizeof(sin);
#elif _WIN32
	int len = sizeof(sin);
#endif
	if (getsockname(this->_sock.fd, (struct sockaddr *)&sin, &len) == -1) {
		perror("getsockname");
		throw std::runtime_error("getsockname");
	}

	std::cout << "Serving HTTP on ";
	std::cout << inet_ntoa(this->_sockaddr_in.sin_addr);
	std::cout << " port " << ntohs(sin.sin_port);
	std::cout << " (http";
	if (this->_config.getEncryption())
		std::cout << "s";
	std::cout << "://"
			  << inet_ntoa(this->_sockaddr_in.sin_addr)
			  << ":" << ntohs(sin.sin_port) << "/) ..." << std::endl;
}

void Zia::Server::_closeSocket(socket_t const &sock)
{
#ifdef unix
	close(sock.fd);
#elif _WIN32
	closesocket(sock.fd);
#endif
}

oZ::ByteArray Zia::Server::_generateByteArray(std::int8_t const *buffer, int const &n) const noexcept
{
	oZ::ByteArray byteArray;

	for (unsigned i = 0; i != n; i++)
		byteArray.push_back(buffer[i]);
	return byteArray;
}

void Zia::Server::_send(const oZ::FileDescriptor &fd, oZ::Context &context) noexcept
{
	oZ::ByteArray byteArray = context.getPacket().getByteArray();

	if (context.getState() == oZ::State::Error)
		return;

	send(fd, reinterpret_cast<const char *>(byteArray.data()), byteArray.size(), 0);
}

oZ::ByteArray Zia::Server::_recv(socket_t const &sock) noexcept
{
	std::pair<std::int8_t *, long> buffer = std::get_temporary_buffer<std::int8_t>(1000);
	int n = recv(sock.fd, reinterpret_cast<char *>(buffer.first), buffer.second, 0);
	if (n <= 0) {
		std::return_temporary_buffer(buffer.first);
		return oZ::ByteArray(0);
	}
	buffer.first[n] = 0;
	oZ::ByteArray byteArray = this->_generateByteArray(buffer.first, n);
	std::return_temporary_buffer(buffer.first);
	return byteArray;
}

void Zia::Server::_launchPipeline(oZ::Context &context)
{
	context.getPacket().setEncryption(this->_config.getEncryption());
	this->_pipeline.runPipeline(context);
}

void Zia::Server::_handleClient(socket_t sock, const Zia::ConfigLoader config)
{
	oZ::Context fakeContext(oZ::Packet(std::move(oZ::ByteArray(1)), sock._endpoint, sock.fd));
	fakeContext.getPacket().setEncryption(this->_config.getEncryption());
#ifdef unix
	fakeContext.getRequest().getHeader().set("PATH", this->_config.getEnv("PATH"));
#elif _WIN32
	fakeContext.getRequest().getHeader().set("PATH", this->_config.getEnv("Path"));
#endif
	this->_pipeline.onConnection(fakeContext);

#ifdef unix
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	setsockopt(sock.fd, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&timeout), sizeof(timeout));
#elif _WIN32
	DWORD timeout = 5 * 1000;
	setsockopt(sock.fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
#endif
	while (this->_running) {
		oZ::ByteArray buffer = this->_recv(sock);
		if (buffer.empty())
			break;
		oZ::Context context(oZ::Packet(std::move(buffer), sock._endpoint, sock.fd));
		this->_launchPipeline(context);
		this->_send(sock.fd, context);
	}
	std::cout << "Connection closed with : " << sock._endpoint.getAddress() << ":" << sock._endpoint.getPort() << std::endl;
	this->_pipeline.onDisconnection(fakeContext);
#ifdef unix
	shutdown(sock.fd, SHUT_RDWR);
#elif _WIN32
	shutdown(sock.fd, 2);
#endif
}

void Zia::Server::start()
{
	this->__IO();

	while (this->_running) {
#ifdef unix
		this->_new_sock.fd = accept(this->_sock.fd, reinterpret_cast<struct sockaddr *>(&this->_new_sockaddr_in), reinterpret_cast<unsigned int *>(&this->_new_sock_len));
#elif _WIN32
		this->_new_sock.fd = accept(this->_sock.fd, reinterpret_cast<struct sockaddr *>(&this->_new_sockaddr_in), &this->_new_sock_len);
#endif
		if (this->_new_sock.fd < 0) {
			if (this->_running)
				perror("accept");
			return;
		}
		this->_new_sock._endpoint = oZ::Endpoint(inet_ntoa(this->_new_sockaddr_in.sin_addr), ntohs(this->_new_sockaddr_in.sin_port));
		std::cout << "Connection from " << this->_new_sock._endpoint.getAddress() << ":" << this->_new_sock._endpoint.getPort() << std::endl;
		this->_threadPool.push_back(std::thread(&Zia::Server::_handleClient, this, this->_new_sock, this->_config));
	}
}

void Zia::Server::__IO() noexcept
{
	std::thread([this] {
		std::string input;
		getline(std::cin, input);
		return this->stop();
	})
		.detach();
}

void Zia::Server::stop() noexcept
{
	this->_running = false;
#ifdef unix
	shutdown(this->_sock.fd, SHUT_RDWR);
#elif _WIN32
	this->_closeSocket(this->_sock);
#endif
}