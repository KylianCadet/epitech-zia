/*
** EPITECH PROJECT, 2020
** test
** File description:
** main
*/

#include "Core.hpp"
#include "Server.hpp"
#include <iostream>

int main(int ac, char **av, char **env)
{
	Zia::Core core(ac, av, env);
	if (core.hasError())
		return 84;
	core.start();
	return 0;
}