#ifndef MODEL_IMPORTER_UTILS_MESH_TYPE
#define MODEL_IMPORTER_UTILS_MESH_TYPE

#include <vulkan/vulkan.h>
#include "Vertex.h"

namespace Retoccilus::Engine::Support3D::Utility {
    /**
     * @brief VulkanDevice class. A helper class to get the Physical Device
     * and Logical Device for Vulkan. We use `[[nodiscard]]` for these methods
     * because we don't want to discard the returned value of these methods.
     *
     * @since 0.0.1[Pre-Version]
     * @author Almond Ryan.
     */
    class VulkanDevice {
    public:
        VkDevice GetLogicalDevice() const { return m_Device; }
        VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }

    private:
        VkDevice m_Device;
        VkPhysicalDevice m_PhysicalDevice;
    };

    /**
     * @brief RtMesh class. A helper class to store and get the Mesh Data for Retoccilus.
     * @throw Device_and_Memory No enough memory | No physical device | No suitable memory.
     */
    class RtMesh {
    public:
        explicit RtMesh(VulkanDevice &device) : m_vulkanDevice(device) {}

        void addVertex(const RtVertex &vertex) { m_Vertices.push_back(vertex); }
        void addIndex(uint32_t index) { m_Indices.push_back(index); }
        void clear() { m_Vertices.clear(); m_Indices.clear(); }

        void UploadVertexDataToVulkan();
        void UploadIndexDataToVulkan();

        const std::vector<RtVertex>& GetVertices() const { return m_Vertices; }
        const std::vector<uint32_t>& GetIndices() const { return m_Indices; }

        VkBuffer GetVertexBuffer() const { return m_VertexBuffer; }
        VkDeviceMemory GetVertexBufferMemory() const { return m_VertexBufferMemory; }
        VkBuffer GetIndexBuffer() const { return m_IndexBuffer; }
        VkDeviceMemory GetIndexBufferMemory() const { return m_IndexBufferMemory; }

    private:
        void CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties,
                          VkBuffer* buffer, VkDeviceMemory* bufferMemory, VkDeviceSize size);

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        VulkanDevice& m_vulkanDevice;
        std::vector<RtVertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        VkBuffer m_VertexBuffer;
        VkDeviceMemory m_VertexBufferMemory;
        VkBuffer m_IndexBuffer;
        VkDeviceMemory m_IndexBufferMemory;
    };
}

#endif // MODEL_IMPORTER_UTILS_MESH_TYPE