#pragma once

#include <string>
#include <vector>

namespace Engine
{
	class Engine_Pipeline
	{
		public:
			Engine_Pipeline(const std::string& vertFilePath, const std::string& fragFilePath);
			~Engine_Pipeline();

		private:
			static std::vector<char> readFile(const std::string& filePath);

			void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
	};
}

