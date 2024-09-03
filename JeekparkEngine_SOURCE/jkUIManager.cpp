#include "jkUIManager.h"

#include "jkUIButton.h"

namespace jk
{
    std::unordered_map<eUIType, UIBase*> UIManager::mUIs = {};
    std::stack<UIBase*> UIManager::mUIBases = {};
    std::queue<eUIType> UIManager::mRequestUiQueue = {};
    UIBase* UIManager::mActiveUI = nullptr;

    void UIManager::Initialize()
    {
        UIButton* button = new UIButton();
        mUIs.insert(std::make_pair(eUIType::Button, button));
    }

    void UIManager::OnLoad(eUIType type)
    {
        auto iter = mUIs.find(type);
        if (iter == mUIs.end())
        {
            OnFail();
            return;
        }
        
        OnComplete(iter->second);
    }

    void UIManager::Update()
    {
        std::stack<UIBase*> uiBases = mUIBases;
        while (!uiBases.empty())
        {
            UIBase* uiBase = uiBases.top();
            if (uiBase)
            {
                uiBase->Update();
                uiBases.pop();
            }
        }

        if (mRequestUiQueue.size() > 0)
        {
            eUIType requestUI = mRequestUiQueue.front();
            mRequestUiQueue.pop();
            OnLoad(requestUI);
        }
    }

    void UIManager::LateUpdate()
    {
        std::stack<UIBase*> uiBases = mUIBases;
        while (!uiBases.empty())
        {
            UIBase* uiBase = uiBases.top();
            if (uiBase)
            {
                uiBase->LateUpdate();
                uiBases.pop();
            }
        }
    }

    void UIManager::Render()
    {
        if (mUIBases.size() <= 0)
        {
            return;
        }

        std::stack<UIBase*> tempStack;
        while (!mUIBases.empty())
        {
            UIBase* tempUIBase = mUIBases.top();
            tempUIBase->Render();
            tempStack.push(tempUIBase);
            mUIBases.pop();
        }
        while (!tempStack.empty())
        {
            mUIBases.push(tempStack.top());
            tempStack.pop();
        }
    }

    void UIManager::OnComplete(UIBase* addUI)
    {
        assert(addUI != nullptr);

        addUI->Initialize();
        addUI->Active();
        addUI->Update();

        if (addUI->IsFullScreen())
        {
            std::stack<UIBase*> uiBases = mUIBases;
            while (!uiBases.empty())
            {
                UIBase* uiBase = uiBases.top();
                uiBases.pop();
                if (uiBase)
                {
                    uiBase->InActive();
                }
            }
        }

        mUIBases.push(addUI);
        mActiveUI = nullptr;
    }

    void UIManager::OnFail()
    {
        mActiveUI = nullptr;
    }

    void UIManager::Release()
    {
        for (auto& iter : mUIs)
        {
            delete iter.second;
            iter.second = nullptr;
        }

    }

    void UIManager::Push(eUIType type)
    {
        mRequestUiQueue.push(type);
    }

    void UIManager::Pop(eUIType type)
    {
        if (mUIBases.size() <= 0)
        {
            return;
        }

        std::stack<UIBase*> tempStack;

        UIBase* uibase = nullptr;
        while (mUIBases.size() > 0)
        {
            uibase = mUIBases.top();
            mUIBases.pop();
            if (uibase->GetType() != type)
            {
                tempStack.push(uibase);
                continue;
            }
            
            if (uibase->IsFullScreen())
            {
                std::stack<UIBase*> uiBases = mUIBases;
                while (!uiBases.empty())
                {
                    UIBase* uiBase = uiBases.top();
                    uiBases.pop();
                    if (uiBase)
                    {
                        uiBase->Active();
                        break;
                    }
                }
            }
            uibase->UIClear();
        }
        while (tempStack.size() > 0)
        {
            uibase = tempStack.top();
            tempStack.pop();
            mUIBases.push(uibase);
        }
    }

}