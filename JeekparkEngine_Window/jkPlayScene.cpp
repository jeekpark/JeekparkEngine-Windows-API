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
#include "jkCat.h"
#include "jkCatScript.h"
#include "jkBoxCollider2D.h"
#include "jkSakuya.h"
#include "jkSakuyaScript.h"
#include "jkApplication.h"
#include "jkCollisionManager.h"
#include "jkRigidbody.h"
extern jk::Application app;

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
            { app.GetClientWidth() / 2.f, app.GetClientHeight() / 2.f}
        );
        Camera* cameraComp = cameraObj->AddComponent<Camera>();
        renderer::mainCamera = cameraComp;

         /////// SAKUYA ////////
        Sakuya* sakuya = object::Instantiate<Sakuya>(enums::eLayerType::Player);
        renderer::mainCamera->SetTarget(sakuya);
        sakuya->GetComponent<Transform>()->SetPosition({ 200.f, 600.f });
        sakuya->GetComponent<Transform>()->SetScale({ 3.f, 3.f });
        sakuya->GetComponent<Transform>()->SetRotation(0.f);
        SakuyaScript* sakuyaScript = sakuya->AddComponent<SakuyaScript>();
        graphics::Texture* sakuyaTex = Resources::Find<graphics::Texture>(L"SakuyaTexture");
        Animator* sakuyaAnimator = sakuya->AddComponent<Animator>();
        sakuyaAnimator->CreateAnimation(L"Idle", sakuyaTex,
            { 0.f, 0.f }, { 64.f, 64.f }, { 32.f, 64.f }, 6, 0.1f);
        sakuyaAnimator->CreateAnimation(L"StartRun", sakuyaTex,
            { 64.f * 9.f, 0.f }, { 64.f, 64.f }, { 32.f, 64.f }, 8, 0.01f);
        sakuyaAnimator->CreateAnimation(L"Run", sakuyaTex,
            { 0.f, 64.f * 1.f }, { 64.f, 64.f }, { 32.f, 64.f }, 16, 0.03f);
        sakuyaAnimator->CreateAnimation(L"EndRun", sakuyaTex,
            { 0.f, 64.f * 2.f }, { 64.f, 64.f }, { 32.f, 64.f }, 8, 0.03f);
        sakuyaAnimator->PlayAnimation(L"Idle", true);
        CircleCollider2D* boxCollider2d = sakuya->AddComponent<CircleCollider2D>();
        boxCollider2d->SetOffset({ -64.f, -64.f });
        Rigidbody* rgd = sakuya->AddComponent<Rigidbody>();



        Sakuya* npc = object::Instantiate<Sakuya>(enums::eLayerType::Player);
        npc->GetComponent<Transform>()->SetPosition({ 400.f, 640.f });
        npc->GetComponent<Transform>()->SetScale({ 3.f, 3.f });
        Animator* npcAnimator = npc->AddComponent<Animator>();
        npcAnimator->CreateAnimation(L"Idle", sakuyaTex,
            { 0.f, 0.f }, { 64.f, 64.f }, { 32.f, 64.f }, 6, 0.1f);
        npcAnimator->PlayAnimation(L"Idle", true);
        CircleCollider2D* npcCollider2d = npc->AddComponent<CircleCollider2D>();
        npcCollider2d->SetOffset({ -64.f, -64.f });
        Rigidbody* npcRgd = npc->AddComponent<Rigidbody>();

        Scene::Initialize();
        object::DontDestroyOnLoad(sakuya);
        
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
        Scene::OnEnter();
        CollisionManager::CollistionLayerCheck(eLayerType::Player, eLayerType::Player, true);
    }
    void PlayScene::OnExit()
    {
        Scene::OnExit();
    }
}
