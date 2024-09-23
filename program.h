#ifndef PROGRAM_H
#define PROGRAM_H

#include"opengl.h"

class app {
public:
	app() {}; // void constructor;

	float delta_time = 0;
	uint16_t window_width = 0, window_height = 0;
};
 
void start(app* sys);
void update(app* sys);
void render(app* sys);
void del(app* sys);

#endif // !PROGRAM_H

