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

#include "Precompiled.h"
#include "Collision.h"
#include "Game.h"
#include "Cell.h"
#include "Renderer.h"
#include "Terrain.h"
#include "Scene.h"


Contact *Body::AddContact()
{
	Contact *contact = nullptr;
	if (mContactCount < MaxContacts - 1)
	{
		contact = &mContacts[mContactCount];
		++mContactCount;
	}
	return contact;
}

void Body::OffsetThreadSafe(SpinLock &spinlock, const Vec2f &v)
{
	//	SpinLockGuard guard(spinlock);
	UNUSED(spinlock);
	mPosition += v;
}

Body::Body()
{
}

Body::~Body()
{
	DBG_ECHO();
}

void Body::Update(float dt)
{
	if (mFlags & BF_STATIC)
	{
		return;
	}

	if (mFlags & BF_ENABLED)
	{
		// Verlet integration
		const float friction = mContactCount > 0 ? 1 - mFriction : 0.003f;
		Vec2f temp = mPosition;
		mPosition = (2.f - friction) * mPosition - (1.f - friction) * mOldPosition + mAcceleration * dt * dt;
		mOldPosition = temp;
		mAcceleration = { 0, 0 };

		// clamp position to world size
		mPosition = Math::Clamp(mPosition, mIsland->GetWorldMin(), mIsland->GetWorldMax());

		// velocity limiting
		auto vel = GetVelocity();
		constexpr float limit = 9.81f;
		if (Math::SqrLength(vel) > limit * limit)
		{
			mOldPosition = mPosition - Math::Normalized(vel) * limit;
		}
	}
}

void Body::CircleCircleCollision(SpinLock &spinlock, Body *circle)
{
	Vec2f dir = mPosition - circle->mPosition;
	const float d = Math::SqrLength(dir);
	const float rs = mRadius + circle->mRadius;
	if (d <= rs * rs)
	{
		const float dist = sqrt(d);
		if (dist > 0.00001f)
		{
			dir /= dist;

			// other PUSHER cannot be pushed :)
			if ((mFlags & BF_PUSHER) && (!(circle->mFlags & BF_PUSHER)))
			{
				// push other body at full penetration depth
				if (!(circle->mFlags & BF_STATIC))
				{
					if (circle->mFlags & BF_RESPONSE_ENABLED)
					{
						float penetrationDepth = mRadius + circle->mRadius - dist;
						// pusher can push water flow
						if (circle->mFlags & BF_WATER)
						{
							penetrationDepth *= 0.05f;
						}
						circle->OffsetThreadSafe(spinlock, -dir * penetrationDepth);
					}
					// write contact
					if ((circle->mFlags & BF_IGNORE_PP_CONTACT_INFO) == 0)
					{
						if (auto contact = circle->AddContact())
						{
							contact->mCell = nullptr;
							contact->mCollidedWith = this;
							contact->mPosition = circle->mPosition + dir * mRadius;
						}
					}
				}
			}
			else
			{
				// push each body at half of penetration depth on 'dist' vector
				float penetrationDepth = 0.5f * (mRadius + circle->mRadius - dist);

				// buoyancy
				// dunno if this is right solution, but looks good
				auto thisIsWater = mFlags & BF_WATER;
				auto otherIsWater = circle->mFlags & BF_WATER;
				auto waterWaterContact = thisIsWater && otherIsWater;
				if ((otherIsWater || thisIsWater) && !waterWaterContact)
				{
					penetrationDepth *= 0.005f;
				}

				if (!(mFlags & BF_STATIC))
				{
					if (circle->mFlags & BF_RESPONSE_ENABLED)
					{
						OffsetThreadSafe(spinlock, dir * penetrationDepth);
					}

					// write contact if needed
					if ((mFlags & BF_IGNORE_PP_CONTACT_INFO) == 0)
					{
						if (auto contact = AddContact())
						{
							contact->mCell = nullptr;
							contact->mCollidedWith = this;
							contact->mPosition = mPosition - dir * mRadius;
						}
					}
				}

				if (!(circle->mFlags & BF_STATIC))
				{
					if (mFlags & BF_RESPONSE_ENABLED)
					{
						circle->OffsetThreadSafe(spinlock, -dir * penetrationDepth);
					}

					// write contact if needed
					if ((circle->mFlags & BF_IGNORE_PP_CONTACT_INFO) == 0)
					{
						if (auto contact = circle->AddContact())
						{
							contact->mCell = nullptr;
							contact->mCollidedWith = this;
							contact->mPosition = circle->mPosition + dir * mRadius;
						}
					}
				}
			}
		}
	}
}

void Body::SetRadius(float r)
{
	mRadius = r;
	mSqrRadius = r * r;
	mArea = Math::Pi * mSqrRadius;
}

float Body::GetRadius() const
{
	return mRadius;
}

void Body::SetFriction(float friction)
{
	mFriction = friction;
}

float Body::GetFriction()
{
	return mFriction;
}

void Body::Serialize(Serializer &sav)
{
	sav &mPosition;
	sav &mOldPosition;
	sav &mRecoveryPosition;
	sav &mRadius;
	sav &mSqrRadius;
	sav &mCollisionGroup;
	sav &mCollisionMask;
	sav &mArea;
	sav &mFlags;
	sav &mFriction;
	sav &mMass;
	sav &mInvMass;
}

void Body::RemoveFromWorld()
{
	mIsland->RemoveBody(shared_from_this());
}

bool Body::WillCollideWith(Body *other)
{
	return (other->mCollisionGroup & mCollisionMask) && (mCollisionGroup & other->mCollisionMask);
}

void Body::SetCollisionGroup(int group)
{
	mCollisionGroup = group;
}

void Body::SetCollisionMask(int mask)
{
	mCollisionMask = mask;
}

float Body::GetArea() const
{
	return mArea;
}

Vec2f Body::GetPosition() const
{
	return mPosition;
}

void Body::SetPosition(const Vec2f &position)
{
	mPosition = position;
	mOldPosition = position;
	mRecoveryPosition = position;
}

void Body::Offset(const Vec2f &offset)
{
	mPosition += offset;
}

void Body::AddForce(const Vec2f &force)
{
	mAcceleration += force * mInvMass;
}

void Body::SetMass(float mass)
{
	mMass = mass;
	mInvMass = 1.0f / mass;
}

float Body::GetMass() const
{
	return mMass;
}

void Body::SetVelocity(const Vec2f &v)
{
	mOldPosition = mPosition - v;
}

void Body::SetHorizontalVelocity(float xVel)
{
	mOldPosition.x = mPosition.x - xVel;
}

void Body::SetVerticalVelocity(float yVel)
{
	mOldPosition.y = mPosition.y - yVel;
}

Vec2f Body::GetVelocity() const
{
	return mPosition - mOldPosition;
}

const Contact &Body::GetContact(int n) const
{
	return mContacts[n];
}

int Body::GetContactCount() const
{
	return mContactCount;
}

void Body::ClearContacts()
{
	mContactCount = 0;
}

void Body::SetFlags(uint8_t mask)
{
	mFlags |= mask;
}

void Body::ResetFlags(uint8_t mask)
{
	mFlags &= ~mask;
}

bool Body::IsFlagsSet(uint8_t mask)
{
	return (mFlags & mask) == mask;
}

void Body::SetGravity(const Vec2f &g)
{
	mGravity = g;
}

Vec2f Body::GetGravity() const
{
	return mGravity;
}

uint8_t Body::GetFlags() const
{
	return mFlags;
}

bool Body::IsOnGround() const
{
	for (int i = 0; i < mContactCount; ++i)
	{
		if (mContacts[i].mCell)
		{
			if (mContacts[i].mPosition.y > mPosition.y)
			{
				return true;
			}
		}
	}

	return false;
}

void Physics::DetermineZoneForEachParticle()
{
	const auto lastTime = chrono::high_resolution_clock::now();
	for (auto &island : mIslandList)
	{
		if (island->IsEnabled())
		{
			island->DetermineZoneForEachParticle();
		}
	}
	const auto currentTime = chrono::high_resolution_clock::now();
	mPrePassTime += static_cast<unsigned int>(
		chrono::duration_cast<chrono::microseconds>(currentTime - lastTime).count());
}

void Physics::UpdateBodies(float dt)
{
	for (auto &island : mIslandList)
	{
		if (island->IsEnabled())
		{
			island->UpdateBodies(dt);
		}
	}
}

void Physics::SolveChains()
{
	for (auto &island : mIslandList)
	{
		island->SolveChains();
	}
}

void Physics::GatherRecoveryPositions(const shared_ptr<SimulationIsland> &island)
{
	for (auto &p : island->mBodyList)
	{
		p->mRecoveryPosition = p->mPosition;
	}
}

void Physics::DoParallelSimulationStep(CalculationMode mode, const shared_ptr<SimulationIsland>& island)
{
	const auto lastTime = chrono::high_resolution_clock::now();

	// Feed worker threads
	{
		// Suspend worker threads
		std::unique_lock<std::mutex> locker(mMutex);

		// Set worker thread mode
		mMode = mode;

		if (mMode == CalculationMode::BodyTerrain)
		{
			mCurrentTerrain = island->mTerrain;
		}
		else
		{
			mCurrentTerrain.reset();
		}

		// Clear thread arrays
		for (vector<Body*> & threadArray : mThreadBodyList)
		{
			threadArray.clear();
		}

		// Push bodies to arrays uniformly
		// +1 here to make sure that last body be pushed to array
		unsigned int bodiesPerThread = island->mBodyList.size() / mThreadCount + 1; 
		unsigned int threadIndex = 0;
		unsigned int bodyIndex = 0;

		for (size_t i = 0; i < island->mBodyList.size(); ++i)
		{
			Body* const body = island->mBodyList[i].get();

			if (body->mFlags & BF_ENABLED)
			{
				mThreadBodyList[threadIndex].push_back(body);

				++bodyIndex;

				if (bodyIndex >= bodiesPerThread)
				{
					++threadIndex;
					bodyIndex = 0;
				}
			}
		}

		// Prepare threads
		for (ThreadMode & threadMode : mThreadMode)
		{
			threadMode = ThreadMode::Start;
		}

		// Send start signal to worker threads
		mStartCalculationConditionVar.notify_all();
	}

	// Wait until each worker thread finish its job
	{
		std::unique_lock<std::mutex> locker(mMutex);

		while (!IsAllThreadsFinished())
		{
			mThreadsFinishedConditionVar.wait(locker);
		}
	}

	if (mMode == CalculationMode::BodyTerrain)
	{
		// Reset strong reference to terrain. This is needed to prevent issues with terrain lifetime
		mCurrentTerrain.reset();
	}

	// Measure performance
	const auto currentTime = chrono::high_resolution_clock::now();
	const auto elapsedTime = static_cast<unsigned int>(chrono::duration_cast<chrono::microseconds>(currentTime - lastTime).count());

	if (mMode == CalculationMode::BodyBody)
	{
		mParticleParticleSolveTime += elapsedTime;
	}
	else if (mMode == CalculationMode::BodyTerrain)
	{
		mParticleBitmapSolveTime += elapsedTime;
	}
}

void Physics::SimulationThread(unsigned int threadIndex)
{
	try
	{
		while (mThreadsActive)
		{
			// Wait until have signal to start
			{
				std::unique_lock<std::mutex> locker(mMutex);

				while (mThreadMode[threadIndex] != ThreadMode::Start)
				{
					mStartCalculationConditionVar.wait(locker);
				}

				// Switch mode 
				mThreadMode[threadIndex] = ThreadMode::Working;
			}

			Body** bodyList = mThreadBodyList[threadIndex].data();
			const size_t bodyCount = mThreadBodyList[threadIndex].size();

			if (mMode == CalculationMode::BodyBody)
			{
				for (size_t bodyIndex = 0; bodyIndex < bodyCount; ++bodyIndex)
				{
					Body* body = bodyList[bodyIndex];

					// Use old style iteration here because for some reason MSVC's std uses mutex inside of its guts
					for (size_t zoneIndex = 0; zoneIndex < body->mZoneList.size(); ++zoneIndex)
					{
						WorldZone* const zone = body->mZoneList[zoneIndex];

						// Zone can be nullptr if body does not touch it
						if (!zone)
						{
							continue;
						}

						const size_t zoneBodyCount = zone->mBodyList.size();
						Body** zoneBodyList = zone->mBodyList.data();

						for (size_t otherBodyIndex = 0; otherBodyIndex < zoneBodyCount; ++otherBodyIndex)
						{
							Body* const otherBody = zoneBodyList[otherBodyIndex];

							// Ignore self-collision
							if (otherBody == body)
							{
								continue;
							}

							if (body->WillCollideWith(otherBody))
							{
								body->CircleCircleCollision(mCalculationSpinLock, otherBody);
							}
						}
					}
				}
			}
			else if (mMode == CalculationMode::BodyTerrain)
			{
				for (size_t bodyIndex = 0; bodyIndex < bodyCount; ++bodyIndex)
				{
					Body* const body = bodyList[bodyIndex];

					// Nine cell is the worst case when particle's diameter is more than Cell::Size. As I know,
					// such huge particles is very rare and max size is strictly defined when creating particle
					// in other cases (size << Cell::Size) only four cell enough
					array<Cell*, 9> cells;
					
					int cellCount = mCurrentTerrain->GetCellsNear(body->mPosition, body->mRadius, cells);
					const bool inside = (cellCount == 1);
					for (int i = 0; i < cellCount; ++i)
					{
						if (cells[i])
						{
							cells[i]->CircleCollision(body, inside);
						}
					}				
				}
			}

			// Report result
			{
				std::lock_guard<std::mutex> guard(mMutex);

				mThreadMode[threadIndex] = ThreadMode::Finished;

				// Last finished thread will notify invoker thread
				if (IsAllThreadsFinished())
				{
					mThreadsFinishedConditionVar.notify_all();
				}
			}
		}
	}
	catch (std::exception &e)
	{
		ReportError(e.what());
	}
}

Physics::Physics(unsigned int threadCount) : mThreadCount(threadCount)
{
	const unsigned int threadLimit = std::thread::hardware_concurrency() * 2;

	if (threadCount > threadLimit)
	{
		Log() << "Warning: Maximum thread count for physics is exceeded! Tried to use " << threadCount << " threads, but maximum amount is "
			<< threadLimit << ". Where hardware simultaneous thread limit is " << std::thread::hardware_concurrency() << LineEnd;
	}

	threadCount = Math::Min(threadCount, threadLimit);

	mThreadBodyList.resize(threadCount);
		
	// spawn threads
	for (unsigned int i = 0; i < threadCount; ++i)
	{
		mThreadMode.push_back(ThreadMode::Finished);	
		thread { [this, i] { 
			SimulationThread(i); }
		}.detach();
	}

	Log() << "Physics successfully initialized! Using " << threadCount << " threads" << LineEnd;
}

Physics::~Physics()
{
    mThreadsActive = false;
            
	DBG_ECHO(); 
}

shared_ptr<SimulationIsland> Physics::CreateIsland()
{
	auto island = make_shared<SimulationIsland>();
	mIslandList.push_back(island);
	return island;
}

void Physics::Step(float dt)
{
	UpdateBodies(dt);
	DetermineZoneForEachParticle();
	
	for (auto &island : mIslandList)
	{
		if (island->IsEnabled())
		{
			DoParallelSimulationStep(CalculationMode::BodyBody, island);
			DoParallelSimulationStep(CalculationMode::BodyTerrain, island);
		}
	}

	SolveChains();
	for (auto &island : mIslandList)
	{
		if (island->IsEnabled())
		{
			GatherRecoveryPositions(island);
		}
	}

	++mStepsDone;
}

void Physics::Serialize(Serializer &sav)
{
	sav &mIslandList;
}

void Physics::Clear()
{
	for (auto &island : mIslandList)
	{
		island->Clear();
	}
	mIslandList.clear();
	mParticleBitmapSolveTime = 0;
	mParticleParticleSolveTime = 0;
	mPrePassTime = 0;
}

unsigned int Physics::GetParticleBitmapSolveTime() const
{
	return mStepsDone ? mParticleBitmapSolveTime / mStepsDone : 0;
}

unsigned int Physics::GetParticleParticleSolveTime() const
{
	return mStepsDone ? mParticleParticleSolveTime / mStepsDone : 0;
}

unsigned int Physics::GetTotalSolveTime() const
{
	const auto totalTime = mParticleBitmapSolveTime + mParticleParticleSolveTime + mPrePassTime;
	return mStepsDone ? totalTime / mStepsDone : 0;
}

unsigned int Physics::GetPrePassTime() const
{
	return mStepsDone ? mPrePassTime / mStepsDone : 0;
}

unsigned int Physics::GetIslandCount() const
{
	return mIslandList.size();
}

unsigned int Physics::GetParticleCount() const
{
	unsigned int pcount = 0;
	for (auto &island : mIslandList)
	{
		pcount += island->mBodyList.size();
	}
	return pcount;
}

void Physics::ResetPerformanceCounters()
{
	mParticleBitmapSolveTime = 0;
	mParticleParticleSolveTime = 0;
	mPrePassTime = 0;
	mStepsDone = 0;
}

Link::Link()
{
}

Link::Link(const shared_ptr<Body> &a, const shared_ptr<Body> &b, float length, float stiffness)
	: mBodyA(a), mBodyB(b), mDefLen(length), mStiffness(stiffness)
{
}

void Link::Solve()
{
	const auto center = (mBodyA->mPosition + mBodyB->mPosition) * 0.5f;
	const auto dPos = mBodyB->mPosition - mBodyA->mPosition;
	const auto push = Math::Normalized(dPos) * mLengthK * mDefLen * 0.5f;

	mBodyA->mPosition += (center - push - mBodyA->mPosition) * mStiffness;
	mBodyB->mPosition += (center + push - mBodyB->mPosition) * mStiffness;
}

void Link::Serialize(Serializer &sav)
{
	sav &mDefLen;
	sav &mStiffness;
	sav &mBodyA;
	sav &mBodyB;
	sav &mLengthK;
}

Chain::Chain()
{
}

Chain::Chain(SimulationIsland *island, float thickness, float length)
{
	Vec2f position;
	const float diameter = thickness * 2;
	const int count = static_cast<int>(length / diameter);
	for (int i = 0; i < count; ++i)
	{
		auto body = island->CreateParticle(thickness);
		body->SetCollisionGroup(CG_CHAIN);
		body->SetCollisionMask(0); // no link-link collision
		body->SetPosition(position);
		position.x += diameter;
		AddBodyToEnd(body, diameter);
	}
}

void Chain::AddBodyToEnd(const shared_ptr<Body> &body, float length)
{
	if (mBodies.size() > 0)
	{
		mLinks.push_back(make_shared<Link>(mBodies.back(), body, length, 1.0f));
	}

	mBodies.push_back(body);
}

void Chain::AddBodyToBegin(const shared_ptr<Body> &body, float length)
{
	if (mBodies.size() > 0)
	{
		mLinks.emplace(mLinks.begin(), make_shared<Link>(mBodies.front(), body, length, 1.0f));
	}
	mBodies.insert(mBodies.begin(), body);
}

void Chain::SetStiffness(float stiffness)
{
	for (auto &link : mLinks)
	{
		link->mStiffness = stiffness;
	}
}

void Chain::Solve()
{
	if (mEnabled)
	{
		for (auto &link : mLinks)
		{
			link->Solve();
		}
	}
}

void Chain::SetEnabled(bool state)
{
	mEnabled = state;
	for (auto &b : mBodies)
	{
		if (state)
		{
			b->mFlags |= BF_ENABLED;
		}
		else
		{
			b->mFlags &= ~BF_ENABLED;
		}
	}
}

bool Chain::IsEnabled() const
{
	return mEnabled;
}

void Chain::Align(const Vec2f &begin, const Vec2f &end)
{
	// scale chain first
	const Vec2f v = end - begin;
	const float distance = Math::Length(v);
	float totalLength = 0.0f;
	for (auto &link : mLinks)
	{
		totalLength += link->mDefLen;
	}
	Scale(distance / totalLength);

	// now align each link
	const Vec2f dir = Math::Normalized(v);
	Vec2f p = begin;
	for (auto &link : mLinks)
	{
		link->mBodyA->SetPosition(p);
		p += dir * link->mDefLen * link->mLengthK;
		link->mBodyB->SetPosition(p);
	}
}

void Chain::Serialize(Serializer &sav)
{
	sav &mEnabled;
	sav &mBodies;
	sav &mLinks;
}

void Chain::Scale(float kLength)
{
	kLength = Math::Max(0.0f, kLength);
	for (auto &link : mLinks)
	{
		link->mLengthK = kLength;
	}
}

float Chain::GetScale() const
{
	return mLinks.size() ? mLinks[0]->mLengthK : 0.0f;
}

void Chain::RemoveLastLink()
{
	if (mLinks.size())
	{
		// remove link and associated body
		mLinks.pop_back();
		mBodies.pop_back();
	}
}

void Chain::RemoveFirstLink()
{
	if (mLinks.size())
	{
		// remove link and associated body
		mLinks.erase(mLinks.begin());
		mBodies.erase(mBodies.begin());
	}
}

Contact::Contact() : mCollidedWith(nullptr), mCell(nullptr)
{
}

void SimulationIsland::DoWorldSubdivision()
{
	mZoneList.clear();
	mWidthZoneCount = static_cast<int>((mWorldMax.x - mWorldMin.x) / Cell::Size);
	mHeightZoneCount = static_cast<int>((mWorldMax.y - mWorldMin.y) / Cell::Size);
	for (int y = 0; y < mHeightZoneCount; ++y)
	{
		for (int x = 0; x < mWidthZoneCount; ++x)
		{
			mZoneList.emplace_back();
			auto &newZone = mZoneList.back();
			newZone.mRect.left = mWorldMin.x + static_cast<float>(x * Cell::Size);
			newZone.mRect.top = mWorldMin.y + static_cast<float>(y * Cell::Size);
			newZone.mRect.width = Cell::Size;
			newZone.mRect.height = Cell::Size;
		}
	}
}

WorldZone *SimulationIsland::PickZone(int i, int j)
{
	int index = i * mWidthZoneCount + j;
	if (index >= 0 && index < static_cast<int>(mZoneList.size()))
	{
		return &mZoneList[index];
	}
	return nullptr;
}

void SimulationIsland::PickZones(const Vec2f &p, array<WorldZone *, 9> &outZones)
{
	int j = static_cast<int>(p.x * Cell::InvSize);
	int i = static_cast<int>(p.y * Cell::InvSize);
	int iMinusOne = i - 1;
	int iPlusOne = i + 1;
	int jMinusOne = j - 1;
	int jPlusOne = j + 1;
	outZones[0] = PickZone(i, j);
	outZones[1] = PickZone(iPlusOne, j);
	outZones[2] = PickZone(iMinusOne, j);
	outZones[3] = PickZone(i, jPlusOne);
	outZones[4] = PickZone(i, jMinusOne);
	outZones[5] = PickZone(iPlusOne, jPlusOne);
	outZones[6] = PickZone(iMinusOne, jPlusOne);
	outZones[7] = PickZone(iPlusOne, jPlusOne);
	outZones[8] = PickZone(iPlusOne, jMinusOne);
}

void SimulationIsland::DetermineZoneForEachParticle()
{
	for (auto &z : mZoneList)
	{
		z.mBodyList.clear();
	}
	for (auto &body : mBodyList)
	{
		if (!(body->GetFlags() & BF_ENABLED))
		{
			continue;
		}
		const auto bodyPos = body->GetPosition();
		const auto bodyRad = body->GetRadius();
		PickZones(body->GetPosition(), body->mZoneList);
		for (auto &zone : body->mZoneList)
		{
			if (!zone)
			{
				continue;
			}
			Vec2f p = { zone->mRect.left, zone->mRect.top };
			Vec2f s = { zone->mRect.width, zone->mRect.height };
			if (Math::IsCircleRectIntersection(p, s, bodyPos, bodyRad))
			{
				zone->mBodyList.push_back(body.get());
			}
			else
			{
				zone = nullptr;
			}
		}
	}
}

SimulationIsland::SimulationIsland()
{
}

SimulationIsland::~SimulationIsland()
{
}

shared_ptr<Body> SimulationIsland::CreateParticle(float radius)
{
	if (radius * 2 > Cell::Size)
	{
		throw Exception(StringBuilder("Trying to create too large physical particle!\n")
			<< "Max size " << Cell::Size << ", but " << radius * 2 << " was passed.");
	}

	auto c = make_shared<Body>();
	c->SetRadius(radius);
	c->mIsland = this;
	mBodyList.push_back(c);
	return c;
}

shared_ptr<Chain> SimulationIsland::CreateChain(float thickness, float length)
{
	auto chain = make_shared<Chain>(this, thickness, length);
	mChainList.push_back(chain);
	return chain;
}

void SimulationIsland::RemoveBody(const shared_ptr<Body> &b)
{
	mBodyList.erase(remove(mBodyList.begin(), mBodyList.end(), b), mBodyList.end());
}

void SimulationIsland::SetWorldSize(const Vec2f &bmin, const Vec2f &bmax)
{
	mWorldMin = bmin;
	mWorldMax = bmax;

	DoWorldSubdivision();
}

const vector<shared_ptr<Body>> &SimulationIsland::GetBodyList() const
{
	return mBodyList;
}

const vector<shared_ptr<Chain>> &SimulationIsland::GetChainList() const
{
	return mChainList;
}

Vec2f SimulationIsland::GetWorldMin() const
{
	return mWorldMin;
}

Vec2f SimulationIsland::GetWorldMax() const
{
	return mWorldMax;
}

void SimulationIsland::Serialize(Serializer &sav)
{
	sav &mBodyList;
	for (auto &dynamicBody : mBodyList)
	{
		dynamicBody->mIsland = this;
	}
	sav &mWorldMax;
	sav &mWorldMin;
	sav &mTerrain;
	sav &mChainList;
	sav &mEnabled;
	if (sav.IsLoading())
	{
		DoWorldSubdivision();
	}
}

ClosestRayCircleIntersectionResult SimulationIsland::ClosestRayCircleCast(
	const Vec2f &origin, const Vec2f &direction, const shared_ptr<Body> &ignoreBody)
{
	ClosestRayCircleIntersectionResult out;
	out.mSqrDistance = FLT_MAX;
	out.mIntersection = false;
	for (auto &circle : mBodyList)
	{
		if (circle == ignoreBody)
		{
			continue;
		}
		Vec2f a, b;
		if (Math::IsRayIntersectCircle(origin, direction, circle->mPosition, circle->mRadius, a, b))
		{
			const float sqrDistance = Math::SqrDistance(b, origin);
			if (sqrDistance < out.mSqrDistance)
			{
				out.mSqrDistance = sqrDistance;
				out.mBody = circle;
				out.mIntA = a;
				out.mIntB = b;
				out.mIntersection = true;
			}
		}
	}
	return out;
}

ClosestRayCastResult SimulationIsland::ClosestRayCast(
	const Vec2f &origin, const Vec2f &direction, const shared_ptr<Body> &ignoreBody)
{

	ClosestRayCircleIntersectionResult circleTraceResult =
		ClosestRayCircleCast(origin, direction, ignoreBody);
	TerrainRayTraceResult terrainTraceResult;

	terrainTraceResult = mTerrain->TraceRay(origin, direction);

	ClosestRayCastResult out;
	if (circleTraceResult.mSqrDistance < Math::SqrDistance(origin, terrainTraceResult.mPosition))
	{
		out.mBody = circleTraceResult.mBody;
		out.mCell = nullptr;
		out.mIntersection = circleTraceResult.mIntersection;
		out.mPosition = circleTraceResult.mIntB;
		out.mSqrDistance = circleTraceResult.mSqrDistance;
	}
	else
	{
		out.mBody = nullptr;
		out.mCell = terrainTraceResult.mCell;
		out.mIntersection = terrainTraceResult.mIntersection;
		out.mPosition = terrainTraceResult.mPosition;
		out.mSqrDistance = Math::SqrDistance(origin, terrainTraceResult.mPosition);
		out.mPixelCoord = terrainTraceResult.mPixelCoord;
	}

	return out;
}

bool SimulationIsland::RayCircleCast(
	const Vec2f &origin, const Vec2f &direction, vector<RayCircleIntersectionResult> &outBodies)
{
	for (auto &circle : mBodyList)
	{
		Vec2f a, b;
		if (Math::IsRayIntersectCircle(origin, direction, circle->mPosition, circle->mRadius, a, b))
		{
			outBodies.emplace_back();
			auto &intRes = outBodies.back();
			intRes.mBody = circle;
			intRes.mFarthestIntPt = a;
			intRes.mClosestIntPt = b;
		}
	}
	return outBodies.size() != 0;
}

void SimulationIsland::Clear()
{
	mBodyList.clear();
	mChainList.clear();
}

void SimulationIsland::UpdateBodies(float dt)
{
	for (auto &dynamicBody : mBodyList)
	{
		dynamicBody->AddForce(dynamicBody->GetGravity());
		dynamicBody->Update(dt);
		dynamicBody->mContactCount = 0;
	}
}

void SimulationIsland::SolveChains()
{
	constexpr int iterationCount = 3; // adjust this to achieve more precise behaviour of the chains
	for (int i = 0; i < iterationCount; ++i)
	{
		for (auto &chain : mChainList)
		{
			chain->Solve();
		}
	}
}

void SimulationIsland::SetTerrain(const shared_ptr<Terrain> &terrain)
{
	mTerrain = terrain;
	SetWorldSize({ 0, 0 }, { (float)mTerrain->GetWidthPixels(), (float)mTerrain->GetHeightPixels() });
}

// transfers body from previous simulation island to this
void SimulationIsland::AcceptBody(const shared_ptr<Body> &body)
{
	if (body->mIsland == this)
	{
		return;
	}
	// save strong ref to body
	auto b = body;
	// remove from prev island
	b->mIsland->RemoveBody(b);
	// add to this
	b->mIsland = this;
	mBodyList.push_back(b);
}

void SimulationIsland::AcceptChain(const shared_ptr<Chain> &chain)
{
	for (auto &body : chain->mBodies)
	{
		AcceptBody(body);
	}
}

void SimulationIsland::SetEnabled(bool state)
{
	mEnabled = state;
}

bool SimulationIsland::IsEnabled() const
{
	return mEnabled;
}
