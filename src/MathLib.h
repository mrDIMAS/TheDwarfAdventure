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

#include "ArbitraryRectangle.h"

static inline Vec2f operator*(const Vec2f &a, const Vec2f &b)
{
	return { a.x * b.x, a.y * b.y };
}

namespace Math
{
	struct CircleDesc
	{
		Vec2f mPosition;
		float mRadius;
	};

	// Alpha is not interpolated!
	inline void LerpRGB(sf::Color &out, const sf::Color &a, const sf::Color &b, float t)
	{
		out.r = static_cast<uint8_t>(a.r + t * (b.r - a.r));
		out.g = static_cast<uint8_t>(a.g + t * (b.g - a.g));
		out.b = static_cast<uint8_t>(a.b + t * (b.b - a.b));
	}



	static constexpr float Epsilon = 1e-6f;

	inline float Dot(const Vec2f &va, const Vec2f &vb)
	{
		return va.x * vb.x + va.y * vb.y;
	}

	inline Vec2f ProjectPointOnLine(const Vec2f &point, const Vec2f &a, const Vec2f &b)
	{
		auto ab = b - a;
		float div = Dot(ab, ab);
		if (div < Epsilon)
		{
			return point;
		}
		return a + ab * (Dot(point - a, ab) / div);
	}

	inline Vec2f MapWorldToScreen(
		const Vec2f &globalPosition, const sf::Transform &viewTransform, const sf::IntRect &viewport)
	{
		auto normalized = viewTransform.transformPoint(globalPosition);
		return { (normalized.x + 1.0f) / 2.0f * viewport.width + viewport.left,
			(-normalized.y + 1.0f) / 2.0f * viewport.height + viewport.top };
	}

	inline Vec2f MapWorldToScreen(const Vec2f &localPosition, const sf::Transform &worldTransform,
		const sf::Transform &viewTransform, const sf::IntRect &viewport)
	{
		auto normalized = viewTransform.transformPoint(worldTransform.transformPoint(localPosition));
		return { (normalized.x + 1.0f) / 2.0f * viewport.width + viewport.left,
			(-normalized.y + 1.0f) / 2.0f * viewport.height + viewport.top };
	}

	class Line final
	{
	public:
		Vec2f mBegin;
		Vec2f mEnd;

		Line()
		{
		}

		Line(const Vec2f &begin, const Vec2f &end) : mBegin(begin), mEnd(end)
		{
		}
	};

	static constexpr float Pi = 3.141592653f;

	extern std::mt19937 gRandomNumbersGenerator;

	inline float Rad2Deg(float rad)
	{
		return rad * 180.0f / Pi;
	}

	inline float Deg2Rad(float rad)
	{
		return rad * Pi / 180.0f;
	}

	inline Vec2f &Normalize(Vec2f &va)
	{
		const float len = sqrt(va.x * va.x + va.y * va.y);
		if (len > 0.0001)
		{
			va /= len;
		}
		return va;
	}

	inline float Sign(float n)
	{
		return n < 0.0f ? -1.0f : 1.0f;
	}

	// Does not support shear. If transform will contain any shear the output result will
	// be invalid
	inline void DecomposeTransform(const sf::Transform &t, Vec2f *p, Vec2f *s, float *r)
	{
		auto m = t.getMatrix();
		// TRS decomposition
		if (p)
		{
			p->x = m[12];
			p->y = m[13];
		}
		if (s)
		{
			s->x = Sign(m[0]) * sqrt(m[0] * m[0] + m[4] * m[4]);
			s->y = Sign(m[5]) * sqrt(m[1] * m[1] + m[5] * m[5]);
		}
		if (r)
		{
			*r = Math::Rad2Deg(atan2f(-m[4], m[0]));
		}
	}

	// result in degrees
	inline float SimplifyAngle(float angle)
	{
		angle = fmodf(angle, 360.0f);
		if (angle < -180.0f)
		{
			angle += 360.0f;
		}
		else if (angle > 180.0f)
		{
			angle -= 360.0f;
		}
		return angle;
	}

	// result in degrees
	inline float AngleBetweenVectors(const Vec2f &a, const Vec2f &b)
	{
		float dot = a.x * b.x + a.y * b.y;
		float cross = a.x * b.y - a.y * b.x;
		return Math::Rad2Deg(atan2f(cross, dot));
	}

	// input angle in degrees
	inline Vec2f RotateVector(const Vec2f &v, float angle)
	{
		const float radAngle = Deg2Rad(angle);
		const float cosAngle = cos(radAngle);
		const float sinAngle = sin(radAngle);
		return { v.x * cosAngle - v.y * sinAngle, v.y * cosAngle + v.x * sinAngle };
	}

	inline sf::Transform ComposeTransform(
		const Vec2f &position, const Vec2f &origin, const Vec2f &scale, float rotation)
	{
		float angle = -rotation / 180.f * Math::Pi;
		float cosine = static_cast<float>(std::cos(angle));
		float sine = static_cast<float>(std::sin(angle));
		float sxc = scale.x * cosine;
		float syc = scale.y * cosine;
		float sxs = scale.x * sine;
		float sys = scale.y * sine;
		float tx = -origin.x * sxc - origin.y * sys + position.x;
		float ty = origin.x * sxs - origin.y * syc + position.y;

		return sf::Transform(sxc, sys, tx, -sxs, syc, ty, 0.f, 0.f, 1.f);
	}

	template <class T> inline T Sqr(T n)
	{
		return n * n;
	}

	inline Vec2f Normalized(const Vec2f &va)
	{
		Vec2f result(va);
		const float len = sqrt(result.x * result.x + result.y * result.y);
		if (len > 0.0001)
		{
			result /= len;
		}
		return result;
	}

	template <class T> inline T Lerp(const T &a, const T &b, float t)
	{
		return a + (b - a) * t;
	}

	template <> inline Vec2f Lerp(const Vec2f &a, const Vec2f &b, float t)
	{
		float x = a.x + (b.x - a.x) * t;
		float y = a.y + (b.y - a.y) * t;
		return { x, y };
	}

	inline Vec2f Perpendicular(const Vec2f &v)
	{
		return Vec2f(v.y, -v.x);
	}

	inline float Length(const Vec2f &v)
	{
		return sqrtf(v.x * v.x + v.y * v.y);
	}

	inline float SqrLength(const Vec2f &v)
	{
		return v.x * v.x + v.y * v.y;
	}

	template <class T> inline T Clamp(T value, const T &min, const T &max)
	{
		if (value < min)
			value = min;
		if (value > max)
			value = max;
		return value;
	}

	template <> inline Vec2f Clamp(Vec2f value, const Vec2f &min, const Vec2f &max)
	{
		value.x = Math::Clamp(value.x, min.x, max.x);
		value.y = Math::Clamp(value.y, min.y, max.y);
		return value;
	}

	inline void SetRNGSeed(unsigned int seed)
	{
		gRandomNumbersGenerator.seed(seed);
	}

	template <typename T> inline T Rnd(T low, T high)
	{
		if (high < low)
		{
			swap(high, low);
		}
		return std::uniform_int_distribution<T>(low, high)(gRandomNumbersGenerator);
	}

	template <> inline float Rnd(float low, float high)
	{
		if (high < low)
		{
			swap(high, low);
		}
		return std::uniform_real_distribution<float>(low, high)(gRandomNumbersGenerator);
	}

	template<class T>
	struct Range
	{
		T min { 0 };
		T max { 0 };
		Range()
		{
		}
		Range(const T & min, const T & max) : min(min), max(max)
		{
		}
		bool Contains(const T & value) const
		{
			return value >= min && value <= max;
		}
		T RandomValue() const
		{
			return Math::Rnd(min, max);
		}
		void Serialize(Serializer &sav)
		{
			sav &min;
			sav &max;
		}
	};

	inline sf::Color RndOpaqueColor(uint8_t min, uint8_t max)
	{
		return { static_cast<uint8_t>(Rnd(static_cast<int>(min), static_cast<int>(max))),
			static_cast<uint8_t>(Rnd(static_cast<int>(min), static_cast<int>(max))),
			static_cast<uint8_t>(Rnd(static_cast<int>(min), static_cast<int>(max))) };
	}

	inline Vec2f CubicBezier(const Vec2f & p0, const Vec2f & p1, const Vec2f & p2, const Vec2f & p3, float t)
	{
		float u = 1.0f - t;
		float w1 = u * u * u;
		float w2 = 3 * u * u * t;
		float w3 = 3 * u *t * t;
		float w4 = t * t * t;
		return { w1 * p0.x + w2 * p1.x + w3 * p2.x + w4 * p3.x, w1 * p0.y + w2 * p1.y + w3 * p2.y + w4 * p3.y };
	}

	inline Vec2f RectPosition(const sf::FloatRect & r)
	{
		return { r.left, r.top };
	}

	inline Vec2f RectSize(const sf::FloatRect & r)
	{
		return { r.width, r.height };
	}

	inline Vec2f RectCenter(const sf::FloatRect & r)
	{
		return { r.left + r.width * 0.5f, r.top + r.height * 0.5f };
	}

	inline bool ProbabilityEventAppeared(float probability)
	{
		return Rnd(0.0f, 1.0f) < Clamp(probability, 0.0f, 1.0f);
	}

	template <typename T> T &RandomElement(vector<T> &v)
	{
		return v[Rnd(0u, v.size() - 1)];
	}

	template <typename T, unsigned int size> T &RandomElement(array<T, size> &v)
	{
		return v[Rnd(0u, v.size() - 1)];
	}

	template <typename T> const T &RandomElement(const vector<T> &v)
	{
		if (v.empty())
		{
			throw Exception(string(__func__) + " - Collection was empty!");
		}

		return v[Rnd(0u, v.size() - 1)];
	}

	template <class T> inline T Max(const T &a, const T &b)
	{
		return a > b ? a : b;
	}

	template <class T> inline T Min(const T &a, const T &b)
	{
		return a < b ? a : b;
	}

	inline bool IsPointOnLine(const Vec2f &p, const Vec2f &a, const Vec2f &b)
	{
		float kx = (p.x - a.x) / (b.x - a.x);
		float ky = (p.y - a.y) / (b.y - a.y);

		return kx >= 0.0f && kx <= 1.0f && ky >= 0.0f && ky <= 1.0f && abs(kx - ky) < 0.001f;
	}

	inline bool IsCircleSquareIntersection(
		const Vec2f &rectPosition, float rectSize, const Vec2f &circle, float radius)
	{
		const auto r = rectPosition.x + rectSize;
		const auto b = rectPosition.y + rectSize;
		// find the closest point to the circle within the rectangle
		const float closestX = Math::Clamp(circle.x, rectPosition.x, r);
		const float closestY = Math::Clamp(circle.y, rectPosition.y, b);
		// calculate the distance between the circle's center and this closest point
		const float distanceX = circle.x - closestX;
		const float distanceY = circle.y - closestY;
		// if the distance is less than the circle's radius, an intersection occurs
		const float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
		return distanceSquared < (radius * radius);
	}

	inline bool IsCircleRectIntersection(
		const Vec2f &rectPosition, const Vec2f &rectSize, const Vec2f &circle, float radius)
	{
		const auto r = rectPosition.x + rectSize.x;
		const auto b = rectPosition.y + rectSize.y;
		// find the closest point to the circle within the rectangle
		const float closestX = Math::Clamp(circle.x, rectPosition.x, r);
		const float closestY = Math::Clamp(circle.y, rectPosition.y, b);
		// calculate the distance between the circle's center and this closest point
		const float distanceX = circle.x - closestX;
		const float distanceY = circle.y - closestY;
		// if the distance is less than the circle's radius, an intersection occurs
		const float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
		return distanceSquared < (radius * radius);
	}

	inline bool IsSquareRectIntersection(const Vec2f &rectPosition, float rectSize,
		const Vec2f &otherRectPosition, const Vec2f &otherRectSize)
	{
		if (otherRectPosition.x < rectPosition.x + rectSize &&
			rectPosition.x < otherRectPosition.x + otherRectSize.x &&
			otherRectPosition.y < rectPosition.y + rectSize)
		{
			return rectPosition.y < otherRectPosition.y + otherRectSize.y;
		}
		else
		{
			return false;
		}
	}

	inline bool IsRectRectIntersection(const Vec2f &rectPosition, const Vec2f &rectSize,
		const Vec2f &otherRectPosition, const Vec2f &otherRectSize)
	{
		if (otherRectPosition.x < rectPosition.x + rectSize.x &&
			rectPosition.x < otherRectPosition.x + otherRectSize.x &&
			otherRectPosition.y < rectPosition.y + rectSize.y)
		{
			return rectPosition.y < otherRectPosition.y + otherRectSize.y;
		}
		else
		{
			return false;
		}
	}

	inline bool IsPointInsideRectangle(const Vec2f &point, const Vec2f &position, const Vec2f &size)
	{
		return (point.x > position.x) && (point.x < position.x + size.x) && (point.y > position.y) &&
			(point.y < position.y + size.y);
	}

	inline bool IsPointInsideCircle(const Vec2f &p, const Vec2f &circlePosition, float sqrCircleRadius)
	{
		return SqrLength(p - circlePosition) <= sqrCircleRadius;
	}

	inline Vec2f ProjectVectorNorm(const Vec2f &a, const Vec2f &planeNormal)
	{
		return a - planeNormal * Dot(a, planeNormal);
	}

	inline Vec2f ProjectVector(const Vec2f &a, const Vec2f &planeNormal)
	{
		const float nlen = SqrLength(planeNormal);
		// normal vector is degenerated
		if (nlen < 0.0001f)
		{
			return Vec2f(0, -0.09f);
		}
		const float t = Math::Dot(a, planeNormal) / nlen;
		return a - planeNormal * t;
	}

	inline float GetLineSlopeDegrees(const Vec2f &a, const Vec2f &b)
	{
		auto d = a - b;
		return Math::Rad2Deg(atan(d.y / d.x));
	}

	inline bool IsRayIntersectRect(const Vec2f &origin, const Vec2f &direction, const Vec2f &min,
		const Vec2f &max, float &t0, float &t1)
	{
		const Vec2f invDir = { 1.0f / direction.x, 1.0f / direction.y };
		float tmin, tmax;
		if (direction.x >= 0)
		{
			tmin = (min.x - origin.x) * invDir.x;
			tmax = (max.x - origin.x) * invDir.x;
		}
		else
		{
			tmin = (max.x - origin.x) * invDir.x;
			tmax = (min.x - origin.x) * invDir.x;
		}
		float tymin, tymax;
		if (direction.y >= 0)
		{
			tymin = (min.y - origin.y) * invDir.y;
			tymax = (max.y - origin.y) * invDir.y;
		}
		else
		{
			tymin = (max.y - origin.y) * invDir.y;
			tymax = (min.y - origin.y) * invDir.y;
		}
		if ((tmin > tymax) || (tymin > tmax))
		{
			return false;
		}
		if (tymin > tmin)
		{
			tmin = tymin;
		}
		if (tymax < tmax)
		{
			tmax = tymax;
		}
		t0 = tmin;
		t1 = tmax;
		return ((tmin < 1.0f) && (tmax > 0.0f));
	}

	inline bool IsRayIntersectCircle(const Vec2f &origin, const Vec2f &dir, const Vec2f &circlePosition,
		float circleRadius, Vec2f &intA, Vec2f &intB)
	{
		const Vec2f direction = Normalized(dir);
		const Vec2f d = origin - circlePosition;
		const float b = 2.0f * Dot(direction, d);
		const float c = Dot(d, d) - circleRadius * circleRadius;
		const float discriminant = b * b - 4.0f * c;
		if (discriminant < 0.0f)
		{
            intA = { FLT_MAX, FLT_MAX };
            intB = { FLT_MAX, FLT_MAX };
            return false;
		}
		const float discrRoot = sqrt(discriminant);
		const float r1 = (-b + discrRoot) * 0.5f;
		const float r2 = (-b - discrRoot) * 0.5f;
		if (r1 < 0 || r2 < 0)
		{
            intA = { FLT_MAX, FLT_MAX };
            intB = { FLT_MAX, FLT_MAX };
            return false;
		}
		intA = origin + direction * r1;
		intB = origin + direction * r2;
		return true;
	}

	// Based on an algorithm in Andre LeMothe's "Tricks of the Windows Game
	// Programming Gurus"
	inline bool IsLineIntersectLine(
		const Vec2f &p0, const Vec2f &p1, const Vec2f &p2, const Vec2f &p3, Vec2f &out)
	{
		const Vec2f s1 = p1 - p0;
		const Vec2f s2 = p3 - p2;

		float s = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / (-s2.x * s1.y + s1.x * s2.y);
		float t = (s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / (-s2.x * s1.y + s1.x * s2.y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			out = p0 + (t * s1);
			return true;
		}

		return false;
	}

	template <class T> inline T Wrap(T kX, T const kLowerBound, T const kUpperBound)
	{
		int range_size = kUpperBound - kLowerBound + 1;
		if (kX < kLowerBound)
		{
			kX += range_size * ((kLowerBound - kX) / range_size + 1);
		}
		return kLowerBound + (kX - kLowerBound) % range_size;
	}

	// original source code from HPL Engine by Frictional Games
	// bug fixed by Dmitriy Stepanov
	template <> inline float Wrap(float n, float minLimit, float maxLimit)
	{
		if (n >= minLimit && n <= maxLimit)
		{
			return n;
		}

		if (maxLimit == 0.0f && minLimit == 0.0f)
		{
			return 0.0f;
		}

		maxLimit = maxLimit - minLimit;

		float fOffSet = minLimit;
		minLimit = 0;
		n = n - fOffSet;

		float fNumOfMax = floor(abs(n / maxLimit));

		if (n >= maxLimit)
		{
			n = n - fNumOfMax * maxLimit;
		}
		else if (n < minLimit)
		{
			n = ((fNumOfMax + 1.0f) * maxLimit) + n;
		}

		return n + fOffSet;
	}

	inline float ShortAngleDistance(float a0, float a1)
	{
		float da = fmod((a1 - a0), 360.0f);
		return fmod((2.0f * da), 360.0f) - da;
	}

	inline float AngleLerp(float a0, float a1, float t)
	{
		return a0 + ShortAngleDistance(a0, a1) * t;
	}

	inline bool RoughlyEqual(float a, float b, float accuracy = 0.025f)
	{
		return abs(a - b) <= accuracy;
	}

	inline float Distance(const Vec2f &a, const Vec2f &b)
	{
		return Length(a - b);
	}

	inline float SqrDistance(const Vec2f &a, const Vec2f &b)
	{
		return SqrLength(a - b);
	}

	// Inflates rect around center
	template<class T>
	inline sf::Rect<T> Inflate(const sf::Rect<T>& rect, T x, T y)
	{
		return {
			rect.left - 2 * x,
			rect.top - 2 * y,
			rect.width + x,
			rect.height + y
		};
	}

	inline float Snap(float num, const float step)
	{
		const float lowSnapBound = num - fmod(num, step);
		const float highSnapBound = lowSnapBound + step;
		const float centerSnapBound = (lowSnapBound + highSnapBound) * 0.5f;
		return num > centerSnapBound ? highSnapBound : lowSnapBound;
	}

	inline bool IsCircleCircleIntersection(const Vec2f &posA, float radiusA, const Vec2f &posB, float radiusB)
	{
		const float d = radiusA + radiusB;
		return SqrDistance(posA, posB) < d * d;
	}

	struct PolarCoordinates
	{
		float radius;
		float mAngle; // in degrees
	};



	inline PolarCoordinates CartesianToPolar(const Vec2f &cartesian)
	{
		PolarCoordinates polar;
		polar.radius = Length(cartesian);
		polar.mAngle = atan2f(cartesian.y, cartesian.x) * 180.0f / Pi;
		return polar;
	}

	inline float Determinant(float a, float b, float c, float d)
	{
		return a * d - b * c;
	}

	struct Edge
	{
		Vec2f mA; // begin
		Vec2f mB; // end

		Vec2f GetClosestPointTo(const Vec2f & pt)
		{
			if (Math::SqrDistance(pt, mA) < Math::SqrDistance(pt, mB))
			{
				return mA;
			}
			return mB;
		}
	};

	class Polygon
	{
	private:
		vector<Vec2f> mPoints;

	public:
		Polygon()
		{

		}

		~Polygon()
		{

		}

		void AddPoint(const Vec2f & pt)
		{
			mPoints.push_back(pt);
		}

		Vec2f Center() const
		{
			Vec2f sum;
			for (auto & pt : mPoints)
			{
				sum += pt;
			}
			const float invSize = 1.0f / mPoints.size();
			return sum * invSize;
		}

		Vec2f GetClosestPointTo(const Vec2f & pt)
		{
			if (mPoints.empty())
			{
				return { 0, 0 };
			}

			float closestSqrDistance = FLT_MAX;
			int closestIndex = -1;
			for (size_t i = 0; i < mPoints.size(); ++i)
			{
				float d = Math::SqrDistance(mPoints[i], pt);
				if (d < closestSqrDistance)
				{
					closestSqrDistance = d;
					closestIndex = i;
				}
			}

			return mPoints[closestIndex];
		}

		// https://wrf.ecse.rpi.edu//Research/Short_Notes/pnpoly.html
		bool Contains(const Vec2f & point) const
		{
			bool c = false;
			for (size_t i = 0, j = mPoints.size() - 1; i < mPoints.size(); j = i++)
			{
				if (((mPoints[i].y > point.y) != (mPoints[j].y > point.y)) &&
					(point.x < (mPoints[j].x - mPoints[i].x) * (point.y - mPoints[i].y) / (mPoints[j].y - mPoints[i].y) + mPoints[i].x))
				{
					c = !c;
				}
			}
			return c;
		}

		vector<Vec2f> & GetPoints()
		{
			return mPoints;
		}

		void GetBounds(Vec2f & min, Vec2f & max) const
		{
			min = { FLT_MAX, FLT_MAX };
			max = { -FLT_MAX, -FLT_MAX };

			for (auto & pt : mPoints)
			{
				if (pt.x > max.x)
				{
					max.x = pt.x;
				}
				if (pt.y > max.y)
				{
					max.y = pt.y;
				}
				if (pt.x < min.x)
				{
					min.x = pt.x;
				}
				if (pt.y < min.y)
				{
					min.y = pt.y;
				}
			}
		}

		int GetPointCount() const
		{
			return mPoints.size();
		}

		// Scale polygon using origin point
		void UniformScale(const Vec2f & origin, float scale)
		{
			for (auto & pt : mPoints)
			{
				Vec2f d = scale * (pt - origin);
				pt = d + origin;
			}
		}

		Vec2f GetPoint(int i) const
		{
			return mPoints[i];
		}

		void Serialize(Serializer & sav)
		{
			sav & mPoints;
		}
	};

	inline Vec2f RandomPoint(const sf::FloatRect & rect)
	{
		return { Rnd(rect.left, rect.left + rect.width), Rnd(rect.top, rect.top + rect.height) };
	}

	inline Vec2f RandomPoint(const Vec2f & min, const Vec2f & max)
	{
		return { Rnd(min.x, max.x), Rnd(min.y, max.y) };
	}

	// Brute-force algorithm, make it better :)
	inline Vec2f RandomPoint(const Polygon & poly)
	{
		Vec2f min, max;
		poly.GetBounds(min, max);

		int tryCount = 50;

		Vec2f point;

		do
		{
			point = RandomPoint(min, max);

			if (poly.Contains(point))
			{
				break;
			}
		} while (--tryCount > 0);

		return point;
	}

	class Triangle final
	{
	public:
		Vec2f mPoints[3];

		Vec2f Center() const
		{
			return (mPoints[0] + mPoints[1] + mPoints[2]) * 0.333333f;
		}

		Vec2f GetClosestPointTo(const Vec2f & pt)
		{
			float closestSqrDistance = FLT_MAX;
			int closestIndex = -1;
			for (size_t i = 0; i < 3; ++i)
			{
				float d = Math::SqrDistance(mPoints[i], pt);
				if (d < closestSqrDistance)
				{
					closestSqrDistance = d;
					closestIndex = i;
				}
			}

			return mPoints[closestIndex];
		}


		Triangle()
		{
		}

		Triangle(const Vec2f &a, const Vec2f &b, const Vec2f &c) : mPoints { a, b, c }
		{
		}

		inline bool IsPointInside(const Vec2f &pt) const noexcept
		{
			const auto v0 = mPoints[2] - mPoints[0];
			const auto v1 = mPoints[1] - mPoints[0];
			const auto v2 = pt - mPoints[0];

			const float d00 = Math::Dot(v0, v0);
			const float d01 = Math::Dot(v0, v1);
			const float d11 = Math::Dot(v1, v1);
			const float d20 = Math::Dot(v2, v0);
			const float d21 = Math::Dot(v2, v1);
			const float denom = d00 * d11 - d01 * d01;

			if (abs(denom) <= Math::Epsilon)
			{
				return false;
			}

			const float invDenom = 1.0f / denom;

			const float v = (d11 * d20 - d01 * d21) * invDenom;
			const float w = (d00 * d21 - d01 * d20) * invDenom;
			const float u = 1.0f - v - w;

			return (u >= 0.0f) && (v >= 0.0f) && (u + v < 1.0f);
		}

		inline bool IsIntersectCircle(const Vec2f &circlePos, float radius) const noexcept
		{
			// case 1: any point of triangle is inside circle
			const bool anyVertexInside = Math::IsPointInsideCircle(mPoints[0], circlePos, radius) ||
				Math::IsPointInsideCircle(mPoints[1], circlePos, radius) ||
				Math::IsPointInsideCircle(mPoints[2], circlePos, radius);
			if (anyVertexInside)
			{
				return true;
			}

			// case 2: circle's center is inside triangle
			const bool circleCenterInside = IsPointInside(circlePos);
			if (circleCenterInside)
			{
				return true;
			}

			// case 3 (worst speed): circle touch triangle
			const float sqrRadius = Math::Sqr(radius);

			// define triangle's edges
			const Edge edges[] = {
				{ mPoints[0], mPoints[1] }, { mPoints[1], mPoints[2] }, { mPoints[2], mPoints[0] } };

			for (auto &edge : edges)
			{
				// project circle position onto each egde
				const auto projection = Math::ProjectPointOnLine(circlePos, edge.mA, edge.mB);

				// if projection lies on edge
				if (Math::IsPointOnLine(projection, edge.mA, edge.mB))
				{
					// check distance to projection
					if (Math::SqrDistance(projection, circlePos) <= sqrRadius)
					{
						// if less than circle's radius, we have intersection
						return true;
					}
				}
			}

			return false;
		}

		void Serialize(Serializer & sav)
		{
			sav & mPoints[0];
			sav & mPoints[1];
			sav & mPoints[2];
		}
	};

} // namespace Math
