//
// Created by Abdulrahmen on 10/17/2024.
//

#include "HalfEdge.h"

std::vector<HalfEdge*> HalfEdge::GenerateHalfEdges(std::vector<Vertex> &vertices, std::vector<Face> &faces)
{
    std::vector<HalfEdge*> result;

    /*
     * General process: Go through each face of the mesh to generate Face/Edge/Vertex/Next elements of the half-edge
     * Then go through each faces again in an inner-loop to find the other face with 2 shared vertices, that'll be 'twin'
     * half-edge
     */
    for (auto& face : faces)
    {
        auto indices = face.vIndices;

        // Create 3 half edges, first one is the stored ref
        Vertex* firstVert = &vertices[indices[0]];
        Vertex* secondVert = &vertices[indices[1]];
        Vertex* thirdVert = &vertices[indices[2]];

        HalfEdge* firstHalfEdge = new HalfEdge();
        firstHalfEdge->vertex = firstVert;
        firstHalfEdge->vID = indices[0];
        firstHalfEdge->face = &face;

        HalfEdge* secondHalfEdge = new HalfEdge();
        secondHalfEdge->vertex = secondVert;
        secondHalfEdge->vID = indices[1];
        secondHalfEdge->face = &face;

        HalfEdge* thirdHalfEdge = new HalfEdge();
        thirdHalfEdge->vertex = thirdVert;
        thirdHalfEdge->vID = indices[2];
        thirdHalfEdge->face = &face;

        firstHalfEdge->next = secondHalfEdge;
        secondHalfEdge->next = thirdHalfEdge;
        thirdHalfEdge->next = firstHalfEdge;

        result.push_back(firstHalfEdge);
        result.push_back(secondHalfEdge);
        result.push_back(thirdHalfEdge);
    }

    for (auto& halfEdge : result)
    {
        for (auto& possibleTwin : result)
        {
            if (halfEdge == possibleTwin) continue;

            // Either same directionality or opposite directionality
            // We encounter one issue that idk if its relevant to fix, but if we have split normals, then we're basically forced to duplicate vertices and consider them non-unique...
            // By just comparing position vectors we avoid the issue of split-edges
            const bool bIsTwin =   (halfEdge->vertex->Position == possibleTwin->next->vertex->Position && halfEdge->next->vertex->Position == possibleTwin->vertex->Position);
            //                    || (halfEdge->vID == possibleTwin->vID && halfEdge->next->vID == possibleTwin->next->vID);

            if (bIsTwin)
            {
                halfEdge->twin = possibleTwin;
                //possibleTwin->twin = halfEdge;
            }
        }
    }

    return result;
}
