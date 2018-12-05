/**********************************************************************
 * The Dwarf Adventure - 2D adventure RPG with destructible world     *
 *                                                                    *
 * Copyright(C) 2016-2019 Dmitry Stepanov a.k.a mrDIMAS a.k.a v1al    *
 *                                                                    *
 * This program is free software: you can redistribute it and/or      *
 * modify it under the terms of the GNU General Public License as     *
 * published by the Free Software Foundation, either version 3 of     *
 * the License, or (at your option) any later version.                *
 *                                                                    *
 * This program is distributed in the hope that it will be useful,    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * along with this program. If not, see https://www.gnu.org/licenses/ *
 *                                                                    *
 **********************************************************************/
#pragma once

#include "Serializer.h"
#include "CellPixel.h"
#include "SpinLock.h"

class Body;
class Physics;
class Terrain;
class Cell;
class SimulationIsland;

enum CollisionGroup : uint32_t
{
	CG_NONE = 0,
	CG_GENERIC = BIT(0),
	CG_WATER = BIT(1),
	CG_ACTOR = BIT(2),
	CG_CHAIN = BIT(3),
	CG_PROJECTILE = BIT(4),
	CG_DEBRIS = BIT(5),
	CG_CHEST = BIT(6),
	CG_SMOKE = BIT(7),
	CG_UNIVERSAL = UINT_MAX
};

struct Contact
{
	// Not null means that collision occured with terrain
	Cell *mCell;
	sf::Vector2i mPixelCoord;
	Body *mCollidedWith;
	Vec2f mPosition;
	Vec2f mNormal;
	CellPixel mPixel;
	Contact();
};

struct TerrainRayTraceResult
{
	Vec2f mPosition { FLT_MAX, FLT_MAX };
	CellPixel mPixel;
	bool mIntersection { false };
	shared_ptr<Cell> mCell;
	sf::Vector2i mPixelCoord;
};

struct Link
{
	float mDefLen { 0.0f };
	float mStiffness { 1.0f };
	float mLengthK { 1.0f };
	shared_ptr<Body> mBodyA;
	shared_ptr<Body> mBodyB;
	Link();
	Link(const shared_ptr<Body> &a, const shared_ptr<Body> &b, float length, float stiffness);
	void Solve();
	void Serialize(Serializer &sav);
};

// Collision between links is disabled
class Chain
{
private:
	bool mEnabled { true };

public:
	vector<shared_ptr<Body>> mBodies;
	vector<shared_ptr<Link>> mLinks;
	Chain();
	Chain(SimulationIsland *island, float thickness, float length);
	void AddBodyToEnd(const shared_ptr<Body> &body, float length);
	void AddBodyToBegin(const shared_ptr<Body> &body, float length);
	void SetStiffness(float stiffness);
	void Solve();
	void SetEnabled(bool state);
	bool IsEnabled() const;
	void Align(const Vec2f &begin, const Vec2f &end);
	void Serialize(Serializer &sav);
	void Scale(float kLength);
	float GetScale() const;
	void RemoveLastLink();
	void RemoveFirstLink();
};

enum BodyFlag : uint8_t
{
	// If 0 - body will gather contacts, but won't do collision response
	BF_RESPONSE_ENABLED = BIT(0),
	// If 1 - no dynamics applied and no static-static collision will be performed
	BF_STATIC = BIT(1),
	BF_ENABLED = BIT(2),
	// If 1 - body will push other body when collidin', otherwise they will be sliding around
	BF_PUSHER = BIT(3),
	// If 1 - no contact info will be gathered on Particle-Particle collision stage
	BF_IGNORE_PP_CONTACT_INFO = BIT(4),
	// If 1 - no contact info will be gathered on Particle-Terrain collision stage
	BF_IGNORE_PT_CONTACT_INFO = BIT(5),
	// Incompatible with BF_STATIC and BF_PUSHER
	BF_WATER = BIT(6),
	BF_SLEEP = BIT(7),
};

struct WorldZone
{
	vector<Body *> mBodyList;
	sf::FloatRect mRect;
};

class Body : public enable_shared_from_this<Body>
{
public:
	static constexpr float DefaultGravityX = 0.0f;
	static constexpr float DefaultGravityY = 800.0f;
	static constexpr int MaxContacts = 12;
	// List of zones in which this body placed, used in Physics
	array<WorldZone *, 9> mZoneList;

private:
	float mArea { 0 };
	Vec2f mPosition;
	Vec2f mOldPosition;
	Vec2f mAcceleration;
	Vec2f mGravity { DefaultGravityX, DefaultGravityY };
	// Valid position from the last frame, used to push body back to non-colliding area of terrain
	// cell
	Vec2f mRecoveryPosition;
	// Collision group and mask are used in collision filtering. Collision appears only if: (b_group &
	// a_mask) && (a_group & b_mask). By default all bodies are in group CG_GENERIC and masked to
	// collide with all possible groups (0xFFFFFFFF)
	uint32_t mCollisionGroup { CG_GENERIC };
	uint32_t mCollisionMask { CG_UNIVERSAL };
	float mMass { 1 };
	float mInvMass { 1 };
	float mFriction { 0.985f };
	float mRadius { 0 };
	float mSqrRadius { 0 };
	float mDeactivationTime { 0 };
	SimulationIsland *mIsland { nullptr };
	uint8_t mFlags { BF_ENABLED | BF_RESPONSE_ENABLED };
	uint8_t mContactCount { 0 };
	array<Contact, MaxContacts> mContacts;
	// declare friends to reduce GetXXX calls
	friend class Cell;
	friend class Chain;
	friend struct Link;
	friend class Physics;
	friend class SimulationIsland;
	// Returns pointer to last free contact, nullptr if there is no free contacts
	Contact *AddContact();
	// Locks mutex and performs offset
	void OffsetThreadSafe(SpinLock &spinlock, const Vec2f &v);

public:
	Body();
	~Body();
	void Update(float dt);
	void CircleCircleCollision(SpinLock &spinlock, Body *circle);
	void SetRadius(float r);
	float GetRadius() const;
	void SetFriction(float friction);
	float GetFriction();
	void Serialize(Serializer &sav);
	void RemoveFromWorld();
	bool WillCollideWith(Body *other);
	void SetCollisionGroup(int group);
	void SetCollisionMask(int mask);
	float GetArea() const;
	Vec2f GetPosition() const;
	void SetPosition(const Vec2f &position);
	void Offset(const Vec2f &offset);
	void AddForce(const Vec2f &force);
	void SetMass(float mass);
	float GetMass() const;
	void SetVelocity(const Vec2f &v);
	void SetHorizontalVelocity(float xVel);
	void SetVerticalVelocity(float yVel);
	Vec2f GetVelocity() const;
	const Contact &GetContact(int n) const;
	int GetContactCount() const;
	void ClearContacts();
	void SetFlags(uint8_t mask);
	void ResetFlags(uint8_t mask);
	bool IsFlagsSet(uint8_t mask);
	void SetGravity(const Vec2f &g);
	Vec2f GetGravity() const;
	uint8_t GetFlags() const;
	bool IsOnGround() const;
};

struct ClosestRayCircleIntersectionResult
{
	shared_ptr<Body> mBody;
	bool mIntersection { false };
	Vec2f mIntA { FLT_MAX, FLT_MAX };
	Vec2f mIntB { FLT_MAX, FLT_MAX };
	float mSqrDistance { 0 };
};

struct RayCircleIntersectionResult
{
	shared_ptr<Body> mBody;
	Vec2f mClosestIntPt;
	Vec2f mFarthestIntPt;
};

struct ClosestRayCastResult
{
	shared_ptr<Body> mBody;
	shared_ptr<Cell> mCell;
	Vec2f mPosition;
	sf::Vector2i mPixelCoord;
	float mSqrDistance { 0 };
	bool mIntersection { false };
};

// Physics world. No interaction between islands. Used to do simulation on separate scenes
class SimulationIsland
{
private:
	friend class Physics;
	Vec2f mWorldMin;
	Vec2f mWorldMax;
	vector<shared_ptr<Body>> mBodyList;
	vector<shared_ptr<Chain>> mChainList;
	int mWidthZoneCount { 0 };
	int mHeightZoneCount { 0 };
	bool mEnabled { true };
	vector<WorldZone> mZoneList;
	shared_ptr<Terrain> mTerrain;
	// World sub-division zones
	void DoWorldSubdivision();
	WorldZone *PickZone(int i, int j);
	void PickZones(const Vec2f &p, array<WorldZone *, 9> &outZones);
	void DetermineZoneForEachParticle();

public:
	SimulationIsland();
	~SimulationIsland();
	shared_ptr<Body> CreateParticle(float radius);
	shared_ptr<Chain> CreateChain(float thickness, float length);
	void RemoveBody(const shared_ptr<Body> &b);
	void SetWorldSize(const Vec2f &bmin, const Vec2f &bmax);
	const vector<shared_ptr<Body>> &GetBodyList() const;
	const vector<shared_ptr<Chain>> &GetChainList() const;
	Vec2f GetWorldMin() const;
	Vec2f GetWorldMax() const;
	void Serialize(Serializer &sav);
	// Returns result of intersection with closest body
	ClosestRayCircleIntersectionResult ClosestRayCircleCast(const Vec2f &origin, const Vec2f &direction, const shared_ptr<Body> &ignoreBody);
	ClosestRayCastResult ClosestRayCast(const Vec2f &origin, const Vec2f &direction, const shared_ptr<Body> &ignoreBody = nullptr);
	bool RayCircleCast(const Vec2f &origin, const Vec2f &direction, vector<RayCircleIntersectionResult> &outBodies);
	void Clear();
	void UpdateBodies(float dt);
	void SolveChains();
	void SetTerrain(const shared_ptr<Terrain> &terrain);
	// transfers body from previous simulation island to this
	void AcceptBody(const shared_ptr<Body> &body);
	void AcceptChain(const shared_ptr<Chain> &chain);
	void SetEnabled(bool state);
	bool IsEnabled() const;
};


// Multithreaded manager of simulation islands
class Physics
{
public:
	Physics(unsigned int threadCount);
	~Physics();
	shared_ptr<SimulationIsland> CreateIsland();
	void Step(float dt = 1.0f / 60.0f);
	void Serialize(Serializer &sav);
	void Clear();
	// Averaged result in microseconds
	unsigned int GetParticleBitmapSolveTime() const;
	// Averaged result in microseconds
	unsigned int GetParticleParticleSolveTime() const;
	// Averaged result in microseconds
	unsigned int GetTotalSolveTime() const;
	// Averaged result in microseconds
	unsigned int GetPrePassTime() const;
	unsigned int GetIslandCount() const;
	unsigned int GetParticleCount() const;
	void ResetPerformanceCounters();
private:
	enum class CalculationMode
	{
		BodyBody,
		BodyTerrain,
	};

	enum class ThreadMode
	{
		Start,
		Working,
		Finished,
	};

	unsigned int mThreadCount;
	vector<vector<Body*>> mThreadBodyList;

	vector<shared_ptr<SimulationIsland>> mIslandList;
	// Performance info
	unsigned int mParticleBitmapSolveTime { 0 };
	unsigned int mParticleParticleSolveTime { 0 };
	unsigned int mPrePassTime { 0 };
	unsigned int mStepsDone { 0 };
	mutex mMutex;
	SpinLock mCalculationSpinLock;
	shared_ptr<Terrain> mCurrentTerrain;
	bool mThreadsActive { true };
	
	void DetermineZoneForEachParticle();	
	void UpdateBodies(float dt);	
	void SolveChains();
	void GatherRecoveryPositions(const shared_ptr<SimulationIsland> &island);
	

	CalculationMode mMode;
	std::vector<ThreadMode> mThreadMode;
	std::condition_variable mStartCalculationConditionVar;	
	std::condition_variable mThreadsFinishedConditionVar;
	bool IsAllThreadsFinished() const
	{
		for (const ThreadMode & threadMode : mThreadMode)
		{
			if (threadMode != ThreadMode::Finished)
			{
				return false;
			}
		}

		return true;
	}

	void DoParallelSimulationStep(CalculationMode mode, const shared_ptr<SimulationIsland> &island);
	void SimulationThread(unsigned int threadIndex);
};
