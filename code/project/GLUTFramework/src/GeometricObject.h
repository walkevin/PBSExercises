/*
 * GeometricObject.h
 *
 *  Created on: Nov 23, 2013
 *      Author: kevin
 *      Interface for analytic geometric objects
 *
 */

#ifndef GEOEMTRICOJBECT_H_
#define GEOEMTRICOJBECT_H_

#include <vector>
typedef float geometry_type;

class GeometricObject {
public:
	GeometricObject();
	virtual ~GeometricObject();

	virtual std::vector<geometry_type> getVertices() = 0;
	virtual std::vector<geometry_type> getNormals() = 0;
	virtual std::vector<unsigned int> getIndices() = 0;
	virtual std::vector<geometry_type> getColors() = 0;


	virtual unsigned int getNumElements() = 0;
};


//TODO: Move GeometricObjects to Scene or sth like that. Modify constructors such that custom colorisation is possible
#endif /* GEOMETRICOBJECT_H_ */
