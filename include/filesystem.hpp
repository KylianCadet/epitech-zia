/*
** EPITECH PROJECT, 2020
** test
** File description:
** filesystem
*/

#pragma once


#ifdef unix
	#include <experimental/filesystem>
#elif _WIN32
	#include <filesystem>
#endif

#ifdef unix
	namespace fs = std::experimental::filesystem;
#elif _WIN32
	namespace fs = std::filesystem;
#endif
