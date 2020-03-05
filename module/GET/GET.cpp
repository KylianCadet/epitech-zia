/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** GET
*/

#include "GET.hpp"
#include "filesystem.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

const char *Zia::Module::GET::getName(void) const
{
	return "GET";
}

void Zia::Module::GET::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
	pipeline.registerCallback(
		oZ::State::Interpret,
		oZ::Priority::Medium,
		this, &Zia::Module::GET::onInterpret);
}

const std::string &StringToUpper(std::string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

bool Zia::Module::GET::_set404(oZ::Context &context) const noexcept
{
	context.getResponse().setCode(oZ::HTTP::Code::NotFound);
	std::string sHTML = "<html><body><h1>Not Found</h1></body></html>";
	context.getResponse().getBody() += sHTML;
	context.getResponse().getHeader().set("Content-Type", "text/html");
	context.getResponse().getHeader().set("Content-Length", std::to_string(sHTML.length()));
	return true;
}

bool Zia::Module::GET::_setRegularFile(oZ::Context &context, const std::string &path) const noexcept
{
	std::ifstream ifs(path);
	std::string buffer(std::istreambuf_iterator<char>(ifs), {});
	ifs.close();

	fs::path a(path);
	std::string extention = a.extension().string();
	StringToUpper(extention);
	std::string contentType = ContentTypes[extention];
	if (contentType.empty())
		contentType = "application/octet-stream";

	context.getResponse().setCode(oZ::HTTP::Code::OK);
	context.getResponse().getHeader().set("Content-Type", contentType);
	context.getResponse().getHeader().set("Content-Length", std::to_string(buffer.size()));

	// Do not set body if HEAD method
	if (context.getRequest().getMethod() != oZ::HTTP::Method::Head)
		context.getResponse().getBody() += buffer;
	return true;
}

bool Zia::Module::GET::_setError(oZ::Context &context) const noexcept
{
	context.getResponse().setCode(oZ::HTTP::Code::InternalServerError);
	std::string sHTML = "<html><body><h1>Internal Server Error</h1></body></html>";
	context.getResponse().getBody() += sHTML;
	context.getResponse().getHeader().set("Content-Type", "text/html");
	context.getResponse().getHeader().set("Content-Length", std::to_string(sHTML.length()));
	return true;
}

bool Zia::Module::GET::_setDirectory(oZ::Context &context, const std::string &path) const noexcept
{
	std::string sHTML = "<html><body><h1>Directory listing for " + context.getRequest().getURI() + "</h1>";
	sHTML += "<body><hr><ul>";
	try {
		for (const auto &entry : fs::directory_iterator(path)) {
			continue;
		}
	} catch (std::exception const &e) {
		return this->_setError(context);
	}
	for (const auto &entry : fs::directory_iterator(path)) {
		std::string file_path(entry.path().filename().string());
		if (fs::is_directory(entry.path()))
			file_path += "/";
		sHTML += "<li>";
		sHTML += "<a href=\"";
		sHTML += file_path;
		sHTML += "\"</a>";
		sHTML += file_path;
		sHTML += "</li>";
	}
	sHTML += "</ul><hr></body></html>";
	context.getResponse().setCode(oZ::HTTP::Code::OK);
	context.getResponse().getHeader().set("Content-Type", "text/html");
	context.getResponse().getHeader().set("Content-Length", std::to_string(sHTML.length()));
	context.getResponse().getBody() += sHTML;
	return true;
}

bool Zia::Module::GET::onInterpret(oZ::Context &context)
{
	std::string currentPath = fs::current_path().string();
	std::string path = currentPath + context.getRequest().getURI();

	// Allow GET and HEAD
	if (context.getRequest().getMethod() != oZ::HTTP::Method::Get && context.getRequest().getMethod() != oZ::HTTP::Method::Head)
		return true;

	if (!fs::exists(path))
		return this->_set404(context);
	if (fs::is_regular_file(path))
		return this->_setRegularFile(context, path);
	if (fs::is_directory(path))
		return this->_setDirectory(context, path);
	return this->_setError(context);
}

OPEN_ZIA_MAKE_ENTRY_POINT(Zia::Module::GET);