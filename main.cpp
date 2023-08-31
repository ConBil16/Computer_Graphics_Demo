/*
 *  CSCI 441, Computer Graphics, Fall 2022
 *
 *  Project: a3
 *  File: main.cpp
 *
 *  Description:
 *      Formerly lab 5, adapted to a3
 *
 *  Author: Eric Tyskiewicz, 2022
 *
 */

#include "finalEngine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

///*****************************************************************************
//
// Our main function
int main() {

    auto labEngine = new finalEngine();
    labEngine->initialize();
    if (labEngine->getError() == CSCI441::OpenGLEngine::OPENGL_ENGINE_ERROR_NO_ERROR) {
        labEngine->run();
    }
    labEngine->shutdown();
    delete labEngine;

	return EXIT_SUCCESS;
}
