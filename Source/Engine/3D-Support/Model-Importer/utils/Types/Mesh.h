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
        /**
         * @brief GetLogicalDevice. Get the Logical Device for Vulkan.
         * @return m_Device (Type: VkDevice, the logical device)
         */
        [[nodiscard]] VkDevice GetLogicalDevice() const {
            return m_Device;
        }

        /**
         * @brief GetPhysicalDevice. Get the Physical Device for Vulkan.
         * @return m_PhysicalDevice (Type: VkPhysicalDevice, the physical device)
         */
        [[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const {
            return m_PhysicalDevice;
        }

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
        /**
         * @brief Constructor. Initialize the m_Device.
         * @param device - The VulkanDevice instance.
         */
        explicit RtMesh(VulkanDevice &device) : m_Device(device) {}

        /**
         * @brief addVertex. Add the vertex to the m_Vertices.
         * @param vertex - The Vertex Instance.
         */
        void addVertex(const RtVertex &vertex) {
            m_Vertices.push_back(vertex);
        }

        /**
         * @brief addIndex. Add the index to the m_Indices.
         * @param index - The index (unsigned long int) instance of model.
         */
        void addIndex(uint32_t index) {
            m_Indices.push_back(index);
        }

        /**
         * @brief Clear all the instance of Vertices and Indices.
         */
        void clear() {
            m_Vertices.clear();
            m_Indices.clear();
        }

        /**
         * @brief UploadVertexDataToVulkan. Upload the vertex data to Vulkan.
         *
         * Create the Buffer of Vulkan, make VOF and other buffers to Vulkan Device.
         */
        void UploadVertexDataToVulkan() {
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;

            CreateBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         &vertexBuffer, &vertexBufferMemory,
                         m_Vertices.size() * sizeof(RtVertex));

            void* data;
            vkMapMemory(m_Device.GetLogicalDevice(), vertexBufferMemory, 0, m_Vertices.size() * sizeof(RtVertex), 0, &data);
            memcpy(data, m_Vertices.data(), m_Vertices.size() * sizeof(RtVertex));
            vkUnmapMemory(m_Device.GetLogicalDevice(), vertexBufferMemory);

            m_VertexBuffer = vertexBuffer;
            m_VertexBufferMemory = vertexBufferMemory;
        }

        /**
         * @brief UploadIndexDataToVulkan. Upload the index data to Vulkan.
         *
         * Upload the Buffer of Vulkan, make VOF and other buffers to Vulkan Device.
         */
        void UploadIndexDataToVulkan() {
            VkBuffer indexBuffer;
            VkDeviceMemory indexBufferMemory;

            CreateBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         &indexBuffer, &indexBufferMemory, m_Indices.size() * sizeof(uint32_t));

            void* data;
            vkMapMemory(m_Device.GetLogicalDevice(), indexBufferMemory, 0, m_Indices.size() * sizeof(uint32_t), 0, &data);
            memcpy(data, m_Indices.data(), m_Indices.size() * sizeof(uint32_t));
            vkUnmapMemory(m_Device.GetLogicalDevice(), indexBufferMemory);

            m_IndexBuffer = indexBuffer;
            m_IndexBufferMemory = indexBufferMemory;
        }

        /**
         * @brief Getter.
         * @return m_Vertices
         */
        [[nodiscard]] const std::vector<RtVertex> &GetVertices() const {
            return m_Vertices;
        }

        /**
         * @brief Getter.
         * @return m_Indices.
         */
        [[nodiscard]] const std::vector<uint32_t> &GetIndices() const {
            return m_Indices;
        }

        /**
         * @brief Getter.
         * @return m_VertexBuffer
         */
        [[nodiscard]] VkBuffer GetVertexBuffer() const {
            return m_VertexBuffer;
        }

        /**
         * @brief Getter.
         * @return m_VertexBufferMemory
         */
        [[nodiscard]] VkDeviceMemory GetVertexBufferMemory() const {
            return m_VertexBufferMemory;
        }

        /**
         * @brief Getter.
         * @return m_IndexBuffer
         */
        [[nodiscard]] VkBuffer GetIndexBuffer() const {
            return m_IndexBuffer;
        }

        /**
         * @brief Getter.
         * @return m_IndexBufferMemory
         */
        [[nodiscard]] VkDeviceMemory GetIndexBufferMemory() const {
            return m_IndexBufferMemory;
        }

    private:
        /**
         * @brief CreateBuffer. Create the Buffer of Vulkan.
         *
         * Create the buffer of Vulkan.
         * @param usageFlags
         * @param properties
         * @param buffer
         * @param bufferMemory
         * @param size
         *
         * @throw Vertex: Failed to create vertex buffer.
         */
        void CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties,
                          VkBuffer* buffer, VkDeviceMemory* bufferMemory, VkDeviceSize size) {
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usageFlags;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(m_Device.GetLogicalDevice(), &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
//                throw std::runtime_error("Failed to create vertex buffer!");
                qFatal("Fatal Error Occurred: Failed to create vertex buffer, "
                       "application will exit now. If we continue to create the buffer, might cause crashed.");
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(m_Device.GetLogicalDevice(), *buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(m_Device.GetLogicalDevice(), &allocInfo, nullptr, bufferMemory) != VK_SUCCESS) {
                qFatal("Fatal Error Occurred: Failed to allocating the memory for Vulkan Device,"
                       "Please check the physical memory is full or the system occurred the memory exchange."
                );
            }

            vkBindBufferMemory(m_Device.GetLogicalDevice(), *buffer, *bufferMemory, 0);
        }

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
            VkPhysicalDeviceMemoryProperties memProperties;
            vkGetPhysicalDeviceMemoryProperties(m_Device.GetPhysicalDevice(), &memProperties);

            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }

            qCritical("Failed to find suitable memory type for Vulkan Physical Device");
        }


        VulkanDevice& m_Device;
        std::vector<RtVertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        VkBuffer m_VertexBuffer;
        VkDeviceMemory m_VertexBufferMemory;
        VkBuffer m_IndexBuffer;
        VkDeviceMemory m_IndexBufferMemory;
    };
}

#endif // MODEL_IMPORTER_UTILS_MESH_TYPE