#ifndef LSpriteMover_H
#define LSpriteMover_H

#include "LSpriteObj.h"

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

#endif