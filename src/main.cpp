#include "system/device.hpp"
#include "raytracer.hpp"
#include <iostream>

using namespace std;
using namespace glm;

int main() {
    try {
        Device device(640, 480);

        RayTracer rt(&device);

        auto floor = rt.make_shape<Plane>(vec4(0.0, 1.0, 0.0, 0.0), make_unique<UniformLambertianMaterial>(vec3(1.0, 1.0, 1.0), 1.0)); // On ajoute un plan
        auto top = rt.make_shape<Plane>(vec4(0.0, -1.0, 0.0, 12.0), make_unique<UniformLambertianMaterial>(vec3(1.0, 1.0, 1.0), 1.0)); // On ajoute un plan
        auto left = rt.make_shape<Plane>(vec4(-1.0, 0.0, 0.0, 10.0), make_unique<UniformLambertianMaterial>(vec3(.0, .0, 1.0), 1.0)); // On ajoute un plan
        auto right = rt.make_shape<Plane>(vec4(1.0, 0.0, 0.0, 10.0), make_unique<UniformLambertianMaterial>(vec3(.0, 1.0, .0), 1.0)); // On ajoute un plan
        auto front = rt.make_shape<Plane>(vec4(0.0, .0, -1.0, 10.0), make_unique<UniformLambertianMaterial>(vec3(1.0, 1.0, 1.0), 1.0)); // On ajoute un plan
        auto back = rt.make_shape<Plane>(vec4(0.0, .0, 1.0, 100.0), make_unique<UniformLambertianMaterial>(vec3(.0, .0, 1.0), 1.0)); // On ajoute un plan

        auto sphere1 = rt.make_shape<Sphere>(vec3(-3.0, 2.0, 3.0), 2.0f, make_unique<TransmitterMaterial>(1.f, 2.42f)); // On ajoute une sphère
        auto sphere2 = rt.make_shape<Sphere>(vec3(3.0, 3.0, 2.0), 1.0f, make_unique<MirrorMaterial>(1.f)); // On ajoute une sphère

        rt.addCamera<StaticCamera>(vec3(0.92, 4.42, -13.59), vec3(0.89, 4.36, -12.593));

        auto light = rt.make_light<SpotLight>(vec3(0.0, 10.0, 0.0), vec3(0.0, -1.0, 0.0), cosf(radians(70.f)), vec3(800, 800, 800));

        while(device.run()) {
            device.begin();
            device.update();

            rt.render();

            device.end();

            cout << device.renderingTime() <<  "ms" << endl;
        }
    }

    catch(std::runtime_error const &except) {
        cerr << except.what() << endl;
    }

    return 0;
}

