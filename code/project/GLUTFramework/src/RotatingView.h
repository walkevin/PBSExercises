/*
 * RotatingView.h
 *
 *  Created on: Nov 25, 2013
 *      Author: kevin
 */

#ifndef ROTATINGVIEW_H_
#define ROTATINGVIEW_H_

typedef double value_t;

#include <string>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GlutFramework.h"
#include "ShaderLoader.h"


class RotatingView {
public:
	RotatingView(ShaderLoader sh_, std::string PVMatrixName);
	virtual ~RotatingView();
	void setAzimuth(value_t azimuth_);
	void setPolar(value_t polar_);
	void setDistance(value_t distance_);
	void updateView();
	void keyboardEvent(int key);

private:
	ShaderLoader sh;
	glm::mat4 getProjectionViewMatrix(value_t azimuth_, value_t polar_, value_t distance_);
	std::string PVMatrixName;

	value_t azimuth;
	value_t polar;
	value_t distance;

	value_t angleChangeSpeed;
	value_t zoomSpeed;
};

#endif /* ROTATINGVIEW_H_ */
