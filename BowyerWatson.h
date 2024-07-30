#pragma once

#include "Voronoi.h"

#include "Triangle.h"

#include <SFML/System/Vector2.hpp>

#include <vector>

namespace Triangularization
{

	class BowyerWatson
	{

	public:
		explicit BowyerWatson(const std::vector<sf::Vector2f>& aPointsList);

		void GenerateTriangulation();

		const std::vector<Triangle>& GetTriangleList() const { return myTriangleList; }

		const Triangle& GetSuperTriangle() const { return mySuperTriangle; }

	private:

		void GenerateSuperTriangle();
		void GenerateTriangleMap();
		void CleanMapFromSuperTriangle();

		std::vector<Triangle> GenerateAndRemoveBadTriangles(const sf::Vector2f& aPoint);
		void CreateNewTriangleInTriangleList(const sf::Vector2f& aPoint, const std::vector<Edge>& somePoligons);

		std::vector<Edge> GeneratePoligon(const std::vector<Triangle>& someBadTriangles) const;

		void CalculateMidPoint();
		void CalculateContainingCircle();
		void CalculateSuperTriangleVertexes();

		Triangle mySuperTriangle;
		sf::Vector2f myMidPoint;
		float myCircleRadius;
		std::vector<Triangle> myTriangleList;
		std::vector<sf::Vector2f> myPointsList;

	};

}



