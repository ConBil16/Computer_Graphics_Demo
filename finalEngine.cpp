#include "finalEngine.hpp"

#include <CSCI441/objects.hpp>

//*************************************************************************************
//
// Helper Functions

#ifndef M_PI
#define M_PI 3.14159265
#endif

/// \desc Simple helper function to return a random number between 0.0f and 1.0f.
GLfloat getRand() {
    return (GLfloat)rand() / (GLfloat)RAND_MAX;
}

//*************************************************************************************
//
// Public Interface

finalEngine::finalEngine()
         : CSCI441::OpenGLEngine(4, 1,
                                 1280, 720,
                                 "Midterm Project: The Fellowship") {

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
    _leftMouseButtonState = GLFW_RELEASE;
}

finalEngine::~finalEngine() {
    delete _arcball;
}

void finalEngine::handleKeyEvent(GLint key, GLint action) {
    if(key != GLFW_KEY_UNKNOWN)
        _keys[key] = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));

    if(action == GLFW_PRESS) {
        switch (key) {
            // quit!
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                setWindowShouldClose();
                break;
            case GLFW_KEY_0:
                cageIf = !cageIf;
                break;
            case GLFW_KEY_1:
                curveIf = !curveIf;
                break;
            case GLFW_KEY_2:
                arcIf = !arcIf;
                break;
            default:
                break; // suppress CLion warning
        }
    }
}

void finalEngine::handleMouseButtonEvent(GLint button, GLint action) {
    // if the event is for the left mouse button
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
        // update the left mouse button's state
        _leftMouseButtonState = action;
    }

}

void finalEngine::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(_mousePosition.x == MOUSE_UNINITIALIZED) {
        _mousePosition = currMousePosition;
    }

    // if the left mouse button is being held down while the mouse is moving
    if(_leftMouseButtonState == GLFW_PRESS) {
        if (_arcballActive) {
            if (_keys[GLFW_KEY_LEFT_SHIFT]) { //if shift held, zoom
                _arcball->zoom(_mousePosition.y - currMousePosition.y);
            } else { //if not zooming, rotate
                _arcball->rotate((currMousePosition.x - _mousePosition.x) * 0.005f,
                                 (_mousePosition.y - currMousePosition.y) * 0.005f);
            }
        }

    }

    // update the mouse position
    _mousePosition = currMousePosition;
}

//*************************************************************************************
//
// Engine Setup

void finalEngine::_setupGLFW() {
    CSCI441::OpenGLEngine::_setupGLFW();

    // set our callbacks
    glfwSetKeyCallback(_window, lab05_engine_keyboard_callback);
    glfwSetMouseButtonCallback(_window, lab05_engine_mouse_button_callback);
    glfwSetCursorPosCallback(_window, lab05_engine_cursor_callback);
}

void finalEngine::_setupOpenGL() {
    glEnable( GL_DEPTH_TEST );					                        // enable depth testing
    glDepthFunc( GL_LESS );							                // use less than depth test

    glEnable(GL_BLEND);									            // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	    // use one minus blending equation

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	        // clear the frame buffer to black
}

void finalEngine::_setupShaders() {
    _flatShaderProgram = new CSCI441::ShaderProgram( "shaders/flatShader.v.glsl", "shaders/flatShader.f.glsl" );
    _flatShaderProgramUniformLocations.mvpMatrix           = _flatShaderProgram->getUniformLocation("mvpMatrix");
    _flatShaderProgramUniformLocations.color = _flatShaderProgram->getUniformLocation("color");
    //Attribute location
    _flatShaderProgramAttributeLocations.vPos              = _flatShaderProgram->getAttributeLocation("vPos");

    _lightingShaderProgram = new CSCI441::ShaderProgram("shaders/midtermGourad.v.glsl", "shaders/midtermGourad.f.glsl" );
    _lightingShaderUniformLocations.mvpMatrix      = _lightingShaderProgram->getUniformLocation("mvpMatrix");
    _lightingShaderUniformLocations.normMatrix = _lightingShaderProgram->getUniformLocation("normMatrix");
    _lightingShaderUniformLocations.modelMatrix = _lightingShaderProgram->getUniformLocation("modelMatrix");
    _lightingShaderUniformLocations.activeCamPos = _lightingShaderProgram->getUniformLocation("activeCamPos");

    _lightingShaderUniformLocations.diffuseMaterialColor  = _lightingShaderProgram->getUniformLocation("diffuseMaterialColor");
    _lightingShaderUniformLocations.ambientMaterialColor = _lightingShaderProgram->getUniformLocation("ambientMaterialColor");
    _lightingShaderUniformLocations.specularMaterialColor = _lightingShaderProgram->getUniformLocation("specularMaterialColor");
    _lightingShaderUniformLocations.materialGloss = _lightingShaderProgram->getUniformLocation("materialGloss");

    _lightingShaderUniformLocations.sLightPos = _lightingShaderProgram->getUniformLocation("sLightPos");
    _lightingShaderUniformLocations.sLightColor = _lightingShaderProgram->getUniformLocation("sLightColor");
    _lightingShaderUniformLocations.sLightDir = _lightingShaderProgram->getUniformLocation("sLightDir");
    _lightingShaderUniformLocations.sLightAngle = _lightingShaderProgram->getUniformLocation("sLightAngle");

    _lightingShaderUniformLocations.pLightPos = _lightingShaderProgram->getUniformLocation("pLightPos");
    _lightingShaderUniformLocations.pLightColor = _lightingShaderProgram->getUniformLocation("pLightColor");

    _lightingShaderUniformLocations.dLightColor = _lightingShaderProgram->getUniformLocation("dLightColor");
    _lightingShaderUniformLocations.dLightDir = _lightingShaderProgram->getUniformLocation("dLightDir");

    _lightingShaderAttributeLocations.vPos         = _lightingShaderProgram->getAttributeLocation("vPos");
    _lightingShaderAttributeLocations.vNorm = _lightingShaderProgram->getAttributeLocation("vNorm");

    _textureShaderProgram = new CSCI441::ShaderProgram("shaders/texture.v.glsl", "shaders/texture.f.glsl" );
    // query uniform locations
    _textureShaderUniformLocations.mvpMatrix      = _textureShaderProgram->getUniformLocation("mvpMatrix");
    _textureShaderUniformLocations.texMap = _textureShaderProgram->getUniformLocation("texMap");

    // query attribute locations
    _textureShaderAttributeLocations.vPos         = _textureShaderProgram->getAttributeLocation("vPos");
    _textureShaderAttributeLocations.vNormal      = _textureShaderProgram->getAttributeLocation("vNormal");
    _textureShaderAttributeLocations.texCoord = _textureShaderProgram->getAttributeLocation("texCoord");

    // set static uniforms
    _textureShaderProgram->setProgramUniform(_textureShaderUniformLocations.texMap,0);

}

void finalEngine::_setupBuffers() {

    glGenVertexArrays( 1, &SkyBoxVao);
    glGenBuffers(1, &SkyBoxVbo);
    glGenBuffers(1, &SkyBoxIbo);

    /*glGenVertexArrays(1, &_bezierVao);
    glGenBuffers(1, &_bezierVbo);
    glGenBuffers(1, &_bezierIbo);

    glGenVertexArrays(1, &_cageVao);
    glGenBuffers(1, &_cageVbo);
    glGenBuffers(1, &_cageIbo);*/




    _createQuad(SkyBoxVao, SkyBoxVbo, SkyBoxIbo, _numVAOPoints);


    CSCI441::setVertexAttributeLocations( _lightingShaderAttributeLocations.vPos, _lightingShaderAttributeLocations.vNorm );


    // Make heroes
    // ethe

    // connor
    _connor = new ConnorVehicle(_lightingShaderProgram->getShaderProgramHandle(),
                        _lightingShaderUniformLocations.mvpMatrix,
                        _lightingShaderUniformLocations.modelMatrix,
                        _lightingShaderUniformLocations.normMatrix,
                        _lightingShaderUniformLocations.diffuseMaterialColor,
                        _lightingShaderUniformLocations.specularMaterialColor,
                        _lightingShaderUniformLocations.ambientMaterialColor,
                        _lightingShaderUniformLocations.materialGloss);

    _ster = new Vehicle(_lightingShaderProgram->getShaderProgramHandle(),
                        _lightingShaderUniformLocations.mvpMatrix,
                        _lightingShaderUniformLocations.modelMatrix,
                        _lightingShaderUniformLocations.normMatrix,
                        _lightingShaderUniformLocations.diffuseMaterialColor,
                        _lightingShaderUniformLocations.specularMaterialColor,
                        _lightingShaderUniformLocations.ambientMaterialColor,
                        _lightingShaderUniformLocations.materialGloss);


    _createGroundBuffers();
    _generateEnvironment();



}

void finalEngine::_createQuad(GLuint vao, GLuint vbo, GLuint ibo, GLsizei &numVAOPoints) const {
    struct VertexNormalTextured {
        float x, y, z;
        float nx, ny, nz;
        float s, t;
    };

    // create our skybox
    VertexNormalTextured cubeVertices[24] = {
            //Front
            { -1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  0.25f, 0.34f }, // 0 - BL
            {  1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  .5f, 0.34f }, // 1 - BR
            { -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  0.25f, .66f }, // 2 - TL
            {  1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  .5f, .66f },  // 3 - TR

            //Right
            {1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, .5f, .34f}, //4 - BL
            {1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, .75f, .34f}, //5 - BR
            {1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, .5f, .66f}, //6 - TL
            {1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, .75f, .66f}, //7 - TR
            //Left
            {-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, .34f}, //8 - BL
            {-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, .25f, .34f}, //9 - BR
            {-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, .66f}, //10 - TL
            {-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, .25f, .66f}, //11 - TR
            //Back
            { -1.0f, -1.0f,  -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.34f }, // 12 - BL
            {  1.0f, -1.0f,  -1.0f,  0.0f, 1.0f, 0.0f,  .75f, 0.34f }, // 13 - BR
            { -1.0f,  1.0f,  -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, .66f }, // 14 - TL
            {  1.0f,  1.0f,  -1.0f,  0.0f, 1.0f, 0.0f,  .75f, .66f },  // 15 - TR
            //Top
            {-1.0f, 1.0f, 1.0f, 0.0f,1.0f, 0.0f, .25f, .66f },//16 - BL
            {1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, .5f, .66f}, //17 - BR
            {-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, .25f, 1.0f}, //18 - TL
            {1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, .5f, 1.0f}, //19- TR
            //Bottom
            {-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, .25f, 0.0f}, //20 - BL
            {1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, .5f, 0.0f}, //21 - BR
            {-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, .25f, .33f}, //23 - TL
            {1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, .5f, .33f} //22 - TR
    };

    GLushort cubeIndicies[36] = { 0, 1, 2, 1, 2, 3,
    4, 5, 6, 5, 6, 7,
    8, 9, 10, 9, 10, 11,
    12, 13, 14, 13, 14, 15,
    16, 17, 18, 17, 18, 19,
    20, 21, 22, 21, 22, 23};


    numVAOPoints = 36;

    glBindVertexArray( vao );

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( cubeVertices ), cubeVertices, GL_STATIC_DRAW );

    glEnableVertexAttribArray( _textureShaderAttributeLocations.vPos );
    glVertexAttribPointer( _textureShaderAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormalTextured), (void*) 0 );

    glEnableVertexAttribArray( _textureShaderAttributeLocations.vNormal );
    glVertexAttribPointer( _textureShaderAttributeLocations.vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormalTextured), (void*) (sizeof(float) * 3) );

    // do TO DO #16 again
    glEnableVertexAttribArray(_textureShaderAttributeLocations.texCoord );
    glVertexAttribPointer( _textureShaderAttributeLocations.texCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormalTextured), (void*) (sizeof(float) * 6) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( cubeIndicies ), cubeIndicies, GL_STATIC_DRAW );

    fprintf( stdout, "[INFO]: quad read in with VAO/VBO/IBO %d/%d/%d & %d points\n", vao, vbo, ibo, numVAOPoints );
//    // UNCOMMENT END HERE
}

void finalEngine::_createGroundBuffers() {

    // borrowed from buildings
    const GLfloat GRID_WIDTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_LENGTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_SPACING_WIDTH = 1.0f;
    const GLfloat GRID_SPACING_LENGTH = 1.0f;
    // precomputed parameters based on above
    const GLfloat LEFT_END_POINT = -GRID_WIDTH / 2.0f - 5.0f;
    const GLfloat RIGHT_END_POINT = GRID_WIDTH / 2.0f + 5.0f;
    const GLfloat BOTTOM_END_POINT = -GRID_LENGTH / 2.0f - 5.0f;
    const GLfloat TOP_END_POINT = GRID_LENGTH / 2.0f + 5.0f;


    struct Vertex {
        GLfloat x, y, z;
        GLfloat nX, nY, nZ;
    };

    std::vector<Vertex> groundQuad;

    std::vector<GLushort> indices;
    _numGroundPoints = 0;

    for(int i = LEFT_END_POINT; i < RIGHT_END_POINT; i += GRID_SPACING_WIDTH) {
        groundQuad.push_back({float(i) / WORLD_SIZE, 0.0f, BOTTOM_END_POINT / WORLD_SIZE, 0.0f, 1.0f, 0.0f});
        groundQuad.push_back({float(i) / WORLD_SIZE, 0.0f, TOP_END_POINT / WORLD_SIZE, 0.0f, 1.0f, 0.0f});
        indices.push_back(_numGroundPoints);
        _numGroundPoints++;
        indices.push_back(_numGroundPoints);
        _numGroundPoints++;
    }
    for(int j = BOTTOM_END_POINT; j < TOP_END_POINT; j += GRID_SPACING_LENGTH) {
        groundQuad.push_back({LEFT_END_POINT / WORLD_SIZE, 0.0f, float(j) / WORLD_SIZE, 0.0f, 1.0f, 0.0f});
        groundQuad.push_back({RIGHT_END_POINT / WORLD_SIZE, 0.0f, float(j) / WORLD_SIZE, 0.0f, 1.0f, 0.0f});
        indices.push_back(_numGroundPoints);
        _numGroundPoints++;
        indices.push_back(_numGroundPoints);
        _numGroundPoints++;
    }





    glGenVertexArrays(1, &_groundVAO);
    glBindVertexArray(_groundVAO);

    GLuint vbods[2];       // 0 - VBO, 1 - IBO
    glGenBuffers(2, vbods);
    glBindBuffer(GL_ARRAY_BUFFER, vbods[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _numGroundPoints, &groundQuad[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vPos);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vNorm);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vNorm, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * _numGroundPoints, &indices[0], GL_STATIC_DRAW);
}

void finalEngine::_generateEnvironment() {
    //******************************************************************
    // parameters to make up our grid size and spacing, feel free to
    // play around with this
    const GLfloat GRID_WIDTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_LENGTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_SPACING_WIDTH = 1.0f;
    const GLfloat GRID_SPACING_LENGTH = 1.0f;
    // precomputed parameters based on above
    const GLfloat LEFT_END_POINT = -GRID_WIDTH / 2.0f - 5.0f;
    const GLfloat RIGHT_END_POINT = GRID_WIDTH / 2.0f + 5.0f;
    const GLfloat BOTTOM_END_POINT = -GRID_LENGTH / 2.0f - 5.0f;
    const GLfloat TOP_END_POINT = GRID_LENGTH / 2.0f + 5.0f;
    //******************************************************************

    srand( time(0) );                                                   // seed our RNG

    // psych! everything's on a grid.
    for(int i = LEFT_END_POINT; i < RIGHT_END_POINT; i += GRID_SPACING_WIDTH) {
        for(int j = BOTTOM_END_POINT; j < TOP_END_POINT; j += GRID_SPACING_LENGTH) {
            // don't just draw a building ANYWHERE.
            if( i % 2 && j % 2 && getRand() < 0.4f ) {
                bool fancyBuilding = (bool)(round(getRand()));
                // translate to spot
                glm::mat4 transToSpotMtx = glm::translate( glm::mat4(1.0), glm::vec3(i, 0.0f, j) );

                // compute random height
                GLdouble height = powf(getRand(), 2.5)*10 + 1;
                // scale to building size
                glm::mat4 scaleToHeightMtx = glm::scale( glm::mat4(1.0), glm::vec3(1, height, 1) );

                // translate up to grid
                glm::mat4 transToHeight = glm::mat4(1.0f);
                if (fancyBuilding) {
                    transToHeight = glm::translate(glm::mat4(1.0), glm::vec3(0, height / 2.0f, 0));
                }

                // compute full model matrix
                glm::mat4 modelMatrix = transToHeight * scaleToHeightMtx * transToSpotMtx;

                // compute random color
                glm::vec4 diffuseColor( getRand(), getRand(), getRand(), 1.0f );
                glm::vec4 ambientColor = glm::vec4(glm::vec3 (diffuseColor) * 0.1f, 1.0f);
                // fprintf(stdout, "%f %f %f \n", ambientColor.x, ambientColor.y, ambientColor.z);
                glm::vec4 specularColor (1.0f, 1.0f, 1.0f, 1.0f);
                float materialGloss = 1.0f;
                // store building properties
                BuildingData currentBuilding = {modelMatrix, diffuseColor, ambientColor, specularColor, materialGloss, fancyBuilding};
                _buildings.emplace_back( currentBuilding );
            }
        }
    }
}

void finalEngine::_setupScene() {
    _arcball = new ArcballCam();
    _arcball->setPosition(glm::vec3(60.0f, 40.0f, 30.0f) );
    _arcball->setTheta(-M_PI / 3.0f );
    _arcball->setPhi(M_PI / 2.8f );
    _arcball->recomputeOrientation();
    _cameraSpeed = glm::vec2(0.25f, 0.02f);


    //New Top Down camera
    //TODO: Implement new arcball camera
    _topDown = new ArcballCam();


    // lighting uniforms
    // directional
    glm::vec3 dLightDir = glm::vec3(-1.0, -1.0, -1.0);
    glm::vec3 dLightColor = glm::vec3(1.0, 1.0, 1.0);

    glProgramUniform3fv(	_lightingShaderProgram->getShaderProgramHandle(),
                            _lightingShaderUniformLocations.dLightDir,
                            1,
                            &dLightDir[0]
    );
    glProgramUniform3fv(	_lightingShaderProgram->getShaderProgramHandle(),
                            _lightingShaderUniformLocations.dLightColor,
                            1,
                            &dLightColor[0]
    );

    // spotlight
    glm::vec3 sLightPos = glm::vec3(0.1, 0.0, 0.0);
    glm::vec3 sLightColor = glm::vec3(1.0, 1.0, 1.0);
    glm::vec3 sLightDir = glm::vec3(1.0, 0.0, 0.0);
    float sLightAngle = 0.2f;

    glProgramUniform3fv(	_lightingShaderProgram->getShaderProgramHandle(),
                            _lightingShaderUniformLocations.sLightPos,
                            1,
                            &sLightPos[0]
    );
    glProgramUniform3fv(	_lightingShaderProgram->getShaderProgramHandle(),
                            _lightingShaderUniformLocations.sLightColor,
                            1,
                            &sLightColor[0]
    );
    glProgramUniform3fv(	_lightingShaderProgram->getShaderProgramHandle(),
                            _lightingShaderUniformLocations.sLightDir,
                            1,
                            &sLightDir[0]
    );
    glProgramUniform1f(	_lightingShaderProgram->getShaderProgramHandle(),
                            _lightingShaderUniformLocations.sLightAngle,
                            sLightAngle
    );

    // point
    glm::vec3 pLightPos = glm::vec3(0.1, -0.1, 0.1);
    glm::vec3 pLightColor = glm::vec3(1.0, 1.0, 1.0);

    glProgramUniform3fv(	_lightingShaderProgram->getShaderProgramHandle(),
                            _lightingShaderUniformLocations.pLightPos,
                            1,
                            &pLightPos[0]
    );
    glProgramUniform3fv(	_lightingShaderProgram->getShaderProgramHandle(),
                            _lightingShaderUniformLocations.pLightColor,
                            1,
                            &pLightColor[0]
    );

}

//*************************************************************************************
//
// Engine Cleanup

void finalEngine::_cleanupShaders() {
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    delete _lightingShaderProgram;
}

void finalEngine::_cleanupBuffers() {
    fprintf( stdout, "[INFO]: ...deleting VAOs....\n" );
    CSCI441::deleteObjectVAOs();
    glDeleteVertexArrays( 1, &_groundVAO );

    fprintf( stdout, "[INFO]: ...deleting VBOs....\n" );
    CSCI441::deleteObjectVBOs();

    fprintf( stdout, "[INFO]: ...deleting models..\n" );
    delete _connor;
    delete _ster;
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void finalEngine::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // use our lighting shader program
    _textureShaderProgram->useProgram();

    if (_arcballActive) {
        glUniform3fv(_lightingShaderUniformLocations.activeCamPos, 1, &_arcball->getPosition()[0]);
    }


    //// BEGIN DRAWING THE GROUND PLANE ////
    // draw the ground plane
    glm::mat4 poop = glm::translate(glm::mat4(1.0f), _arcball->getPosition());
    _computeAndSendMatrixUniforms(poop, viewMtx, projMtx);

    // Quad

    glBindTexture(GL_TEXTURE_2D, _textureHandle);

    glBindVertexArray( _textureHandle );
    glDrawElements( GL_TRIANGLES, _numVAOPoints, GL_UNSIGNED_SHORT, (void*)0 );
    glClear(GL_DEPTH_BUFFER_BIT);

    //Need to make

    _lightingShaderProgram->useProgram();
    glm::mat4 groundModelMtx = glm::scale( glm::mat4(1.0f), glm::vec3(WORLD_SIZE, 1.0f, WORLD_SIZE));
    _computeAndSendMatrixUniforms(groundModelMtx, viewMtx, projMtx);

    glm::vec4 diffuseGroundColor(0.9f, 0.9f, 0.9f, 1.0f);
    glm::vec4 ambientGroundColor(0.1f, 0.1f, 0.1f, 1.0f);
    glm::vec4 specularGroundColor(1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4fv(_lightingShaderUniformLocations.diffuseMaterialColor, 1, &diffuseGroundColor[0]);
    glUniform4fv(_lightingShaderUniformLocations.ambientMaterialColor, 1, &ambientGroundColor[0]);
    glUniform4fv(_lightingShaderUniformLocations.specularMaterialColor, 1, &specularGroundColor[0]);
    glUniform1f(_lightingShaderUniformLocations.materialGloss, 1.0f);

    glBindVertexArray(_groundVAO);
    glDrawElements(GL_LINES, _numGroundPoints, GL_UNSIGNED_SHORT, (void*)0);
    //// END DRAWING THE GROUND PLANE ////

    //// BEGIN DRAWING THE BUILDINGS ////


    //TODO: Replace buildings with trees

    for( const BuildingData& currentBuilding : _buildings ) {
        _computeAndSendMatrixUniforms(currentBuilding.modelMatrix, viewMtx, projMtx);

        glUniform4fv(_lightingShaderUniformLocations.diffuseMaterialColor, 1, &currentBuilding.diffuseColor[0]);
        glUniform4fv(_lightingShaderUniformLocations.ambientMaterialColor, 1, &currentBuilding.ambientColor[0]);
        glUniform4fv(_lightingShaderUniformLocations.specularMaterialColor, 1, &currentBuilding.specularColor[0]);
        glUniform1f(_lightingShaderUniformLocations.materialGloss, currentBuilding.materialGloss);

        if (currentBuilding.fancyBuilding) { CSCI441::drawSolidCube(1.0); }
        else { CSCI441::drawSolidCone(0.5,1.0,16,16);}
    }
    //// END DRAWING THE BUILDINGS ////

    //// BEGIN DRAWING THE PLANE ////
    glm::mat4 modelMtx(1.0f);
    // we are going to cheat and use our look at point to place our plane so that it is always in view
    _connor->drawPlane(modelMtx, viewMtx, projMtx);

    modelMtx = glm::translate(modelMtx, _connor->charPos);
    modelMtx = glm::rotate(modelMtx, _connor->charRot, glm::vec3(0.0f, 1.0f, 0.0f));

    //temporary float to orient plane with _direction
    //float placeholderAngle = glm::acos(glm::dot(glm::normalize(_arcball->getDirection()), glm::vec3(1.0f, 0.0f, 0.0f)));

    // rotate the plane with our camera theta direction (we need to rotate the opposite direction so that we always look at the back)
    // rotate the plane with our camera phi direction
    // modelMtx = glm::rotate(modelMtx, glm::radians(90.0f), CSCI441::X_AXIS );
    // modelMtx = glm::rotate(modelMtx, glm::radians(-90.0f), CSCI441::Z_AXIS );
    // draw our plane now



    //// END DRAWING THE PLANE ////

}

void finalEngine::_setupTextures() {
    _textureHandle = _loadAndRegisterTexture("texture/cubeMapNight.jpg");

}


GLuint finalEngine::_loadAndRegisterTexture(const char* FILENAME) {
    // our handle to the GPU
    GLuint textureHandle = 0;

    // enable setting to prevent image from being upside down
    stbi_set_flip_vertically_on_load(true);

    // will hold image parameters after load
    GLint imageWidth, imageHeight, imageChannels;
    // load image from file
    GLubyte* data = stbi_load( FILENAME, &imageWidth, &imageHeight, &imageChannels, 0);

    // if data was read from file
    if( data ) {
        const GLint STORAGE_TYPE = (imageChannels == 4 ? GL_RGBA : GL_RGB);
        glGenTextures(1, &textureHandle);
        glBindTexture(GL_TEXTURE_2D, textureHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, STORAGE_TYPE, imageWidth, imageHeight, 0, STORAGE_TYPE, GL_UNSIGNED_BYTE, data);

        fprintf( stdout, "[INFO]: %s texture map read in with handle %d\n", FILENAME, textureHandle);

        // release image memory from CPU - it now lives on the GPU
        stbi_image_free(data);
    } else {
        // load failed
        fprintf( stderr, "[ERROR]: Could not load texture map \"%s\"\n", FILENAME );
    }

    // return generated texture handle
    return textureHandle;
}

void finalEngine::_loadControlPointsFromFile(const char* FILENAME, GLuint *numBezierPoints, GLuint *numBezierCurves, glm::vec3* &bezierPoints) {
    // open the file
    FILE *file = fopen(FILENAME, "r");

    // check that the file opened properly
    if(!file) {
        fprintf( stderr, "[ERROR]: Could not open \"%s\"\n", FILENAME );
    } else {
        // first value is the number of points in the file
        fscanf( file, "%u\n", numBezierPoints );

        *numBezierCurves = (*numBezierPoints-1)/3;

        fprintf( stdout, "[INFO]: Reading in %u control points\n", *numBezierPoints );

        // allocate memory
        bezierPoints = (glm::vec3*)malloc( sizeof( glm::vec3 ) * *numBezierPoints );
        if(!bezierPoints) {
            fprintf( stderr, "[ERROR]: Could not allocate space for control points\n" );
        } else {
            // read in all the points
            for( int i = 0; i < *numBezierPoints; i++ ) {
                // each line is formatted as "x,y,z\n" as comma seperated floats
                fscanf( file, "%f,%f,%f\n", &(bezierPoints[i].x), &(bezierPoints[i].y), &(bezierPoints[i].z));
            }
        }
    }
    // close the file
    fclose(file);
}

glm::vec3 finalEngine::_evalBezierCurve(const glm::vec3 P0, const glm::vec3 P1, const glm::vec3 P2, const glm::vec3 P3, const GLfloat T) {
    glm::vec3 bezierPoint = (((1-T)*(1-T)*(1-T)) * P0) + (3.0f*((1-T) * (1-T)) * T * P1) + (3.0f*(1-T)*(T * T) * P2) + ((T * T * T) * P3);
    return bezierPoint;
}

void finalEngine::_createCurve(GLuint vao, GLuint vbo, GLsizei &numVAOPoints) const {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    int resolution = 24;


    numVAOPoints = _bezierCurve.numCurves * (resolution + 1);

    std::vector<glm::vec3> points;

    for(int i = 0; i < _bezierCurve.numCurves; i++){
        glm::vec3 p0 = _bezierCurve.controlPoints[3 * i];
        glm::vec3 p1 = _bezierCurve.controlPoints[(3 * i) + 1];
        glm::vec3 p2 = _bezierCurve.controlPoints[(3 * i) + 2];
        glm::vec3 p3 = _bezierCurve.controlPoints[(3 * i) + 3];

        for (int j = 0; j <= resolution; j++){
            points.push_back(_evalBezierCurve(p0, p1, p2, p3, (float)j/(float)resolution));
        }
    }

    glBufferData( GL_ARRAY_BUFFER, numVAOPoints * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW );

    glEnableVertexAttribArray( _flatShaderProgramAttributeLocations.vPos );
    glVertexAttribPointer( _flatShaderProgramAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, 0, nullptr );


    fprintf( stdout, "[INFO]: bezier curve read in with VAO/VBO %d/%d & %d points\n", vao, vbo, numVAOPoints );
}

void finalEngine::_createCage(GLuint vao, GLuint vbo, GLsizei &numVAOPoints) const {
    numVAOPoints = _bezierCurve.numControlPoints;

    glBindVertexArray( vao );

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, numVAOPoints * sizeof(glm::vec3), _bezierCurve.controlPoints, GL_STATIC_DRAW );

    glEnableVertexAttribArray(_flatShaderProgramAttributeLocations.vPos);
    glVertexAttribPointer(_flatShaderProgramAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    fprintf( stdout, "[INFO]: control points cage read in with VAO/VBO %d/%d & %d points\n", vao, vbo, numVAOPoints );
}

void finalEngine::_updateScene() {

    if (_keys[GLFW_KEY_D]) {
        _connor->turnRight(0.05f);
    }
    if (_keys[GLFW_KEY_A]) {
        _connor->turnLeft(0.05f);
    }
    if (_keys[GLFW_KEY_S]) {
        _connor->moveBackward(0.05f);
    }
    if (_keys[GLFW_KEY_W]) {
        _connor->moveForward(0.05f);
    }
    _arcball->setPostion1(_connor->charPos);
}


glm::vec3 finalEngine::_checks(glm::vec3 newPos) {
    // keep character in bounds
    if (newPos.x > WORLD_SIZE - 1) {
        newPos.x = WORLD_SIZE - 1;
    }
    if (newPos.x < -WORLD_SIZE + 1) {
        newPos.x = -WORLD_SIZE + 1;
    }
    if (newPos.z > WORLD_SIZE - 1) {
        newPos.z = WORLD_SIZE - 1;
    }
    if (newPos.z < -WORLD_SIZE + 1) {
        newPos.z = -WORLD_SIZE + 1;
    }
    return newPos;
}

void finalEngine::run() {
    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while( !glfwWindowShouldClose(_window) ) {	        // check if the window was instructed to be closed
        glDrawBuffer( GL_BACK );				        // work with our back frame buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

        // Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
        // when using a Retina display the actual window can be larger than the requested window.  Therefore,
        // query what the actual size of the window we are rendering to is.
        GLint framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize( _window, &framebufferWidth, &framebufferHeight );

        GLint miniWindowWidth = 100;
        GLint miniWindowHeight = 100;

        // update the viewport - tell OpenGL we want to render to the whole window
        glViewport( 0, 0, framebufferWidth, framebufferHeight );

        // set the projection matrix based on the window size
        // use a perspective projection that ranges
        // with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 1000].
        glm::mat4 projectionMatrix = glm::perspective( 45.0f, (GLfloat) framebufferWidth / (GLfloat) framebufferHeight, 0.001f, 1000.0f );

        // set up our look at matrix to position our camera
        glm::mat4 viewMatrix(1.0);
        viewMatrix = _arcball -> getViewMatrix();

        // draw everything to the window
        _renderScene(viewMatrix, projectionMatrix);

        _updateScene();

        glfwSwapBuffers(_window);                       // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen
    }
}

//*************************************************************************************
//
// Private Helper FUnctions

void finalEngine::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.mvpMatrix, mvpMtx);
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.modelMatrix, modelMtx);

    glm::mat3 normalMtx = glm::mat3(glm::transpose(glm::inverse(modelMtx)));
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.normMatrix, normalMtx);


}

void finalEngine::_computeAndSendTransformationMatrices(CSCI441::ShaderProgram* shaderProgram,
                                                        glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
                                                        GLint mvpMtxLocation, GLint modelMtxLocation, GLint normalMtxLocation) {
    // ensure our shader program is not null
    if( shaderProgram ) {
        // precompute the MVP matrix CPU side
        glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
        // precompute the Normal matrix CPU side
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

        // send the matrices to the shader
        shaderProgram->setProgramUniform(mvpMtxLocation, mvpMatrix);
        shaderProgram->setProgramUniform(modelMtxLocation, modelMatrix);
        shaderProgram->setProgramUniform(normalMtxLocation, normalMatrix);
    }
}

//*************************************************************************************
//
// Callbacks

void lab05_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods ) {
    auto engine = (finalEngine*) glfwGetWindowUserPointer(window);

    // pass the key and action through to the engine
    engine->handleKeyEvent(key, action);
}

void lab05_engine_cursor_callback(GLFWwindow *window, double x, double y ) {
    auto engine = (finalEngine*) glfwGetWindowUserPointer(window);

    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent(glm::vec2(x, y));
}

void lab05_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods ) {
    auto engine = (finalEngine*) glfwGetWindowUserPointer(window);

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent(button, action);
}
