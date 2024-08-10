#include "jkPlayScene.h"

#include "jkGameObject.h"
#include "jkSpriteRenderer.h"
#include "jkTransform.h"
#include "jkPlayer.h"
#include "jkInput.h"
#include "jkTitleScene.h"
#include "jkSceneManager.h"
#include "jkObject.h"
#include "jkTexture.h"
#include "jkResources.h"
#include "jkPlayerScript.h"
#include "jkCamera.h"
#include "jkRenderer.h"
#include "jkAnimator.h"

namespace jk
{
    PlayScene::PlayScene()
    {
    }
    PlayScene::~PlayScene()
    {
    }
    void PlayScene::Initialize()
    {
        GameObject* cameraObj = object::Instantiate<GameObject>(
            enums::eLayerType::None,
            {347.f, 450.f}
        );
        Camera* cameraComp = cameraObj->AddComponent<Camera>();
        renderer::mainCamera = cameraComp;


        mPlayer = object::Instantiate<Player>(enums::eLayerType::Particle);
        //SpriteRenderer* sr = mPlayer->AddComponent<SpriteRenderer>();

        mPlayer->AddComponent<PlayerScript>();

        graphics::Texture* pacmanTexture = Resources::Find<graphics::Texture>(L"MapleEffect");
        Animator* animator = mPlayer->AddComponent<Animator>();
        animator->CreateAnimation(L"CatFrontMove", pacmanTexture,
            Vector2(0.f, 0.f), Vector2(386.f, 246.f), Vector2::Zero, 8, .05f);
        mPlayer->GetComponent<Transform>()->SetPosition({ 100.f, 100.f });
        mPlayer->GetComponent<Transform>()->SetScale({ 2.f, 2.f });
        mPlayer->GetComponent<Transform>()->SetRotation(30.f);

        animator->PlayAnimation(L"CatFrontMove", true);
        
        

        GameObject* bg = object::Instantiate<GameObject>(enums::eLayerType::Player);
        SpriteRenderer* bgSr = bg->AddComponent<SpriteRenderer>();

        
        graphics::Texture* bgTexture = Resources::Find<graphics::Texture>(L"Bubble");
        
        bgSr->SetTexture(bgTexture);


        Scene::Initialize();

    }
    void PlayScene::Update()
    {
        Scene::Update();
    }
    void PlayScene::LateUpdate()
    {
        Scene::LateUpdate();
        if (Input::GetKeyDown(eKeyCode::N))
        {
            SceneManager::LoadScene(L"TitleScene");
        }
    }
    void PlayScene::Render(HDC hdc)
    {
        Scene::Render(hdc);
        //wchar_t str[50] = L"Play Scene";
        //TextOut(hdc, 0, 0, str, (int)wcsnlen_s(str, 50));
    }
    void PlayScene::OnEnter()
    {
    }
    void PlayScene::OnExit()
    {
    }
}
