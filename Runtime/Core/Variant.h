#pragma once

#include "EngineDefs.h"
#include <stdint.h>
#include <variant>



#define _VARIANT_TYPES						\
	char,									\
	unsigned char,							\
	int,									\
	uint32_t,								\
	bool,									\
	float,									\
	double,									\
	void*									



#define VARIANT_TYPES std::variant<_VARIANT_TYPES>
typedef std::variant<_VARIANT_TYPES, VARIANT_TYPES> VariantInternal;

namespace V
{
	class V_API Variant
	{
	public:
		Variant() {}
		~Variant() {}

		Variant(const Variant& var) { m_variant = var.GetVariantRaw(); }

		template <class T, class = std::enable_if<!std::is_same<T, Variant>::value>>
		Variant(T value)
		{
			m_variant = value;
		}

		Variant& operator=(const Variant& rhs);

		template<class T,class = std::enable_if<!std::is_same<T,Variant>::value>>
		Variant& operator=(T rhs) { return m_variant = rhs; }

		const VariantInternal& GetVariantRaw() const { return m_variant; }

		template<class T>
		inline const T& Get() const {
			return std::get<T>(m_variant);
		}


	private:
		VariantInternal m_variant;

	};

}


	