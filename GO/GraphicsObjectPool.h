#pragma once

#include "Graphic/GO/GraphicsObject.h"
#include "Graphic/BO/IndexBufferObject.h"
#include "Graphic/BO/VertexBufferObject.h"
#include "Graphic/Content/Material.h"

namespace Framework
{
    struct SpriteGraphicsObject;
    struct TextGraphicsObject;

    class GraphicsObjectPool
    {
        NON_COPYABLE_CLASS(GraphicsObjectPool);
        friend class GraphicsManager;

    public:
        template <class T>
        void AddGO(std::shared_ptr<T> go);
        
        template <class T>
        void RemoveGO(const std::shared_ptr<T>& go);

    private:
        GraphicsObjectPool();

        void Render(const glm::mat4& viewProjectionMatrix);
        void UpdateToGPU(const std::shared_ptr<Material>& material, const glm::mat4& viewProjectionMatrix);

    private:
        static constexpr u32 k_MaxVerticesPerBatch = 4096u;

        std::vector<std::shared_ptr<GraphicsObject2D>> m_GO2D;
        std::shared_ptr<Material> m_LastGO2DMaterial;

        //std::unordered_map<std::shared_ptr<Material>, std::vector<std::unique_ptr<GraphicsObject>>> m_GO3D;

        IndexBufferObject<u16> m_IBO;
        VertexBufferObject m_VBO;

        bool m_Dirty;
    };

    template <class T>
    void GraphicsObjectPool::AddGO(std::shared_ptr<T> go)
    {
        if constexpr (IS_ANY_OF(T, SpriteGraphicsObject, TextGraphicsObject))
        {
            m_Dirty = true;
            m_GO2D.emplace_back(std::move(go));
        }
    }

    template <class T>
    void GraphicsObjectPool::RemoveGO(const std::shared_ptr<T>& go)
    {
        if constexpr (IS_ANY_OF(T, SpriteGraphicsObject, TextGraphicsObject))
        {
            const auto it = std::remove(m_GO2D.begin(), m_GO2D.end(), go);
            if (it != m_GO2D.end())
            {
                m_GO2D.erase(it);
                m_Dirty = true;
            }
        }
    }
 }
