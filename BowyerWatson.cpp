#include "BowyerWatson.h"

#include <algorithm>

namespace Triangularization
{
	namespace TriangularizationPrivate
	{
		const float squareRootOfTree = sqrt(3.f);
	}

	BowyerWatson::BowyerWatson(const std::vector<sf::Vector2f>& aPointsList)
		: mySuperTriangle(0.f)
		, myPointsList(aPointsList)
	{
	}

	void BowyerWatson::GenerateTriangulation()
	{
		GenerateSuperTriangle();

		GenerateTriangleMap();

		CleanMapFromSuperTriangle();
	}

	void BowyerWatson::GenerateSuperTriangle()
	{
		CalculateMidPoint();

		CalculateContainingCircle();

		CalculateSuperTriangleVertexes();
	}

	void BowyerWatson::GenerateTriangleMap()
	{
		myTriangleList.clear();
		myTriangleList.push_back(mySuperTriangle);

		for (const auto& point : myPointsList)
		{
			auto badTriangles = GenerateAndRemoveBadTriangles(point);

			auto poligon = GeneratePoligon(badTriangles);

			CreateNewTriangleInTriangleList(point, poligon);
		}
	}

	void BowyerWatson::CleanMapFromSuperTriangle()
	{
		for (const auto& vertex : mySuperTriangle.GetVertexes())
		{
			auto lastValidIterator = std::remove_if(myTriangleList.begin(), myTriangleList.end(),
				[&vertex](const auto& aTriangle) {return aTriangle.ContainVertex(vertex); });

			myTriangleList.erase(lastValidIterator, myTriangleList.end());
		}
	}

	std::vector<Triangle> BowyerWatson::GenerateAndRemoveBadTriangles(const sf::Vector2f& aPoint)
	{
		std::vector<Triangle> badTriangles;

		auto triangleIt = --myTriangleList.end();
		auto triangleStart = myTriangleList.begin();

		while (triangleIt != triangleStart)
		{
			if (triangleIt->IsPointInsideIncircle(aPoint))
			{
				badTriangles.push_back(*triangleIt);
				auto triangleToErase = triangleIt--;
				myTriangleList.erase(triangleToErase);
			}
			else
			{
				--triangleIt;
			}
		}

		if (triangleStart->IsPointInsideIncircle(aPoint))
		{
			badTriangles.push_back(*triangleStart);
			myTriangleList.erase(triangleStart);
		}

		return badTriangles;
	}

	void BowyerWatson::CreateNewTriangleInTriangleList(const sf::Vector2f& aPoint, const std::vector<Edge>& somePoligons)
	{
		for (const auto& poligon : somePoligons)
		{
			const auto firstSeg = Mathematics::CalculateNormalizedVector(poligon.first - aPoint);
			const auto secondSeg = Mathematics::CalculateNormalizedVector(poligon.second - aPoint);
			const auto dot = Mathematics::DotProduct(firstSeg, secondSeg);

			auto newTriangle = Triangle{ aPoint, poligon.first, poligon.second };

			if ((dot != 1.f && dot != -1.f) && newTriangle.CalculateDelta() > 0)
			{
				myTriangleList.push_back(newTriangle);
			}
		}
	}

	std::vector<Edge> BowyerWatson::GeneratePoligon(const std::vector<Triangle>& someBadTriangles) const
	{
		std::vector<Edge> poligon;

		if (someBadTriangles.size() == 1)
		{
			const auto& triangleToAdd = someBadTriangles[0];
			for (const auto& edge : triangleToAdd.GetEdges())
			{
				poligon.push_back(edge);
			}
		}
		else
		{
			for (int triangleIdx = 0; triangleIdx < someBadTriangles.size(); ++triangleIdx)
			{
				const auto& triangleToCheck = someBadTriangles[triangleIdx];

				for (const auto& edge : triangleToCheck.GetEdges())
				{
					auto sharedEdgeTriangle = std::find_if(someBadTriangles.begin(), someBadTriangles.end(),
						[&edge, &triangleToCheck](const auto& aBadTriangle) {return aBadTriangle != triangleToCheck && aBadTriangle.ShareEdge(edge); });
					
					if (sharedEdgeTriangle == someBadTriangles.end())
					{
						poligon.push_back(edge);
					}
				}
			}
		}


		return poligon;
	}

	void BowyerWatson::CalculateMidPoint()
	{
		myMidPoint = Mathematics::CalculateCentroid(myPointsList);
	}

	void BowyerWatson::CalculateContainingCircle()
	{
		auto farthestPos = myMidPoint;
		auto distance = 0.f;

		for (const auto& point : myPointsList)
		{
			const auto newPos = point;
			const auto newDistance = Mathematics::CalculateDistance(newPos, farthestPos);
			if (newDistance > distance)
			{
				distance = newDistance;
				farthestPos = newPos;
			}
		}

		myCircleRadius = distance;
	}

	void BowyerWatson::CalculateSuperTriangleVertexes()
	{
		const auto triangleHeight = myCircleRadius * 3;

		const auto firstVertex = myMidPoint + (sf::Vector2f(1.f, 0.f) * (triangleHeight / 2));
		const auto baseMidPoint = myMidPoint + (sf::Vector2f(-1.f, 0.f) * (triangleHeight / 2));

		const auto sideLenght = (triangleHeight * 2) / TriangularizationPrivate::squareRootOfTree;

		const auto secondVertex = baseMidPoint + (sf::Vector2f(0.f, 1.f) * (sideLenght / 2));
		const auto thirdVertex = baseMidPoint + (sf::Vector2f(0.f, -1.f) * (sideLenght / 2));;

		mySuperTriangle = Triangle(firstVertex, secondVertex, thirdVertex);
	}
}

