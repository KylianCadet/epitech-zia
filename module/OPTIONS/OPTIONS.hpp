/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** OPTIONS
*/

#pragma once

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

namespace Zia {
	namespace Module {
		class OPTIONS : public oZ::IModule {
		public:
			OPTIONS() = default;
			virtual ~OPTIONS() = default;

		public:
			virtual const char *getName(void) const;
			virtual void onRegisterCallbacks(oZ::Pipeline &pipeline);

		private:
			bool onInterpret(oZ::Context &context);
		};
	};
};