#include "Precompiled.h"

#include "Graphic/GO/GraphicsObjectPool.h"
#include "Engine/Entity.h"
#include "Engine/Transform.h"
#include "Graphic/GraphicsAPI.h"
#include "Graphic/GraphicsConstants.h"

namespace Framework
{
    GraphicsObjectPool::GraphicsObjectPool()
        : m_GO2D()
        , m_LastGO2DMaterial(nullptr)
        , m_Dirty(false)
    {
    }

    void GraphicsObjectPool::Render(const glm::mat4& viewProjectionMatrix)
    {
        // TODO: Enable gl depth buffer in OpenGLGraphicsAPI.cpp if using 3D objects

        /*for (auto& [material, goGroup] : m_GO3D)
        {
            m_VBO.Register(material->GetGraphicsTechnique().GetAllInputs());

            for (u32 goIdx = 0; goIdx < goGroup.size(); ++goIdx)
            {
                std::unique_ptr<GraphicsObject>& go = goGroup[goIdx];
                go->UpdateToBuffers(m_VBO, m_IBO);
            }

            UpdateToGPU(material, viewProjectionMatrix); 
        }*/

        if (!m_GO2D.empty())
        {
            if (m_Dirty)
            {
                std::sort(m_GO2D.begin(), m_GO2D.end(), [](const auto& lhs, const auto& rhs) 
                { 
                    if (lhs->renderLayer == rhs->renderLayer)
                    {
                        ASSERT(lhs->material && rhs->material);
                        return lhs->material->GetDefaultTextureID() < rhs->material->GetDefaultTextureID();
                    }
                    return lhs->renderLayer < rhs->renderLayer; 
                });
                m_Dirty = false;
            }
            
            m_LastGO2DMaterial = m_GO2D[0]->material;
            m_VBO.Register(m_LastGO2DMaterial->GetGraphicsTechnique());

            for (auto& go : m_GO2D)
            {
                if (m_VBO.GetVertexCount() > k_MaxVerticesPerBatch || (m_LastGO2DMaterial && *m_LastGO2DMaterial != *go->material))
                {
                    UpdateToGPU(m_LastGO2DMaterial, viewProjectionMatrix);
                    m_VBO.Register(go->material->GetGraphicsTechnique());
                }

                go->UpdateToBuffers(m_VBO, m_IBO);

                m_LastGO2DMaterial = go->material; 
            }

            UpdateToGPU(m_LastGO2DMaterial, viewProjectionMatrix);
        }
    }

    void GraphicsObjectPool::UpdateToGPU(const std::shared_ptr<Material>& material, const glm::mat4& viewProjectionMatrix) 
    { 
        if (m_VBO.IsEmpty() || m_IBO.IsEmpty() || !material) 
            return; 
 
        m_VBO.UpdateToGPU(); // Send the vertices 
        m_IBO.UpdateToGPU(); // Send the indices 
        
        material->SetShaderConstant("viewProjection", viewProjectionMatrix); 
        material->UpdateToGPU(); 
 
        GraphicsAPI::DrawElements(PrimitiveType::TriangleList, m_IBO.GetIndexCount(), m_IBO.GetIndexTypeSize()); 
 
        m_VBO.Clear(); 
        m_IBO.Clear(); 
    } 
}