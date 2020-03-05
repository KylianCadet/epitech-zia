/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** TRACE
*/

#include "TRACE.hpp"

const char *Zia::Module::TRACE::getName(void) const
{
	return "TRACE";
}

#include <iostream>

void Zia::Module::TRACE::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
	pipeline.registerCallback(
		oZ::State::Interpret,
		oZ::Priority::Medium,
		this, &Zia::Module::TRACE::onInterpret);
}

#include "filesystem.hpp"
#include <filesystem.hpp>

bool Zia::Module::TRACE::onInterpret(oZ::Context &context)
{
	oZ::HTTP::Method method = context.getRequest().getMethod();
	if (method != oZ::HTTP::Method::Trace)
		return true;

	context.getResponse().getHeader().set("Content-Type", "message/http");
	context.getResponse().getHeader().set("Content-Length", "0");
	context.getResponse().setCode(oZ::HTTP::Code::OK);
	return false;
}

OPEN_ZIA_MAKE_ENTRY_POINT(Zia::Module::TRACE);