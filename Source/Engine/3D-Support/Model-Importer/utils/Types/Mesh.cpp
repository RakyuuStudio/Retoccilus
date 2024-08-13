#include "Mesh.h"
#include <stdexcept>

namespace Retoccilus::Engine::Support3D::Utility {
    void RtMesh::CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties, VkBuffer *buffer,
                              VkDeviceMemory *bufferMemory, VkDeviceSize size) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usageFlags;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_vulkanDevice.GetLogicalDevice(), &bufferInfo,
                           nullptr, buffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create buffer");
        }

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(m_vulkanDevice.GetLogicalDevice(), *buffer,
                                      &memoryRequirements);

        VkMemoryAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_vulkanDevice.GetLogicalDevice(), &allocateInfo,
                             nullptr, bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate memory for buffer");
        }

        vkBindBufferMemory(m_vulkanDevice.GetLogicalDevice(), *buffer, *bufferMemory, 0);
    }

    uint32_t RtMesh::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(m_vulkanDevice.GetPhysicalDevice(),
                                            &memoryProperties);

        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
            if ((typeFilter & (i << i))
            && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type");
    }

    void RtMesh::UploadVertexDataToVulkan() {
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;

        CreateBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     &vertexBuffer, &vertexBufferMemory, m_Vertices.size() * sizeof(RtVertex));

        void* data;
        vkMapMemory(m_vulkanDevice.GetLogicalDevice(), vertexBufferMemory,
                    0, m_Vertices.size() * sizeof(RtVertex), 0, &data);
        memcpy(data, m_Vertices.data(), m_Vertices.size() * sizeof(RtVertex));
        vkUnmapMemory(m_vulkanDevice.GetLogicalDevice(), vertexBufferMemory);

        m_VertexBuffer = vertexBuffer;
        m_VertexBufferMemory = vertexBufferMemory;
    }

    void RtMesh::UploadIndexDataToVulkan() {
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        CreateBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     &indexBuffer, &indexBufferMemory, m_Indices.size() * sizeof(uint32_t));

        void* data;
        vkMapMemory(m_vulkanDevice.GetLogicalDevice(), indexBufferMemory,
                    0, m_Indices.size() * sizeof(uint32_t), 0, &data);
        memcpy(data, m_Indices.data(), m_Indices.size() * sizeof(uint32_t));
        vkUnmapMemory(m_vulkanDevice.GetLogicalDevice(), indexBufferMemory);

        m_IndexBuffer = indexBuffer;
        m_IndexBufferMemory = indexBufferMemory;
    }
}