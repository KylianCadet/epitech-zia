/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** PHP
*/

#pragma once

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

namespace Zia {
	namespace Module {
		class PHP : public oZ::IModule {
		public:
			PHP() = default;
			virtual ~PHP() = default;

		public:
			virtual const char *getName(void) const;
			void onConnection(oZ::Context &context) override;
			virtual void onRegisterCallbacks(oZ::Pipeline &pipeline);

		private:
			bool onInterpret(oZ::Context &context);
			bool _executeCGI(const std::string &path, oZ::Context &context);
			void _getCGIbin();
			void _unixProcess(oZ::Context &context, std::string &res) noexcept;
			void _winProcess(oZ::Context &context, std::string &res) noexcept;
			void _setHeader(oZ::Context &context, std::string &res, std::size_t &i) noexcept;

			const char **_buildEnv(oZ::Context &context);

		private:
			std::string _path = "";
			bool _error = false;
		};
	};
};