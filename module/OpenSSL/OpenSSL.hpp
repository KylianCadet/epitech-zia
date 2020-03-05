/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** OpenSSL
*/

#pragma once

#include <openZia/Endpoint.hpp>
#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>
#include <openssl/ssl.h>

typedef struct ssl_socket
{
	const oZ::FileDescriptor fd;
	const oZ::Endpoint endpoint;
	SSL *ssl = nullptr;
	BIO *rbio = nullptr;
	BIO *wbio = nullptr;
	bool error = false;
	bool not_ssl = false;
} ssl_socket_t;

namespace Zia {
	namespace Module {
		class OpenSSL : public oZ::IModule {
		public:
			OpenSSL();
			virtual ~OpenSSL();

		public:
			virtual const char *getName(void) const;
			virtual void onRegisterCallbacks(oZ::Pipeline &pipeline);
			void onConnection(oZ::Context &context) override;
			void onDisconnection(oZ::Context &context) override;

		private:
			oZ::ByteArray _generateByteArray(std::int8_t const *buffer, int const &n) const noexcept;
			oZ::ByteArray _recv(oZ::ByteArray &byteArray, ssl_socket_t *sock) noexcept;
			ssl_socket_t _getSock(const oZ::Endpoint &endpoint) noexcept;
			ssl_socket_t _getSock(const oZ::FileDescriptor &fd) noexcept;
			ssl_socket_t _badSock() const noexcept;

		private:
			bool decrypt(oZ::Context &context);
			bool encrypt(oZ::Context &context);

		private:
			SSL_CTX *_ctx;
			std::vector<ssl_socket_t> _socks;
			bool _error = false;
		};
	};
};