#include "GlutFramework.h"
class ConcreteViewer : public glutFramework::GlutFramework{
	/** The display function is called at a specified frames-per-second (FPS). Any
	 animation drawing code can be run in the display method.
	 @param dTime - the change in time (seconds)
	 */
	void display(float dTime);
};
