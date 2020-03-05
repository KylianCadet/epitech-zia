/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** parseHeader
*/

#pragma once

#include <iostream>
#include <map>
#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

std::unordered_map<std::string, oZ::HTTP::Method> methodMap = {
	{"GET", oZ::HTTP::Method::Get},
	{"OPTIONS", oZ::HTTP::Method::Option},
	{"HEAD", oZ::HTTP::Method::Head},
	{"POST", oZ::HTTP::Method::Post},
	{"PUT", oZ::HTTP::Method::Put},
	{"DELETE", oZ::HTTP::Method::Delete},
	{"TRACE", oZ::HTTP::Method::Trace},
	{"CONNECT", oZ::HTTP::Method::Connect},
};

namespace Zia {
	namespace Module {
		class parseHeader : public oZ::IModule {
		public:
			parseHeader() = default;
			virtual ~parseHeader() = default;

		public:
			virtual const char *getName(void) const;
			virtual void onRegisterCallbacks(oZ::Pipeline &pipeline);

		private:
			void _initByteString(oZ::ByteArray &byteArray) noexcept;
			void _initHeader(oZ::HTTP::Header &header, oZ::HTTP::Body &body) noexcept;
			void _initRequest(oZ::HTTP::Request &request, oZ::Context &context) noexcept;
			void _initQuery(std::string &requestString, oZ::HTTP::Request &request) const noexcept;

			bool onInterpret(oZ::Context &context);

		protected:
		private:
			std::string _byteString;
		};
	};
};