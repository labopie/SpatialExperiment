#pragma once

#include <SFML/System/Vector2.hpp>
#include <utility>
#include <array>

using Edge = std::pair<sf::Vector2f, sf::Vector2f>;

class Triangle
{
public:
	explicit Triangle(float aVertexDefaultValue);

	Triangle(const sf::Vector2f& aFirstVertex, const sf::Vector2f& aSecondVertex, const sf::Vector2f& aThirdVertex);

	bool operator==(const Triangle& anotherTriangle) const;

	bool operator!=(const Triangle& anotherTriangle) const;

	bool ContainVertex(const Triangle& anotherTriangle) const;
	bool ContainVertex(const sf::Vector2f& aVertexToCheck) const;

	bool ShareEdge(const Triangle& anotherTriangle) const;
	bool ShareEdge(const Edge& anEdgeToCheck) const;

	const Edge& GetSharedEdge(const Triangle& anotherTriangle) const;

	bool IsPointInsideIncircle(const sf::Vector2f& aPointToCheck) const;

	float CalculateCircumRadius() const;

	float CalculateDelta() const;

	const sf::Vector2f GetCircumCenter() const { return myCircumcenter; }

	const sf::Vector2f& GetVertex(int index) const { return myVertexes[index]; }
	const std::array<sf::Vector2f, 3>& GetVertexes() const { return myVertexes; }

	const std::array<Edge, 3>& GetEdges() const { return myEdges; }

private:

	sf::Vector2f CalculateCircumCenter() const;

	std::array<sf::Vector2f, 3> myVertexes;
	std::array<Edge, 3> myEdges;

	sf::Vector2f myCircumcenter;
};