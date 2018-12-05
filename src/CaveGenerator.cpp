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
#include "CaveGenerator.h"
#include "Location.h"
#include "Terrain.h"
#include "Scene.h"
#include "Renderer.h"
#include "BossAreaGenerator.h"
#include "PerlinNoise.h"
#include "LocationMap.h"
#include "Light.h"
#include "TonnelVertex.h"

CaveGenerator::CaveGenerator(Level * level, const shared_ptr<LocationDefinition>& params)
	: LocationGenerator(level, params)
{

}

shared_ptr<Location> CaveGenerator::Generate()
{
	CreateBlankLocation();
	CreateRandomTerrain();
	
	const shared_ptr<Terrain> terrain = mLocation->GetScene()->GetTerrain();

	sf::FloatRect mArea = {
		{ 0, 0 }, terrain->GetSizePixels()
	};

	mLocation->SetClass(LocationClass::Underground);
	terrain->SetTopBackgroundlessPartHeight(0);

	Renderer* renderer = game->GetRenderer();

	vector<shared_ptr<Texture>> backgroundLayers = {
		renderer->RequestTexture("data/textures/backs/caves/layer0.png"),
		renderer->RequestTexture("data/textures/backs/caves/layer1.png"),
		renderer->RequestTexture("data/textures/backs/caves/layer2.png"),
		renderer->RequestTexture("data/textures/backs/caves/layer3.png"),
		renderer->RequestTexture("data/textures/backs/caves/layer4.png"),
		renderer->RequestTexture("data/textures/backs/caves/layer5.png")
	};
	CreateParallaxBackground(false, backgroundLayers, true);

	const int pointCount = terrain->GetSize().x * terrain->GetSize().y / 2;

	sf::FloatRect bossArea;

	bossArea.width = Math::Rnd(700.0f, 1000.0f);
	bossArea.height = Math::Rnd(250.0f, 400.0f);
	bossArea.left = mArea.left + mArea.width * 0.5f - bossArea.width * 0.5f;
	bossArea.top = mArea.top + mArea.height - bossArea.height - 100;

	const float flatteningFactor = 4;

	vector<shared_ptr<TonnelVertex>> vertices;

	// Create vertices
	for (int i = 0; i < pointCount; ++i)
	{
		float x, y;

		int maxTries = 30;
		do
		{
			x = Math::Rnd(mArea.left, mArea.left + mArea.width);
			y = Math::Rnd(mArea.top, mArea.top + flatteningFactor * mArea.height);

			if (!bossArea.contains({ x, y }))
			{
				vertices.push_back(make_shared<TonnelVertex>(Vec2f(x, y)));
				break;
			}
		} while (--maxTries > 0);
	}

	auto bossRect = Math::RotatedRectangle(bossArea);

	// Connect vertices
	for (int aid = 0; aid < pointCount; aid++)
	{
		for (int bid = aid + 1; bid < pointCount; bid++)
		{
			bool isEdge = true;
			shared_ptr<TonnelVertex> &a = vertices[aid];
			const shared_ptr<TonnelVertex> &b = vertices[bid];
			float abDist = Math::SqrDistance(a->GetPosition(), b->GetPosition());

			if (bossRect.IsIntersectLine(a->GetPosition(), b->GetPosition(), nullptr))
			{
				isEdge = false;
			}

			if (isEdge)
			{
				for (int cid = 0; cid < pointCount; cid++)
				{
					if (cid == aid || cid == bid)
					{
						continue;
					}
					const auto &c = vertices[cid];
					const float acDist = Math::SqrDistance(a->GetPosition(), c->GetPosition());
					const float bcDist = Math::SqrDistance(b->GetPosition(), c->GetPosition());
					if (acDist < abDist && bcDist < abDist)
					{
						isEdge = false;
						break;
					}
				}
			}

			if (isEdge)
			{
				a->LinkWith(b);
			}
		}
	}

	// Apply narrowing
	const float invYNarrowing = 1.0f / flatteningFactor;
	for (const shared_ptr<TonnelVertex> & vertex : vertices)
	{
		Vec2f newPosition = vertex->GetPosition();

		newPosition.y -= mArea.top;
		newPosition.y *= invYNarrowing;
		newPosition.y += mArea.top;

		vertex->SetPosition(newPosition);
	}

	// Draw caves
	{
		const float lineWidth = 28;
		MaskOperation args;
		args.mBounds = Bounds::Circle;
		args.mRadius = lineWidth;
		for (const shared_ptr<TonnelVertex> &vertex : vertices)
		{
			for (const shared_ptr<TonnelVertex> &edgeVertex : vertex->GetEdgeVertices())
			{
				mLocation->GetMap()->DrawLine(vertex->GetPosition(), edgeVertex->GetPosition(), lineWidth, LocationMap::PassageColor);
				args.mPosition = vertex->GetPosition();
				Vec2f d = { edgeVertex->GetPosition() - vertex->GetPosition() };
				const int steps = static_cast<int>(Math::Length(d) / (lineWidth * 0.5f));
				d /= (float)steps;
				for (int i = 0; i < steps; i++)
				{
					args.mPosition += d;
					args.mRadius = Math::Rnd(lineWidth, lineWidth * 1.4f) + Terrain::SmoothKernelSize;
					args.mDoBitmaskOnlyOnVisibleDestPixels = false;
					args.mFirstPassBitwiseOperation = BitwiseOp::And;
					args.mFirstPassMask = ~(PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_LAVA | PA_ACID);
					terrain->Mask(args);
				}
			}
		}
	}
	
	// Place lights
	const int convergenceLimit = 10;
	for (size_t k = 0; k < vertices.size(); ++k)
	{
		for (int j = 0; j < convergenceLimit; ++j)
		{	
			shared_ptr<TonnelVertex> randomVertex;
			do
			{
				randomVertex  = Math::RandomElement(vertices);
			} while (randomVertex->GetEdgeVertices().empty());

			const Vec2f begin = randomVertex->GetPosition();
			const Vec2f end = Math::RandomElement(randomVertex->GetEdgeVertices())->GetPosition();

			const Vec2f p = (begin + end) * 0.5f;
			const float r = Math::Rnd(50.0f, 70.0f);
			bool tooClose = false;
			for (auto &otherLight : mLocation->GetScene()->GetLightList())
			{
				if (Math::IsCircleCircleIntersection(otherLight->GetGlobalPosition(), otherLight->GetRadius(), p, r))
				{
					tooClose = true;
					break;
				}
			}
			if (!tooClose)
			{			
				CreateCrystalLight(p, r);
			}
		}
	}
	
	PlaceOre({ { 0, 0 }, mLocation->GetScene()->GetTerrain()->GetSizePixels() });

	PerlinNoise png;
	const int width = terrain->GetWidthPixels();
	const int height = terrain->GetHeightPixels();
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			auto pixel = terrain->GetBackgroundMaskPixel(x, y);
			float noiseX = 10 * (x / static_cast<float>(width));
			float noiseY = 10 * (y / static_cast<float>(height));
			float alpha = png.OctaveNoise(noiseX, noiseY, 0.0f, 2, 0.7f);

			if (alpha > 0.58f)
			{
				*pixel = 0;
			}
		}
	}
	
	// GenerateBoss(location, v->mCaveGen->mBossArea);
	
	for (const shared_ptr<TonnelVertex> & vertex : vertices)
	{
		for (const shared_ptr<TonnelVertex> &edgeVertex : vertex->GetEdgeVertices())
		{
			if (!Math::ProbabilityEventAppeared(0.2f))
			{
				continue;
			}

			const Vec2f delta = Math::Normalized(edgeVertex->GetPosition() - vertex->GetPosition());
			const float t = Math::Rnd(0.0f, 1.0f);
			Vec2f position = vertex->GetPosition() + delta * t;
			if (bossArea.contains(position))
			{
				continue;
			}

			auto monster = mMonsterGenerator->Generate(mLocation);
			monster->SetPosition(position);
			monster->SetPatrolLine(Math::Line(vertex->GetPosition(), edgeVertex->GetPosition()));
			mLocation->AddActor(monster);
			monster->SetLevel(mLocation->GetDefinition()->GetBaseLevel() + Math::Rnd(0, 2));
		}
	}

	for (const shared_ptr<TonnelVertex> & vertex : vertices)
	{
		mLocation->AddTonnelVertex(vertex);
	}

	PlaceTreasureChests();

	return mLocation;
}
