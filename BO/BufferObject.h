#pragma once

namespace Framework
{
    enum class BufferDataMode
    {
        Static,
        Dynamic
    };

    class BufferObject
    {
        friend class BufferObjectPool;
        friend class GraphicsManager;

    public:
        BufferObject(BufferDataMode dataMode = BufferDataMode::Static);
        virtual ~BufferObject();

        virtual void UpdateToGPU() {}   // <GraphicsAPI specific>
        
        void SetDataMode(BufferDataMode dataMode) { m_DataMode = dataMode; }
        BufferDataMode GetDataMode() const { return m_DataMode; }

        u32 GetID() const { return m_ID; }
        bool IsValid() const { return m_ID != 0; }

    private:
        virtual void Initialize() {}    // <GraphicsAPI specific>
        virtual void DeInitialize() {}  // <GraphicsAPI specific>

    protected:
        u32 m_ID;
        BufferDataMode m_DataMode;
    };
}