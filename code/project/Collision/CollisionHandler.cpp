/*
 * CollisionHandler.cpp
 *
 *  Created on: Dec 2, 2013
 *      Author: kevin
 */

#include "CollisionHandler.h"
#include <Eigen/Geometry>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>

namespace CollisionHandlerNS {

CollisionHandler::CollisionHandler() {
}

CollisionHandler::~CollisionHandler() {
	// TODO Auto-generated destructor stub
}


std::tuple<bool, position_t, velocity_t> CollisionHandler::particleVsAllObjects(position_t newParticlePos, position_t oldParticlePos, velocity_t particleVel){

	std::tuple<bool, position_t, velocity_t> tmp = std::make_tuple(false, newParticlePos, particleVel);
	for(std::vector<object_t>::iterator it = objects.begin(); it != objects.end(); ++it){
		std::tuple<bool, position_t, velocity_t> res = particleVsOneObject(newParticlePos, oldParticlePos, particleVel, *it);
		if(std::get<0>(res) == true){
			tmp = res;
		}
	}

	return tmp;
}


std::tuple<bool, position_t, velocity_t> CollisionHandler::particleVsOneObject(position_t newParticlePos, position_t oldParticlePos, velocity_t particleVel, object_t object){

	//If particle is not inside AABB box: return
	if(!pointInsideCube(newParticlePos, object.AABB)){
//		std::cout << "CollisionHandler::particleVsOneObject: Early return: newParticlePos outside AABB box" << std::endl;
		return std::make_tuple(false, newParticlePos, particleVel);
	}

	//Init RNG
	std::mt19937 randEng(42);
	//For each triangle, check intersection and correct if necessary
	for(std::vector<unsigned int>::iterator it = object.indices.begin(); it != object.indices.end(); it+=3){
		//Triangle is specified by *it, *(it+1), *(it+2)
		unsigned int vInd1 = *it, vInd2 = *(it+1), vInd3 = *(it+2);
		position_t v1(object.vertices[object.vertexStride * vInd1], object.vertices[object.vertexStride * vInd1 + 1], object.vertices[object.vertexStride * vInd1 + 2]);
		position_t v2(object.vertices[object.vertexStride * vInd2], object.vertices[object.vertexStride * vInd2 + 1], object.vertices[object.vertexStride * vInd2 + 2]);
		position_t v3(object.vertices[object.vertexStride * vInd3], object.vertices[object.vertexStride * vInd3 + 1], object.vertices[object.vertexStride * vInd3 + 2]);

		//Compute (not normalized) triangle normal by vector product
		vec3 triangleNormal = ((v2-v1).cross(v3-v1)); //note: normal always points away from the plane


		//Check intersection
		bool intersects = false;
		std::cout << "oldParticlePos:\n" << oldParticlePos << std::endl;
		std::cout << "newParticlePos:\n" << newParticlePos << std::endl;
		std::pair<bool, position_t> inters = intersectPlane(line_t(oldParticlePos, newParticlePos), triangleNormal, v1);
		std::cout << "intersectsPlane?: " << inters.first << std::endl;
		std::cout << "intersection At: " << inters.second << std::endl;
		if(inters.first == true){
			intersects = pointInsideTriangle(inters.second, triangle_t(v1, v2, v3));//Watch out!
//			std::cout << "CollisionHandler::particleVsOneObject: Check if point is inside triangle" << std::endl;
		}

		//If intersection point is inside line segment and triangle, compute projection point
		//Particle position is inside object
		if(intersects == true){
//			std::cout << "CollisionHandler::particleVsOneObject: Point is inside triangle" << std::endl;

			Eigen::Hyperplane<collision_t, 3> trianglePlane(triangleNormal, v1);// = Eigen::Hyperplane<collision_t, 3>::Through(v1, v2, v3);

			//Correct position
			position_t correctedPos = trianglePlane.projection(newParticlePos);

			//Check whether the projected point is still in the triangle. If not, clamp it to the triangle's boundary
			if(!pointInsideTriangle(correctedPos, triangle_t(v1,v2,v3))){
				std::pair<bool, position_t> tmp = intersectLines(line_t(correctedPos, inters.second), line_t(v2, v1));
				if(tmp.first){
//					std::cout << "CollisionHandler::particleVsOneObject: Projected Point is outside triangle. Clamping to line(v2,v1)" << std::endl;
					correctedPos = tmp.second;
				}
				else{
					tmp = intersectLines(line_t(correctedPos, inters.second), line_t(v3, v1));
					if(tmp.first){
//						std::cout << "CollisionHandler::particleVsOneObject: Projected Point is outside triangle. Clamping to line(v3,v1)" << std::endl;
						correctedPos = tmp.second;
					}
					else{
//						std::cout << "CollisionHandler::particleVsOneObject: Projected Point is outside triangle. Clamping to line(v3,v2)" << std::endl;
						tmp = intersectLines(line_t(correctedPos, inters.second), line_t(v3, v2));
						correctedPos = tmp.second;
					}
				}
			}

			//Correct velocity
			std::cout << "particleVel:\n" << particleVel << std::endl;
			std::cout << "triangleNormal:\n" << triangleNormal << std::endl;
			velocity_t correctedVel = trianglePlane.projection(particleVel);
			//Add some random perturbation
			collision_t factor = 0.05 * particleVel.squaredNorm();
		    std::uniform_real_distribution<collision_t> uniformDist(-factor, factor);
		    collision_t rand0 = uniformDist(randEng);
		    collision_t rand1 = uniformDist(randEng);

		    std::cout << "correctedVel:\n" << correctedVel << std::endl;
		    correctedVel += rand0 * (v2-v1) + rand1 * (v2-v3);

			return std::make_tuple(true, correctedPos, correctedVel);
		}
	}

	//Particle is inside AABB box but, not inside object itself
	return std::make_tuple(false, newParticlePos, particleVel);
}


std::pair<bool, position_t> CollisionHandler::intersectPlane(line_t lineSegment, vec3 planeNormal, position_t planePoint){
	//Line = lineSegment(0) + s*(lineSegment(1) - lineSegment(0))

	//Compute intersection point for infinite line and infinite plane
	bool intersects = false;
	position_t intersectionPoint(0,0,0);

	position_t diff = lineSegment(1) - lineSegment(0);
	collision_t divisor = planeNormal.dot(diff);

	if(diff == intersectionPoint || divisor == 0.){//line(1) == line(0) || line is on or parallel to the plane
		std::cout << "Line is on plane" << std::endl;
		return std::make_pair(intersects, intersectionPoint);
	}

	collision_t s = -(planeNormal.dot(lineSegment(0) - planePoint) ) / divisor;

	//Check whether intersection point lies between line(0) and line(1)
	if(s > 0. && s <= 1.0005){
		intersects = true;
		intersectionPoint = lineSegment(0) + s * (lineSegment(1) - lineSegment(0));
	}

	return std::make_pair(intersects, intersectionPoint);
}

std::pair<bool, position_t> CollisionHandler::intersectLines(line_t lineSeg1, line_t lineSeg2){
	//lineSeg1(0) + s * A
	//lineSeg2(0) - t * B
	vec3 A = lineSeg1(1) - lineSeg1(0);
	vec3 B = lineSeg2(0) - lineSeg2(1);

	vec3 rhs = lineSeg2(0) - lineSeg1(0);

	int ind0 = 0, ind1 = 1;
	if(A(0) == 0. && B(0) == 0.)
		ind0 = 2;
	else if(A(1) == 0. && B(1) == 0.)
		ind1 = 2;

	collision_t invadbc = 1/(A(ind0) * B(ind1) - A(ind1) * B(ind0));
	collision_t s = invadbc * (B(ind1) * rhs(ind0) - B(ind0) * rhs(ind1));
	collision_t t = invadbc * (-A(ind1) * rhs(ind0) + A(ind0) * rhs(ind1));

	position_t intersectionPoint = lineSeg1(0) + s * A;
	bool intersects = false;
	if(s >= 0 && s <= 1 && t >= 0 && t <= 1){
		intersects = true;
	}

	return std::make_pair(intersects, intersectionPoint);
}

bool CollisionHandler::pointInsideTriangle(position_t x, triangle_t tri){
	//only two dimensions will be considered.
	vec3 ba = tri[1] - tri[0];
	vec3 ca = tri[2] - tri[0];
	vec3 rhs = x - tri[0];

	int ind0 = 0, ind1 = 1;
	if(ba[0] == 0. && ca[0] == 0.)
		ind0 = 2;
	else if(ba[1] == 0. && ca[1] == 0.)
		ind1 = 2;

	collision_t s = ca[ind1] * rhs[ind0] - ca[ind0] * rhs[ind1];
	collision_t t = -ba[ind1] * rhs[ind0] + ba[ind0] * rhs[ind1];

	collision_t adbc = (ba[ind0] * ca[ind1] - ba[ind1] * ca[ind0]);

	if(adbc < 0){
		return (s <= 0. && t < 0. && s+t > adbc );
	}
	else{
		return (s > 0. && t >= 0. && s+t <= 1. );
	}
//	Alternative for general 3D problem
//	//Copied from: http://www.blackpawn.com/texts/pointinpoly/
//
//	// Compute vectors
//	position_t v0 = t(2) - t(0);
//	position_t v1 = t(1) - t(0);
//	position_t v2 = x - t(0);
//
//	// Compute dot products
//	collision_t dot00 = v0.dot(v0);
//	collision_t dot01 = v0.dot(v1);
//	collision_t dot02 = v0.dot(v2);
//	collision_t dot11 = v1.dot(v1);
//	collision_t dot12 = v1.dot(v2);
//
//	// Compute barycentric coordinates
//	collision_t invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
//	collision_t u = (dot11 * dot02 - dot01 * dot12) * invDenom;
//	collision_t v = (dot00 * dot12 - dot01 * dot02) * invDenom;
//
//	// Check if point is in triangle
//	return (u >= 0) && (v >= 0) && (u + v <= 1);
}

bool CollisionHandler::pointInsideCube(position_t x, cube_t c){
	return x(0) > c(0) && x(0) < c(1) && x(1) > c(2) && x(1) < c(3) && x(2) > c(4) && x(2) < c(5);
}

void CollisionHandler::addObject(std::vector<collision_t> vertices, int vertexStride, std::vector<unsigned int> indices){
	object_t obj;
	obj.vertices = vertices;
	obj.indices = indices;
	obj.vertexStride = vertexStride;

	//Compute bounding box (Simple AABB)
	std::array<collision_t, 3> minima = {vertices[0], vertices[1], vertices[2]};
	std::array<collision_t, 3> maxima = {vertices[0], vertices[1], vertices[2]};

	std::vector<collision_t>::iterator it;
	for(it = vertices.begin(); it != vertices.end(); it+=vertexStride){
		for(int i = 0; i < 3; i++){
			if(*(it + i) < minima[i]){
				minima[i] = *(it + i);
			}
			else if(*(it + i) > maxima[i]){
				maxima[i] = *(it + i);
			}
		}
	}

	cube_t AABB;
	AABB << minima[0], maxima[0], minima[1], maxima[1], minima[2], maxima[2];
	obj.AABB = AABB;

	objects.push_back(obj);
}

} /* namespace CollisionHandler */
