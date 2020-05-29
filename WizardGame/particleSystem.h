#ifndef PARTICLES_H

#define PARTICLES_H
#include <time.h>
#include <glm/glm.hpp>
#include <stdlib.h>

struct Particle {
    glm::vec3 Position, Velocity;
    int Life;
};

class Emitter {
public:
	Emitter(float Velocity,int Life, glm::vec4 Color) : o("./res/cube.obj",Color,"./res/basicShader",true) {
		o.setScale(glm::vec3(0.3,0.3,0.3));
		this->Life = Life;
		this->Velocity = Velocity;
	}
	virtual ~Emitter() {
        particles.clear();
        std::cout << "DELETED EMITTER\n";
	}
	void deleteParticles() {
        particles.clear();
	}
	void Emit(int number) {
		for (int i = 0; i<number; i++) {
			float r = getRandom();
			Particle p;
			p.Velocity = glm::vec3(getRandom(),getRandom(),getRandom());
			p.Life = 0;
			p.Position = position;
			particles.push_back(p);
		}
	}
	void Update() {
		for (int i = 0; i<particles.size(); i++) {
			particles[i].Position += (particles[i].Velocity-0.5f)*Velocity;
			particles[i].Position.y -= 0.5f;
			particles[i].Life++;
			if (particles[i].Life > Life) {
				particles.erase(particles.begin()+i);
			}
		}
	}
	void Draw(Camera camera) {
		for (Particle p : particles) {
			o.setPos(p.Position);
			o.Draw(camera);
		}
		// std::cout << "particle length is " << particles.size() << "\n";
	}
	void setPos(glm::vec3 pos) {
		this->position = pos;
	}
	float getRandom() {
		times++;
		srand (static_cast <unsigned> (time(0)+times));
		float val = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		return val;
	}
private:
	double times;
	float Velocity;
	glm::vec3 position;
	int Life;
	glm::vec4 Color;
	Object o;
	std::vector<Particle> particles;
};

#endif
