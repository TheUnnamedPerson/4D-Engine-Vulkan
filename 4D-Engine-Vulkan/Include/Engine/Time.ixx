module;

#include <chrono>
#include <type_traits>
#include <iostream>
#include <string>
#include <iomanip>

export module Engine4D.Time;

namespace Engine4D
{

	export class TimeClass
	{
		public:
			double deltaTime;
			double fixedDeltaTime;

			std::chrono::steady_clock::time_point now() {
				return std::chrono::steady_clock::now();
			}

			double toSeconds(std::chrono::nanoseconds duration) {
				return duration.count() / 1000000000.0;
			}

			void printDeltaTime() {
				std::cout << "Delta Time: " << std::setprecision(16) << deltaTime << std::endl;
			}
	};
}