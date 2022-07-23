# RT: A rayracer implemented in C++

![alt text](https://github.com/tomokveld/RT/blob/main/examples/CSG_reflect.png?raw=true)


### Non-exhaustive list of potential additions:

    - Clean up code
        - Suffix for members variables of class
        - Float suffix on numbers
        - Use references/pointers as much as possible
        - Minimize allocations
        - Remove shared_pointer use and replace with raw pointers?
    - GUI build with QT
    - YAML parser
    - Extend the OBJ parser
        - MTL parsing
        - Add flags (e.g. ability to disable normals)
        - Replace this with an external library?
    - PNG support
    - Parallel processing: given the nature of ray tracing it should be easily parallelizable. A possible approach would be to use a concurrent mpmc lockless queue, i.e. a work queue which dispatches work to workers/renderers.
    - Shadow ray optimization, there is no need to find the closest intersection, only interested in knowing whether there is an intersection.
    - Cache the transforms; most objects have the identity matrix as a transform, generally transforms occur at the top level, i.e. group level. So there is no reason to have every object store every matrix and inverse matrix. It would be better to store them in a map such that each object only has to carry a pointer to this map.
    - Optimize matrix operation code
    - Supersampling
        - Jitter
        - Adaptive supersampling
    - Bilinear interpolation
    - Bounded depth of reflection: a threshold is now used for the number of bounces, an additional feature would be to further limit it by the ray contribution, i.e. stop if transmitted attenuation becomes too small.
    - Variable glossyness of reflections. When rays collide with a rough material, the reflected ray is perturbed randomly by an amount determined by the roughness.
    - Variable glossyness of refraction, similar to reflection glossyness
    - Texture mapping
        - Diffuse color has been included, but there are many more options...
            - Reflected color
            - Transparency
    - Reflective mapping
    - Anisotropic shading
    - Height fields
    - Single Image Random Dot Stereograms (SIRDS)
    - Procedural textures
    - Bump mapping
    - Metaballs
    - Fractals
        - 3D
        - 4D quaternion
    - Shadow attenuation use Beer's law so that the thicker objects cast darker shadows than thinner ones with the same transparency constant
    - More versatile light control, e.g. Warn model
    - Motion blur
    - Monte Carlo Caustics & Indirect Light
    - Extension of existing primitive shapes, for instance a cylinder could be extended to include a radius and maximum phi parameter such that it is possible to scale the cylinder without using transformations.
    - Add more shapes
    - Photon mapping
    - Sub-Surface Scattering
    - Metropolis Light Transport
    - Different types of cameras
    - Different types of lights
    - Different types of materials
    - Path tracing
    - Set material of groups transfer to members
