/*=====================================================================================*/
/*! \file		FEMElementTri.cpp
	\author		peterkau
	\brief		Implementation of class FEMElementTri
 */
/*=====================================================================================*/

#include "SimpleFEMDefs.h"
#include "FEMElementTri.h"
#include "FEMMesh.h"
#include <algorithm>

typedef double value_t;
/// TASK 3
void FEMElementTri::Assemble(FEMMesh *pMesh) const
{
	//Compute area of triangle
	Vector2 node0 = pMesh->GetNodePosition(GetGlobalNodeForElementNode(0));
	Vector2 node1 = pMesh->GetNodePosition(GetGlobalNodeForElementNode(1));
	Vector2 node2 = pMesh->GetNodePosition(GetGlobalNodeForElementNode(2));
	value_t areaTriangle = 0.5*abs((node0.x() - node2.x())*(node1.y() - node0.y()) - (node0.x() - node1.x())*(node2.y() - node0.y()));

	//Compute single gradients
	Vector2 gradTmp0, gradTmp1, gradTmp2;
	computeSingleBasisDerivGlobalLES(0, gradTmp0, pMesh);
	computeSingleBasisDerivGlobalLES(1, gradTmp1, pMesh);
	computeSingleBasisDerivGlobalLES(2, gradTmp2, pMesh);

	//Compute \int gradU * gradV dx
	value_t grad01, grad02, grad12;
	grad01 = areaTriangle * (gradTmp0 | gradTmp1);
	grad02 = areaTriangle * (gradTmp0 | gradTmp2);
	grad12 = areaTriangle * (gradTmp1 | gradTmp2);

	//Add to appropriate elements in stiffness matrix
	size_t g0 = GetGlobalNodeForElementNode(0), g1 = GetGlobalNodeForElementNode(1), g2 = GetGlobalNodeForElementNode(2);
	pMesh->AddToStiffnessMatrix(max(g0, g1), min(g0, g1), grad01);
	pMesh->AddToStiffnessMatrix(max(g0, g2), min(g0, g2), grad02);
	pMesh->AddToStiffnessMatrix(max(g1, g2), min(g1, g2), grad12);

	//Row-sum of local stiffness matrix must be zero since \sum_i^3 \lambda_i = 1 where \lambda_i are the basisfunctions
	pMesh->AddToStiffnessMatrix(g0, g0, -grad01 - grad02);
	pMesh->AddToStiffnessMatrix(g1, g1, -grad01 - grad12);
	pMesh->AddToStiffnessMatrix(g2, g2, -grad12 - grad02);


	//====================
	////Alternative, more straight-forward solution
	//Vector2 node0 = pMesh->GetNodePosition(GetGlobalNodeForElementNode(0));
	//Vector2 node1 = pMesh->GetNodePosition(GetGlobalNodeForElementNode(1));
	//Vector2 node2 = pMesh->GetNodePosition(GetGlobalNodeForElementNode(2));

	//value_t areaTriangle = 0.5*abs((node0.x() - node2.x())*(node1.y() - node0.y()) - (node0.x() - node1.x())*(node2.y() - node0.y()));

	//for (int i = 0; i < 3; i++){
	//	for (int j = i; j < 3; j++){
	//		size_t iGlobal = GetGlobalNodeForElementNode(i);
	//		size_t jGlobal = GetGlobalNodeForElementNode(j);
	//		
	//		Vector2 gradU, gradV;
	//		computeSingleBasisDerivGlobalLES(i, gradU, pMesh);
	//		computeSingleBasisDerivGlobalLES(j, gradU, pMesh);
	//		double tmp = areaTriangle*(gradU | gradU);

	//		//Make dot product and add result to Galerkin (aka Stiffness) Matrix. Only fill lower triangular part of matrix!!!!
	//		if (iGlobal > jGlobal)
	//			pMesh->AddToStiffnessMatrix(iGlobal, jGlobal, tmp);
	//		else
	//			pMesh->AddToStiffnessMatrix(jGlobal, iGlobal, tmp);

	//	}			
	//}
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
			Vector2 n0 = Vector2(-e0.y(), e0.x()).normalized();
			value_t h0 = (node0 - node2) | n0;
			basisDerivGlobal = n0 / h0;
			break;
		}
	case 1:
		{
			Vector2 e1 = node0 - node2;
			Vector2 n1 = Vector2(-e1.y(), e1.x()).normalized();
			value_t h1 = (node1 - node0) | n1;
			basisDerivGlobal = n1 / h1;
			break;
		  }
	case 2:
		{
			Vector2 e2 = node1 - node0;
			Vector2 n2 = Vector2(-e2.y(), e2.x()).normalized();
			value_t h2 = (node2 - node1) | n2;
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
	for (int i = 0; i < 3; i++){
		Vector2 node = pMesh->GetNodePosition(m_nodes[i]);
		mat(i, 0) = node.x();
		mat(i, 1) = node.y();
		mat(i, 2) = 1.;
	}

	//Init the "delta" vector
	Vector3 RHS = Vector3(0, 0, 0);
	RHS[nodeId] = 1.;

	Vector3 coeffs = mat.inverse() * RHS;

	basisDerivGlobal[0] = coeffs[0];
	basisDerivGlobal[1] = coeffs[1];
	//Coeffs are ok
	//std::cout << "Coefficients" << basisDerivGlobal[0] << "\t" << basisDerivGlobal[1] << std::endl;
}
