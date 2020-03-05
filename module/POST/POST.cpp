/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** POST
*/

#include "POST.hpp"

const char *Zia::Module::POST::getName(void) const
{
	return "POST";
}

#include <iostream>

void Zia::Module::POST::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
	pipeline.registerCallback(
		oZ::State::Interpret,
		oZ::Priority::Medium,
		this, &Zia::Module::POST::onInterpret);
}

#include "filesystem.hpp"

bool Zia::Module::POST::onInterpret(oZ::Context &context)
{
	oZ::HTTP::Method method = context.getRequest().getMethod();
	if (method != oZ::HTTP::Method::Post)
		return true;

	context.getResponse().setCode(oZ::HTTP::Code::OK);

	context.getResponse().getHeader().set("Content-Type", "message/http");
	context.getResponse().getHeader().set("Content-Length", "0");

	return false;
}

OPEN_ZIA_MAKE_ENTRY_POINT(Zia::Module::POST);