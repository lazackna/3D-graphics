#pragma once
#include <vector>
#include <string>
#include <glm/vec3.hpp>
static std::vector<std::string> split(std::string str, const std::string& seperator)
{
	std::vector<std::string> ret;
	size_t index;
	while (true)
	{
		index = str.find(seperator);
		if (index == std::string::npos)
			break;
		ret.push_back(str.substr(0, index));
		str = str.substr(index + 1);
	}
	ret.push_back(str);
	return ret;
}

static glm::vec3 stringToVec3(std::string vectorString) {
	std::vector<std::string> values = split(vectorString, ",");
	return glm::vec3(std::stof(values[0]), std::stof(values[1]), std::stof(values[2]));
}