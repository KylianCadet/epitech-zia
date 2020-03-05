/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** OpenSSL
*/

#include "OpenSSL.hpp"
#include <iostream>
#include <openssl/ssl.h>

Zia::Module::OpenSSL::OpenSSL()
{
	SSL_library_init();
	OpenSSL_add_all_algorithms();

	this->_ctx = SSL_CTX_new(SSLv23_server_method());;
	if (SSL_CTX_use_certificate_file(this->_ctx, "./mycert.pem", SSL_FILETYPE_PEM) <= 0) {
		std::cerr << "SSL_CTX_use_certificate_file" << std::endl;
		this->_error = true;
	}
	if (SSL_CTX_use_PrivateKey_file(this->_ctx, "./mycert.pem", SSL_FILETYPE_PEM) <= 0) {
		std::cerr << "SSL_CTX_use_PrivateKey_file" << std::endl;
		this->_error = true;
	}
	if (!SSL_CTX_check_private_key(this->_ctx)) {
		std::cerr << "Private key does not match the public certificate" << std::endl;
		this->_error = true;
	}
}

Zia::Module::OpenSSL::~OpenSSL()
{
	FIPS_mode_set(0);
	CRYPTO_set_locking_callback(NULL);
	CRYPTO_set_id_callback(NULL);
	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
}

const char *Zia::Module::OpenSSL::getName(void) const
{
	return "OpenSSL";
}

void Zia::Module::OpenSSL::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
	// decrypt request before parsing
	pipeline.registerCallback(
		oZ::State::BeforeParse,
		oZ::Priority::Medium,
		this, &Zia::Module::OpenSSL::decrypt);

	// encrypt request before server send
	pipeline.registerCallback(
		oZ::State::Completed,
		oZ::Priority::Medium,
		this, &Zia::Module::OpenSSL::encrypt);
}

#include "network.h"
#include <iostream>

void Zia::Module::OpenSSL::onConnection(oZ::Context &context)
{
	if (!context.getPacket().hasEncryption())
		return;

	const oZ::FileDescriptor fd = context.getPacket().getFileDescriptor();
	const oZ::Endpoint endpoint = context.getPacket().getEndpoint();

	SSL *ssl = SSL_new(this->_ctx);
	BIO *rbio = BIO_new(BIO_s_mem()); /* SSL reads from, we write to. */
	BIO *wbio = BIO_new(BIO_s_mem()); /* SSL writes to, we read from. */

	SSL_set_accept_state(ssl);
	SSL_set_bio(ssl, rbio, wbio);

	ssl_socket_t new_sock = {fd, endpoint, ssl, rbio, wbio};

	this->_socks.push_back(new_sock);
}

ssl_socket_t Zia::Module::OpenSSL::_badSock() const noexcept
{
	return ssl_socket_t{};
}

ssl_socket_t Zia::Module::OpenSSL::_getSock(const oZ::FileDescriptor &fd) noexcept
{
	for (auto &Elem : this->_socks)
		if (Elem.fd == fd)
			return Elem;
	return this->_badSock();
}

ssl_socket_t Zia::Module::OpenSSL::_getSock(const oZ::Endpoint &endpoint) noexcept
{
	for (auto &Elem : this->_socks)
		if (Elem.endpoint == endpoint)
			return Elem;
	return this->_badSock();
}

oZ::ByteArray Zia::Module::OpenSSL::_generateByteArray(std::int8_t const *buffer, int const &n) const noexcept
{
	oZ::ByteArray byteArray;

	for (unsigned i = 0; i != n; i++)
		byteArray.push_back(buffer[i]);
	return byteArray;
}

oZ::ByteArray Zia::Module::OpenSSL::_recv(oZ::ByteArray &byteArray, ssl_socket_t *sock) noexcept
{
	// Writing byteArray into the read Bio
	int n = BIO_write(sock->rbio, byteArray.data(), byteArray.size());

	// Checking SSL_accept
	if (!SSL_is_init_finished(sock->ssl)) {
		std::pair<std::int8_t *, long> buffer = std::get_temporary_buffer<std::int8_t>(1000);
		int n = SSL_do_handshake(sock->ssl);
		switch (SSL_get_error(sock->ssl, n)) {
			case SSL_ERROR_WANT_READ:
				n = BIO_read(sock->wbio, buffer.first, buffer.second);
				if (n <= 0) {
					// NOT SSL PACKET
					sock->not_ssl = true;
					return oZ::ByteArray(0);
				}
				buffer.first[n] = 0;
				send(sock->fd, reinterpret_cast<const char *>(buffer.first), n, 0);
				break;
			default:
				break;
		}
		std::return_temporary_buffer(buffer.first);
		if (!SSL_is_init_finished(sock->ssl))
			return oZ::ByteArray(0);
	};

	// Reading the previously written byteArray with a SSL_read call
	std::pair<std::int8_t *, long> buffer = std::get_temporary_buffer<std::int8_t>(1000);
	n = SSL_read(sock->ssl, buffer.first, buffer.second);
	switch (SSL_get_error(sock->ssl, n)) {
		case SSL_ERROR_WANT_READ:
			// Peer has requested SSL renegotiation.
			n = BIO_read(sock->wbio, buffer.first, buffer.second);
			if (n <= 0) {
				// Connection has timed out (server) and client is trying to reload page
				sock->error = true;
				return oZ::ByteArray(0);
			}
			buffer.first[n] = 0;
			send(sock->fd, reinterpret_cast<const char *>(buffer.first), n, 0);
			return oZ::ByteArray(0);
		case SSL_ERROR_SSL:
			// Pipe is surelly broken
			return oZ::ByteArray(0);
		default:
			break;
	}
	buffer.first[n] = 0;
	oZ::ByteArray newByteArray = this->_generateByteArray(buffer.first, n);
	std::return_temporary_buffer(buffer.first);

	return newByteArray;
}

void Zia::Module::OpenSSL::onDisconnection(oZ::Context &context)
{
	const oZ::FileDescriptor fd = context.getPacket().getFileDescriptor();
	ssl_socket_t sock = this->_getSock(fd);

	SSL_free(sock.ssl);
}

bool Zia::Module::OpenSSL::decrypt(oZ::Context &context)
{
	if (!context.getPacket().hasEncryption())
		return true;
	if (this->_error) {
		context.setState(oZ::State::Error);
		return true;
	}
	ssl_socket_t sock = this->_getSock(context.getPacket().getEndpoint());
	if (sock.error) {
		context.setState(oZ::State::Error);
		std::cout << "Cannot retrieve ssl socket" << std::endl;
		return true;
	}

	if (sock.not_ssl)
		return true;

	oZ::ByteArray byteArray = this->_recv(context.getPacket().getByteArray(), &sock);

	if (sock.not_ssl)
		return true;

	if (byteArray.size() == 0) {
		context.setState(oZ::State::Error);
		return true;
	}
	context.getPacket().getByteArray().clear();
	context.getPacket().getByteArray() = byteArray;
	return true;
}

bool Zia::Module::OpenSSL::encrypt(oZ::Context &context)
{
	if (!context.getPacket().hasEncryption())
		return true;
	if (this->_error) {
		context.setState(oZ::State::Error);
		return true;
	}
	ssl_socket_t sock = this->_getSock(context.getPacket().getEndpoint());
	if (sock.error || sock.not_ssl)
		return true;

	oZ::ByteArray byteArray = context.getPacket().getByteArray();

	int n = SSL_write(sock.ssl, byteArray.data(), byteArray.size());

	std::pair<std::int8_t *, long> buffer = std::get_temporary_buffer<std::int8_t>(1001);
	oZ::ByteArray newByteArray;
	do {
		n = BIO_read(sock.wbio, buffer.first, 1000);
		if (n <= 0)
			continue;
		buffer.first[n] = 0;
		for (unsigned i = 0; i != n; i++)
			newByteArray.push_back(buffer.first[i]);
	} while (n > 0);

	std::return_temporary_buffer(buffer.first);

	context.getPacket().getByteArray().clear();
	context.getPacket().getByteArray() = newByteArray;
	return true;
}

OPEN_ZIA_MAKE_ENTRY_POINT(Zia::Module::OpenSSL);