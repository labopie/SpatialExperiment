#include "Triangle.h"

#include "Math.h"

Triangle::Triangle(float aVertexDefaultValue)
{
	const auto defaultValue = sf::Vector2f(aVertexDefaultValue, aVertexDefaultValue);
	myVertexes.fill(defaultValue);
}

Triangle::Triangle(const sf::Vector2f& aFirstVertex, const sf::Vector2f& aSecondVertex, const sf::Vector2f& aThirdVertex)
{
	myVertexes[0] = aFirstVertex;
	myVertexes[1] = aSecondVertex;
	myVertexes[2] = aThirdVertex;

	myEdges[0] = { myVertexes[0], myVertexes[1] };
	myEdges[1] = { myVertexes[1], myVertexes[2] };
	myEdges[2] = { myVertexes[0], myVertexes[2] };
}

bool Triangle::operator==(const Triangle& anotherTriangle) const
{
	for (const auto& vertex : anotherTriangle.myVertexes)
	{
		if (!ContainVertex(vertex))
		{
			return false;
		}
	}

	return true;
}

bool Triangle::operator!=(const Triangle& anotherTriangle) const
{
	return !(*this == anotherTriangle);
}

bool Triangle::ContainVertex(const sf::Vector2f& aVertexToCheck) const
{
	for (const auto& vertex : myVertexes)
	{
		if (Mathematics::CalculateDistance(aVertexToCheck, vertex) < 0.1f)
		{
			return true;
		}
	}

	return false;
}

bool Triangle::ShareEdge(const Edge& anEdgeToCheck) const
{
	for (const auto& edge : myEdges)
	{
		if ((Mathematics::CalculateDistance(edge.first, anEdgeToCheck.first) < 0.1f) && (Mathematics::CalculateDistance(edge.second, anEdgeToCheck.second) < 0.1f))
		{
			return true;
		}
		if ((Mathematics::CalculateDistance(edge.first, anEdgeToCheck.second) < 0.1f) && (Mathematics::CalculateDistance(edge.second, anEdgeToCheck.first) < 0.1f))
		{
			return true;
		}
	}
	return false;
}

bool Triangle::IsPointInsideIncircle(const sf::Vector2f& aPointToCheck) const
{
	const auto radius = CalculateCircumRadius();
	const auto center = CalculateCircumCenter();

	return Mathematics::CalculateDistance(aPointToCheck, center) < radius;
}

float Triangle::CalculateCircumRadius() const
{
	const auto segmentA = Mathematics::CalculateDistance(myVertexes[0], myVertexes[1]);
	const auto segmentB = Mathematics::CalculateDistance(myVertexes[1], myVertexes[2]);
	const auto segmentC = Mathematics::CalculateDistance(myVertexes[0], myVertexes[2]);

	const auto s = (segmentA + segmentB + segmentC) * 0.5f;
	auto delta = 4.f * sqrt(s * (segmentA + segmentB - s) * (segmentB + segmentC - s) * (segmentA + segmentC - s));

	return (segmentA * segmentB * segmentC) / delta;
}

float Triangle::CalculateDelta() const
{
	const auto segmentA = Mathematics::CalculateDistance(myVertexes[0], myVertexes[1]);
	const auto segmentB = Mathematics::CalculateDistance(myVertexes[1], myVertexes[2]);
	const auto segmentC = Mathematics::CalculateDistance(myVertexes[0], myVertexes[2]);

	const auto s = (segmentA + segmentB + segmentC) * 0.5f;

	return 4.f * sqrt(s * (segmentA + segmentB - s) * (segmentB + segmentC - s) * (segmentA + segmentC - s));
}

sf::Vector2f Triangle::CalculateCircumCenter() const
{
	sf::Vector2f circumCenter;

	const float dx = myVertexes[1].x - myVertexes[0].x;
	const float dy = myVertexes[1].y - myVertexes[0].y;
	const float ex = myVertexes[2].x - myVertexes[0].x;
	const float ey = myVertexes[2].y - myVertexes[0].y;

	const float bl = dx * dx + dy * dy;
	const float cl = ex * ex + ey * ey;
	// ABELL - This is suspect for div-by-0.
	const float d = dx * ey - dy * ex;

	circumCenter.x = myVertexes[0].x + (ey * bl - dy * cl) * 0.5f / d;
	circumCenter.y = myVertexes[0].y + (dx * cl - ex * bl) * 0.5f / d;

	return circumCenter;
}
