/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** PUT
*/

#pragma once

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

namespace Zia {
	namespace Module {
		class PUT : public oZ::IModule {
		public:
			PUT() = default;
			virtual ~PUT() = default;

		public:
			virtual const char *getName(void) const;
			virtual void onRegisterCallbacks(oZ::Pipeline &pipeline);

		private:
			bool onInterpret(oZ::Context &context);

			bool _modifyFile(oZ::Context &context, const std::string &path) const noexcept;
			bool _createFile(oZ::Context &context, const std::string &path) const noexcept;
		};
	};
};