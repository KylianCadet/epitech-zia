/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** FillByteArray
*/

#pragma once

#include <openZia/Endpoint.hpp>
#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

namespace Zia {
	namespace Module {
		class FillByteArray : public oZ::IModule {
		public:
			FillByteArray() = default;
			~FillByteArray() = default;

		public:
			virtual const char *getName(void) const;
			virtual void onRegisterCallbacks(oZ::Pipeline &pipeline);

		private:
			bool onInterpret(oZ::Context &context);
			oZ::ByteArray _generateByteArray(std::int8_t const *buffer, int const &n) const noexcept;
		};
	};
};
