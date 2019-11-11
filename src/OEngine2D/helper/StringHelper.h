#pragma once
#include "util.h"

namespace oengine2d {
	struct StringHelper {
		static inline std::vector<std::string> Explode(const std::string& sep, const std::string& str) {
			std::vector<std::string> ret;
			std::string::size_type start = 0;
			std::string::size_type pos = str.find(sep, start);
			while (pos != std::string::npos) {
				ret.emplace_back(str.substr(start, pos - start));
				start += sep.size();

				pos = str.find(sep, start);
			}

			if (start < str.size()) {
				ret.emplace_back(str.substr(start));
			}
			return ret;
		}

		static inline std::string Implode(const std::string& sep, const std::vector<std::string>& strs, size_t start = 0) {
			std::string ret;
			if (!strs.empty() && strs.size() > start) {
				bool first = true;
				for (size_t i = start; i < strs.size(); ++i) {
					if (first)
						first = false;
					else
						ret += ".";
					ret += strs[i];
				}
			}

			return ret;
		}
	};
}