#pragma once
#include <glm/glm.hpp>
#include "json.hpp"
#include "tigl.h"

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

namespace glm {

	void to_json(nlohmann::json& j, const vec2& v) {
		j = nlohmann::json(nlohmann::json::array({ v[0],v[1] }));
	}

	void from_json(const nlohmann::json& j, vec2& v) {
		v = glm::vec2(j[0], j[1]);
	}

	void to_json(nlohmann::json& j, const vec3& v) {
		j = nlohmann::json(nlohmann::json::array({ v[0],v[1],v[2] }));
	}

	void from_json(const nlohmann::json& j, vec3& v) {
		v = glm::vec3(j[0], j[1], j[2]);
	}

	void to_json(nlohmann::json& j, const vec4& v) {
		j = nlohmann::json(nlohmann::json::array({ v[0],v[1],v[2],v[3] }));
	}

	void from_json(const nlohmann::json& j, vec4& v) {
		v = glm::vec4(j[0], j[1], j[2], j[3]);
	}
}

namespace tigl {
	void to_json(nlohmann::json& j, const Vertex& v) {
		j["position"] = v.position;
		j["normal"] = v.normal;
		j["color"] = v.color;
		j["texcoord"] = v.texcoord;
	}

	void from_json(const nlohmann::json& j, Vertex& v) {
		v.position = j["position"];
		v.normal = j["normal"];
		v.color = j["color"];
		v.texcoord = j["texcoord"];
	}
}