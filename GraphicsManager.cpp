/* 
 * Copyright (C) Ubisoft Entertainment - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Precompiled.h"

#include <memory>
#include <algorithm>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Graphic/GraphicsManager.h"
#include "Graphic/GraphicsAPI.h"

#include "Engine/Transform.h"
#include "Engine/Editor.h"
#include "Engine/Engine.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/VisualComponent.h"
#include "Engine/Components/SpriteVisualComponent.h"
#include "Engine/Components/TextFieldComponent.h"


namespace Framework
{
    void GraphicsManager::Initialize(const Config& config)
    {
        m_Window.Initialize(config);

        GraphicsAPI::Initialize(config, m_Window);

        const s32 ttfResult = TTF_Init();
        ASSERT_OR_ALERT(ttfResult != -1, "TTF_Init() failed: %s", SDL_GetError());

        DEBUG_EXP( ubi::wout.Load(config, m_Window); )

        DEBUG_EXP( Editor::Initialize(m_Window); )

        DEBUG_EXP( m_DebugRenderer.Initialize(config); )

        m_Window.Show();

        DEBUG_EXP( SDL_ShowCursor(1); )
    }

    void GraphicsManager::DeInitialize()
    {
        m_BufferObjectPool.Release();

        DEBUG_EXP( m_DebugRenderer.DeInitialize(); )

        m_VisualComponents.clear();

        DEBUG_EXP( Editor::DeInitialize() );
        DEBUG_EXP( ubi::wout.Unload() );

        GraphicsAPI::ClearRenderBuffer();
        GraphicsAPI::DeInitialize();

        TTF_Quit();

        m_Window.DeInitialize();
    }

    void GraphicsManager::AddVisualComponent(std::shared_ptr<VisualComponent> visual)
    {
        if (const std::shared_ptr<SpriteVisualComponent> spriteComponent = std::dynamic_pointer_cast<SpriteVisualComponent>(visual))
        {
            if (visual->GetIsVisible())
            {
                m_GraphicsObjectPool.AddGO(spriteComponent->m_GO);
            }   
        }
        else if (const std::shared_ptr<TextFieldComponent> textComponent = std::dynamic_pointer_cast<TextFieldComponent>(visual))
        {
            m_GraphicsObjectPool.AddGO(textComponent->m_GO);
        }

        m_VisualComponents.push_back(std::move(visual));
    }

    void GraphicsManager::RemoveVisualComponent(const std::shared_ptr<VisualComponent>& visual)
    {
        if (const std::shared_ptr<SpriteVisualComponent> spriteComponent = std::dynamic_pointer_cast<SpriteVisualComponent>(visual))
        {
            m_GraphicsObjectPool.RemoveGO(spriteComponent->m_GO);
        }
        else if (const std::shared_ptr<TextFieldComponent> textComponent = std::dynamic_pointer_cast<TextFieldComponent>(visual))
        {
            m_GraphicsObjectPool.RemoveGO(textComponent->m_GO);
        }

        const auto it = std::remove_if( m_VisualComponents.begin(), m_VisualComponents.end(), [visual](const auto& item) { return item == visual; } );
        if (it != m_VisualComponents.end())
        {
            m_VisualComponents.erase(it);
        }
    }

    glm::vec4 GraphicsManager::GetScreenToWorldPosition(glm::vec2 screenPos) const
    {
        screenPos.x = screenPos.x * 2.0f - 1.0f;
        screenPos.y = screenPos.y * -2.0f + 1.0f;

        return glm::inverse(m_CurrentCamera->GetViewProjectionMatrix()) * glm::vec4(screenPos.x, screenPos.y, 0.0f, 1.0f);
    }

    void GraphicsManager::Update(double deltaTime)
    {
        m_BufferObjectPool.Update();

        for (const auto& visual : m_VisualComponents)
        {
            visual->Update(deltaTime);
        }
    }

    void GraphicsManager::Render()
    {
        if (m_CurrentCamera)
        {
            GraphicsAPI::ClearRenderBuffer();

            const glm::mat4& viewProjectionMatrix = m_CurrentCamera->GetViewProjectionMatrix();

            m_GraphicsObjectPool.Render(viewProjectionMatrix);

            DEBUG_EXP( m_DebugRenderer.Render(viewProjectionMatrix); )

            ubi::wout.Flush(viewProjectionMatrix); // Render stream output buffer to SDL window

            DEBUG_EXP( Editor::Render() );

            m_Window.SwapBuffers();
        }
    }

    void GraphicsManager::ResizeWindow(u32 windowWidth, u32 windowHeight)
    {
        m_Window.Resize(windowWidth, windowHeight, false);
        GraphicsAPI::OnWindowResized(windowWidth, windowHeight);

        if (m_CurrentCamera)
            m_CurrentCamera->SetDirty();

        for (const auto& visual : m_VisualComponents)
            visual->OnWindowResized();
    }

    void GraphicsManager::HandleWindowEvent(const SDL_Event& windowEvent)
    {
        switch (windowEvent.window.event)
        {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            {
                ResizeWindow( windowEvent.window.data1, windowEvent.window.data2 );
                break;
            }
            case SDL_WINDOWEVENT_CLOSE:
            {
                SDL_Event event = windowEvent;
                event.type = SDL_QUIT;
                SDL_PushEvent( &event );
                break;
            }
            default:;
        }
    }
    
};
