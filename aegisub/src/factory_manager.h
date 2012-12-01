// Copyright (c) 2010, Thomas Goyne <plorkyeran@aegisub.org>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Aegisub Group nor the names of its contributors
//     may be used to endorse or promote products derived from this software
//     without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Aegisub Project http://www.aegisub.org/

/// @file factory_manager.h
/// @brief Template/base-class for factory classes
/// @ingroup utility
///

#pragma once

#include <algorithm>
#include <cctype>
#include <map>
#include <vector>

#include <wx/string.h>

template <class func>
class FactoryBase {
protected:
	typedef std::map<std::string, std::pair<bool, func> > map;
	typedef typename map::iterator iterator;

	static map *classes;

	static void DoRegister(func function, std::string name, bool hide, std::vector<std::string> &subtypes) {
		if (!classes) classes = new map;

		if (subtypes.empty()) {
			classes->insert(std::make_pair(name, std::make_pair(hide, function)));
		}
		else {
			for (auto const& subtype : subtypes)
				classes->insert(std::make_pair(name + '/' + subtype, std::make_pair(hide, function)));
		}
	}

	static func Find(std::string name) {
		if (!classes) return nullptr;

		iterator factory = classes->find(name);
		if (factory != classes->end()) return factory->second.second;
		return nullptr;
	}

public:
	static void Clear() {
		delete classes;
	}
	static std::vector<std::string> GetClasses(std::string favourite="") {
		std::vector<std::string> list;
		if (!classes) return list;
		std::string cmp;
		std::transform(favourite.begin(), favourite.end(), favourite.begin(), ::tolower);
		for (auto const& cls : *classes) {
			cmp.clear();
			std::transform(cls.first.begin(), cls.first.end(), std::back_inserter(cmp), ::tolower);
			if (cmp == favourite) list.insert(list.begin(), cls.first);
			else if (!cls.second.first) list.push_back(cls.first);
		}
		return list;
	}
	virtual ~FactoryBase() {
		delete classes;
	}
};

template<class Base>
class Factory0 : public FactoryBase<Base *(*)()> {
	typedef Base *(*func)();
	template<class T>
	static Base* create() {
		return new T;
	}
public:
	static Base* Create(std::string name) {
		func factory = FactoryBase<func>::Find(name);
		if (factory) {
			return factory();
		}
		else {
			return nullptr;
		}
	}

	template<class T>
	static void Register(std::string name, bool hide = false, std::vector<std::string> subTypes = std::vector<std::string>()) {
		FactoryBase<func>::DoRegister(&Factory0<Base>::template create<T>, name, hide, subTypes);
	}
};

template<class Base, class Arg1>
class Factory1 : public FactoryBase<Base *(*)(Arg1)> {
	typedef Base *(*func)(Arg1);
	template<class T>
	static Base* create(Arg1 a1) {
		return new T(a1);
	}
public:
	static Base* Create(std::string name, Arg1 a1) {
		func factory = FactoryBase<func>::Find(name);
		if (factory) {
			return factory(a1);
		}
		else {
			return nullptr;
		}
	}

	template<class T>
	static void Register(std::string name, bool hide = false, std::vector<std::string> subTypes = std::vector<std::string>()) {
		FactoryBase<func>::DoRegister(&Factory1<Base, Arg1>::template create<T>, name, hide, subTypes);
	}
};
