#pragma once

namespace oengine2d {
	template <typename T>
	class Singleton {
	public:
		static T& GetInstance() {
			static T s_instance;
			return s_instance;
		}
	};
}