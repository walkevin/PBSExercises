/*=====================================================================================*/
/*! \file		FEMElementTri.cpp
	\author		peterkau
	\brief		Implementation of class FEMElementTri
 */
/*=====================================================================================*/

#include "SimpleFEMDefs.h"
#include "FEMElementTri.h"
#include "FEMMesh.h"

// TASK 3
void FEMElementTri::Assemble(FEMMesh *pMesh) const
{
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			size_t iGlobal = GetGlobalNodeForElementNode(i);
			size_t jGlobal = GetGlobalNodeForElementNode(j);

			//Compute gradients
			Vector2 gradU, gradV;
			computeSingleBasisDerivGlobalLES(i, gradU, pMesh);
			computeSingleBasisDerivGlobalLES(i, gradV, pMesh);

			//Make dot product and add result to Galerkin (aka Stiffness) Matrix
			pMesh->AddToStiffnessMatrix(iGlobal, jGlobal, gradU | gradV);
		}
	}
}

// TASK 2
void FEMElementTri::computeSingleBasisDerivGlobalGeom(size_t nodeId, Vector2 &basisDerivGlobal, const FEMMesh *pMesh) const
{
	Vector2 node0 = pMesh->GetNodePosition(m_nodes[0]);
	Vector2 node1 = pMesh->GetNodePosition(m_nodes[1]);
	Vector2 node2 = pMesh->GetNodePosition(m_nodes[2]);

	switch (nodeId){
	case 0:
		{
			Vector2 e0 = node2 - node1;
			Vector2 n0 = Vector2(-e0.y, e0.x).normalized();
			double h0 = (node0 - node2) | n0;
			basisDerivGlobal = n0 / h0;
			break;
		}
	case 1:
		{
			Vector2 e1 = node0 - node2;
			Vector2 n1 = Vector2(-e1.y, e1.x).normalized();
			double h1 = (node1 - node0) | n1;
			basisDerivGlobal = n1 / h1;
			break;
		  }
	case 2:
		{
			Vector2 e2 = node1 - node0;
			Vector2 n2 = Vector2(-e2.y, e2.x).normalized();
			double h2 = (node2 - node1) | n2;
			basisDerivGlobal = n2 / h2;
			break;
		  }
	default:
		assert(false);
	}
}

// TASK 1
void FEMElementTri::computeSingleBasisDerivGlobalLES(size_t nodeId, Vector2 &basisDerivGlobal, const FEMMesh *pMesh) const
{
	Matrix3x3 mat = Matrix3x3();
	Vector2 node0 = pMesh->GetNodePosition(m_nodes[0]);
	Vector2 node1 = pMesh->GetNodePosition(m_nodes[1]);
	Vector2 node2 = pMesh->GetNodePosition(m_nodes[2]);

	//Initialize matrix
	mat(0, 0) = node0.x;
	mat(0, 1) = node0.y;
	mat(0, 2) = 1.;
	mat(1, 0) = node1.x;
	mat(1, 1) = node1.y;
	mat(1, 2) = 1.;
	mat(2, 0) = node2.x;
	mat(2, 1) = node2.y;
	mat(2, 2) = 1.;

	//Init the "delta" vector
	Vector3 RHS = Vector3(0, 0, 0);
	RHS[nodeId] = 1.;

	Vector3 coeffs = mat.inverse() * RHS;

	basisDerivGlobal.x = coeffs[0];
	basisDerivGlobal.y = coeffs[1];
}
