/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** OPTIONS
*/

#include "OPTIONS.hpp"

const char *Zia::Module::OPTIONS::getName(void) const
{
	return "OPTIONS";
}

#include <iostream>

void Zia::Module::OPTIONS::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
	pipeline.registerCallback(
		oZ::State::Interpret,
		oZ::Priority::Medium,
		this, &Zia::Module::OPTIONS::onInterpret);
}

#include "filesystem.hpp"
#include <filesystem.hpp>

bool Zia::Module::OPTIONS::onInterpret(oZ::Context &context)
{
	oZ::HTTP::Method method = context.getRequest().getMethod();
	if (method != oZ::HTTP::Method::Option)
		return true;

	context.getResponse().getHeader().set("Allow", "OPTIONS, GET, POST, PUT, HEAD, DELETE, TRACE");
	context.getResponse().getHeader().set("Content-Type", "message/http");
	context.getResponse().getHeader().set("Content-Length", "0");

	if (context.getRequest().getBody().empty())
		context.getResponse().setCode(oZ::HTTP::Code::NoContent);
	else
		context.getResponse().setCode(oZ::HTTP::Code::OK);
	return false;
}

OPEN_ZIA_MAKE_ENTRY_POINT(Zia::Module::OPTIONS);