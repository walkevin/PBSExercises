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
#include <map>
#include <array>

#include "../GLUTFramework/src/GeometricObject.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace CollisionHandlerNS {

CollisionHandler::CollisionHandler(){
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
			break;
		}
	}

	return tmp;
}


std::tuple<bool, position_t, velocity_t> CollisionHandler::particleVsOneObject(position_t newParticlePos, position_t oldParticlePos, velocity_t particleVel, object_t object){

	//If particle is not inside AABB box: return
	if(!( pointInsideCube(newParticlePos, object.AABB) || pointInsideCube(oldParticlePos, object.AABB) )){
		return std::make_tuple(false, newParticlePos, particleVel);
	}

	//For each triangle, check intersection and correct if necessary
	for(std::vector<unsigned int>::iterator it = object.indices.begin(); it != object.indices.end(); it+=3){
		//Triangle is specified by *it, *(it+1), *(it+2)
		unsigned int vInd1 = *it, vInd2 = *(it+1), vInd3 = *(it+2);
		position_t v1(object.vertices[object.vertexStride * vInd1], object.vertices[object.vertexStride * vInd1 + 1], object.vertices[object.vertexStride * vInd1 + 2]);
		position_t v2(object.vertices[object.vertexStride * vInd2], object.vertices[object.vertexStride * vInd2 + 1], object.vertices[object.vertexStride * vInd2 + 2]);
		position_t v3(object.vertices[object.vertexStride * vInd3], object.vertices[object.vertexStride * vInd3 + 1], object.vertices[object.vertexStride * vInd3 + 2]);

		//Compute (not normalized) triangle normal by vector product
		vec3 triangleNormal = ((v2-v1).cross(v3-v1)).normalized(); //note: normal always points away from the plane

		//Check intersection
		bool intersects = false;
		std::pair<bool, position_t> inters = intersectPlane(line_t(oldParticlePos, newParticlePos), triangleNormal, v1);
		if(inters.first == true){
			intersects = pointInsideTriangle(inters.second, triangle_t(v1, v2, v3));//Watch out!
		}

		//If intersection point is inside line segment and triangle, compute projection point
		//Particle position is inside object
		if(intersects == true){

			Eigen::Hyperplane<collision_t, 3> trianglePlane(triangleNormal, v1);// = Eigen::Hyperplane<collision_t, 3>::Through(v1, v2, v3);

			//Correct position
			position_t correctedPos = trianglePlane.projection(newParticlePos);

//			//Check whether the projected point is still in the triangle. If not, clamp it to the triangle's boundary
			if(!pointInsideTriangle(correctedPos, triangle_t(v1,v2,v3))){
				std::pair<bool, position_t> tmp = intersectLines(line_t(correctedPos, inters.second), line_t(v2, v1));
				if(tmp.first){
					correctedPos = tmp.second;
				}
				else{
					tmp = intersectLines(line_t(correctedPos, inters.second), line_t(v3, v1));
					if(tmp.first){
						correctedPos = tmp.second;
					}
					else{
						tmp = intersectLines(line_t(correctedPos, inters.second), line_t(v3, v2));
						correctedPos = tmp.second;
					}
				}
			}

			//Add some perturbation in triangle normal outwards direction (avoid being pushed through afterwards by other particles)
			correctedPos += 4 * (inters.second - newParticlePos);

			//Correct velocity
			velocity_t correctedVel = 0.7 * (trianglePlane.projection(v1 + particleVel) - v1);//v1: particleVel is only a direction vector

//		    //Add some perturbation in triangle normal inwards direction (so as to stick with the surface and not free float in space)
//			correctedVel += 0.5 * (particleVel - correctedVel);

//		    //Add some random perturbation
//			//Init RNG
//			std::mt19937 randEng(42);
//			collision_t factor = 0.2 * particleVel.norm();
//		    std::uniform_real_distribution<collision_t> uniformDist(-factor, factor);
//		    collision_t rand0 = uniformDist(randEng);
//		    collision_t rand1 = uniformDist(randEng);
//
//		    correctedVel += rand0 * (v2-v1) + rand1 * (v2-v3);

		    //Report collisions
		    collisionPositions.push_back(inters.second);

		    //Dirty hack with clamping

		    Eigen::Vector3f threshold = 0.0003 * (inters.second + triangleNormal.cross(correctedVel));
		    if(threshold.squaredNorm() > 0.1){
		    	threshold.normalize();
		    	threshold *= 0.1;
		    }
		    Eigen::Vector3f aClamp = threshold + 0.001 * (inters.second + correctedVel);
		    if(aClamp.squaredNorm() > 0.1){
		    	aClamp.normalize();
		    	aClamp *= 0.1;
		    }
		    collisionVelocities.push_back(aClamp);

//		    Eigen::Vector3f threshold = 0.0003 * (inters.second + triangleNormal.cross(correctedVel));
//		    collisionVelocities.push_back(threshold + 0.001 * (inters.second + correctedVel));

		    collisionVelocitiesOrthogonal.push_back(threshold);

//		    collisionVelocities.push_back(0.001 * Eigen::Vector3f(1,1,1));
//		    collisionVelocitiesOrthogonal.push_back(0.001 * Eigen::Vector3f(1,1,1));


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
		return std::make_pair(intersects, intersectionPoint);
	}

	collision_t s = -(planeNormal.dot(lineSegment(0) - planePoint) ) / divisor;

	//Check whether intersection point lies between line(0) and line(1)
	if(s > 0. && s <= 1.){
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
//	//only two dimensions will be considered => Optimized version if x is guaranteed to be on the same plane as tri (buggy)
//	vec3 ba = tri[1] - tri[0];
//	vec3 ca = tri[2] - tri[0];
//	vec3 rhs = x - tri[0];
//
//	int ind0 = 0, ind1 = 1;
//	if(ba[0] == 0. && ca[0] == 0.){
//		ind0 = 1; ind1 = 2;
//	}
//	else if(ba[1] == 0. && ca[1] == 0.){
//		ind0 = 0; ind1 = 2;
//	}
//
//	collision_t s = ca[ind1] * rhs[ind0] - ca[ind0] * rhs[ind1];
//	collision_t t = -ba[ind1] * rhs[ind0] + ba[ind0] * rhs[ind1];
//
//	collision_t adbc = (ba[ind0] * ca[ind1] - ba[ind1] * ca[ind0]);
//
//	if(adbc < 0){
//		return (s <= 0. && t < 0. && s+t > adbc );
//	}
//	else{
//		return (s > 0. && t >= 0. && s+t <= 1. );
//	}

	//Alternative for general 3D problem
	//Copied from: http://www.blackpawn.com/texts/pointinpoly/

	// Compute vectors
	position_t v0 = tri(2) - tri(0);
	position_t v1 = tri(1) - tri(0);
	position_t v2 = x - tri(0);

	// Compute dot products
	collision_t dot00 = v0.dot(v0);
	collision_t dot01 = v0.dot(v1);
	collision_t dot02 = v0.dot(v2);
	collision_t dot11 = v1.dot(v1);
	collision_t dot12 = v1.dot(v2);

	// Compute barycentric coordinates
	collision_t invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	collision_t u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	collision_t v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return (u >= 0) && (v >= 0) && (u + v <= 1);
}

bool CollisionHandler::pointInsideCube(position_t x, cube_t c){
	double tol = 0.001;
	return x(0) > c(0) - tol && x(0) < c(1) + tol && x(1) > c(2) - tol && x(1) < c(3) + tol && x(2) > c(4) - tol && x(2) < c(5) + tol;
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

void CollisionHandler::rotateObjects(double angle)
{
	glm::vec3 euler(0, angle, 0);
	glm::quat myQuat(euler);
	glm::mat4 transformation = glm::toMat4(myQuat);

	for(int i = 0; i < objects.size(); i++)
	{
		std::vector<geometry_type> objectData = objects[i].vertices;
		Eigen::Map<Eigen::MatrixXf> rawVertices(objectData.data(), 4, objectData.size() / 4);//rawVertices operates on the same data as pyrData
		Eigen::Map<Eigen::Matrix<float, 4, 4> > transform(glm::value_ptr(transformation));
		rawVertices = transform * rawVertices;
		objects[i].vertices = objectData;
		
		std::vector<collision_t>::iterator it;
		std::vector<collision_t> vertices = objects[i].vertices;

		std::array<collision_t, 3> minima = {vertices[0], vertices[1], vertices[2]};
		std::array<collision_t, 3> maxima = {vertices[0], vertices[1], vertices[2]};

		int vertexStride = 4;
		
		for(it = vertices.begin(); it != vertices.end(); it+=vertexStride)
		{
			for(int i = 0; i < 3; i++)
			{
				if(*(it + i) < minima[i]){
					minima[i] = *(it + i);
				}
				else if(*(it + i) > maxima[i]){
					maxima[i] = *(it + i);
				}
			}
		}
		objects[i].AABB << minima[0], maxima[0], minima[1], maxima[1], minima[2], maxima[2]; 		
	}
}
	

void CollisionHandler::clearObjects()
{
	while(!objects.empty())
		objects.pop_back();
}

std::vector<position_t> CollisionHandler::getCollisionPositions(){
	return collisionPositions;
}

std::vector<velocity_t> CollisionHandler::getCollisionVelocities(){
	return collisionVelocities;
}

std::vector<velocity_t> CollisionHandler::getCollisionVelocitiesOrthogonal(){
	return collisionVelocitiesOrthogonal;
}

void CollisionHandler::clearCollisionPositions(){
  collisionPositions.clear();
}

void CollisionHandler::clearCollisionVelocities(){
  collisionVelocities.clear();
}

void CollisionHandler::clearCollisionVelocitiesOrthogonal(){
  collisionVelocitiesOrthogonal.clear();
}

} /* namespace CollisionHandler */
