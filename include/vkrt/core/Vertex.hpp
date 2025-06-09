#ifndef VKRT_VERTEX_HPP
#define VKRT_VERTEX_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace vkrt {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec4 tangent;

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal && uv == other.uv && tangent == other.tangent;
    }
};
} // namespace vkrt

namespace std {

template <typename T>
inline void hash_combine(std::size_t& seed, const T& val) {
    seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<>
struct hash<vkrt::Vertex> {
    size_t operator()(const vkrt::Vertex& vertex) const {
        size_t seed = 0;
        hash_combine(seed, std::hash<glm::vec3>()(vertex.position));
        hash_combine(seed, std::hash<glm::vec3>()(vertex.normal));
        hash_combine(seed, std::hash<glm::vec2>()(vertex.uv));
        hash_combine(seed, std::hash<glm::vec4>()(vertex.tangent));
        return seed;
    }
};

} // namespace std

#endif // VKRT_VERTEX_HPP