#include "LSpriteObj.h"

LSpriteObj::LSpriteObj() :
	mTexture{ nullptr }, mSurface{ nullptr }, mRenContext{ nullptr },
	mFileName{ "" }, mFolderName{ "" },
	mPosBox{ 0.f, 0.f, 0.f, 0.f }
{

}

LSpriteObj::LSpriteObj(SDL_Renderer* ren) :
	mTexture{ nullptr }, mSurface{ nullptr }, mRenContext{ ren },
	mFileName{ "" }, mFolderName{ "" },
	mPosBox{ 0.f, 0.f, 0.f, 0.f }
{

}

bool LSpriteObj::loadFromFile(std::string filePath, bool destroySurface)
{
	destroy();

	std::string imgName = filePath.substr(filePath.find('/') + 1, filePath.length() - 7),
		fldName = filePath.substr(0, 6);

	mSurface = IMG_Load(filePath.c_str());
	if (mSurface == nullptr)
	{
		SDL_Log("could not load image %s, from folder %s! SDL error : %s", imgName.c_str(), fldName.c_str(), SDL_GetError());
	}
	else
	{
		mTexture = SDL_CreateTextureFromSurface(mRenContext, mSurface);
		if (mTexture == nullptr)
		{
			SDL_Log("could not create texture from surface! SDL error : %s", SDL_GetError());
		}
		else
		{
			if (destroySurface == true)
			{
				SDL_DestroySurface(mSurface);
				mSurface = nullptr;
			}

			mFileName = imgName;
			mFolderName = fldName;

			if (mSurface != nullptr)
			{
				mPosBox.w = static_cast<float>(mSurface->w);
				mPosBox.h = static_cast<float>(mSurface->h);
			}
		}
	}

	return mTexture != nullptr;
}

bool LSpriteObj::loadTextureFromSurface()
{
	if (mTexture != nullptr)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}

	if (mSurface == nullptr)
	{
		SDL_Log("surface needs to be load before creating texture");
		return false;
	}

	mTexture = SDL_CreateTextureFromSurface(mRenContext, mSurface);
	if (mTexture == nullptr)
	{
		SDL_Log("could not create texture from surface! SDL error : %s", SDL_GetError());
		return false;
	}

	return true;
}

bool LSpriteObj::colorKeySurface(SDL_Color color)
{
	if (mSurface == nullptr)
	{
		SDL_Log("surface needs to be loaded in order to color key");
		return false;
	}

	return SDL_SetSurfaceColorKey(mSurface, true, SDL_MapSurfaceRGB(mSurface, color.r, color.g, color.b));
}

void LSpriteObj::destroy(bool fullDestroy)
{
	if (mTexture != nullptr)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}

	SDL_DestroySurface(mSurface);
	mSurface = nullptr;

	mFileName = "";
	mFolderName = "";

	if (fullDestroy)
	{
		mRenContext = nullptr;
	}
}

bool LSpriteObj::render(float x, float y, SDL_FRect* clip, float width, float height, double degrees, SDL_FPoint* center, SDL_FlipMode flipMode)
{
	if (mTexture == nullptr)
	{
		SDL_Log("texture needs to be loaded in order to be rendered!");
		return false;
	}

	SDL_FRect dstRect{ x, y, mPosBox.w, mPosBox.h };

	if (clip != nullptr)
	{
		dstRect.w = clip->w;
		dstRect.h = clip->h;
	}

	if (width > 0)
	{
		dstRect.w = width;
	}
	if (height > 0)
	{
		dstRect.h = height;
	}

	mPosBox.x = x;
	mPosBox.y = y;

	return SDL_RenderTextureRotated(mRenContext, mTexture, clip, &dstRect, degrees, center, flipMode);
}

LSpriteObj::~LSpriteObj()
{
	destroy(true);
}