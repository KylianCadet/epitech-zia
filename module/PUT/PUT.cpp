/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** PUT
*/

#include "PUT.hpp"

const char *Zia::Module::PUT::getName(void) const
{
	return "PUT";
}

#include <iostream>

void Zia::Module::PUT::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
	pipeline.registerCallback(
		oZ::State::Interpret,
		oZ::Priority::Medium,
		this, &Zia::Module::PUT::onInterpret);
}

#include "filesystem.hpp"
#include <filesystem.hpp>
#include <iostream>
#include <fstream>

bool Zia::Module::PUT::_modifyFile(oZ::Context &context, const std::string &path) const noexcept
{
	std::ofstream outfile;

	outfile.open(path);
	outfile << context.getRequest().getBody();
	outfile.close();

	if (context.getRequest().getBody().empty())
		context.getResponse().setCode(oZ::HTTP::Code::NoContent);
	else
		context.getResponse().setCode(oZ::HTTP::Code::OK);
	return false;
}

bool Zia::Module::PUT::_createFile(oZ::Context &context, const std::string &path) const noexcept
{
	std::ofstream outfile(path);

	outfile << context.getRequest().getBody();
	context.getResponse().setCode(oZ::HTTP::Code::Created);
	return false;
}

bool Zia::Module::PUT::onInterpret(oZ::Context &context)
{
	oZ::HTTP::Method method = context.getRequest().getMethod();
	if (method != oZ::HTTP::Method::Put)
		return true;

	std::string currentPath = fs::current_path().string();
	std::string path = currentPath + context.getRequest().getURI();

	context.getResponse().getHeader().set("Content-Type", "message/http");
	context.getResponse().getHeader().set("Content-Length", "0");

	if (fs::exists(path))
		return this->_modifyFile(context, path);
	return this->_createFile(context, path);
}

OPEN_ZIA_MAKE_ENTRY_POINT(Zia::Module::PUT);