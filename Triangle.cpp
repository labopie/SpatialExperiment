#include "Triangle.h"

#include "Math.h"

const static Edge theInvalidEdge = std::make_pair(sf::Vector2f{ -999.f, -999.f }, sf::Vector2f{ -999.f, -999.f });

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

	myCircumcenter = CalculateCircumCenter();
}

bool Triangle::operator==(const Triangle& anotherTriangle) const
{
	return
		(Mathematics::CalculateDistance(myVertexes[0], anotherTriangle.myVertexes[0]) < 0.1f || Mathematics::CalculateDistance(myVertexes[0], anotherTriangle.myVertexes[1]) < 0.1f || Mathematics::CalculateDistance(myVertexes[0], anotherTriangle.myVertexes[2]) < 0.1f) &&
		(Mathematics::CalculateDistance(myVertexes[1], anotherTriangle.myVertexes[0]) < 0.1f || Mathematics::CalculateDistance(myVertexes[1], anotherTriangle.myVertexes[1]) < 0.1f || Mathematics::CalculateDistance(myVertexes[1], anotherTriangle.myVertexes[2]) < 0.1f) &&
		(Mathematics::CalculateDistance(myVertexes[2], anotherTriangle.myVertexes[0]) < 0.1f || Mathematics::CalculateDistance(myVertexes[2], anotherTriangle.myVertexes[1]) < 0.1f || Mathematics::CalculateDistance(myVertexes[2], anotherTriangle.myVertexes[2]) < 0.1f);
}

bool Triangle::operator!=(const Triangle& anotherTriangle) const
{
	return !(*this == anotherTriangle);
}

bool Triangle::ShareEdge(const Triangle& anotherTriangle) const
{
	for (const auto& edge : myEdges)
	{
		if (anotherTriangle.ShareEdge(edge))
		{
			return true;
		}
	}

	return false;
}

const Edge& Triangle::GetSharedEdge(const Triangle& anotherTriangle) const
{
	for (const auto& edge : myEdges)
	{
		if (anotherTriangle.ShareEdge(edge))
		{
			return edge;
		}
	}

	return theInvalidEdge;
}

bool Triangle::ContainVertex(const Triangle& anotherTriangle) const
{
	for (const auto& edge : myEdges)
	{
		if (anotherTriangle.ContainVertex(edge.first) || anotherTriangle.ContainVertex(edge.second))
		{
			return true;
		}
	}

	return false;
}

bool Triangle::ContainVertex(const sf::Vector2f& aVertexToCheck) const
{
	return Mathematics::CalculateDistance(aVertexToCheck, myVertexes[0]) < 0.1f || 
		   Mathematics::CalculateDistance(aVertexToCheck, myVertexes[1]) < 0.1f ||
		   Mathematics::CalculateDistance(aVertexToCheck, myVertexes[2]) < 0.1f;
}

bool Triangle::ShareEdge(const Edge& anEdgeToCheck) const
{
	return
		Mathematics::CalculateDistance(myEdges[0].first, anEdgeToCheck.first) < 0.1f && Mathematics::CalculateDistance(myEdges[0].second, anEdgeToCheck.second) < 0.1f ||
			Mathematics::CalculateDistance(myEdges[0].first, anEdgeToCheck.second) < 0.1f && Mathematics::CalculateDistance(myEdges[0].second, anEdgeToCheck.first) < 0.1f ||
				Mathematics::CalculateDistance(myEdges[1].first, anEdgeToCheck.first) < 0.1f && Mathematics::CalculateDistance(myEdges[1].second, anEdgeToCheck.second) < 0.1f ||
					Mathematics::CalculateDistance(myEdges[1].first, anEdgeToCheck.second) < 0.1f && Mathematics::CalculateDistance(myEdges[1].second, anEdgeToCheck.first) < 0.1f ||
						Mathematics::CalculateDistance(myEdges[2].first, anEdgeToCheck.first) < 0.1f && Mathematics::CalculateDistance(myEdges[2].second, anEdgeToCheck.second) < 0.1f ||
							Mathematics::CalculateDistance(myEdges[2].first, anEdgeToCheck.second) < 0.1f && Mathematics::CalculateDistance(myEdges[2].second, anEdgeToCheck.first) < 0.1f;
}

bool Triangle::IsPointInsideIncircle(const sf::Vector2f& aPointToCheck) const
{
	const auto radius = CalculateCircumRadius();

	return Mathematics::CalculateDistance(aPointToCheck, myCircumcenter) < radius;
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
