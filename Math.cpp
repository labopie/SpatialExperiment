#include "Math.h"

#include <limits>

namespace Mathematics
{
	float CalculateMagnitude(const sf::Vector2f& aFirstVector)
	{
		return sqrt(aFirstVector.x * aFirstVector.x) + (aFirstVector.y * aFirstVector.y);
	}

    float CalculateDistance(const sf::Vector2f& aVertex1, const sf::Vector2f& aVertex2)
    {
        float xSquared = (aVertex2.x - aVertex1.x) * (aVertex2.x - aVertex1.x);
        float ySquared = (aVertex2.y - aVertex1.y) * (aVertex2.y - aVertex1.y);

        return static_cast<float>(sqrt(xSquared + ySquared));
    }

	float CalculateDeterminant(const sf::Vector2f& aVertex1, const sf::Vector2f& aVertex2)
	{
		return (aVertex1.x * aVertex2.y) - (aVertex1.y * aVertex2.x);
	}

    //I'm using this formula:
    // s = (a+b+c) / 2
    // r = (a * b*c) / 4 * sqrt(s*(a+b-s)*(a+c-s)*(b+c-s))
    float CalculateCircumradius(const sf::Vector2f& aVertex1, const sf::Vector2f& aVertex2, const sf::Vector2f& aVertex3)
    {
        float a = CalculateDistance(aVertex1, aVertex2);
        float b = CalculateDistance(aVertex1, aVertex3);
        float c = CalculateDistance(aVertex2, aVertex3);

        float s = (a + b + c) * 0.5f;

        float temp = s * (a + b - s) * (b + c - s) * (a + c - s);
        float squareRoot = 4 * sqrt(temp);
        float radius = (a * b * c) / squareRoot;

        return radius;
    }

    sf::Vector2f CalculateNormalizedVector(const sf::Vector2f& aVector)
    {
        const auto squaredLength = aVector.x * aVector.x + aVector.y * aVector.y;

        if (squaredLength > 0.f)
        {
            const auto invLen = 1.f / sqrt(squaredLength);
            return sf::Vector2f(aVector.x * invLen, aVector.y * invLen);
        }

        return aVector;
    }

    sf::Vector2f CalculateCentroid(const std::vector<sf::Vector2f>& somePoints)
    {
        sf::Vector2f centroid = somePoints[0];

        for (int idx = 1; idx < somePoints.size(); ++idx)
        {
            centroid.x += somePoints[idx].x;
            centroid.y += somePoints[idx].y;
        }

        centroid.x /= somePoints.size();
        centroid.y /= somePoints.size();

        return centroid;
    }

    sf::Vector2f CalculateCircumcenter(const sf::Vector2f& aVertex1, const sf::Vector2f& aVertex2, const sf::Vector2f& aVertex3)
    {
        const float dx = aVertex2.x - aVertex1.x;
        const float dy = aVertex2.y - aVertex1.y;
        const float ex = aVertex3.x - aVertex1.x;
        const float ey = aVertex3.y - aVertex1.y;

        const float bl = dx * dx + dy * dy;
        const float cl = ex * ex + ey * ey;
        // ABELL - This is suspect for div-by-0.
        const float d = dx * ey - dy * ex;

        const double x = aVertex1.x + (ey * bl - dy * cl) * 0.5 / d;
        const double y = aVertex1.y + (dx * cl - ex * bl) * 0.5 / d;

        return sf::Vector2f(x, y);
    }

    bool LineVsLine(const sf::Vector2f& aFirstStartPos, const sf::Vector2f& aFirstEndPos, const sf::Vector2f& aSecondStartPos, const sf::Vector2f& aSecondEndPos)
    {
        const sf::Vector2f s1 = aFirstEndPos - aFirstStartPos;
        const sf::Vector2f s2 = aSecondEndPos - aSecondStartPos;
        const sf::Vector2f p = aFirstStartPos - aSecondStartPos;

        float fDet = (-s2.x * s1.y + s1.x * s2.y);
        if (fDet == 0)
        { 
            return false;
        }

        float invfDet = 1.0f / fDet;
        float s = (-s1.y * p.x + s1.x * p.y) * invfDet;
        float t = (s2.x * p.y - s2.y * p.x) * invfDet;

        bool isIntersectingFirstLineSegment = (0.0f <= t && t <= 1.0f);
        bool isIntersectingSecondLineSegment = (0.0f <= s && s <= 1.0f);

        return isIntersectingFirstLineSegment && isIntersectingSecondLineSegment;
    }

    bool PointInsideCircle(const sf::Vector2f& aCenter, float aRadius, const sf::Vector2f& aPointToCheck)
    {
        return CalculateDistance(aCenter, aPointToCheck) <= aRadius;
    }

    float DotProduct(const sf::Vector2f& aFirstVector, const sf::Vector2f& aSecondVector)
    {
        return (aFirstVector.x * aSecondVector.x) + (aFirstVector.y * aSecondVector.y);
    }
}