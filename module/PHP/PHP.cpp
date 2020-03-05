/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** PHP
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

#include "PHP.hpp"

const char *Zia::Module::PHP::getName(void) const
{
	return "PHP";
}

#include "filesystem.hpp"

std::string slash()
{
#ifdef unix
	return std::string("/");
#elif _WIN32
	return std::string("\\");
#endif
}

std::string extention()
{
#ifdef unix
	return std::string("");
#elif _WIN32
	return std::string(".exe");
#endif
}

void Zia::Module::PHP::_getCGIbin()
{
#ifdef unix
	std::vector<std::string> token = splitString(this->_path, ':');
#elif _WIN32
	std::vector<std::string> token = splitString(this->_path, ';');
#endif

	for (auto &p : token) {
		std::string path = p + slash() + "php-cgi" + extention();
		if (fs::exists(fs::path(path))) {
			this->_path = p + "/php-cgi";
			// DEBUG
			// std::cout << "php-cgi absolute path : " << this->_path << std::endl;
			return;
		}
	}
	throw std::invalid_argument("cannot retrive");
}

void Zia::Module::PHP::onConnection(oZ::Context &context)
{
	try {
		this->_path = context.getRequest().getHeader().get("PATH");
		this->_getCGIbin();
	} catch (const std::exception &e) {
		std::cerr << "PHP module : cannot find php-cgi binary" << std::endl;
		this->_error = true;
	}
}

#include <iostream>

void Zia::Module::PHP::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
	pipeline.registerCallback(
		oZ::State::Interpret,
		oZ::Priority::High,
		this, &Zia::Module::PHP::onInterpret);
}

#include "network.h"
#include <fstream>

#ifdef unix
#include <sys/wait.h>
#include <unistd.h>
#elif _WIN32
#include <io.h>
#include <namedpipeapi.h>
#include <windows.h>
#endif

void Zia::Module::PHP::_unixProcess(oZ::Context &context, std::string &res) noexcept
{
#ifdef unix
	int pipes[2];
	pipe(pipes);

	pid_t pid = fork();
	if (pid == 0) {
		const char *bin = this->_path.c_str();
		const char *argv[] = {bin, nullptr};
		const char **env = this->_buildEnv(context);
		close(pipes[0]);
		dup2(pipes[1], 1);
		int rc = execve(bin, const_cast<char *const *>(argv), const_cast<char *const *>(env));
		for (size_t i = 0; env[i] != nullptr; ++i)
			delete env[i];
		delete env;
		exit(rc);
	} else {
		close(pipes[1]);
		int exitCode = 0;
		waitpid(pid, &exitCode, 0);
		std::pair<std::int8_t *, long> buffer = std::get_temporary_buffer<std::int8_t>(1001);
		int n = 0;
		do {
			n = read(pipes[0], buffer.first, 1000);
			if (n <= 0)
				continue;
			buffer.first[n] = 0;
			for (unsigned i = 0; i != n; i++)
				res.push_back(buffer.first[i]);
		} while (n > 0);
		close(pipes[0]);
		std::return_temporary_buffer(buffer.first);
	}
#endif
}

void Zia::Module::PHP::_winProcess(oZ::Context &context, std::string &res) noexcept
{
#ifdef _WIN32
	SECURITY_ATTRIBUTES secattr;
	ZeroMemory(&secattr, sizeof(secattr));
	secattr.nLength = sizeof(secattr);
	secattr.bInheritHandle = TRUE;

	HANDLE rPipe, wPipe;
	CreatePipe(&rPipe, &wPipe, &secattr, 0);
	STARTUPINFO sInfo;
	ZeroMemory(&sInfo, sizeof(sInfo));
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&pInfo, sizeof(pInfo));
	sInfo.cb = sizeof(sInfo);
	sInfo.dwFlags = STARTF_USESTDHANDLES;
	sInfo.hStdInput = NULL;
	sInfo.hStdOutput = wPipe;
	sInfo.hStdError = wPipe;
	std::string cmd = this->_path + " " + fs::current_path().string() + context.getRequest().getURI();
	if (CreateProcess(0, const_cast<LPSTR>(cmd.c_str()), 0, 0, TRUE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, 0, 0, &sInfo, &pInfo)) {
		WaitForSingleObject(pInfo.hProcess, INFINITE);
		CloseHandle(pInfo.hProcess);
		CloseHandle(pInfo.hThread);
	}
	char buf[10001];
	DWORD redword;
	ReadFile(rPipe, buf, 10000, &redword, 0);
	buf[redword] = '\0';
	res += buf;
	CloseHandle(rPipe);
	CloseHandle(wPipe);
#endif
}

void Zia::Module::PHP::_setHeader(oZ::Context &context, std::string &res, std::size_t &i) noexcept
{
	std::vector<std::string> tokens = splitString(res, '\n');
	bool header = true;
	while (header) {
		std::vector<std::string> head = splitString(tokens[i], ':');
		if (head.size() != 2) {
			header = false;
			i++;
			continue;
		}
		if (head[1][0] == ' ')
			head[1] = head[1].substr(1, head[1].size());
		context.getResponse().getHeader().set(head[0], head[1]);
		i++;
	}
}

bool Zia::Module::PHP::_executeCGI(const std::string &path, oZ::Context &context)
{
	std::string res;
#ifdef unix
	this->_unixProcess(context, res);
#elif _WIN32
	this->_winProcess(context, res);
#endif
	std::size_t i = 0;
	this->_setHeader(context, res, i);

	std::vector<std::string> tokens = splitString(res, '\n', i);
	context.getResponse().getHeader().set("Content-Length", std::to_string(tokens[i].length()));
	context.getResponse().getBody() += tokens[i];
	context.getResponse().setCode(oZ::HTTP::Code::OK);
	return false;
}

bool Zia::Module::PHP::onInterpret(oZ::Context &context)
{
	std::string currentPath = fs::current_path().string();
	std::string path = currentPath + context.getRequest().getURI();

	std::string extention = fs::path(path).extension().string();
	if (extention != ".php")
		return true;

	if (this->_executeCGI(path, context)) {
		context.setState(oZ::State::Error);
		return true;
	}
	return false;
}

const char **Zia::Module::PHP::_buildEnv(oZ::Context &context)
{
	std::map<std::string, std::string> env;

	env["DOCUMENT_ROOT"] = fs::current_path().string();
	env["AUTH_TYPE"] = "";
	env["CONTENT_LENGTH"] = std::to_string(context.getRequest().getBody().length());
	env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["HTTP_ACCEPT"] = context.getRequest().getHeader().get("Accept");
	env["HTTP_USER_AGENT"] = context.getRequest().getHeader().get("User-Agent");
	env["PATH_INFO"] = context.getRequest().getURI().substr(context.getRequest().getURI().rfind('/'), context.getRequest().getURI().length());
	env["PATH_TRANSLATED"] = env["DOCUMENT_ROOT"] + context.getRequest().getURI();
	std::string queryString = context.getRequest().getURI() + "?";
	for (auto it = context.getRequest().getQueryParameters().begin(); it != context.getRequest().getQueryParameters().end();) {
		queryString.append(it->first);
		queryString.append("=");
		queryString.append(it->second);
		it++;
		if (it == context.getRequest().getQueryParameters().end())
			continue;
		queryString.append("&");
	}
	env["QUERY_STRING"] = queryString;
	env["REMOTE_ADDR"] = context.getPacket().getEndpoint().getAddress();
	env["REMOTE_HOST"] = context.getPacket().getEndpoint().getAddress();
	env["REMOTE_IDENT"] = context.getPacket().getEndpoint().getAddress();
	env["REMOTE_USER"] = context.getPacket().getEndpoint().getAddress();
	env["REQUEST_METHOD"] = oZ::HTTP::MethodName(context.getRequest().getMethod());
	env["SCRIPT_NAME"] = context.getRequest().getURI();
	env["SCRIPT_FILENAME"] = env["DOCUMENT_ROOT"] + context.getRequest().getURI();
	env["SERVER_NAME"] = "Zia";
	env["SERVER_PORT"] = "8080"; // Need to set port in fakeContent
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "C++17";
	env["REDIRECT_STATUS"] = "true";

	size_t i = 0;
	const char **e = new const char *[env.size() + 1];
	for (auto it = env.begin(); it != env.end(); ++it) {
		std::string s = it->first + "=" + it->second;
		int size = s.size();
		char *buff = new char[size + 1];
		s.copy(buff, size);
		buff[size] = '\0';
		e[i++] = buff;
		// std::cout << buff << std::endl;
	}
	e[i] = nullptr;
	return e;
}

OPEN_ZIA_MAKE_ENTRY_POINT(Zia::Module::PHP);