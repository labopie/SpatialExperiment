#pragma once

#include "Voronoi.h"

#include <vector>

#include <SFML/System/Vector2.hpp>

namespace VoronoiMap {

    struct TriangleMapCreationInput
    {
        std::vector<sf::Vector2f>& myPoints;
        sf::Vector2f myCentroid;
    };

    class VoronoiMapCreator
    {
    public:

        void CreateVoronoiMap(const TriangleMapCreationInput& anInput);

        void CreateTriangleMap(const TriangleMapCreationInput& anInput);

        const TriangleMap& GetTriangleMap() const;

        const VoronoiMap& GetVoronoiMap() const;

    private:

        struct VoronoiBuildingBlocks
        {
            Edge mySharedEdge;
            std::vector<const Triangle*> myInvolvedTriangles;
            Edge myEdge;

            bool SharePoint(const VoronoiBuildingBlocks& anotherBuildingBlock) const { return myInvolvedTriangles.at(0)->ContainVertex(*anotherBuildingBlock.myInvolvedTriangles.at(0)); }
        };

        void CreateCircuncenterList();

        void BuildListOfVoronoiEdges();

        void BuildPseudoVoronoiCell();

        void BuildVoronoiMap();

        TriangleMap myTriangleMap;

        std::vector<sf::Vector2f> myCircumcenterList;

        VoronoiMap myVoronoiMap;
        
        std::vector<VoronoiBuildingBlocks> myBuildingBlocks;

        std::vector<std::vector<VoronoiBuildingBlocks>> myPseudoVoronoiCell;
    };

} // namespace Moria::Services::VoronoiMap