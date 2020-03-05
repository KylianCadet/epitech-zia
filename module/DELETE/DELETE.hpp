/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** DELETE
*/

#pragma once

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

namespace Zia {
	namespace Module {
		class _DELETE : public oZ::IModule {
		public:
			_DELETE() = default;
			virtual ~_DELETE() = default;

		public:
			virtual const char *getName(void) const;
			virtual void onRegisterCallbacks(oZ::Pipeline &pipeline);

		private:
			bool onInterpret(oZ::Context &context);
		};
	};
};