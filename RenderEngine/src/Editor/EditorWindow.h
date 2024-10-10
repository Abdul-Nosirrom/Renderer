#pragma once
#include <string>

#include "Core.h"

class RENDERENGINE_API EditorWindow
{
    friend class Editor;

public:
    virtual void Init() {}
    void RenderPrimary();
    virtual void OnRender() = 0;
    virtual void RenderHelp() {}

    void SetTitle(const std::string& title) { m_Title = title; }

    void SetIsVisible(bool bNewVisibility) { bVisibility = bNewVisibility; }
    bool IsVisible() const { return bVisibility; }

protected:
    bool bVisibility;
    std::string m_Title;
};



