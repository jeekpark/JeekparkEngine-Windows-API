#pragma once

#include "jkComponent.h"


namespace jk
{
    using namespace jk::math;
    class Camera : public Component
    {
    public:
        Camera();
        ~Camera();

        void Initialize()override;
        void Update() override;
        void LateUpdate() override;
        void Render() override;

        void SetTarget(GameObject* target) { mTarget = target; }

        Vector2 WorldToScreenPoint(Vector2 pos) const { return pos - mDistance; };
        Vector2 ScreenToWorldPoint(Vector2 pos) const { return pos + mDistance; };

    private:
        
        Vector2 mDistance;
        Vector2 mResolution;
        Vector2 mLookPosition;

        class GameObject* mTarget;
    };

}