#ifndef LSpriteObj_H
#define LSpriteObj_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>

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
	bool render(float x, float y, SDL_FRect* clip = nullptr, float width = kOriginalSize, float height = kOriginalSize, double degrees = 0.0, SDL_FPoint* center = nullptr, SDL_FlipMode flipmode = SDL_FLIP_NONE);

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

#endif