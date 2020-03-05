/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** DELETE
*/

#include "DELETE.hpp"

const char *Zia::Module::_DELETE::getName(void) const
{
	return "DELETE";
}

#include <iostream>

void Zia::Module::_DELETE::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
	pipeline.registerCallback(
		oZ::State::Interpret,
		oZ::Priority::Medium,
		this, &Zia::Module::_DELETE::onInterpret);
}

#include "filesystem.hpp"
#include <filesystem.hpp>

bool Zia::Module::_DELETE::onInterpret(oZ::Context &context)
{
	std::string currentPath = fs::current_path().string();
	std::string path = currentPath + context.getRequest().getURI();

	oZ::HTTP::Method method = context.getRequest().getMethod();
	if (method != oZ::HTTP::Method::Delete)
		return true;

	context.getResponse().getHeader().set("Content-Type", "message/http");
	context.getResponse().getHeader().set("Content-Length", "0");

	if (!fs::is_regular_file(path)) {
		context.getResponse().setCode(oZ::HTTP::Code::NotFound);
		return false;
	}
	try {
		fs::remove(fs::path(path));
	} catch (std::exception &e) {
		context.getResponse().setCode(oZ::HTTP::Code::BadRequest);
		return false;
	}
	context.getResponse().setCode(oZ::HTTP::Code::OK);
	return false;
}

OPEN_ZIA_MAKE_ENTRY_POINT(Zia::Module::_DELETE);