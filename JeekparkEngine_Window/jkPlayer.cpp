#include "jkPlayer.h"

#include "jkInput.h"
#include "jkTransform.h"
#include "jkTime.h"

namespace jk
{
    void Player::Initialize()
    {
        GameObject::Initialize();
    }
    void Player::Update()
    {
        GameObject::Update();
    }
    void Player::LateUpdate()
    {
        GameObject::LateUpdate();
    }
    void Player::Render(HDC hdc)
    {
        GameObject::Render(hdc);
    }
}