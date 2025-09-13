#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <iostream>
#include <string>

class LSpriteMover;

class LSpriteObj
{
public:
	static constexpr float kOriginalSize = -1.f;

	LSpriteObj();
	LSpriteObj(SDL_Renderer* ren);

	LSpriteObj(const LSpriteObj&) = delete;
	LSpriteObj& operator=(const LSpriteObj&) = delete;

	LSpriteObj(LSpriteObj&&) = delete;
	LSpriteObj& operator=(LSpriteObj&&) = delete;

	bool loadFromFile(std::string filePath, bool destroySurface = false);

	bool colorKeySurface(SDL_Color color);

	bool loadTextureFromSurface();
	bool render( float x, float y, SDL_FRect* clip = nullptr, float width = kOriginalSize, float height = kOriginalSize, double degrees = 0.0, SDL_FPoint* center = nullptr, SDL_FlipMode flipmode = SDL_FLIP_NONE);

	void destroy(bool fullDestroy = false);

	void setRenderingContext(SDL_Renderer* ren) { mRenContext = ren; }
	void setPosBox(SDL_FRect box) { mPosBox = box; }

	void setWidth(float width) { mPosBox.w = width; }
	void setHeight(float height) { mPosBox.h = height; }

	void setXPos(float x) { mPosBox.x = x; }
	void setYPos(float y) { mPosBox.y = y; }

	std::string getFilePath() const { return mFolderName + '/' + mFileName; }

	float getWidth() const { return mPosBox.w; }
	float getHeight() const { return mPosBox.h; }

	float getXPos() const { return mPosBox.x; }
	float getYPos() const { return mPosBox.y; }

	bool isLoaded() const { return mTexture != nullptr; }
	bool getRenderer() const { return mRenContext; }
	SDL_FRect getPosBox() const { return mPosBox; };

	~LSpriteObj();
private:

	std::string mFileName;
	std::string mFolderName;

	SDL_FRect mPosBox;

	SDL_Surface* mSurface;
	SDL_Texture* mTexture;

	SDL_Renderer* mRenContext;
};

class LSpriteMover 
{
public:
	LSpriteMover();
	LSpriteMover(LSpriteObj* sprite);

	~LSpriteMover();

	LSpriteMover(const LSpriteMover&) = delete;
	LSpriteMover& operator= (const LSpriteMover&) = delete;

	LSpriteMover(LSpriteMover&&) = delete;
	LSpriteMover& operator= (LSpriteMover&&) = delete;

	bool iDirsContainsValue(int val);
	bool inRectBounds(SDL_FRect* bounds, bool updateInfringeDir = true);
	void scaleMoveAmmountByDir();
	bool moveSprite(float xF, float yF, SDL_FRect* bounds = nullptr, bool dynamUp = true, bool boundsError = false, bool updateSpritePos = true);

	void setSprite(LSpriteObj* sprite) { mSprite = sprite; }
	void setPosBox(SDL_FRect box) { mPosBox = box; }

	bool updatePosBox();

	bool updateXPos();
	bool updateYPos();

	void setXPos(float x) { mPosBox.x = x; }
	void setYPos(float y) { mPosBox.y = y; }

	bool updateWidth();
	bool updateHeight();

	void setWidth(float width) { mPosBox.w = width; }
	void setHeight(float height) { mPosBox.h = height; }

	bool isSpriteSet() const { return mSprite != nullptr; }

	SDL_FRect getPosBox() const { return mPosBox; }

	float getXPos() const { return mPosBox.x; }
	float getYPos() const { return mPosBox.y; }

	float getWidth() const { return mPosBox.w; }
	float getHeight() const { return mPosBox.h; }

	void destroy(bool destroySprite = false);

private:

	SDL_FRect mPosBox;
	
	float mXMoveAmmount;
	float mYMoveAmmount;

	LSpriteObj* mSprite;

	enum class infringeDir
	{
		NONE = 0,
		UP = 1, 
		LEFT = 2,
		DOWN = 3,
		RIGHT = 4
	} mIDirs[2];
};

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		SDL_Log("%s", SDL_GetError());
		return 1;
	}

	constexpr int screenWidth = 800, screenHeight = 400;
	
	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Color bgColor{ 0xFF, 0xFF, 0x00, 0xaa };
	SDL_FRect bounds{ 0.f, 0.f, screenWidth, screenHeight };

	if (window = SDL_CreateWindow("SDL", screenWidth, screenHeight, 0); window == nullptr)
	{
		SDL_Log("%s", SDL_GetError());
		return 2;
	}

	if (renderer = SDL_CreateRenderer(window, nullptr); renderer == nullptr)
	{
		SDL_Log("%s", SDL_GetError());
		return 3;
	}

	LSpriteObj sprite{ renderer };
	LSpriteMover mover{ &sprite };
	
	if (sprite.loadFromFile("images/red_dot.png") == false)
	{
		SDL_Log("could not load file! SDL error: %s", SDL_GetError());
		return 1;
	}

	sprite.colorKeySurface(SDL_Color{ 0xFF, 0xFF, 0xFF });
	sprite.loadTextureFromSurface();

	mover.updatePosBox();

	float xMoveAmm = 0.f, yMoveAmm = 0.f;

	SDL_Event e;
	SDL_zero(e);
	bool running{ true };

	while (running == true)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_EVENT_QUIT:
				running = false;
				break;

			case SDL_EVENT_KEY_DOWN:
				switch (e.key.key)
				{
				case SDLK_UP:
					yMoveAmm = -1.f;
					break;
				
				case SDLK_LEFT:
					xMoveAmm = -1.f;
					break;

				case SDLK_DOWN:
					yMoveAmm = 1.f;
					break;

				case SDLK_RIGHT:
					xMoveAmm = 1.f;
					break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
		SDL_RenderClear(renderer);
		mover.moveSprite(xMoveAmm, yMoveAmm, &bounds, false);
		sprite.render(sprite.getXPos(), sprite.getYPos());
		SDL_RenderPresent(renderer);
	}

	sprite.destroy();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}

LSpriteObj::LSpriteObj() :
	mTexture{ nullptr }, mSurface{ nullptr }, mRenContext{ nullptr },
	mFileName{ "" }, mFolderName{ "" },
	mPosBox{ 0.f, 0.f, 0.f, 0.f }
{

}

LSpriteObj::LSpriteObj (SDL_Renderer* ren) : 
	mTexture{nullptr}, mSurface{nullptr}, mRenContext{ren}, 
	mFileName{ "" }, mFolderName{ "" },
	mPosBox{0.f, 0.f, 0.f, 0.f}
{ 

}

bool LSpriteObj::loadFromFile(std::string filePath, bool destroySurface)
{
	destroy();

	std::string imgName = filePath.substr(filePath.find('/') + 1, filePath.length() - 7 ),
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
	SDL_DestroyTexture(mTexture);
	mTexture = nullptr;

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

LSpriteMover::LSpriteMover() :
	mPosBox{ 0.f, 0.f, 0.f, 0.f },
	mXMoveAmmount{ 0.f }, mYMoveAmmount{ 0.f },
	mIDirs{ infringeDir::NONE },
	mSprite{ nullptr }
{

}

LSpriteMover::LSpriteMover(LSpriteObj* sprite) : mSprite{ sprite },
	mPosBox{ 0.f, 0.f, 0.f, 0.f },
	mXMoveAmmount{ 0.f }, mYMoveAmmount{ 0.f },
	mIDirs{ infringeDir::NONE }
{

}

bool LSpriteMover::iDirsContainsValue(int val)
{
	if (static_cast<int>(mIDirs[0]) == val)
	{
		return true;
	}

	else if (static_cast<int>(mIDirs[1]) == val)
	{
		return true;
	}

	return false;
}

bool LSpriteMover::inRectBounds(SDL_FRect* bounds, bool updateInfringeDir)
{
	bool retVal = true;
	mIDirs[0] = infringeDir::NONE;
	mIDirs[1] = infringeDir::NONE;

	if (mPosBox.x + mXMoveAmmount < bounds->x)
	{
		std::cout << "too left\n";
		if (updateInfringeDir == true) { mIDirs[0] = infringeDir::LEFT; }
		retVal = false;
	}
	if (mPosBox.x + mXMoveAmmount + mPosBox.w > bounds->x + bounds->w)
	{
		std::cout << "too right\n";
		if (updateInfringeDir == true) { mIDirs[0] = infringeDir::RIGHT; }
		retVal = false;
	}
	if (mPosBox.y + mYMoveAmmount < bounds->y)
	{
		std::cout << "too high\n";
		if (updateInfringeDir == true) { mIDirs[1] = infringeDir::UP; }
		retVal = false;
	}
	if (mPosBox.y + mYMoveAmmount + mPosBox.h > bounds->y + bounds->h)
	{
		std::cout << "too low\n";
		if (updateInfringeDir == true) { mIDirs[1] = infringeDir::DOWN; }
		retVal = false;
	}

	return retVal;
}

void LSpriteMover::scaleMoveAmmountByDir()
{
	if ( mIDirs[1] == infringeDir::UP || mIDirs[1] == infringeDir::DOWN)
	{
		mYMoveAmmount = 0;
	}
	if (mIDirs[0] == infringeDir::LEFT || mIDirs[0] == infringeDir::RIGHT)
	{
		mXMoveAmmount = 0;
	}
}

bool LSpriteMover::moveSprite(float xF, float yF, SDL_FRect* bounds, bool dynamUp, bool boundsError, bool updateSpritePos)
{
	if (mSprite == nullptr)
	{
		SDL_Log("set sprite target before attempting to move!\n");
		return false;
	}

	bool checkBounds = bounds != nullptr;

	mXMoveAmmount = xF;
	mYMoveAmmount = yF;

	if (checkBounds == true && inRectBounds(bounds) == false)
	{
		if (mIDirs[0] == infringeDir::NONE && mIDirs[1] == infringeDir::NONE)
		{
			mXMoveAmmount = 0;
			mYMoveAmmount = 0;
		}
		else { scaleMoveAmmountByDir(); }

		std::cout << "infringed on bounds " << mPosBox.x << ' ' << mPosBox.y << ' ' << static_cast<int>(mIDirs[0])
			<< ' ' << static_cast<int>(mIDirs[1]) << ' ' << mXMoveAmmount << ' ' << mYMoveAmmount << " \n";

		if (boundsError == true)
		{
			SDL_Log("error, would move out of bounds!\n");
			return false;
		}
	}

	mPosBox.x += mXMoveAmmount;
	mPosBox.y += mYMoveAmmount;

	if (updateSpritePos == true)
	{
		mSprite->setXPos(mPosBox.x);
		mSprite->setYPos(mPosBox.y);
	}

	return true;
}

bool LSpriteMover::updatePosBox()
{
	if (mSprite == nullptr)
	{
		SDL_Log("set target sprite to update position box of sprite mover object!\n");
		return false;
	}

	mPosBox = mSprite->getPosBox();
	return true;
}

bool LSpriteMover::updateXPos()
{
	if (mSprite == nullptr)
	{
		SDL_Log("set target sprite to update x position of sprite mover object!\n");
		return false;
	}

	mPosBox.x = mSprite->getXPos();
	return true;
}

bool LSpriteMover::updateYPos()
{
	if (mSprite == nullptr)
	{
		SDL_Log("set target sprite to update y position of sprite mover object!\n");
		return false;
	}

	mPosBox.y = mSprite->getYPos();
	return true;
}

bool LSpriteMover::updateWidth()
{
	if (mSprite == nullptr)
	{
		SDL_Log("set target sprite to update width of sprite mover object!\n");
		return false;
	}

	mPosBox.w = mSprite->getWidth();
	return true;
}

bool LSpriteMover::updateHeight()
{
	if (mSprite == nullptr)
	{
		SDL_Log("set target sprite to update height of sprite mover object!\n");
		return false;
	}

	mPosBox.h = mSprite->getHeight();
	return true;
}

void LSpriteMover::destroy(bool destroySprite)
{
	if (destroySprite == true)
	{
		mSprite->destroy(true);
	}

	mPosBox.x = 0.f;
	mPosBox.y = 0.f;

	mPosBox.w = 0.f;
	mPosBox.h = 0.f;

	mXMoveAmmount = 0.f;
	mYMoveAmmount = 0.f;
}

LSpriteMover::~LSpriteMover()
{
	destroy();
}