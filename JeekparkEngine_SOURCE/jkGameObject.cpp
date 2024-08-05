#include "jkGameObject.h"

#include "jkInput.h"
#include "jkTime.h"

namespace jk
{
    GameObject::GameObject()
    {
    }

    GameObject::~GameObject()
    {
        for (Component* comp : mComponents)
        {
            delete comp;
            comp = nullptr;
        }
    }

    void GameObject::Initialize()
    {
        for (Component* comp : mComponents)
        {
            comp->Initialize();
        }
    }

    void GameObject::Update()
    {
        for (Component* comp : mComponents)
        {
            comp->Update();
        }
    }

    void GameObject::LateUpdate()
    {
        for (Component* comp : mComponents)
        {
            comp->LateUpdate();
        }
    }

    void GameObject::Render(HDC hdc)
    {
        for (Component* comp : mComponents)
        {
            comp->Render(hdc);
        }
    }
}