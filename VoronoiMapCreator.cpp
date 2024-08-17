#include "VoronoiMapCreator.h"
#include "Math.h"
#include "BowyerWatson.h"

#include <algorithm>
#include <cmath>

namespace VoronoiMap {
    
    void VoronoiMapCreator::CreateVoronoiMap(const TriangleMapCreationInput& anInput)
    {
        CreateTriangleMap(anInput);

        BuildListOfVoronoiEdges();

        BuildPseudoVoronoiCell();

        BuildVoronoiMap();
    }
    
    void VoronoiMapCreator::CreateTriangleMap(const TriangleMapCreationInput& anInput)
    {
        Triangularization::BowyerWatson triangularizationAlgo{ anInput.myPoints };
        triangularizationAlgo.GenerateTriangulation();

        myTriangleMap.myTriangles.clear();
        myTriangleMap.myTriangles = triangularizationAlgo.GetTriangleList();
    }

    const TriangleMap& VoronoiMapCreator::GetTriangleMap() const
    {
        return myTriangleMap;
    }

    const VoronoiMap& VoronoiMapCreator::GetVoronoiMap() const
    {
        return myVoronoiMap;
    }

    void VoronoiMapCreator::CreateCircuncenterList()
    {
        for (const auto& triangle : myTriangleMap.myTriangles)
        {
            myCircumcenterList.push_back(triangle.GetCircumCenter());
        }
    }

    void VoronoiMapCreator::BuildListOfVoronoiEdges()
    {
        myBuildingBlocks.clear();

        const int trianglesCount = myTriangleMap.myTriangles.size();
        
        for (int idx = 0; idx < trianglesCount; ++idx)
        {
            const auto& triangle = myTriangleMap.myTriangles.at(idx);

            for (int secondIdx = idx + 1; secondIdx < trianglesCount; ++secondIdx)
            {
                const auto& secondTriangle = myTriangleMap.myTriangles.at(secondIdx);

                if (triangle.ShareEdge(secondTriangle))
                {
                    VoronoiBuildingBlocks vbc;
                    vbc.mySharedEdge = triangle.GetSharedEdge(secondTriangle);
                    vbc.myEdge = Edge{ triangle.GetCircumCenter(), secondTriangle.GetCircumCenter() };
                    vbc.myInvolvedTriangles = { &triangle, &secondTriangle };

                    myBuildingBlocks.push_back(vbc);
                }
                
            }
        }
    }

    void VoronoiMapCreator::BuildPseudoVoronoiCell()
    {
        myPseudoVoronoiCell.clear();

        const int buildingBlocksCount = myBuildingBlocks.size();

        for (int idx = 0; idx < buildingBlocksCount; ++idx)
        {
            std::vector<VoronoiBuildingBlocks> pseudoCell;

            const auto& bb = myBuildingBlocks.at(idx);

            pseudoCell.push_back(bb);

            for (int secondIdx = idx + 1; secondIdx < buildingBlocksCount; ++secondIdx)
            {
                const auto& secondBB = myBuildingBlocks.at(secondIdx);

                if (bb.SharePoint(secondBB))
                {
                    pseudoCell.push_back(secondBB);
                }
            }

            myPseudoVoronoiCell.push_back(pseudoCell);
        }
    }

    void VoronoiMapCreator::BuildVoronoiMap()
    {
        myVoronoiMap.myPoints.clear();

        for (const auto& pseudoCell : myPseudoVoronoiCell)
        {
            if (pseudoCell.size() > 1)
            {

                VoronoiCell vc;

                for (const auto innerBlock : pseudoCell)
                {
                    vc.myEdges.push_back(innerBlock.myEdge);
                }

                vc.myCenter = pseudoCell.at(0).mySharedEdge.first == pseudoCell.at(1).mySharedEdge.first ?
                    pseudoCell.at(0).mySharedEdge.first : pseudoCell.at(0).mySharedEdge.second;

                myVoronoiMap.myPoints.push_back(vc);
            }
        }
    }
    

} // namespace Moria::Services::VoronoiMap
