#pragma once
#include "Core.h"
#include "Keyboard.h"
#include "Mouse.h"

class RENDERENGINE_API InputHandler
{
    friend class Window;
public:
    static InputHandler& Get();
    static Mouse& GetMouse() { return Get().m_Mouse; }
    static Keyboard& GetKeyboard() { return Get().m_Keyboard; }

    InputHandler(InputHandler const&) = delete;
    InputHandler& operator=(InputHandler const&) = delete;

private:
    InputHandler() = default;
    ~InputHandler() = default;

    Keyboard m_Keyboard;
    Mouse m_Mouse;

};



