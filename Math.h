#pragma once

#include <algorithm>
#include <vector>
#include <array>

#include <SFML/System/Vector2.hpp>

namespace Mathematics
{

	sf::Vector2f CalculateNormalizedVector(const sf::Vector2f& aFirstVector);

	sf::Vector2f CalculateCentroid(const std::vector<sf::Vector2f>& somePoints);

	float CalculateMagnitude(const sf::Vector2f& aFirstVector);

	float CalculateDistance(const sf::Vector2f& aVertex1, const sf::Vector2f& aVertex2);

	float CalculateDeterminant(const sf::Vector2f& aVertex1, const sf::Vector2f& aVertex2);

	float CalculateCircumradius(const sf::Vector2f& aVertex1, const sf::Vector2f& aVertex2, const sf::Vector2f& aVertex3);

	sf::Vector2f CalculateCircumcenter(const sf::Vector2f& aVertex1, const sf::Vector2f& aVertex2, const sf::Vector2f& aVertex3);

	bool LineVsLine(const sf::Vector2f& aFirstStartPos, const sf::Vector2f& aFirstEndPos, const sf::Vector2f& aSecondStartPos, const sf::Vector2f& aSecondEndPos);

	bool PointInsideCircle(const sf::Vector2f& aCenter, float aRadius, const sf::Vector2f& aPointToCheck);

	float DotProduct(const sf::Vector2f& aFirstVector, const sf::Vector2f& aSecondVector);
}

//Test

