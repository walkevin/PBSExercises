#include <iostream>

#include "CollisionHandler.h"


int main(){
	std::vector<position_t> vertices;
	vertices.push_back(position_t(1.0, 1.0, 1.0));
	vertices.push_back(position_t(1.0, 2.0, 1.0));
	vertices.push_back(position_t(2.0, 1.0, 1.0));

	std::vector<int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	position_t p_old (1.5, 0.4, 0.0);
	position_t p_new (1.2, 2.6, 2.0);
	velocity_t v (0.0, 0.0, 8.0);

	using namespace CollisionHandlerNS;

	CollisionHandler* c = new CollisionHandler();
	c->addObject(vertices, indices);

	std::tuple<bool, position_t, velocity_t> res = c->particleVsAllObjects(p_new, p_old, v);

	std::cout << "Intersects?: " << std::get<0>(res) << std::endl;
	std::cout << "New Position: " << std::get<1>(res) << std::endl;
	std::cout << "New Velocity: " << std::get<2>(res) << std::endl;
}
