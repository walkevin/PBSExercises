/*
 * Ball.h
 *
 *  Created on: Nov 23, 2013
 *      Author: kevin
 */

#ifndef BALL_H_
#define BALL_H_
#include <vector>

class Ball {
public:
	Ball(int N, int M, float R);
	virtual ~Ball();

	std::vector<float> getVertices();
	std::vector<float> getNormals();
	std::vector<unsigned int> getIndices();
	unsigned int getNumElements();

private:
	int N;// # vertices on longitude
	int M;// # vertices on latitude
	float R; //radius of ball
	unsigned int numElements;
};

#endif /* BALL_H_ */
