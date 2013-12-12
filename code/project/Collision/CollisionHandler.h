/*
 * CollisionHandler.h
 *
 *  Created on: Dec 2, 2013
 *      Author: kevin
 */

#ifndef COLLISIONHANDLER_H_
#define COLLISIONHANDLER_H_

#include <array>
#include <vector>
#include <utility>
#include <Eigen/Core>
#include <tuple>
#include <random>

namespace CollisionHandlerNS{

typedef float collision_t;

typedef Eigen::Matrix<collision_t, 3, 1> position_t;
typedef Eigen::Matrix<collision_t, 3, 1> velocity_t;
typedef Eigen::Matrix<collision_t, 3, 1> vec3;
typedef Eigen::Matrix<position_t, 2, 1> line_t;
typedef Eigen::Matrix<position_t, 3, 1> triangle_t;
typedef Eigen::Matrix<collision_t, 6, 1> cube_t;

struct object_t{
	std::vector<collision_t> vertices;
	int vertexStride;
	std::vector<vec3> normals;
	std::vector<unsigned int> indices;
	cube_t AABB;
};

class CollisionHandler {
public:
	CollisionHandler();
	virtual ~CollisionHandler();

	std::tuple<bool, position_t, velocity_t> particleVsAllObjects(position_t newParticlePos, position_t oldParticlePos, velocity_t particleVel);

	/*
	 * addObject
	 *
	 * computes the AABB bounding box of the object and adds it to the list of collision objects. com. The objects must be triangulated.
	 * The AABB bounding box is ordered as follows: minX, maxX, minY, maxY, minZ, maxZ
	 *
	 * Arguments:
	 * 1. vertices, list of vertices
	 * 2. indices, list of indices (must be divisble by three)
	 *
	 */
	void addObject(std::vector<collision_t> vertices, int vertexStride, std::vector<unsigned int> indices);
//private:
	std::tuple<bool, position_t, velocity_t> particleVsOneObject(position_t newParticlePos, position_t oldParticlePos, velocity_t particleVel, object_t object);

	/*
	 * intersectPlane
	 *
	 * Computes whether lineSegment intersects the plane defined by its normal and an arbitrary point on the plane
	 * Arguments:
	 *
	 * 1. lineSegment, defined by two points
	 * 2. planeNormal, 3D-vector defining the (not normalized) normal to the plane
	 * 3. planePoint, an arbitrary point on the plane
	 *
	 * Return values
	 * bool: true if segment intersects plane
	 * position_t: the intersection point if segment intersects plane, (0,0,0) otherwise
	 */
	std::pair<bool, position_t> intersectPlane(line_t lineSegment, vec3 planeNormal, position_t planePoint);

	/*
	 * intersectLines
	 *
	 * Computes whether lineSeg1 intersects lineSeg2
	 * Arguments:
	 *
	 * 1. lineSeg1, defined by two points
	 * 2. lineSeg2, defined by two points
	 *
	 * Return values
	 * bool: true if lines intersect
	 * position_t: the intersection point if the line segments intersect, otherwise undefined
	 */
	std::pair<bool, position_t> intersectLines(line_t lineSeg1, line_t lineSeg2);

	/*
	 * pointInsideTriangle
	 *
	 * Checks whether a point is inside a (2 dimensional) triangle
	 * Hint: In a 3D triangle, if the point is on the same plane as the triangle, an arbitrary dimension can be discarded, hence the problem reduces to the 2D case.
	 * Arguments:
	 * 1. x, the point to be checked
	 * 2. t, the triangle
	 *
	 * Return value:
	 * bool: true if the point is inside the triangle, false otherwise
	 */
	bool pointInsideTriangle(position_t x, triangle_t t);

	/*
	 * pointInsideCube
	 *
	 * Checks whether a point is inside a cube (oriented on xyz)
	 *
	 * Arguments:
	 * 1. x, the point to be checked
	 * 2. c, the cube
	 *
	 * Return value:
	 * bool: true if the point is inside the cube, false otherwise
	 */
	bool pointInsideCube(position_t x, cube_t c);

	std::vector<object_t> objects;
};

} /* namespace CollisionHandler */
#endif /* COLLISIONHANDLER_H_ */
