#ifndef LAB05_LAB05_ENGINE_HPP
#define LAB05_LAB05_ENGINE_HPP

#include <CSCI441/FreeCam.hpp>
#include <CSCI441/OpenGLEngine.hpp>
#include <CSCI441/ShaderProgram.hpp>
#include <CSCI441/TextureUtils.hpp>

#include "EtheOrb.hpp"
#include "ArcballCam.h"
#include "FPVCam.h"
#include "ConnorVehicle.hpp"
#include "SterlingVehicle.hpp"

#include <vector>
#include <ctime>

class finalEngine : public CSCI441::OpenGLEngine {
public:
    finalEngine();
    ~finalEngine();

    void run() final;


    /// \desc handle any key events inside the engine
    /// \param key key as represented by GLFW_KEY_ macros
    /// \param action key event action as represented by GLFW_ macros
    void handleKeyEvent(GLint key, GLint action);

    /// \desc handle any mouse button events inside the engine
    /// \param button mouse button as represented by GLFW_MOUSE_BUTTON_ macros
    /// \param action mouse event as represented by GLFW_ macros
    void handleMouseButtonEvent(GLint button, GLint action);

    /// \desc handle any cursor movement events inside the engine
    /// \param currMousePosition the current cursor position
    void handleCursorPositionEvent(glm::vec2 currMousePosition);

    /// \desc value off-screen to represent mouse has not begun interacting with window yet
    static constexpr GLfloat MOUSE_UNINITIALIZED = -9999.0f;
    void _createQuad(GLuint vao, GLuint vbo, GLuint ibo, GLsizei &numVAOPoints) const;

private:
    bool cageIf = false;
    bool curveIf = false;
    bool arcIf = false;

    GLuint SkyBoxVao;
    GLuint SkyBoxVbo;
    GLuint SkyBoxIbo;
    GLsizei _numVAOPoints = 36;
    GLuint _textureHandle;

    //Texture stuff
    CSCI441::ShaderProgram* _textureShaderProgram = nullptr;   // the wrapper for our shader program
    /// \desc stores the locations of all of our shader uniforms
    struct TextureShaderUniformLocations {
        /// \desc precomputed MVP matrix location
        GLint mvpMatrix;
        GLint texMap;

    } _textureShaderUniformLocations;
    /// \desc stores the locations of all of our shader attributes
    struct TextureShaderAttributeLocations {
        /// \desc vertex position location
        GLint vPos;
        /// \desc vertex normal location
        /// \note not used in this lab
        GLint vNormal;
        GLint texCoord;

    } _textureShaderAttributeLocations;


    //Bezier Curve Stuff
    GLuint _bezierVao;
    GLuint _bezierVbo;
    GLuint _bezierIbo;
    GLsizei _numBezierPoints = 0;

    //Cage
    GLuint _cageVao;
    GLuint _cageVbo;
    GLuint _cageIbo;
    GLsizei _numCagePoints = 0;

    void _setupGLFW() final;
    void _setupOpenGL() final;
    void _setupShaders() final;
    void _setupBuffers() final;
    void _setupScene() final;

    //Texture functions
    GLuint _loadAndRegisterTexture(const char* FILENAME);
    void _setupTextures() final;

    //Bezier functions
    void _loadControlPointsFromFile(const char* FILENAME, GLuint *numBezierPoints, GLuint *numBezierCurves, glm::vec3* &bezierPoints);
    void _createCurve(GLuint vao, GLuint vbo, GLsizei &numVAOPoints) const;
    [[nodiscard]] static glm::vec3 _evalBezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, GLfloat t);
    void _createCage(GLuint vao, GLuint vbo, GLsizei &numVAOPoints) const;
    struct BezierCurve {
        /// \desc control points array
        glm::vec3* controlPoints = nullptr;
        /// \desc number of control points in the curve system.
        /// \desc corresponds to the size of controlPoints array
        GLuint numControlPoints;
        /// \desc number of curves in the system
        GLuint numCurves;
        GLfloat currPos = 0.0f;
    } _bezierCurve;

    //Flat shader stuff
    CSCI441::ShaderProgram* _flatShaderProgram = nullptr;
    struct FlatShaderProgramUniformLocations {
        /// \desc precomputed MVP matrix location
        GLint mvpMatrix;
        /// \desc the color to apply location
        GLint color;
    } _flatShaderProgramUniformLocations;

    struct FlatShaderProgramAttributeLocations {
        /// \desc vertex position location
        GLint vPos;
    } _flatShaderProgramAttributeLocations;

    //Other functions
    void _cleanupBuffers() final;
    void _cleanupShaders() final;

    /// \desc draws everything to the scene from a particular point of view
    /// \param viewMtx the current view matrix for our camera
    /// \param projMtx the current projection matrix for our camera
    void _renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) const;
    /// \desc handles moving our FreeCam as determined by keyboard input
    void _updateScene();

    glm::vec3 _checks(glm::vec3 newPos);

    /// \desc tracks the number of different keys that can be present as determined by GLFW
    static constexpr GLuint NUM_KEYS = GLFW_KEY_LAST;
    /// \desc boolean array tracking each key state.  if true, then the key is in a pressed or held
    /// down state.  if false, then the key is in a released state and not being interacted with
    GLboolean _keys[NUM_KEYS];

    /// \desc last location of the mouse in window coordinates
    glm::vec2 _mousePosition;
    /// \desc current state of the left mouse button
    GLint _leftMouseButtonState;

    /// \desc the static fixed camera in our world
    ArcballCam* _arcball;
    ArcballCam* _topDown;

    bool _arcballActive = true;
    /// \desc pair of values to store the speed the camera can move/rotate.
    /// \brief x = forward/backward delta, y = rotational delta
    glm::vec2 _cameraSpeed;

    /// \desc our plane model
    ConnorVehicle* _connor;
    Vehicle* _ster;

    /// \desc the size of the world (controls the ground size and locations of buildings)
    static constexpr GLfloat WORLD_SIZE = 55.0f;
    /// \desc VAO for our ground
    GLuint _groundVAO;
    /// \desc the number of points that make up our ground object
    GLsizei _numGroundPoints;

    /// \desc creates the ground VAO
    void _createGroundBuffers();

    /// \desc smart container to store information specific to each building we wish to draw
    struct BuildingData {
        /// \desc transformations to position and size the building
        glm::mat4 modelMatrix;
        /// \desc color to draw the building
        glm::vec4 diffuseColor;
        glm::vec4 ambientColor;
        glm::vec4 specularColor;
        float materialGloss;
        bool fancyBuilding;
    };
    /// \desc information list of all the buildings to draw
    std::vector<BuildingData> _buildings;

    /// \desc generates building information to make up our scene
    void _generateEnvironment();

    /// \desc shader program that performs lighting
    CSCI441::ShaderProgram* _lightingShaderProgram = nullptr;   // the wrapper for our shader program
    /// \desc stores the locations of all of our shader uniforms
    struct LightingShaderUniformLocations {
        /// \desc precomputed MVP matrix location
        GLint mvpMatrix;
        GLint normMatrix;
        GLint modelMatrix;
        GLint activeCamPos;

        /// \desc material color location
        GLint diffuseMaterialColor;
        GLint ambientMaterialColor;
        GLint specularMaterialColor;
        GLint materialGloss;

        GLint sLightPos;
        GLint sLightColor;
        GLint sLightDir;
        GLint sLightAngle;

        GLint pLightPos;
        GLint pLightColor;

        GLint dLightColor;
        GLint dLightDir;

    } _lightingShaderUniformLocations;
    /// \desc stores the locations of all of our shader attributes
    struct LightingShaderAttributeLocations {
        /// \desc vertex position location
        GLint vPos;
        GLint vNorm;

    } _lightingShaderAttributeLocations;

    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
    static void _computeAndSendTransformationMatrices(CSCI441::ShaderProgram* shaderProgram,
                                                      glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
                                                      GLint mvpMtxLocation, GLint modelMtxLocation = -1, GLint normalMtxLocation = -1);
};

void lab05_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods );
void lab05_engine_cursor_callback(GLFWwindow *window, double x, double y );
void lab05_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods );

#endif // LAB05_LAB05_ENGINE_HPP
