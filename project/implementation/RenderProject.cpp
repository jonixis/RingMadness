#include "RenderProject.h"

/* Initialize the Project */
void RenderProject::init()
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

//plane Variables
vmml::Matrix4f planeModelMatrix;
vmml::Matrix4f planeModelMatrixTwo;
vmml::Matrix4f planeRotationMatrix;
vmml::Matrix4f planeTransaltionMatrix;
vmml::Vector3f planePosition;
vmml::Vector3f PLANESPEED = vmml::Vector3f(0.0f,0.0f,1.0f);
float planeCurrentPitch = 0;
float planeCurrentYaw = 0;
float planeMaxMinPitch = 0;
float planeCurrentRoll = 0;
float planeLastRoll = 0;


//camera Variables
vmml::Matrix4f cameraModelMatrix;
vmml::Vector3f cameraPosition;

/* This function is executed when initializing the renderer */
void RenderProject::initFunction()
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

	// set shader versions (optional)
	bRenderer().getObjects()->setShaderVersionDesktop("#version 120");
	bRenderer().getObjects()->setShaderVersionES("#version 100");
    
	// load materials and shaders before loading the model
	ShaderPtr customShader = bRenderer().getObjects()->generateShader("customShader", { 2, true, true, true, true, true, true, true, true, true, false, false, false });	// automatically generates a shader with a maximum of 2 lights
	
    // load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
    //ShaderPtr flameShader = bRenderer().getObjects()->loadShaderFile("flame", 0, false, true, true, false, false);
    
    // load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
	ShaderPtr flameShader = bRenderer().getObjects()->loadShaderFile_o("flame", 0, AMBIENT_LIGHTING);
    
    ShaderPtr terrainShader = bRenderer().getObjects()->generateShader("terrainShader", { 4, true, true, true, true, true, true, true, true, true, false, false, false });
    
    // load material from file using the shader created above
	MaterialPtr flameMaterial = bRenderer().getObjects()->loadObjMaterial("flame.mtl", "flame", flameShader);
    
    
    
    
    //Load cube.frag & cube.vert for the skyCube
    ShaderPtr skyShader = bRenderer().getObjects()->loadShaderFile_o("cube");
    
    // Order of pictures to generate cubematp. left, right, bottom, top, front,  back
    skyShader->setUniform("texCube", bRenderer().getObjects()->loadCubeMap("textureCube", std::vector<std::string>({ "TropicalSunnyDay_lf.png", "TropicalSunnyDay_rt.png", "TropicalSunnyDay_dn.png", "TropicalSunnyDay_up.png", "TropicalSunnyDay_ft.png", "TropicalSunnyDay_bk.png" })));
   
    //Load Skycube object cube.obj. Name of shader used.
    bRenderer().getObjects()->loadObjModel_o("cube.obj", skyShader);
    bRenderer().getObjects()->loadObjModel("plane.obj");
    bRenderer().getObjects()->loadObjModel_o("terrain1.obj",flameShader);
    bRenderer().getObjects()->loadObjModel_o("untitled.obj", flameShader);
    
    planeModelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, 0.0f)) * vmml::create_rotation(M_PI_F * 0.5f, vmml::Vector3f::UNIT_Y);
    
    

    
	// create additional properties for a model
	PropertiesPtr flameProperties = bRenderer().getObjects()->createProperties("flameProperties");
	PropertiesPtr streamProperties = bRenderer().getObjects()->createProperties("streamProperties");
    
    // automatically generates a shader with a maximum of 4 lights (number of lights may vary between 0 and 4 during rendering without performance loss)
	//bRenderer().getObjects()->loadObjModel("cave.obj", true, true, false, 4, true, false);
    
    // automatically generates a shader with a maximum of 4 lights (number of lights may vary between 0 and 4 during rendering without performance loss)
	bRenderer().getObjects()->loadObjModel_o("cave.obj", 4, FLIP_T | FLIP_Z | VARIABLE_NUMBER_OF_LIGHTS);
    
    // automatically loads shader files according to the name of the material
	//bRenderer().getObjects()->loadObjModel("cave_stream.obj", true, true, true, 4, false, false, streamProperties);
    
    // automatically loads shader files according to the name of the material
	bRenderer().getObjects()->loadObjModel_o("cave_stream.obj", 4, FLIP_T | FLIP_Z | SHADER_FROM_FILE, streamProperties);
    
    // the custom shader created above is used
	//bRenderer().getObjects()->loadObjModel("crystal.obj", false, true, customShader);
    
    // the custom shader created above is used
	bRenderer().getObjects()->loadObjModel_o("crystal.obj", customShader, FLIP_Z);
    
    // create custom shader with a maximum of 1 light
	//bRenderer().getObjects()->loadObjModel("torch.obj", false, true, false, 1, false, true);
    
    // create custom shader with a maximum of 1 light
	bRenderer().getObjects()->loadObjModel_o("torch.obj", 1, FLIP_Z | AMBIENT_LIGHTING);

    
	// create sprites
    // create a sprite using the material created above, to pass additional properties a Properties object is used
	bRenderer().getObjects()->createSprite_o("flame", flameMaterial, NO_OPTION, flameProperties);
    
    // create a sprite displaying sparks as a texture
	bRenderer().getObjects()->createSprite("sparks", "sparks.png");
    
    // create a sprite displaying the title as a texture
	bRenderer().getObjects()->createSprite("bTitle", "basicTitle_light.png");

	// create text sprites
	FontPtr font = bRenderer().getObjects()->loadFont("KozGoPro-ExtraLight.otf", 50);
	if (Input::isTouchDevice())
		bRenderer().getObjects()->createTextSprite("instructions", vmml::Vector3f(1.f, 1.0f, 1.f), "Double tap on the left half of the screen to start", font);
	else
		bRenderer().getObjects()->createTextSprite("instructions", vmml::Vector3f(1.f, 1.f, 1.f), "Press space to start", font);

    
    
	// create camera
	//bRenderer().getObjects()->createCamera("camera", vmml::Vector3f(-33.0, 0.f, -13.0), vmml::Vector3f(0.f, -M_PI_F / 2, 0.f));
    bRenderer().getObjects()->createCamera("camera");
    bRenderer().getObjects()->getCamera("camera")->lookAt(vmml::Vector3f(-30.0f,0.0f,0.0f), vmml::Vector3f::ZERO, vmml::Vector3f::UNIT_Y);
    
	// create lights
	bRenderer().getObjects()->createLight("sunLight", vmml::Vector3f(0.0f, 150.0f, 0.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 0.5f, 0.01f, 1000.0f);
	bRenderer().getObjects()->createLight("secondLight", vmml::Vector3f(148.0f, -3.0f, 15.0f), vmml::Vector3f(0.3f, 1.0f, 0.3f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.8f, 100.0f);
	bRenderer().getObjects()->createLight("thirdLight", vmml::Vector3f(218.0f, -3.0f, 0.0f), vmml::Vector3f(0.8f, 0.2f, 0.2f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.8f, 100.0f);

    
	// postprocessing
	bRenderer().getObjects()->createFramebuffer("fbo");					// create framebuffer object
	bRenderer().getObjects()->createTexture("fbo_texture1", 0.f, 0.f);	// create texture to bind to the fbo
	bRenderer().getObjects()->createTexture("fbo_texture2", 0.f, 0.f);	// create texture to bind to the fbo
	//ShaderPtr blurShader = bRenderer().getObjects()->loadShaderFile("blurShader", 0, false, false, false, false, false);			// load shader that blurs the texture
	ShaderPtr blurShader = bRenderer().getObjects()->loadShaderFile_o("blurShader", 0);			// load shader that blurs the texture
	MaterialPtr blurMaterial = bRenderer().getObjects()->createMaterial("blurMaterial", blurShader);								// create an empty material to assign either texture1 or texture2 to
	bRenderer().getObjects()->createSprite("blurSprite", blurMaterial);																// create a sprite using the material created above

    
	// Update render queue
	updateRenderQueue("camera", 0.0f);
}




/* Draw your scene here */
void RenderProject::loopFunction(const double &deltaTime, const double &elapsedTime)
{
//	bRenderer::log("FPS: " + std::to_string(1 / deltaTime));	// write number of frames per second to the console every frame

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
		modelMatrix = vmml::create_translation(vmml::Vector3f(-1.0f / bRenderer().getView()->getAspectRatio(), -0.6f, -0.65f)) * scaling;
		// draw
		bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getTextSprite("instructions"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
    }

	//// Camera Movement ////
    updatePlane("camera", deltaTime);
	
	//// Torch Light ////
	if (_running){
		if (deltaTime > 0.0f){
			_offset += 5 * deltaTime;
			_randomOffset += (randomNumber(10.0f, 20.0f)) * deltaTime;
		}
		// let the light follow the camera
		GLfloat flickeringLightPosX = -bRenderer().getObjects()->getCamera("camera")->getPosition().x();
		GLfloat flickeringLightPosY = -bRenderer().getObjects()->getCamera("camera")->getPosition().y();
		GLfloat flickeringLightPosZ = -bRenderer().getObjects()->getCamera("camera")->getPosition().z();
		// let the light flicker
		flickeringLightPosX += 2*sin(flickeringLightPosY + 0.5f*_randomOffset);
		flickeringLightPosY += 2*sin(flickeringLightPosX + 0.5f*_randomOffset);
		//bRenderer().getObjects()->getLight("torchLight")->setPosition(vmml::Vector3f(flickeringLightPosX, flickeringLightPosY, flickeringLightPosZ) - bRenderer().getObjects()->getCamera("camera")->getForward()*10.0f);
	}
	else{
		// set the light to be at the camera position
		//bRenderer().getObjects()->getLight("torchLight")->setPosition(-bRenderer().getObjects()->getCamera("camera")->getPosition() - bRenderer().getObjects()->getCamera("camera")->getForward()*10.0f);
	}

	/// Update render queue ///
	updateRenderQueue("camera", deltaTime);

	// Quit renderer when escape is pressed
	if (bRenderer().getInput()->getKeyState(bRenderer::KEY_ESCAPE) == bRenderer::INPUT_PRESS)
		bRenderer().terminateRenderer();
}

/* This function is executed when terminating the renderer */
void RenderProject::terminateFunction()
{
	bRenderer::log("I totally terminated this Renderer :-)");
}

/* Update render queue */
void RenderProject::updateRenderQueue(const std::string &camera, const double &deltaTime)
{
    vmml::Matrix4f modelMatrix = vmml::create_translation(-1.0f*(bRenderer().getObjects()->getCamera(camera)->getPosition())) * vmml::create_scaling(vmml::Vector3f(20000.0f));
    
    //cube

    glClear(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    bRenderer().getModelRenderer()->queueModelInstance("cube", "cube_instance", camera, modelMatrix, std::vector<std::string>({"sunLight", "secondLight", "thirdLight" }));
    
    glDisable(GL_CULL_FACE);
    
    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
    
    
	/*** Cave ***/
	// translate and scale 
	modelMatrix = vmml::create_translation(vmml::Vector3f(30.f, -24.0, 0.0)) * vmml::create_scaling(vmml::Vector3f(0.3f));
	// submit to render queue
	//bRenderer().getModelRenderer()->queueModelInstance("cave", "cave_instance", camera, modelMatrix, std::vector<std::string>({ "torchLight", "sunLight", "secondLight", "thirdLight" }), true, true);
    
    //Terrain //
    modelMatrix = vmml::create_translation(vmml::Vector3f(0.f, -150.0f, 0.0f)) * vmml::create_scaling(vmml::Vector3f(30.0f));
    bRenderer().getModelRenderer()->queueModelInstance("terrain1", "terrain1_instance", camera, modelMatrix, std::vector<std::string>({ "sunLight", "secondLight", "thirdLight" }), true, true);
    
    modelMatrix = vmml::create_translation(vmml::Vector3f(0.f, -145.0f, 0.0f)) * vmml::create_scaling(vmml::Vector3f(1.0f));
    bRenderer().getModelRenderer()->queueModelInstance("untitled", "untitled_instance", camera, modelMatrix, std::vector<std::string>({ "sunLight", "secondLight", "thirdLight" }), true, true);
    
    modelMatrix = vmml::create_translation(vmml::Vector3f(30.f, -24.0, 0.0)) * vmml::create_scaling(vmml::Vector3f(0.3f));
	
	/*** Cave stream ***/
	bRenderer().getObjects()->getProperties("streamProperties")->setScalar("offset", _offset);		// pass offset for wave effect
	// submit to render queue
	//bRenderer().getModelRenderer()->queueModelInstance("cave_stream", "cave_stream_instance", camera, modelMatrix, std::vector<std::string>({ "sunLight", "secondLight", "thirdLight" }), true, false, true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 1.0f);

	/*** Crystal (blue) ***/
	// translate and scale
	modelMatrix = vmml::create_translation(vmml::Vector3f(78.0f, -17.0f, 5.5f)) * vmml::create_scaling(vmml::Vector3f(0.1f));
	// submit to render queue
	bRenderer().getObjects()->setAmbientColor(vmml::Vector3f(0.2f, 0.2f, 1.0f));
	//bRenderer().getModelRenderer()->queueModelInstance("crystal", "crystal_blue", camera, modelMatrix, std::vector<std::string>({ "sunLight" }), true, false, true);

	/*** Crystal (green) ***/
	// translate and scale 
	modelMatrix = vmml::create_translation(vmml::Vector3f(148.0f, -17.0f, 15.0f)) * vmml::create_scaling(vmml::Vector3f(0.1f));
	// submit to render queue
	bRenderer().getObjects()->setAmbientColor(vmml::Vector3f(0.2f, 0.7f, 0.2f));
	//bRenderer().getModelRenderer()->queueModelInstance("crystal", "crystal_green", camera, modelMatrix, std::vector<std::string>({ "secondLight" }), true, false, true);

	/*** Crystal (red) ***/
	// translate and scale 
	modelMatrix = vmml::create_translation(vmml::Vector3f(218.0f, -17.0f, 4.0f)) * vmml::create_scaling(vmml::Vector3f(0.1f));
	// submit to render queue
	bRenderer().getObjects()->setAmbientColor(vmml::Vector3f(0.8f, 0.2f, 0.2f));
	//bRenderer().getModelRenderer()->queueModelInstance("crystal", "crystal_red", camera, modelMatrix, std::vector<std::string>({ "thirdLight" }), true, false, true);
	bRenderer().getObjects()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());

	///*** Torch ***/
	// Position the torch relative to the camera
	modelMatrix = bRenderer().getObjects()->getCamera(camera)->getInverseViewMatrix();		// position and orient to match camera
	modelMatrix *= vmml::create_translation(vmml::Vector3f(0.75f, -1.1f, 0.8f)) * vmml::create_scaling(vmml::Vector3f(1.2f)) * vmml::create_rotation(1.64f, vmml::Vector3f::UNIT_Y); // now position it relative to the camera
	// submit to render queue
	//bRenderer().getModelRenderer()->queueModelInstance("torch", "torch_instance", camera, modelMatrix, std::vector<std::string>({}));

	/*** Flame ***/
	// pass additional properties to the shader
	bRenderer().getObjects()->getProperties("flameProperties")->setScalar("offset", _randomOffset);		// pass offset for wave effect
	// create three flames
	for (GLfloat z = 0.0f; z < 3.0f; z++)
	{
		// translate
		vmml::Matrix4f translation = vmml::create_translation(vmml::Vector3f(0.65f / bRenderer().getView()->getAspectRatio(), 0.6f + (0.08f*z), (-z / 100.0f - 0.50f)));
		// rotate
		GLfloat rot = 0.0f;
		if (fmod(z, 2.0f) != 0.0f)
			rot = M_PI_F;
			
		vmml::Matrix4f rotation = vmml::create_rotation(rot, vmml::Vector3f::UNIT_Z);
		// scale
		GLfloat ParticleScale = 1.225f - (0.23f*z);
		vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(ParticleScale / bRenderer().getView()->getAspectRatio(), ParticleScale, ParticleScale));
		// model matrix
		modelMatrix = translation * scaling * rotation;
		// submit to render queue
		//bRenderer().getModelRenderer()->queueModelInstance(bRenderer().getObjects()->getModel("flame"), ("flame_instance" + std::to_string(z)), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false, true, GL_SRC_ALPHA, GL_ONE, (-1.0f - 0.01f*z));  // negative distance because always in foreground
	}

	/*** Sparks ***/
	for (GLfloat z = 1.0f; z < 2.0f; z++)
	{
		// translate
		vmml::Matrix4f translation = vmml::create_translation(vmml::Vector3f(0.65f / bRenderer().getView()->getAspectRatio(), 0.65f, (-z / 100.0f - 0.58f)));
		// rotate
		GLfloat rot = 1.0f;
		if (_running)
			rot = randomNumber(1.0f, 1.1f)*_randomOffset*z;
		vmml::Matrix4f rotation = vmml::create_rotation(rot, vmml::Vector3f::UNIT_Z);
		// scale
		GLfloat ParticleScale = 0.55f - (0.25f*z);
		vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(ParticleScale / bRenderer().getView()->getAspectRatio(), 4.0f*ParticleScale, ParticleScale));
		// model matrix
		modelMatrix = translation * scaling * rotation;

		// submit to render queue
		//bRenderer().getModelRenderer()->queueModelInstance(bRenderer().getObjects()->getModel("sparks"), ("sparks_instance" + std::to_string(z)), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false, true, GL_SRC_ALPHA, GL_ONE, (-2.0f - 0.01f*z)); // negative distance because always in foreground
	}
    //plane //
    bRenderer().getModelRenderer()->queueModelInstance("plane", "plane_instance", camera, planeModelMatrixTwo, std::vector<std::string>({"sunLight", "secondLight", "thirdLight" }), true, true);

}



void RenderProject::updatePlane(const std::string &camera, const double &deltaTime){
    
    float planeTargetPitch = 0;
    float planeTargetYaw = 0;
    
    //Double tap to stop game only on the left half of the screen.
    if (bRenderer().getInput()->doubleTapRecognized()){
        if(bRenderer().getInput()->getLastDoubleTapLocation().lastPositionX < bRenderer().getView()->getWidth() / 2){
            _running = !_running;
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
                planeTargetPitch = (touch.currentPositionY - touch.startPositionY) * 0.0001f;
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
        
        //Roll and yaw of plane with automatical leveling of roll
        if(planeTargetYaw == 0){
            if (planeCurrentRoll > 0) {
                planeCurrentRoll = planeCurrentRoll;// - (planeCurrentRoll * 0.1f);
            } else if(planeCurrentRoll < 0){
                planeCurrentRoll  = planeCurrentRoll;// - (planeCurrentRoll * 0.1f);
            } else{
                planeCurrentRoll = 0;
            }
            planeCurrentYaw = planeCurrentYaw;
        }else{
            planeCurrentYaw = planeTargetYaw;
            planeCurrentRoll = fmax(fmin(planeTargetYaw * 200.0, 1.4),-1.4);
        }
        
        //PlaneRotation
        planeRotationMatrix = vmml::create_rotation(0.0f, vmml::Vector3f::UNIT_Z) * vmml::create_rotation(planeCurrentPitch, vmml::Vector3f::UNIT_X) * vmml::create_rotation(planeCurrentYaw, vmml::Vector3f::UNIT_Y);
        
        //Plane translation
        planeTransaltionMatrix = planeModelMatrix * vmml::create_translation(PLANESPEED);
        
        //Plane Movement form rotation and translation
        planeModelMatrix = planeTransaltionMatrix * planeRotationMatrix;
        
        //planePosition
        planePosition = vmml::Vector3f(planeModelMatrix[0][3],planeModelMatrix[1][3],planeModelMatrix[2][3]);
        
        //calcualte camera Position from planeModelMatrix translated 30.f behind the plane.
        
        cameraModelMatrix = planeModelMatrix * vmml::create_translation(vmml::Vector3f(0.0f,0.0f,-30.0f));
        cameraPosition = vmml::Vector3f(cameraModelMatrix[0][3],cameraModelMatrix[1][3], cameraModelMatrix[2][3]);
        
        //Camera view direction and
        bRenderer().getObjects()->getCamera(camera)->lookAt(cameraPosition, planePosition, vmml::Vector3f::UNIT_Y);
        
        planeModelMatrixTwo = planeModelMatrix * vmml::create_rotation(-planeCurrentRoll, vmml::Vector3f::UNIT_Z);
        
        
    }
}

/* For iOS only: Handle device rotation */
void RenderProject::deviceRotated()
{
	if (bRenderer().isInitialized()){
		// set view to full screen after device rotation
		bRenderer().getView()->setFullscreen(true);
		bRenderer::log("Device rotated");
	}
}

/* For iOS only: Handle app going into background */
void RenderProject::appWillResignActive()
{
	if (bRenderer().isInitialized()){
		// stop the renderer when the app isn't active
		bRenderer().stopRenderer();
	}
}

/* For iOS only: Handle app coming back from background */
void RenderProject::appDidBecomeActive()
{
	if (bRenderer().isInitialized()){
		// run the renderer as soon as the app is active
		bRenderer().runRenderer();
	}
}

/* For iOS only: Handle app being terminated */
void RenderProject::appWillTerminate()
{
	if (bRenderer().isInitialized()){
		// terminate renderer before the app is closed
		bRenderer().terminateRenderer();
	}
}

/* Helper functions */
GLfloat RenderProject::randomNumber(GLfloat min, GLfloat max){
	return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}
