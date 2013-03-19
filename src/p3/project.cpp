/**
 * @file project.cpp
 * @brief GLSL project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "p3/project.hpp"
#include <fstream>

// A namespace declaration. All proejct files use this namespace.
// Add this declration (and its closing) to all source/headers you create.
// Note that all #includes should be BEFORE the namespace declaration.
namespace _462 {

// shader loading code

/**
 * Load a file as either a vertex shader or a fragment shader, and attach
 * it to a program.
 * @param file The filename to load
 * @param type Either GL_VERTEX_SHADER, or GL_FRAGMENT_SHADER
 * @param program The shader program to which to attach the loaded shader.
 * @return True on success.
 */
static bool load_shader( const char* file, GLint type, GLhandleARB program )
{
    std::ifstream infile;
    char* buffer;
    char error_msg[2048];
    GLhandleARB shader;

    infile.open( file );

    if( infile.fail() ) {
        std::cout << "ERROR: cannot open file: " << file << std::endl;
        infile.close();
        return false;
    }

    // calculate length
    infile.seekg( 0, std::ios::end );
    int length = infile.tellg();
    infile.seekg( 0, std::ios::beg );
    // allocate space for entire program
    buffer = (char *) malloc( (length + 1) * sizeof *buffer );
    if ( !buffer )
        return false;
    // copy entire program into memory
    infile.getline( buffer, length, '\0' );
    infile.close();

    // create shader object
    shader = glCreateShaderObjectARB( type );
    // link shader source
    const char* src = buffer; // workaround for const correctness
    glShaderSource( shader, 1, &src, NULL );
    // compile shaders
    glCompileShader( shader );
    // check success
    GLint result;
    glGetObjectParameterivARB( shader, GL_OBJECT_COMPILE_STATUS_ARB, &result );
    if ( result != GL_TRUE ) {
        glGetInfoLogARB( shader, sizeof error_msg, NULL, error_msg );
        std::cout << "GLSL COMPILE ERROR in " << file << ": " << error_msg << std::endl;
        return false;
    } else {
        std::cout << "Compiled shaders successfully" << std::endl;
    }

    // attach the shader object to the program object
    glAttachObjectARB( program, shader );

    free( buffer );
    return true;
}

/**
 * Loads a vertex and fragment shader from the given files and attaches them to the given
 * shader program object.
 * @param vert_file The filename of the vetex shader.
 * @param frag_file The filename of the fragment shader.
 * @return True on success.
 */
static bool create_shader( GLhandleARB program, const char* vert_file, const char* frag_file )
{
    bool rv = true;

    std::cout
        << "Loading vertex shader " << vert_file
        << "and fragment shader " << frag_file << std::endl;

    // Load vertex shader
    rv = rv && load_shader( vert_file, GL_VERTEX_SHADER, program );
    // Load fragment shader
    rv = rv && load_shader( frag_file, GL_FRAGMENT_SHADER, program );

    if ( !rv )
        return false;

    // link
    glLinkProgram( program );

    // check for success
    GLint result;
    glGetProgramiv( program, GL_LINK_STATUS, &result );
    if ( result == GL_TRUE ) {
        std::cout << "Successfully linked shader" << std::endl;
        return true;
    } else {
        std::cout << "FAILED to link shader" << std::endl;
        return false;
    }
}


// definitions of functions for the GlslProject class

// constructor, invoked when object is created
GlslProject::GlslProject()
{
    // TODO any basic construction or initialization of members
    // Warning: Although members' constructors are automatically called,
    // ints, floats, pointers, and classes with empty contructors all
    // will have uninitialized data!
}

// destructor, invoked when object is destroyed
GlslProject::~GlslProject()
{
    // Warning: Do not throw exceptions or call virtual functions from deconstructors!
    // They will cause undefined behavior (probably a crash, but perhaps worse).
}

/**
 * Initialize the project, loading the mesh from the given filename.
 * Also do any necessary opengl initialization.
 * @param renderer Object that renders the scene.
 * @param width The width in pixels of the framebuffer.
 * @param height The height in pixels of the framebuffer.
 * @return true on success, false on error.
 * @see scene/mesh.hpp
 */
bool GlslProject::initialize( const SceneRenderer* renderer, int width, int height )
{
    bool rv = true;

    // copy renderer for later use
    this->renderer = renderer;

    // create a test shader
    program  = glCreateProgramObjectARB();
    rv = rv && create_shader( program, "shaders/vertex_outline.glsl", "shaders/fragment_outline.glsl" );

    this->width = width;
    this->height = height;

    // texture
    glGenTextures(1, &depthTexID);
    glGenTextures(1, &colorTexID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB,colorTexID);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB,depthTexID);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);

  // framebuffer
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_RECTANGLE_ARB,colorTexID,0);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_RECTANGLE_ARB,depthTexID,0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return rv;

}

/**
 * Clean up the project. Free any memory, etc.
 */
void GlslProject::destroy()
{
    glDeleteObjectARB( program );

    // TODO any cleanup code
    glDeleteTextures(1, &colorTexID);
    glDeleteTextures(1, &depthTexID);
    glDeleteFramebuffersEXT(1,&fbo);
}

/**
 * Render the scene with outlines using shaders.
 */
void GlslProject::render()
{
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fbo);
    renderer->render_scene();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);

    glUseProgramObjectARB( program );
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB,depthTexID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB,colorTexID);

	/***
	/* DECLARE AND SEND VALUES TO THE UNIFORM VARIABLES
	/**/

    GLint tex = glGetUniformLocation( program, "tex" );
    glUniform1iARB( tex, 0);

	GLint depth = glGetUniformLocation( program, "depth" );
    glUniform1iARB( depth, 1);

	GLint far_loc = glGetUniformLocation(program, "far");
	glUniform1iARB(far_loc, renderer->get_camera()->far_clip); 

	GLint near_loc = glGetUniformLocation(program, "near");
	glUniform1iARB(near_loc, renderer->get_camera()->near_clip); 

	GLint width_loc = glGetUniformLocationARB(program, "width" );
	glUniform1fARB(width_loc,width); //Bind width to width

	GLint height_loc = glGetUniformLocationARB(program, "height" );
	glUniform1fARB(height_loc,height);

	GLint threshold = glGetUniformLocationARB(program, "threshold" );
	glUniform1fARB(threshold,0.5f);
	
	/****/

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-1.0f, -1.0f,-1.0f);
        
    glTexCoord2f(width, 0.0f);
    glVertex3f(1.0f,-1.0f,-1.0f);
        
    glTexCoord2f(width, height);
    glVertex3f(1.0f,1.0f,-1.0f);
        
    glTexCoord2f(0.0f,height);
    glVertex3f(-1.0f,1.0f,-1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // then unbind the shader when finished
    glUseProgramObjectARB( 0 );
	renderer->render_scene();
}

} /* _462 */

