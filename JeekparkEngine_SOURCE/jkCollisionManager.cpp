#include "jkCollisionManager.h"

#include "jkScene.h"
#include "jkSceneManager.h"
#include "jkGameObject.h"
#include "jkCollider.h"
#include "jkTransform.h"
#include "jkComponent.h"

namespace jk
{
    std::bitset<(UINT)eLayerType::Max> CollisionManager::mCollisionLayerMaxtirx[(UINT)eLayerType::Max] = {};
    std::unordered_map<UINT64, bool> CollisionManager::mCollisionMap = {};
    void CollisionManager::Initialize()
    {
    }
    void CollisionManager::Update()
    {
        for (UINT row = 0; row < (UINT)eLayerType::Max; ++row)
        {
            for (UINT col = 0; col < (UINT)eLayerType::Max; ++col)
            {
                if (mCollisionLayerMaxtirx[row][col] == true)
                {
                    LayerCollision((eLayerType)row, (eLayerType)col);
                }
            }
        }
    }
    void CollisionManager::LateUpdate()
    {
    }
    void CollisionManager::Render()
    {

    }
    void CollisionManager::Clear()
    {
        mCollisionMap.clear();
        for (UINT row = 0; row < (UINT)eLayerType::Max; ++row)
        {
            for (UINT col = 0; col < (UINT)eLayerType::Max; ++col)
            {
                mCollisionLayerMaxtirx[row][col] = false;
            }
        }
    }
    void CollisionManager::CollistionLayerCheck(eLayerType left, eLayerType right, bool enable)
    {
        int row = 0;
        int col = 0;

        if (left < right)
        {
            row = (int)left;
            col = (int)right;
        }
        else
        {
            row = (int)right;
            col = (int)left;
        }
        mCollisionLayerMaxtirx[row][col] = enable;
    }
    void CollisionManager::LayerCollision(eLayerType left, eLayerType right)
    {
        const std::vector<GameObject*>& leftObjs = SceneManager::GetGameObjects(left);
        const std::vector<GameObject*>& rightObjs = SceneManager::GetGameObjects(right);

        for (GameObject* leftObj : leftObjs)
        {
            if (leftObj->IsActive() == false)
            {
                continue;
            }
            Collider* leftCollider = leftObj->GetComponent<Collider>();
            if (leftCollider == nullptr)
            {
                continue;
            }

            for (GameObject* rightObj : rightObjs)
            {
                if (rightObj->IsActive() == false)
                {
                    continue;
                }
                Collider* rightCollider = rightObj->GetComponent<Collider>();
                if (rightCollider == nullptr)
                {
                    continue;
                }
                if (leftObj == rightObj)
                {
                    continue;
                }

                ColliderCollision(leftCollider, rightCollider);
            }
        }
    }
    void CollisionManager::ColliderCollision(Collider* left, Collider* right)
    {
        CollisionID id = {};
        id.left = left->GetID();
        id.right = right->GetID();

        auto iter = mCollisionMap.find(id.id);
        if (iter == mCollisionMap.end())
        {
            mCollisionMap.insert(std::make_pair(id.id, false));
            iter = mCollisionMap.find(id.id);
        }

        if (Intersect(left, right))
        {
            if (iter->second == false)
            {
                left->OnCollisionEnter(right);
                right->OnCollisionEnter(left);
                iter->second = true;
            }
            else
            {
                left->OnCollisionStay(right);
                right->OnCollisionStay(left);
            }
        }
        else
        {
            if (iter->second == true)
            {
                left->OnCollisionExit(right);
                right->OnCollisionExit(left);
                iter->second = false;
            }
        }
        
    }
    bool CollisionManager::Intersect(Collider* left, Collider* right)
    {
        Transform* leftTransform = left->GetOwner()->GetComponent<Transform>();
        Transform* rightTransform = right->GetOwner()->GetComponent<Transform>();

        Vector2 leftPos = leftTransform->GetPosition() + left->GetOffset();
        Vector2 rightPos = rightTransform->GetPosition() + right->GetOffset();

        Vector2 leftSize = left->GetSize();
        Vector2 rightSize = right->GetSize();

        enums::eColliderType leftType = left->GetColliderType();
        enums::eColliderType rightType = right->GetColliderType();

        if (leftType == enums::eColliderType::Box2D &&
            rightType == enums::eColliderType::Box2D)
        {
            if (leftPos.x < rightPos.x + rightSize.x &&
                leftPos.x + leftSize.x > rightPos.x &&
                leftPos.y < rightPos.y + rightSize.y &&
                leftPos.y + leftSize.y > rightPos.y)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        
        if (leftType == enums::eColliderType::Circle2D &&
            rightType == enums::eColliderType::Circle2D)
        {
            Vector2 leftCirclePos = leftPos + leftSize * 0.5f;
            Vector2 rightCirclePos = rightPos + rightSize * 0.5f;

            float distance = (leftCirclePos - rightCirclePos).Length();
            if (distance < (leftSize.x + rightSize.x) * 0.5f)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        if (leftType == enums::eColliderType::Circle2D &&
                rightType == enums::eColliderType::Box2D)
        {

          
        }
        
    }
}