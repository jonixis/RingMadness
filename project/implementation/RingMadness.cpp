#include "RingMadness.h"
#include "../headers/Ball.hpp"

/* Helper functions */
GLfloat RingMadness::randomNumber(GLfloat min, GLfloat max){
    return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}

/* Initialize the Project */
void RingMadness::init()
{
	bRenderer::loadConfigFile("config.json");	// load custom configurations replacing the default values in Configuration.cpp

	// let the renderer create an OpenGL context and the main window
	if(Input::isTouchDevice())
		bRenderer().initRenderer(true);										// full screen on iOS
	else
		bRenderer().initRenderer(1920, 1080, false, "The Cave - Demo");		// windowed mode on desktop
		//bRenderer().initRenderer(View::getScreenWidth(), View::getScreenHeight(), true);		// full screen using full width and height of the screen

	// start main loop 
	bRenderer().runRenderer();
}

//ball and score variables
int score = 0;
std::ostringstream scoreStr;

Ball ball0, ball1, ball2, ball3, ball4, ball5, ball6, ball7, ball8, ball9, ball10;

GLfloat mapRadius = 200.f;
GLfloat maxHeight = 100.f;
float plane_radius = 5.f;



//plane Variables
vmml::Matrix4f planeModelMatrix;
vmml::Matrix4f planeModelMatrixTwo;
vmml::Matrix4f planeRotationMatrix;
vmml::Matrix4f planeTransaltionMatrix;
vmml::Vector3f planePosition;
vmml::Vector3f PLANESPEED = vmml::Vector3f(0.0f,0.0f, 1.0f);
float planeCurrentPitch = 0;
float planeCurrentYaw = 0;
float planeMaxMinPitch = 0;
float planeCurrentRoll = 0;
float planeLastRoll = 0;

bool thirdPerson = true;

//cloud Variables
vmml::Vector4f cloudPosition = vmml::Vector3f(500.0f,200.0f,100.0f);

//Sun variables
vmml::Vector4f sunPosition;

// Fog
vmml::Vector4f fogColor = vmml::Vector4f(0.95f, 0.95f, 0.95f);

// Camera Variables
vmml::Matrix4f cameraModelMatrix;
vmml::Vector3f cameraPosition;
vmml::Vector3f cameraTargetPosition;

// Shaders
ShaderPtr terrainShader;
ShaderPtr seaShader;
ShaderPtr objectShader;
ShaderPtr ballShader;

/* This function is executed when initializing the renderer */
void RingMadness::initFunction()
{
	// get OpenGL and shading language version
	bRenderer::log("OpenGL Version: ", glGetString(GL_VERSION));
	bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// initialize variables
	_offset = 0.0f;
	_randomOffset = 0.0f;
	_cameraSpeed = 40.0f;
	_running = false; _lastStateSpaceKey = bRenderer::INPUT_UNDEFINED;
	_viewMatrixHUD = Camera::lookAtForHUD(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);
    
    thirdPerson = true;

	// set shader versions (optional)
	bRenderer().getObjects()->setShaderVersionDesktop("#version 120");
	bRenderer().getObjects()->setShaderVersionES("#version 100");
    
    //Load cube.frag & cube.vert for the skyCube
    ShaderPtr skyShader = bRenderer().getObjects()->loadShaderFile_o("cube");
    seaShader = bRenderer().getObjects()->loadShaderFile_o("sea");
    terrainShader = bRenderer().getObjects()->loadShaderFile_o("terrain");
    
    //Object Shader for all objects (E.g. Clouds, houses, etc...)
    objectShader = bRenderer().getObjects()->loadShaderFile_o("object");
    
    
    // Order of pictures to generate cubematp. left, right, bottom, top, front,  back
    skyShader->setUniform("texCube", bRenderer().getObjects()->loadCubeMap("textureCube", std::vector<std::string>({ "Sky.png", "Sky.png", "SkyBottom.png", "SkyTop.png", "Sky.png", "Sky.png" })));
    
    
    //Load Skycube object cube.obj. Name of shader used.
    bRenderer().getObjects()->loadObjModel_o("cube.obj", skyShader);
    
    // Terrain model
    bRenderer().getObjects()->loadObjModel_o("terrain.obj",terrainShader);
    bRenderer().getObjects()->loadObjModel_o("sea.obj", seaShader);
    bRenderer().getObjects()->loadObjModel_o("cloud1.obj", objectShader);
    
    // Sun model
    ShaderPtr sunShader = bRenderer().getObjects()->loadShaderFile("sun");
    bRenderer().getObjects()->loadObjModel_o("sun.obj", sunShader);
    
    // Plane model
    ShaderPtr planeShader = bRenderer().getObjects()->loadShaderFile("plane");
    planeShader->setUniform("Ia", vmml::Vector3f(0.9f));
    planeShader->setUniform("Id", vmml::Vector3f(0.9f));
    planeShader->setUniform("Is", vmml::Vector3f(0.9f));
    bRenderer().getObjects()->loadObjModel_o("plane.obj", planeShader);
    bRenderer().getObjects()->loadObjModel_o("rotor.obj",terrainShader);
    
    // Ball models
    ballShader = bRenderer().getObjects()->loadShaderFile("ball");
    bRenderer().getObjects()->loadObjModel_o("sphere.obj", ballShader);
    
    ball0 = Ball(vmml::Vector3f(0.f, 50.f, 0.f));
    ball1 = Ball(vmml::Vector3f(randomNumber(-mapRadius, mapRadius), randomNumber(-maxHeight, maxHeight), randomNumber(-mapRadius, mapRadius)));
    ball2 = Ball(vmml::Vector3f(randomNumber(-mapRadius, mapRadius), randomNumber(-maxHeight, maxHeight), randomNumber(-mapRadius, mapRadius)));
    ball3 = Ball(vmml::Vector3f(randomNumber(-mapRadius, mapRadius), randomNumber(-maxHeight, maxHeight), randomNumber(-mapRadius, mapRadius)));
    ball4 = Ball(vmml::Vector3f(randomNumber(-mapRadius, mapRadius), randomNumber(-maxHeight, maxHeight), randomNumber(-mapRadius, mapRadius)));
    ball5 = Ball(vmml::Vector3f(randomNumber(-mapRadius, mapRadius), randomNumber(-maxHeight, maxHeight), randomNumber(-mapRadius, mapRadius)));
    ball6 = Ball(vmml::Vector3f(randomNumber(-mapRadius, mapRadius), randomNumber(-maxHeight, maxHeight), randomNumber(-mapRadius, mapRadius)));
    ball7 = Ball(vmml::Vector3f(randomNumber(-mapRadius, mapRadius), randomNumber(-maxHeight, maxHeight), randomNumber(-mapRadius, mapRadius)));
    ball8 = Ball(vmml::Vector3f(randomNumber(-mapRadius, mapRadius), randomNumber(-maxHeight, maxHeight), randomNumber(-mapRadius, mapRadius)));
    ball9 = Ball(vmml::Vector3f(randomNumber(-mapRadius, mapRadius), randomNumber(-maxHeight, maxHeight), randomNumber(-mapRadius, mapRadius)));
    ball10 = Ball(vmml::Vector3f(randomNumber(-mapRadius, mapRadius), randomNumber(-maxHeight, maxHeight), randomNumber(-mapRadius, mapRadius)));
    

    //plane Start Position
    planeModelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, 0.0f)) * vmml::create_rotation(M_PI_F * 0.5f, vmml::Vector3f::UNIT_Y);
    
    //camera Start Position
    cameraPosition = vmml::Vector3f(-30.0f,0.0f,0.0f);
    
    //Sun Position
    sunPosition = vmml::Vector3f(0.0f,300.0f,0.0f);
    terrainShader->setUniform("sunPosition", sunPosition);
    seaShader->setUniform("sunPosition", sunPosition);
    objectShader->setUniform("sunPosition", sunPosition);
    
    //Fog
    terrainShader->setUniform("fogColor", fogColor);
    seaShader->setUniform("fogColor", fogColor);
    objectShader->setUniform("fogColor", fogColor);
    
    
    // create a sprite displaying the title as a texture
	bRenderer().getObjects()->createSprite("bTitle", "basicTitle_light.png");

	// create text sprites
	FontPtr font = bRenderer().getObjects()->loadFont("KozGoPro-ExtraLight.otf", 50);
	if (Input::isTouchDevice())
		bRenderer().getObjects()->createTextSprite("instructions", vmml::Vector3f(1.f, 1.0f, 1.f), "Double tap on the upper left half of the screen to start", font);
	else
		bRenderer().getObjects()->createTextSprite("instructions", vmml::Vector3f(1.f, 1.f, 1.f), "Press space to start", font);

	// create camera
	//bRenderer().getObjects()->createCamera("camera", vmml::Vector3f(-33.0, 0.f, -13.0), vmml::Vector3f(0.f, -M_PI_F / 2, 0.f));
    bRenderer().getObjects()->createCamera("camera");
    bRenderer().getObjects()->getCamera("camera")->lookAt(vmml::Vector3f(-30.0f,0.0f,0.0f), vmml::Vector3f::ZERO, vmml::Vector3f::UNIT_Y);
    
	// postprocessing
	bRenderer().getObjects()->createFramebuffer("fbo");					// create framebuffer object
	bRenderer().getObjects()->createTexture("fbo_texture1", 0.f, 0.f);	// create texture to bind to the fbo
	bRenderer().getObjects()->createTexture("fbo_texture2", 0.f, 0.f);	// create texture to bind to the fbo
	ShaderPtr blurShader = bRenderer().getObjects()->loadShaderFile_o("blurShader", 0);			// load shader that blurs the texture
	MaterialPtr blurMaterial = bRenderer().getObjects()->createMaterial("blurMaterial", blurShader);								// create an empty material to assign either texture1 or texture2 to
	bRenderer().getObjects()->createSprite("blurSprite", blurMaterial);																// create a sprite using the material created above

	// Update render queue
	updateRenderQueue("camera", 0.0f);
}




/* Draw your scene here */
void RingMadness::loopFunction(const double &deltaTime, const double &elapsedTime)
{
	//bRenderer::log("FPS: " + std::to_string(1 / deltaTime));	// write number of frames per second to the console every frame

	//// Draw Scene and do post processing ////

	/// Begin post processing ///
	GLint defaultFBO;
	if (!_running){
		bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth() / 5, bRenderer().getView()->getHeight() / 5);		// reduce viewport size
		defaultFBO = Framebuffer::getCurrentFramebuffer();	// get current fbo to bind it again after drawing the scene
		bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture("fbo_texture1"), false);	// bind the fbo
	}

	/// Draw scene ///	
	
	bRenderer().getModelRenderer()->drawQueue(/*GL_LINES*/);
	bRenderer().getModelRenderer()->clearQueue();
	
	if (!_running){
		/// End post processing ///		
        /*** Blur ***/
		// translate
		vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, -0.5));
		// blur vertically and horizontally
		bool b = true;		int numberOfBlurSteps = 2;
		for (int i = 0; i < numberOfBlurSteps; i++) {
			if (i == numberOfBlurSteps - 1){
				bRenderer().getObjects()->getFramebuffer("fbo")->unbind(defaultFBO); //unbind (original fbo will be bound)
				bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth(), bRenderer().getView()->getHeight());								// reset vieport size
			}
			else
				bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture(b ? "fbo_texture2" : "fbo_texture1"), false);
			bRenderer().getObjects()->getMaterial("blurMaterial")->setTexture("fbo_texture", bRenderer().getObjects()->getTexture(b ? "fbo_texture1" : "fbo_texture2"));
			bRenderer().getObjects()->getMaterial("blurMaterial")->setScalar("isVertical", static_cast<GLfloat>(b));
			// draw
			bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getModel("blurSprite"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
			b = !b;
		}
	
        /*** Title ***/
        // translate and scale 
        GLfloat titleScale = 0.5f;
        vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
		modelMatrix = vmml::create_translation(vmml::Vector3f(-0.4f, 0.0f, -0.65f)) * scaling;
        // draw
		bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getModel("bTitle"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false);

		/*** Instructions ***/
		titleScale = 0.1f;
		scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
		modelMatrix = vmml::create_translation(vmml::Vector3f(-1.1f / bRenderer().getView()->getAspectRatio(), -0.6f, -0.65f)) * scaling;
		// draw
		bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getTextSprite("instructions"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
    }

	//// Camera Movement ////
    updatePlane("camera", deltaTime);
    
    //// Update balls ////
    if(ball0.hit != true) checkBallCollision(ball0);
    if(ball1.hit != true) checkBallCollision(ball1);
    if(ball2.hit != true) checkBallCollision(ball2);
    if(ball3.hit != true) checkBallCollision(ball3);
    if(ball4.hit != true) checkBallCollision(ball4);
    if(ball5.hit != true) checkBallCollision(ball5);
    if(ball6.hit != true) checkBallCollision(ball6);
    if(ball7.hit != true) checkBallCollision(ball7);
    if(ball8.hit != true) checkBallCollision(ball8);
    if(ball9.hit != true) checkBallCollision(ball9);
    if(ball10.hit != true) checkBallCollision(ball10);
	
	/// Update render queue ///
	updateRenderQueue("camera", deltaTime);

	// Quit renderer when escape is pressed
	if (bRenderer().getInput()->getKeyState(bRenderer::KEY_ESCAPE) == bRenderer::INPUT_PRESS)
		bRenderer().terminateRenderer();
}

/* This function is executed when terminating the renderer */
void RingMadness::terminateFunction()
{
	bRenderer::log("I totally terminated this Renderer :-)");
}

float i = 0;
/* Update render queue */
void RingMadness::updateRenderQueue(const std::string &camera, const double &deltaTime)
{
    i += 1;
    
    // Skybox    
    vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f(cameraPosition.x(),cameraPosition.y()+10000.0f, cameraPosition.z())) * vmml::create_scaling(vmml::Vector3f(20000.0f));
    glEnable(GL_CULL_FACE);
    bRenderer().getModelRenderer()->queueModelInstance("cube", "cube_instance", camera, modelMatrix, std::vector<std::string>({}));
    glDisable(GL_CULL_FACE);    
    
    
    //Clouds
    modelMatrix = vmml::create_translation(vmml::Vector3f(cloudPosition)) * vmml::create_scaling(20.0f);
    bRenderer().getModelRenderer()->queueModelInstance("cloud1", "cloud1_instance", camera, modelMatrix, std::vector<std::string>({ "sunLight", "secondLight", "thirdLight" }), true, true);
    
    //moving sun
    //terrainShader->setUniform("sunPosition", vmml::Vector4f((sin(i*0.001)*1000.0),300.0f,0.0f,1.0f));
    //seaShader->setUniform("sunPosition", vmml::Vector4f((sin(i*0.001)*1000.0),300.0f,0.0f,1.0f));
    
    //Static Sun
    modelMatrix = vmml::create_translation(vmml::Vector3f(sunPosition)) * vmml::create_scaling(vmml::Vector3f(50.0f));
    bRenderer().getModelRenderer()->queueModelInstance("sun", "sun_instance", camera, modelMatrix, std::vector<std::string>({}), true, true);
    
    //Terrain
    modelMatrix = vmml::create_translation(vmml::Vector3f(0.f, -150.0f, 0.0f)) * vmml::create_scaling(vmml::Vector3f(30.0f));
    terrainShader->setUniform("modelMatrix", modelMatrix);
    bRenderer().getModelRenderer()->queueModelInstance("terrain", "terrain_instance", camera, modelMatrix, std::vector<std::string>({}), true, true);
    
    //sea //
    seaShader->setUniform("time", i);
    modelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, -145.0f,0.0f)) * vmml::create_scaling(vmml::Vector3f(30.0f));
    bRenderer().getModelRenderer()->queueModelInstance("sea", "sea_instance", camera, modelMatrix, std::vector<std::string>({}), true, true);
    
    //plane //
    ShaderPtr planeShader = bRenderer().getObjects()->getShader("plane");
    planeShader->setUniform("ModelMatrix", planeModelMatrixTwo);
    planeShader->setUniform("LightPos", sunPosition);

    bRenderer().getModelRenderer()->queueModelInstance("plane", "plane_instance", camera, planeModelMatrixTwo, std::vector<std::string>({}), true, true);

    // Rotor
    bRenderer().getModelRenderer()->queueModelInstance("rotor", "rotor_instance", camera, planeModelMatrixTwo * vmml::create_translation(vmml::Vector3f(0.0f,-0.4f,3.9f)) * vmml::create_rotation(i*0.1f, vmml::Vector3f::UNIT_Z), std::vector<std::string>({}), true, true);
    
    
    // Balls
    ShaderPtr ballShader = bRenderer().getObjects()->getShader("ball");
    bRenderer().getModelRenderer()->drawModel("sphere", "camera", ball0.matrix, std::vector<std::string>({ }));
    bRenderer().getModelRenderer()->drawModel("sphere", "camera", ball1.matrix, std::vector<std::string>({ }));
    bRenderer().getModelRenderer()->drawModel("sphere", "camera", ball2.matrix, std::vector<std::string>({ }));
    bRenderer().getModelRenderer()->drawModel("sphere", "camera", ball3.matrix, std::vector<std::string>({ }));
    bRenderer().getModelRenderer()->drawModel("sphere", "camera", ball4.matrix, std::vector<std::string>({ }));
    bRenderer().getModelRenderer()->drawModel("sphere", "camera", ball5.matrix, std::vector<std::string>({ }));
    bRenderer().getModelRenderer()->drawModel("sphere", "camera", ball6.matrix, std::vector<std::string>({ }));
    bRenderer().getModelRenderer()->drawModel("sphere", "camera", ball7.matrix, std::vector<std::string>({ }));
    bRenderer().getModelRenderer()->drawModel("sphere", "camera", ball8.matrix, std::vector<std::string>({ }));
    bRenderer().getModelRenderer()->drawModel("sphere", "camera", ball9.matrix, std::vector<std::string>({ }));
    bRenderer().getModelRenderer()->drawModel("sphere", "camera", ball10.matrix, std::vector<std::string>({ }));
   
    // Scores
    bRenderer::log("Score: " + std::to_string(score));
    //scoreStr << "Score: " << score;
    //showScore("camera");
}

void RingMadness::updatePlane(const std::string &camera, const double &deltaTime){
    
    float planeTargetPitch = 0;
    float planeTargetYaw = 0;
    
    //Double tap to stop game only on the left half of the screen.
    if (bRenderer().getInput()->doubleTapRecognized()){
        if(bRenderer().getInput()->getLastDoubleTapLocation().lastPositionX < bRenderer().getView()->getWidth() / 2 && bRenderer().getInput()->getLastDoubleTapLocation().lastPositionY < bRenderer().getView()->getHeight() / 2){
            _running = !_running;
        }if(bRenderer().getInput()->getLastDoubleTapLocation().lastPositionX < bRenderer().getView()->getWidth() / 2 && bRenderer().getInput()->getLastDoubleTapLocation().lastPositionY > bRenderer().getView()->getHeight() / 2){
            thirdPerson = !thirdPerson;
        }
    }
    
    if (_running){
        // control using touch
        TouchMap touchMap = bRenderer().getInput()->getTouches();
        int i = 0;
        for (auto t = touchMap.begin(); t != touchMap.end(); ++t)
        {
            Touch touch = t->second;
            // If touch is on the right half, steer plane
            if (touch.startPositionX > bRenderer().getView()->getWidth() / 2){
                
                planeTargetYaw = -(touch.currentPositionX - touch.startPositionX) * 0.0001f;
                //planeTargetYaw = -std::sin((touch.currentPositionX - touch.startPositionX) * 0.0001f) * (3.1415f/2.0f);
                
                planeTargetPitch = (touch.currentPositionY - touch.startPositionY) * 0.0001f;
                //planeTargetPitch = std::sin((touch.currentPositionY - touch.startPositionY) * 0.0001f)*(3.1415f/2.0f);
                
            }
            if (++i > 2)
                break;
            
        }
        
        //pitch of plane
        if(planeTargetPitch == 0){
            planeCurrentPitch = 0;
        }else{
            planeCurrentPitch = fmax(fmin(planeTargetPitch, 0.01f),-0.01);
        }
        
        //Roll and yaw of plane with automatical leveling of roll not working atm
        if(planeTargetYaw == 0){
            if (planeCurrentRoll > 0) {
                planeCurrentRoll = planeCurrentRoll - (planeCurrentRoll * 0.1f);
            } else if(planeCurrentRoll < 0){
                planeCurrentRoll  = planeCurrentRoll - (planeCurrentRoll * 0.1f);
            } else{
                planeCurrentRoll = 0;
            }
            planeCurrentYaw = 0;
        }else{
            planeCurrentYaw = planeTargetYaw;
            planeCurrentRoll = planeTargetYaw * 50.0f;
        }
        
        //PlaneRotation
        planeRotationMatrix = vmml::create_rotation(0.0f, vmml::Vector3f::UNIT_Z) * vmml::create_rotation(planeCurrentPitch, vmml::Vector3f::UNIT_X) * vmml::create_rotation(planeCurrentYaw, vmml::Vector3f::UNIT_Y);
        
        //Plane translation
        planeTransaltionMatrix = planeModelMatrix * vmml::create_translation(PLANESPEED);
        
        //Plane Movement form rotation and translation
        planeModelMatrix = planeTransaltionMatrix * planeRotationMatrix;
        
        //planePosition
        planePosition = vmml::Vector3f(planeModelMatrix[0][3],planeModelMatrix[1][3],planeModelMatrix[2][3]);
        
        
        //Setting targed wich the camera is chasing
        cameraModelMatrix = planeModelMatrix;
        cameraTargetPosition = vmml::Vector3f(cameraModelMatrix[0][3],cameraModelMatrix[1][3], cameraModelMatrix[2][3]);

        //calcualte offset/delay of teh camera accoring to the planes rotations.
//        X-Axis Delay
        if (cameraTargetPosition.x() > cameraPosition.x()) {
            cameraPosition.x() = cameraPosition.x() + (0.02f * (cameraTargetPosition.x() - cameraPosition.x()));
        } else if(cameraTargetPosition.x() < cameraPosition.x()){

            cameraPosition.x() = cameraPosition.x() - (0.02f * (cameraPosition.x() - cameraTargetPosition.x()));
        } else{
            cameraPosition.x() = cameraTargetPosition.x();
        }
        //Y-Axis Delay
        if (cameraTargetPosition.y() > cameraPosition.y()) {
            cameraPosition.y() = cameraPosition.y() + (0.02f * (cameraTargetPosition.y() - cameraPosition.y()));
        } else if(cameraTargetPosition.y() < cameraPosition.y()){
            cameraPosition.y() = cameraPosition.y() - (0.02f * (cameraPosition.y() - cameraTargetPosition.y()));
        } else{
            cameraPosition.y() = cameraTargetPosition.y();
        }
        //Z-Axis Delay
        if (cameraTargetPosition.z() > cameraPosition.z()) {
            cameraPosition.z() = cameraPosition.z() + (0.02f * (cameraTargetPosition.z() - cameraPosition.z()));
        } else if(cameraTargetPosition.z() < cameraPosition.z()){
            cameraPosition.z() = cameraPosition.z() - (0.02f * (cameraPosition.z() - cameraTargetPosition.z()));
        } else{
            cameraPosition.z() = cameraTargetPosition.z();
        }
        
        
        //Third Person view or view with eye at 0,0,0
        if (thirdPerson) {
            bRenderer().getObjects()->getCamera(camera)->lookAt(cameraPosition, planePosition, vmml::Vector3f::UNIT_Y);
        }else{
            bRenderer().getObjects()->getCamera(camera)->lookAt(vmml::Vector3f(0.0f,0.0f,0.0f), planePosition, vmml::Vector3f::UNIT_Y);
        }
        
        terrainShader->setUniform("camPosition", vmml::Vector4f(cameraPosition.x(),cameraPosition.y(),cameraPosition.z(),1.0));
        seaShader->setUniform("camPosition", vmml::Vector4f(cameraPosition.x(),cameraPosition.y(),cameraPosition.z(),1.0));
        objectShader->setUniform("camPosition", vmml::Vector4f(cameraPosition.x(),cameraPosition.y(),cameraPosition.z(),1.0));
        planeModelMatrixTwo = planeModelMatrix * vmml::create_rotation(-planeCurrentRoll, vmml::Vector3f::UNIT_Z);
    }
}

/* Ball collision detection */
void RingMadness::checkBallCollision(Ball &ball)
{
    float dx = planePosition.x()-ball.position.x();
    float dy = planePosition.y()-ball.position.y();
    float dz = planePosition.z()-ball.position.z();
    float dist = sqrt(dx*dx + dy*dy + dz*dz);
    
    if(dist <= (ball.radius + plane_radius)){
        score = score + 10;
        ball.hit = true;
        ball.matrix = ball.matrix * 0;
    }
}

/* Show score */
void RingMadness::showScore(const std::string &camera){
   vmml::Matrix4f textMatrix = vmml::create_scaling(vmml::Vector3f(0.1f, 0.1f, 0.1f)) * vmml::create_translation(vmml::Vector3f(4.f, -5.f, 0.0f));
    
    FontPtr font = bRenderer().getObjects()->loadFont("arial.ttf", 100);
    
    bRenderer().getObjects()->removeTextSprite("scoreText", true);
    bRenderer().getObjects()->createTextSprite("scoreText", vmml::Vector3f(1.f, 1.f, 1.f), scoreStr.str(), font);
    vmml::Matrix4f invViewMatrix = bRenderer().getObjects()->getCamera("camera")->getInverseViewMatrix();
    bRenderer().getModelRenderer()->drawText("scoreText", "camera", invViewMatrix*textMatrix, std::vector<std::string>({ }), false);
}

/* For iOS only: Handle device rotation */
void RingMadness::deviceRotated()
{
	if (bRenderer().isInitialized()){
		// set view to full screen after device rotation
		bRenderer().getView()->setFullscreen(true);
		bRenderer::log("Device rotated");
	}
}

/* For iOS only: Handle app going into background */
void RingMadness::appWillResignActive()
{
	if (bRenderer().isInitialized()){
		// stop the renderer when the app isn't active
		bRenderer().stopRenderer();
	}
}

/* For iOS only: Handle app coming back from background */
void RingMadness::appDidBecomeActive()
{
	if (bRenderer().isInitialized()){
		// run the renderer as soon as the app is active
		bRenderer().runRenderer();
	}
}

/* For iOS only: Handle app being terminated */
void RingMadness::appWillTerminate()
{
	if (bRenderer().isInitialized()){
		// terminate renderer before the app is closed
		bRenderer().terminateRenderer();
	}
}

