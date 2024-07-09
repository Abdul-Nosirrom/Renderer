#pragma once

#include "Mesh.h"

class PrimitiveMesh : public Mesh
{
public:
	enum class EType : UINT
	{
		Cube, Plane, Sphere, Cylinder
	};

	PrimitiveMesh(EType type)
	{
		switch (type)
		{
		case PrimitiveMesh::EType::Cube:
			MakeCube();
			break;
		case PrimitiveMesh::EType::Plane:
			MakePlane(256);
			break;
		case PrimitiveMesh::EType::Sphere:
			MakeSphere(32, 32);
			break;
		case PrimitiveMesh::EType::Cylinder:
			MakeCylinder(32, 2);
			break;
		}
	}

protected:
	void MakeCube();
	void MakePlane(UINT tesseleation);
	void MakeSphere(UINT phiSlices, UINT thetaSlices);
	void MakeCylinder(UINT radialSlices, float height);
};

