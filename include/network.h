/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** nerwork
*/

#pragma once

#ifdef unix
	#include <arpa/inet.h>
	#include <sys/select.h>
	typedef int SOCKET;
#elif _WIN32
	#include <winsock.h>
#endif