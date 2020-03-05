/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** parseHeader
*/

#include <iostream>
#include <vector>

std::vector<std::string> splitString(const std::string &str, const char &delimiter, int max_token = -1) noexcept
{
	std::vector<std::string> tokens;
	std::size_t index = 0;
	while (str.find(delimiter, index) != std::string::npos && max_token != 0) {
		tokens.push_back(str.substr(index, str.find(delimiter, index) - index));
		index = str.find(delimiter, index) + 1;
		max_token--;
	}
	tokens.push_back(str.substr(index, str.length()));
	return (tokens);
}

#include <algorithm>

const std::string &StringToUpper(std::string &str)
{
	std::for_each(str.begin(), str.end(), ::toupper);
	return str;
}

#include "parseHeader.hpp"

const char *Zia::Module::parseHeader::getName(void) const
{
	return "Listing";
}

void Zia::Module::parseHeader::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
	pipeline.registerCallback(
		oZ::State::Parse,
		oZ::Priority::Medium,
		this, &Zia::Module::parseHeader::onInterpret);
}

void Zia::Module::parseHeader::_initByteString(oZ::ByteArray &byteArray) noexcept
{
	for (int i = 0; i != byteArray.size(); i++)
		this->_byteString.push_back(byteArray.at(i));
}

void Zia::Module::parseHeader::_initHeader(oZ::HTTP::Header &header, oZ::HTTP::Body &body) noexcept
{
	bool isBody = false;

	std::vector<std::string> headerRaw = splitString(this->_byteString, '\n');
	for (auto &Elem : headerRaw) {
		// If is body, set the request body then return
		if (isBody) {
			body = Elem;
			return;
		}
		std::vector<std::string> headerRawLine = splitString(Elem, ':', 1);
		// Line only contain \r -> next line will be the body
		if (headerRawLine[0] == "\r") {
			isBody = true;
			continue;
		}
		// Line is not a header, ignore
		if (headerRawLine.size() != 2) {
			continue;
		}
		// Remove trailing space between ':' and the value (cf: "Key: Value")
		if (headerRawLine[1][0] == ' ') {
			headerRawLine[1] = headerRawLine[1].substr(1, headerRawLine[1].length());
		}
		header.set(headerRawLine[0], headerRawLine[1]);
	}
}

void Zia::Module::parseHeader::_initQuery(std::string &requestString, oZ::HTTP::Request &request) const noexcept
{
	std::vector<std::string> tokens = splitString(requestString, '?');
	if (tokens.size() == 1)
		return;
	requestString = tokens[0];
	std::vector<std::string> queryTokens = splitString(tokens[1], '&');
	for (auto &E : queryTokens) {
		std::vector<std::string> keyVal = splitString(E, '=');
		if (keyVal.size() != 2)
			return;
		request.getQueryParameters()[keyVal[0]] = keyVal[1];
	}
	// DEBUG
	// oZ::HTTP::QueryParameters queryParameters = request.getQueryParameters();
	// for (auto &Elem : queryParameters)
	// 	std::cout << Elem.first << "=" << Elem.second << std::endl;
}

void Zia::Module::parseHeader::_initRequest(oZ::HTTP::Request &request, oZ::Context &context) noexcept
{
	std::vector<std::string> headerRaw = splitString(this->_byteString, '\n');
	if (headerRaw.size() < 1) {
		std::cerr << "parseHeader : header is empty" << std::endl;
		context.setState(oZ::State::Error);
		return;
	}
	std::string requestString = headerRaw[0];
	std::vector<std::string> requestStringToken = splitString(requestString, ' ');
	if (requestStringToken.size() != 3) {
		std::cerr << "parseHeader : " << requestString << " : invalid request string" << std::endl;
		context.setState(oZ::State::Error);
		return;
	}
	this->_initQuery(requestStringToken[1], request);
	request.getURI() = requestStringToken[1];
	StringToUpper(requestStringToken[0]);
	request.setMethod(methodMap[requestStringToken[0]]);
}

bool Zia::Module::parseHeader::onInterpret(oZ::Context &context)
{
	this->_initByteString(context.getPacket().getByteArray());
	this->_initHeader(context.getRequest().getHeader(), context.getRequest().getBody());
	this->_initRequest(context.getRequest(), context);
	this->_byteString.clear();
	return false;
}

OPEN_ZIA_MAKE_ENTRY_POINT(Zia::Module::parseHeader);