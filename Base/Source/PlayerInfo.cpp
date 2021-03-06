#include "PlayerInfo.h"
#include "SceneText.h"
#include <iostream>
#include "Application.h"

CPlayerInfo::CPlayerInfo(void)
	: hero_inMidAir_Up(false)
	, hero_inMidAir_Down(false)
	, jumpspeed(0)
	, heroAnimationCounter(0)
	, heroAnimationInvert(false)
	, mapOffset(Vector3(0, 0, 0))
	, playerMesh(NULL)
	, FlipStatus(false)
	, lives(5)
{
}

const float MOVEMENT_SPEED = 250.f;
const float JUMP_SPEED = 40.f;
const float JUMP_POWER = 15.f;
const float MAX_JUMPSPEED = 6.f;
const float CONSTRAINTSPEED = 200.f;
const float xOffset = 8.f;
const float yOffset = 2.f;

CPlayerInfo::~CPlayerInfo(void)
{
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	theHeroPosition.x=0;
	theHeroPosition.y=0;
}

// Returns true if the player is on ground
bool CPlayerInfo::isOnGround(void)
{
	if (hero_inMidAir_Up == false && hero_inMidAir_Down == false)
		return true;

	return false;
}

// Returns true if the player is jumping upwards
bool CPlayerInfo::isJumpUpwards(void)
{
	if (hero_inMidAir_Up == true && hero_inMidAir_Down == false)
		return true;

	return false;
}

// Returns true if the player is on freefall
bool CPlayerInfo::isFreeFall(void)
{
	if (hero_inMidAir_Up == false && hero_inMidAir_Down == true)
		return true;

	return false;
}

// Set the player's status to free fall mode
void CPlayerInfo::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		hero_inMidAir_Up = false;
		hero_inMidAir_Down = true;
		jumpspeed = 0;
	}
}

// Set the player to jumping upwards
void CPlayerInfo::HeroJump()
{
	if (hero_inMidAir_Up == false && hero_inMidAir_Down == false)
	{
		hero_inMidAir_Up = true;
		jumpspeed = JUMP_POWER;
	}
}

// Set position x of the player
void CPlayerInfo::SetPos_x(float pos_x)
{
	theHeroPosition.x = pos_x;
}

// Set position y of the player
void CPlayerInfo::SetPos_y(int pos_y)
{
	theHeroPosition.y = pos_y;
}

// Set Jumpspeed of the player
void CPlayerInfo::SetJumpspeed(int jumpspeed)
{
	this->jumpspeed = jumpspeed;
}

// Stop the player's movement
void CPlayerInfo::SetToStop(void)
{
	hero_inMidAir_Up = false;
	hero_inMidAir_Down = false;
	jumpspeed = 0;
}

Mesh* CPlayerInfo::GetPlayerMesh()
{
	return playerMesh;
}

bool CPlayerInfo::GetFlipStatus()
{
	return FlipStatus;
}

void CPlayerInfo::SetPlayerMesh(Mesh* mesh)
{
	this->playerMesh = mesh;
}

/********************************************************************************
 Hero Move Up Down
 ********************************************************************************/
void CPlayerInfo::MoveUpDown(const bool mode, CMap* m_cMap, double dt)
{
	if (mode)
	{
		Vector3 nextPos = theHeroPosition;
		nextPos.y = theHeroPosition.y - (MOVEMENT_SPEED * dt);
		Vector3 checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
		if (checkPosition.y < 0)
		{
			checkPosition.y = 0;
		}
		if (checkPosition.y + 1 > m_cMap->GetNumOfTiles_Height())
		{
			checkPosition.y = m_cMap->GetNumOfTiles_Height();
		}
		if (fmod(theHeroPosition.x + mapOffset.x, m_cMap->GetTileSize()) < 0.2)
		{
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].shouldCollide)
			{
				theHeroPosition.y = nextPos.y;
			}
			else
			{
				theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
			}
		}
		else
		{
			nextPos.y = theHeroPosition.y - (MOVEMENT_SPEED * dt);
			nextPos.x += xOffset;
			checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].shouldCollide)
			{
				nextPos.y = theHeroPosition.y - (MOVEMENT_SPEED * dt);
				nextPos.x -= xOffset;
				checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
				if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].shouldCollide)
				{
					theHeroPosition.y = nextPos.y;
				}
				else
				{
					theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
				}
			}
			else
			{
				theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
			}
		}
	}
	else
	{
		Vector3 nextPos = theHeroPosition;
		nextPos.y = theHeroPosition.y + (MOVEMENT_SPEED * dt);
		Vector3 checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
		if (checkPosition.y < 0)
		{
			checkPosition.y = 0;
		}
		if (checkPosition.y + 1 > m_cMap->GetNumOfTiles_Height())
		{
			checkPosition.y = m_cMap->GetNumOfTiles_Height();
		}
		if (fmod(theHeroPosition.x + mapOffset.x, m_cMap->GetTileSize()) < 0.2)
		{
			if (!m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].shouldCollide)
			{
				theHeroPosition.y = nextPos.y;
			}
			else
			{
				theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
			}
		}
		else
		{
			nextPos.y = theHeroPosition.y + (MOVEMENT_SPEED * dt);
			nextPos.x += xOffset;
			checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
			if (!m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].shouldCollide)
			{
				nextPos.y = theHeroPosition.y + (MOVEMENT_SPEED * dt);
				nextPos.x -= xOffset;
				checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
				if (!m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x + 1].shouldCollide)
				{
					theHeroPosition.y = nextPos.y;
				}
				else
				{
					theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
				}
			}
			else
			{
				theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
			}
		}
	}
}

/********************************************************************************
 Hero Move Left Right
 ********************************************************************************/
void CPlayerInfo::MoveLeftRight(const bool mode, CMap* m_cMap, double dt)
{
	if (mode)
	{
		Vector3 nextPos = theHeroPosition;
		nextPos.x = theHeroPosition.x - (MOVEMENT_SPEED * dt);
		Vector3 checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
		if (checkPosition.x < 0)
		{
			checkPosition.x = 0;
		}
		if (checkPosition.x > m_cMap->GetNumOfTiles_Width())
		{
			checkPosition.x = m_cMap->GetNumOfTiles_Width();
		}
		if (fmod(theHeroPosition.y + mapOffset.y, m_cMap->GetTileSize()) < 0.2)
		{
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].shouldCollide)
			{
				theHeroPosition.x = nextPos.x;
			}
			else
			{
				theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
			}
		}
		else{
			nextPos = theHeroPosition;
			nextPos.x = theHeroPosition.x - (MOVEMENT_SPEED * dt);
			nextPos.y += yOffset;
			checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].shouldCollide)
			{
				nextPos = theHeroPosition;
				nextPos.x = theHeroPosition.x - (MOVEMENT_SPEED * dt);
				nextPos.y -= yOffset;
				checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
				if (!m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].shouldCollide)
				{
					theHeroPosition.x = nextPos.x;
				}
				else
				{
					theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
				}

			}
			else
			{
				theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
			}
		}
	}else
	{
		Vector3 nextPos = theHeroPosition;
		nextPos.x = theHeroPosition.x + (MOVEMENT_SPEED * dt);
		Vector3 checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
		if (checkPosition.x < 0)
		{
			checkPosition.x = 0;
		}
		if (checkPosition.x+1 > m_cMap->GetNumOfTiles_Width())
		{
			checkPosition.x = m_cMap->GetNumOfTiles_Width();
		}
		if (fmod(theHeroPosition.y + mapOffset.y, m_cMap->GetTileSize()) < 0.1)
		{
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].shouldCollide)
			{
				theHeroPosition.x = nextPos.x;
			}
			else
			{
				theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
			}
		}
		else
		{
			nextPos = theHeroPosition;
			nextPos.x = theHeroPosition.x + (MOVEMENT_SPEED * dt);
			nextPos.y += yOffset;
			checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].shouldCollide)
			{
				nextPos = theHeroPosition;
				nextPos.x = theHeroPosition.x + (MOVEMENT_SPEED * dt);
				nextPos.y -= yOffset;
				checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
				if (!m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x + 1].shouldCollide)
				{
					theHeroPosition.x = nextPos.x;
				}
				else
				{
					theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
				}
			}
			else
			{
				theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
			}
		}
	}
}


// Get position x of the player
int CPlayerInfo::GetPos_x(void)
{
	return theHeroPosition.x;
}

// Get position y of the player
int CPlayerInfo::GetPos_y(void)
{
	return theHeroPosition.y;
}

// Get Jumpspeed of the player
int CPlayerInfo::GetJumpspeed(void)
{
	return jumpspeed;
}

//// Get mapOffset_x
//int CPlayerInfo::GetMapOffset_x(void)
//{
//	return mapOffset_x;
//}
//// Get mapOffset_y
//int CPlayerInfo::GetMapOffset_y(void)
//{
//	return mapOffset_y;
//}

// Update Jump Upwards
void CPlayerInfo::UpdateJumpUpwards()
{
	theHeroPosition.y -= jumpspeed;
	jumpspeed -= JUMP_SPEED;
	if (jumpspeed == 0)
	{
		hero_inMidAir_Up = false;
		hero_inMidAir_Down = true;
	}
}

// Update FreeFall
void CPlayerInfo::UpdateFreeFall()
{
	theHeroPosition.y += jumpspeed;
	jumpspeed = Math::Min(jumpspeed + JUMP_SPEED, MAX_JUMPSPEED);
}

// Set Animation Invert status of the player
void CPlayerInfo::SetAnimationInvert(bool heroAnimationInvert)
{
	this->heroAnimationInvert = heroAnimationInvert;
}
// Get Animation Invert status of the player
bool CPlayerInfo::GetAnimationInvert(void)
{
	return heroAnimationInvert;
}

// Set Animation Counter of the player
void CPlayerInfo::SetAnimationCounter(int heroAnimationCounter)
{
	this->heroAnimationCounter = heroAnimationCounter;
}
// Get Animation Counter of the player
int CPlayerInfo::GetAnimationCounter(void)
{
	return heroAnimationCounter;
}

// Constrain the position of the Hero to within the border
void CPlayerInfo::ConstrainHero(const int leftBorder, const int rightBorder, 
								  const int topBorder, const int bottomBorder, 
								  double dt, CMap* m_cMap, bool constrainX, bool constrainY)
{
	if (constrainX)
	{
		if (mapOffset.x != 0)
		{
			if (theHeroPosition.x < leftBorder)
			{
				float displacement = leftBorder - theHeroPosition.x;
				mapOffset.x -= displacement;
				theHeroPosition.x = leftBorder;
				//mapOffset.x = mapOffset.x - (int)(CONSTRAINTSPEED * dt);
				if (mapOffset.x < 0)
					mapOffset.x = 0;
			}
		}
		if (mapOffset.x < m_cMap->getScreenWidth() - 1.f)
		{
			if (theHeroPosition.x > rightBorder)
			{
				float displacement = theHeroPosition.x - rightBorder;
				mapOffset.x += displacement;
				theHeroPosition.x = rightBorder;
				//mapOffset.x = mapOffset.x + (int)(CONSTRAINTSPEED * dt);
				if (mapOffset.x > m_cMap->theScreen_Width)	// This must be changed to soft-coded
					mapOffset.x = m_cMap->theScreen_Width;
			}
		}
	}
	else
	{
		if (theHeroPosition.x > 800 - m_cMap->GetTileSize())
		{
			theHeroPosition.x = 800 - m_cMap->GetTileSize();
		}
	}

	if (theHeroPosition.x < 0)
	{
		theHeroPosition.x = 0;
	}
	else if (theHeroPosition.x > ((m_cMap->GetNumOfTiles_Width()-1) * m_cMap->GetTileSize() - mapOffset.x) - m_cMap->GetTileSize())
	{
		theHeroPosition.x = ((m_cMap->GetNumOfTiles_Width() - 1) * m_cMap->GetTileSize() - mapOffset.x) - m_cMap->GetTileSize();
	}

	if (constrainY)
	{
		if (theHeroPosition.y > topBorder)
		{
			float displacement = theHeroPosition.y - topBorder;
			mapOffset.y += displacement;
			theHeroPosition.y = topBorder;
		}

		if (theHeroPosition.y < bottomBorder)
		{
			float displacement = bottomBorder - theHeroPosition.y;
			mapOffset.y -= displacement;
			theHeroPosition.y = bottomBorder;
		}
	}
	else
	{
		if (theHeroPosition.y + m_cMap->GetTileSize() > 600)
		{
			theHeroPosition.y = 600 - m_cMap->GetTileSize();
		}
	}
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::HeroUpdate(CMap* m_cMap, double dt, Mesh**meshList)
{

	ConstrainHero(m_cMap->getScreenWidth() * 0.25, m_cMap->getScreenWidth() * 0.5, m_cMap->getScreenHeight() * 0.5, m_cMap->getScreenHeight() * 0.15, dt, m_cMap, true, true);
}