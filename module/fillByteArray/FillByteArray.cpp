/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** FillByteArray
*/

#include "FillByteArray.hpp"
#include <sstream>

const char *Zia::Module::FillByteArray::getName(void) const
{
	return "SSLRead";
}

void Zia::Module::FillByteArray::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
	pipeline.registerCallback(
		oZ::State::Completed,
		oZ::Priority::ASAP,
		this, &Zia::Module::FillByteArray::onInterpret);
}

oZ::ByteArray Zia::Module::FillByteArray::_generateByteArray(std::int8_t const *buffer, int const &n) const noexcept
{
	oZ::ByteArray byteArray;

	for (unsigned i = 0; i != n; i++)
		byteArray.push_back(buffer[i]);
	return byteArray;
}

#include <iostream>

bool Zia::Module::FillByteArray::onInterpret(oZ::Context &context)
{
	std::stringstream ssOut;
	ssOut << "HTTP/1.1 " << std::to_string(static_cast<int>(context.getResponse().getCode())) << std::endl;

	oZ::HTTP::StringMultimap multi = context.getResponse().getHeader().getStringMultimap();
	for (auto it = multi.begin(); it != multi.end(); it++)
		ssOut << it->first << ": " << it->second << std::endl;

	ssOut << std::endl;

	// Do not set body if HEAD request
	if (context.getRequest().getMethod() != oZ::HTTP::Method::Head)
		ssOut << context.getResponse().getBody();

	oZ::ByteArray newByteArray = this->_generateByteArray(reinterpret_cast<const int8_t *>(ssOut.str().c_str()), ssOut.str().length());

	context.getPacket().getByteArray().clear();
	context.getPacket().getByteArray() = newByteArray;
	return true;
}

OPEN_ZIA_MAKE_ENTRY_POINT(Zia::Module::FillByteArray);