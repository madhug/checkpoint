/**
 * @file project.hpp
 * @brief GLSL project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#ifndef _462_GLSL_PROJECT_HPP_
#define _462_GLSL_PROJECT_HPP_

#include "math/camera.hpp"
#include "application/opengl.hpp"

/*
   A namespace declaration. All proejct files use this namespace.
   Add this declration (and its closing) to all source/headers you create.
   Note that all #includes should be BEFORE the namespace declaration.
 */
namespace _462 {

/**
 * Renders a scene.
 */
class SceneRenderer
{
public:
    /// Renders the scene, taking care of everything including the camera.
    virtual void render_scene() const = 0;

    /// Provides access to the camera so you know which values were used.
    virtual const Camera* get_camera() const = 0;
};

/**
 * Implementation of the project.
 */
class GlslProject
{
public:

    // constructor, invoked when object is created
    GlslProject();
    // destructor, invoked when object is destroyed
    ~GlslProject();

    // more detailed specifications for each function are in project.cpp.

    // Initialize the project, loading the mesh from the given filename.
    // Returns true on success.
    bool initialize( const SceneRenderer* renderer, int width, int height );
    // Clean up the project, free any memory, etc.
    void destroy();
    // Renderg the mesh using the given camera.
    void render();

private:

    // the triangle mesh we're rendering
    const SceneRenderer* renderer;

    // example shader program (you'll need more of these!)
    GLhandleARB program;


    // TODO add any other private members/functions here.

    // since this has no meaningful assignment/copy, prevent the compiler from
    // automatically generating those functions
    GlslProject( const GlslProject& );
    GlslProject& operator=( const GlslProject& );

    GLuint fbo;
    GLuint colorTexID, depthTexID;
    int width, height;
};

} /* _462 */

#endif /* _462_GLSL_PROJECT_HPP_ */

