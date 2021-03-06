#include <Nephilim/Game/GameCore.h>
#include <Nephilim/Game/Engine.h>
#include <Nephilim/Graphics/GL/GLHelpers.h>
#include <Nephilim/Foundation/StringList.h>
#include <Nephilim/Foundation/FileSystem.h>
#include <Nephilim/Foundation/Logging.h>

// UI integration
#include <Nephilim/UI/UICanvas.h>

// Scripting of the GameCore
#include <Nephilim/Extensions/ExtensionScripting.h>
#include <Nephilim/Extensions/PluginSDK.h>
#include <Nephilim/Extensions/ExtensionAudio.h>

NEPHILIM_NS_BEGIN

/// Construct the game, its mandatory to call this base constructor when implementing GameCore
GameCore::GameCore()
: m_stackedTime(0.f)
, m_updateStep(1.f / 60.f)
, m_windowTitle("No name window")
, mCloseRequested(false)
, uxScreen(new UxScreen())
{
	stateManager.mGame = this;
}

/// Ensure every game resource is destroyed in order
GameCore::~GameCore()
{

}

/// Get the root of the screen UX hierarchy
UxScreen* GameCore::getScreenRoot()
{
	return uxScreen.get();
}

/// Called before the game initializes, to set some properties
/// For example, useful to allow or disallow automatic plugin loading
/// or boot script running or not. Useful to cancel out some default behaviors the game does
void GameCore::onPreSetup()
{

}

/// Indexes all plugins found in /Plugins
void GameCore::loadPlugins()
{
	typedef PluginSDK::Types(*getPluginTypeFunc)();
	typedef ExtensionScripting*(*createScriptEnvironmentFunc)(GameCore*);
	typedef ExtensionAudio*(*createAudioEnvironmentFunc)(GameCore*);

	StringList dll_list = FileSystem::scanDirectory("Plugins", "dll", false);
	for (auto& dll_name : dll_list)
	{
		// Let's check what the plugin is and load immediately
		PluginLoader* plugin = new PluginLoader(dll_name);
		if (plugin)
		{
			Log("Plugin loaded: %s", dll_name.c_str());

			getPluginTypeFunc funptr = (getPluginTypeFunc)plugin->getFunctionAddress("getPluginType");
			if (funptr)
			{
				PluginSDK::Types pluginType = funptr();

				switch (pluginType)
				{
				case PluginSDK::Scripting:
					{
						 Log("THIS IS A SCRIPTING PLUGIN");
						 createScriptEnvironmentFunc funptr = (createScriptEnvironmentFunc)plugin->getFunctionAddress("createScriptingEnvironment");
						 if (funptr)
						 {
							 ExtensionScripting* scriptingEnvironment = funptr(this);
							 if (scriptingEnvironment)
							 {
								 Log("Got the scripting environment, Registered.");
								 scriptingEnvironments.push_back(scriptingEnvironment);
							 }
						 }
					}
					break;
				case PluginSDK::Audio:
					{
						 Log("THIS IS A AUDIO PLUGIN");
						 createAudioEnvironmentFunc funptr = (createAudioEnvironmentFunc)plugin->getFunctionAddress("createAudioEnvironment");
						 if (funptr)
						 {
							 ExtensionAudio* audioEnvironment = funptr(this);
							 if (audioEnvironment)
							 {
								 Log("Got the audio environment, Registered.");
								 gameAudio.audioEnvironments.push_back(audioEnvironment);
							 }
						 }
					}
					break;


				}
			}
		}
	}
}

/// Create a new scene or return if already exists
World* GameCore::createWorld(const String& name)
{
	World* world = sceneManager.createScene(name);

	// Init this world
	world->graphicsDevice = getRenderer();
	world->contentManager = &contentManager;
	world->_game = this;

	return world;
}

/// Render a given world
void GameCore::renderWorld(World* world)
{
	if (world->_renderSystem)
	{
		world->_renderSystem->render();
	}
}

/// Finds a registered scripting environment or returns nullptr
ExtensionScripting* GameCore::getScriptingEnvironment(const String& name)
{
	for (auto s : scriptingEnvironments)
	{
		if (s->name == name)
		{
			return s;
		}
	}
	return nullptr;
}

/// Finds a registered audio environment to play sounds with or returns nullptr
ExtensionAudio* GameCore::getAudioEnvironment(const String& name)
{
	for (auto s : gameAudio.audioEnvironments)
	{
		if (s->name == name)
		{
			return s;
		}
	}
	return nullptr;
}

/// Broadcast a message to every script
void GameCore::broadcastMessage(const GameMessage &gameMessage)
{
	for (auto s : scriptingEnvironments)
	{
		s->dispatchToAll(gameMessage);
	}
}

/// Get the main world of this game
/// If there are no spawned worlds, one will be created, otherwise the world 0 is always the main one
World* GameCore::getWorld()
{
	if (sceneManager.mScenes.empty())
	{
		return createWorld("main");
	}
	else
	{
		return sceneManager.mScenes[0];
	}
}

/// Get the world by its name
World* GameCore::getWorld(const String& name)
{
	for (auto w : sceneManager.mScenes)
	{
		if (w->name == name)
			return w;
	}
	return nullptr;
}

/// Create a new UICanvas for the game to use
UICanvas* GameCore::createCanvas(const String& name)
{
	UICanvas* _canvas = nullptr;

	/*_canvas = userInterfaceManager.createCanvas(name);

	_canvas->getCore().m_defaultFont = &contentManager.font;
	_canvas->setRect(0.f, 0.f, getWindow()->width(), getWindow()->height());

	uxScreen->_children.push_back(_canvas);
	*/
	return _canvas;
}

/// Get a UI canvas 
UICanvas* GameCore::getCanvas(const String& name)
{
	/*for (auto c : userInterfaceManager.canvasList)
	{
		if (c->m_name == name)
		{
			return c;
		}
	}
	*/
	return nullptr;
}

/// Get a pointer to the engine
Engine* GameCore::getEngine()
{
	return m_creator;
}

/// Get the window/screen handler
Window* GameCore::getWindow()
{
    return m_creator->m_window;
}

/// Returns whether this game is running as a server-only (no graphics)
bool GameCore::isDedicatedServer()
{
	return gameNetwork.isDedicatedServer;
}

/// Sets the base directory to load resources from
void GameCore::setFileRoot(const String & path)
{
	m_fileSystemRoot = path;
}

/// Get the root for file loading on this game
String GameCore::getFileSystemRoot()
{
	return m_fileSystemRoot;
}

GraphicsDevice* GameCore::getRenderer()
{
	return m_creator->getGraphicsDevice();
}

/// Get the opengl version of this surface's context
String GameCore::getGLVersionString()
{
	const GLubyte* str = glGetString(GL_VERSION);
	return reinterpret_cast<const char*>(str);
}

/// Get the opengl renderer device name
String GameCore::getGLRendererString()
{
	const GLubyte* str = glGetString(GL_RENDERER);
	return reinterpret_cast<const char*>(str);
}

/// Get the opengl vendor string
String GameCore::getGLVendorString()
{
	const GLubyte* str = glGetString(GL_VENDOR);
	return reinterpret_cast<const char*>(str);
}

/// Get the flags of the gl context
String GameCore::getGLContextFlags()
{
	//const GLubyte* str = glGetString(GL_VENDOR);
	//return reinterpret_cast<const char*>(str);
	return "";
}

/// Shutdown this game immediately
void GameCore::close()
{
	mCloseRequested = true;
}

/// Set the name of the game
void GameCore::setName(const String& name)
{
	m_gameName = name;
};

/// Get the name of the game
String GameCore::getName()
{
	return m_gameName;
};

/// Set the fixed update step, or, the amount of time, in seconds, that will take between each update.
void GameCore::setUpdateStep(float step){
	m_updateStep = step;
};

/// Callback for updating the game
void GameCore::onUpdate(Time time){}

/// Callback for rendering a frame
void GameCore::onRender(){}

/// Callback when starting to execute the game
void GameCore::onCreate(){}

/// Callback when an event happens
void GameCore::onEvent(const Event &event){}

/// Get the game window title
String GameCore::getWindowTitle()
{
	return m_windowTitle;
}

/// Set the title of the window while the game is active
void GameCore::setWindowTitle(const String &title){
	m_windowTitle = title;
};

/// Inner update of the game
/// Will handle fixed update steps
/// Callbacks to onUpdate(Time time) when appropriate
void GameCore::innerUpdate(Time time)
{
	// Frame skipping when too much time is accumulated
	while(m_stackedTime > 0.5f)
	{
		m_stackedTime -= 0.5f;
	}

	m_stackedTime += time.seconds();
	while(m_stackedTime >= m_updateStep)
	{
		PrimaryUpdate(Time::fromSeconds(m_updateStep));
		m_stackedTime -= m_updateStep;
	}
}



////////////////////////////////////////////////////////////////////////// INTERNAL

/// This will initialize the game effectively and then call onCreate()
void GameCore::PrimaryCreate()
{
	// Prepare our screen contents
	uxScreen->window = getWindow();
	uxScreen->GDI    = getRenderer();
	uxScreen->width  = getWindow()->width();
	uxScreen->height = getWindow()->height();


	// Plugins are ready when the game starts to construct
	loadPlugins();

	onCreate();
}

/// Internal update handling
void GameCore::PrimaryUpdate(Time time)
{
	gameInput.update(time);

	stateManager.update(time);

	onUpdate(time);

	for (auto w : sceneManager.mScenes)
	{
		if (w->mEnabled)
		{
			w->update(time);
		}
	}

	/*for (auto ux : userInterfaceManager.canvasList)
	{
		ux->update(time.seconds());
	}*/

	// Now let's keep our scripts fresh
	for (auto s : scriptingEnvironments)
	{
		s->update(time);
	}
}

/// This will handle the OS event and deliver it down the game structures
void GameCore::PrimaryEventHandling(const Event& event)
{
	if (event.type == Event::Resized)
	{
		uxScreen->width = event.size.width;
		uxScreen->height = event.size.height;
	}

	// Deliver the input to canvases
	/*for (auto c : userInterfaceManager.canvasList)
	{
		c->pushEvent(event);
	}*/

	gameInput.updateWithEvent(event);

	stateManager.pushEvent(event);

	onEvent(event);
}

/// Inner render of the game
/// Callbacks to onRender()
void GameCore::PrimaryRender()
{

	//uxScreen->render();

	stateManager.drawStates(getRenderer());

	// Give the user the opportunity to render something
	onRender();

}

NEPHILIM_NS_END
