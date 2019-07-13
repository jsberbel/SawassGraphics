/* Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

#include <vector>

#include "Core/Utils/GenericUtils.h"
#include "Graphic/Window.h"
#include "Graphic/Util/DebugRenderer.h"
#include "Graphic/GO/GraphicsObjectPool.h"
#include "Graphic/BO/BufferObjectPool.h"

struct SDL_Surface;

namespace Framework
{
    class VisualComponent;
    class CameraComponent;
    class Surface;
    class Texture;
    namespace Debug { struct IPrimitive; }

    class GraphicsManager
    {
        NON_COPYABLE_CLASS(GraphicsManager);
        friend class Engine;

    public:
        void AddVisualComponent(std::shared_ptr<VisualComponent> visual);
        void RemoveVisualComponent(const std::shared_ptr<VisualComponent>& visual);

        const Window& GetWindow() const { return m_Window; }

        const BufferObjectPool& GetBufferObjectPool() const { return m_BufferObjectPool; }
        BufferObjectPool& GetBufferObjectPool() { return m_BufferObjectPool; }

        const GraphicsObjectPool& GetGraphicsObjectPool() const { return m_GraphicsObjectPool; }
        GraphicsObjectPool& GetGraphicsObjectPool() { return m_GraphicsObjectPool; }

        void SetCurrentCamera(std::shared_ptr<CameraComponent> camera) { ASSERT(camera); m_CurrentCamera = std::move(camera); }
        std::shared_ptr<CameraComponent> GetCurrentCamera() const { return m_CurrentCamera; }
        
        glm::vec4 GetScreenToWorldPosition(glm::vec2 screenPos) const;

    #ifdef _DEBUG
        DebugRenderer& GetDebugRenderer() { return m_DebugRenderer; }
    #endif

    private:
        GraphicsManager() = default;
        virtual ~GraphicsManager() = default;

        void Initialize(const Config& config);
        void DeInitialize();

        void Update(double deltaTime);
        void Render();

        void HandleWindowEvent(const SDL_Event& windowEvent);
        void ResizeWindow(u32 windowWidth, u32 windowHeight);

    private:
        Window                                          m_Window;
        std::shared_ptr<CameraComponent>                m_CurrentCamera;
        std::vector<std::shared_ptr<VisualComponent>>   m_VisualComponents;
        BufferObjectPool                                m_BufferObjectPool;
        GraphicsObjectPool                              m_GraphicsObjectPool;

        DEBUG_EXP( DebugRenderer m_DebugRenderer; )
    };
}