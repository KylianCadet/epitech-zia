/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** misc
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

#ifdef unix
#include "unistd.h"
#else
#include <Windows.h>
#endif

std::string getBinaryDir(bool endSlash = true) noexcept
{
#ifdef unix
	char buffer[BUFSIZ];
	size_t n = readlink("/proc/self/exe", buffer, BUFSIZ);
	buffer[n] = 0;
	std::string res(buffer);
	res = res.substr(0, res.find_last_of('/') + endSlash);
	return res;
#else
	char pBuf[MAX_PATH];
	std::string res(pBuf, GetModuleFileNameA(NULL, pBuf, MAX_PATH));
	res = res.substr(0, res.find_last_of('\\') + endSlash);
	return res;
#endif
}

#include <algorithm>

const std::string &StringToUpper(std::string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}