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
        mPlayer->AddComponent<PlayerScript>();

        /*graphics::Texture* pacmanTexture = Resources::Find<graphics::Texture>(L"MapleEffect");
        Animator* animator = mPlayer->AddComponent<Animator>();
        animator->CreateAnimation(L"CatFrontMove", pacmanTexture,
            Vector2(0.f, 0.f), Vector2(386.f, 246.f), Vector2::Zero, 8, .05f);
        animator->PlayAnimation(L"CatFrontMove", true);
        mPlayer->GetComponent<Transform>()->SetPosition({ 100.f, 100.f });
        mPlayer->GetComponent<Transform>()->SetScale({ 2.f, 2.f });
        mPlayer->GetComponent<Transform>()->SetRotation(30.f);*/

        graphics::Texture* pacmanTexture = Resources::Find<graphics::Texture>(L"Cat");
        Animator* animator = mPlayer->AddComponent<Animator>();
        animator->CreateAnimation(L"DownWalk", pacmanTexture,
            Vector2(0.f, 0.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.1f);
        animator->CreateAnimation(L"RightWalk", pacmanTexture,
            Vector2(0.f, 32.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.1f);
        animator->CreateAnimation(L"UpWalk", pacmanTexture,
            Vector2(0.f, 64.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.1f);
        animator->CreateAnimation(L"LeftWalk", pacmanTexture,
            Vector2(0.f, 96.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.1f);
        animator->CreateAnimation(L"SitDown", pacmanTexture,
            Vector2(0.f, 128.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.5f);
        animator->CreateAnimation(L"Grooming", pacmanTexture,
            Vector2(0.f, 160.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.5f);
        

        mPlayer->GetComponent<Transform>()->SetPosition({ 100.f, 100.f });
        mPlayer->GetComponent<Transform>()->SetScale({ 3.f, 3.f });
        mPlayer->GetComponent<Transform>()->SetRotation(0.f);
        animator->PlayAnimation(L"SitDown", false);


        /*GameObject* bg = object::Instantiate<GameObject>(enums::eLayerType::Player);
        SpriteRenderer* bgSr = bg->AddComponent<SpriteRenderer>();
        graphics::Texture* bgTexture = Resources::Find<graphics::Texture>(L"Bubble");
        
        bgSr->SetTexture(bgTexture);*/
        

        /////// CAT ////////
        Cat* cat = object::Instantiate<Cat>(enums::eLayerType::Animal);
        cat->AddComponent<CatScript>();

        graphics::Texture* catTex = Resources::Find<graphics::Texture>(L"Cat");
        Animator* catAnimator = cat->AddComponent<Animator>();
        catAnimator->CreateAnimation(L"DownWalk", catTex,
            Vector2(0.f, 0.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.2f);
        catAnimator->CreateAnimation(L"RightWalk", catTex,
            Vector2(0.f, 32.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.2f);
        catAnimator->CreateAnimation(L"UpWalk", catTex,
            Vector2(0.f, 64.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.2f);
        catAnimator->CreateAnimation(L"LeftWalk", catTex,
            Vector2(0.f, 96.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.2f);
        catAnimator->CreateAnimation(L"SitDown", catTex,
            Vector2(0.f, 128.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.5f);
        catAnimator->CreateAnimation(L"Grooming", catTex,
            Vector2(0.f, 160.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.5f);
        catAnimator->CreateAnimation(L"LayDown", catTex,
            Vector2(0.f, 192.f), Vector2(32.f, 32.f), Vector2::Zero, 4, 0.5f);

        catAnimator->PlayAnimation(L"SitDown", false);

        cat->GetComponent<Transform>()->SetPosition({ 300, 300 });
        cat->GetComponent<Transform>()->SetScale({ 3.f, 3.f });

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
