#include "jkTexture.h"
#include "jkApplication.h"

extern jk::Application app;

namespace jk::graphics
{
    Texture::Texture()
        : Resource(enums::eResourceType::Texture)
    {
    }
    Texture::~Texture()
    {
    }
    HRESULT Texture::Load(const std::wstring& path)
    {
        std::wstring ext = path.substr(path.find_last_of(L".") + 1);

        if (ext == L"bmp")
        {
            mTextureType = eTextureType::Bmp;
            mBitmap = (HBITMAP)LoadImageW(
                nullptr,
                path.c_str(),
                IMAGE_BITMAP,
                0,
                0,
                LR_LOADFROMFILE | LR_CREATEDIBSECTION
            );
            if (mBitmap == nullptr)
            {
                return S_FALSE;
            }
            BITMAP info = {};
            GetObject(mBitmap, sizeof(BITMAP), &info);

            mWidth = info.bmWidth;
            mHeight = info.bmHeight;

            HDC mainDC = app.GetHdc();
            mHdc = CreateCompatibleDC(mainDC);

            HBITMAP oldBitmap = (HBITMAP)SelectObject(mHdc, mBitmap);
            DeleteObject(oldBitmap);
        }
        else if (ext == L"png")
        {
            mTextureType = eTextureType::Png;
            mImage = Gdiplus::Image::FromFile(path.c_str());
            if (mImage == nullptr)
            {
                return S_FALSE;
            }
            mWidth = mImage->GetWidth();
            mHeight = mImage->GetHeight();
        }
        

        return S_OK;
    }
}