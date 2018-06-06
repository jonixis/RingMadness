#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#include "bRenderer.h"
#include "../headers/Ball.hpp"


class RingMadness : public IRenderProject
{
public:
	/* Constructor and Destructor */
	RingMadness() : IRenderProject(){}
    virtual ~RingMadness(){bRenderer::log("RenderProject deleted");}

    /* Initialize the Project */
    void init();
    
	//// RenderProject functions ////

    void extracted();
    
    /* This function is executed when initializing the renderer */
	void initFunction();
    
    /* Draw your scene here */
	void loopFunction(const double &deltaTime, const double &elapsedTime);

	/* This function is executed when terminating the renderer */
	void terminateFunction();

	//// iOS specific ////

    /* For iOS only: Handle device rotation */
    void deviceRotated();
    
    /* For iOS only: Handle app going into background */
    void appWillResignActive();
    
    /* For iOS only: Handle app coming back from background */
    void appDidBecomeActive();
    
    /* For iOS only: Handle app being terminated */
    void appWillTerminate();
    
	/* Make renderer accessible to public so we can get the UIView on iOS */
    Renderer& getProjectRenderer()
    {
        return bRenderer();
    }
    
private:
    
	/* Update render queue */
	void updateRenderQueue(const std::string &camera, const double &deltaTime);

	/* Camera movement */
	void updateCamera(const std::string &camera, const double &deltaTime);
    void updatePlane(const std::string &camera, const double &deltaTime);
    
    /* Ball collision */
    void checkBallCollision(Ball &ball);
    
    void showScore();
    void initBalls();

	/* Helper Functions */
	GLfloat randomNumber(GLfloat min, GLfloat max);

	/* Variables */
	GLfloat _randomOffset;
	GLfloat _offset;
	GLfloat _cameraSpeed;
	double _mouseX, _mouseY;
	bool _running = false; 
	GLint _lastStateSpaceKey = 0;
	vmml::Matrix4f _viewMatrixHUD;
    
    /* Postprocessing*/
    
    void beginPostprocessing(GLint &defaultFBO);
    
    void renderPauseScreen(GLint &defaultFBO);
    
    void endPostprocessing(GLint &defaultFBO);
    
    void renderBloomEffect(GLint &defaultFBO);

};

#endif /* defined(PROJECT_MAIN_H) */
