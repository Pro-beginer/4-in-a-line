#include "LSpriteMover.h"

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
	if (mIDirs[1] == infringeDir::UP || mIDirs[1] == infringeDir::DOWN)
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