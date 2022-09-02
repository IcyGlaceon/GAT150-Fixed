#pragma once
#include <string>

namespace cool{
	class Resource{
	public:
		virtual bool Create(std::string name, ...) = 0;
	};
}