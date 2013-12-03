/*
 * Ball.h
 *
 *  Created on: Nov 23, 2013
 *      Author: kevin
 */

#ifndef BALL_H_
#define BALL_H_

typedef float ball_type;

#include <vector>

class Ball {
public:
	Ball(int N, int M, ball_type R);
	virtual ~Ball();

	std::vector<ball_type> getVertices();
	std::vector<ball_type> getNormals();
	std::vector<unsigned int> getIndices();

	unsigned int getNumElements();

private:
	int N;// # vertices on longitude
	int M;// # vertices on latitude
	float R; //radius of ball
	unsigned int numElements;
};

#endif /* BALL_H_ */
