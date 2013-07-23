#include <Nephilim/UIDocument.h>
#include <Nephilim/UISurface.h>
#include <Nephilim/Text.h>
#include <Nephilim/UILabel.h>

#include <algorithm>

NEPHILIM_NS_BEGIN

UIDocument::UIDocument()
: m_surfaceContainerLock(0)
, m_backgroundColor(Color::Transparent)
{
	
}

void UIDocument::onDraw(Renderer* renderer)
{
	//m_backgroundColor = Color::White;
	renderer->drawDebugQuad(m_bounds.left + m_bounds.width/2, m_bounds.top + m_bounds.height/2, 0, m_bounds.width, m_bounds.height, m_backgroundColor);
	renderer->drawDebugLine(Vec2f(m_bounds.left, m_bounds.top), Vec2f(m_bounds.left + m_bounds.width, m_bounds.top), m_topBorderColor);
	renderer->drawDebugLine(Vec2f(m_bounds.left, m_bounds.top + m_bounds.height), Vec2f(m_bounds.left + m_bounds.width, m_bounds.top + m_bounds.height), m_bottomBorderColor);
	renderer->drawDebugLine(Vec2f(m_bounds.left, m_bounds.top), Vec2f(m_bounds.left, m_bounds.top + m_bounds.height), m_leftBorderColor);
	renderer->drawDebugLine(Vec2f(m_bounds.left + m_bounds.width, m_bounds.top), Vec2f(m_bounds.left + m_bounds.width, m_bounds.top + m_bounds.height), m_rightBorderColor);

	/// Draw surfaces bottom to top
	m_surfaceContainerLock++;
	for(std::vector<UISurface*>::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++)
	{
		(*it)->draw(renderer);
	}
	m_surfaceContainerLock--;
}

UISurface* UIDocument::getSurfaceByName(const String& name)
{
	UISurface* found = NULL;
	for(std::vector<UISurface*>::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++)
	{
		if((*it)->getName() == name)
		{
			found = (*it);
		}
	}

	if(!found)
	{
		for(unsigned int i = 0; i < m_pendingChanges.size(); i++)
		{
			if(m_pendingChanges[i].type == Add && m_pendingChanges[i].surface->getName() == name)
			{
				found = m_pendingChanges[i].surface;
			}
		}
	}

	return found;
}

UICore& UIDocument::getContext(){
	return m_state;
}

/// Makes a list of controls from a selector - CSS like
UIDocument::ControlList UIDocument::selectControls(const String& selector)
{
	ControlList list;

	return list;
}

/// Sets new boundaries to this UIWindow
void UIDocument::setRect(Rect<float> rect){
	m_bounds = rect;

	// resize happened
	for(std::vector<UISurface*>::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++){
		(*it)->setPosition(m_bounds.left, m_bounds.top);
		(*it)->setSize(m_bounds.width, m_bounds.height);
	}

	// Finnaly, since the UIWindow rectangle changed, its time to adjust all the children that require it
};

/// Set the new boundaries for this document
void UIDocument::setRect(float left, float top, float width, float height)
{
	setRect(FloatRect(left,top,width, height));
}

/// Get the position of the exact middle of this UIWindow
Vec2f UIDocument::getMiddlePosition(){
	return Vec2f(m_bounds.left + m_bounds.width/2, m_bounds.top + m_bounds.height/2);
};

void UIDocument::clearUnusedSurfaces()
{

}

//debug
void UIDocument::debugData()
{
	Log("Surface count: %d", m_surfaces.size());
	for(std::vector<UISurface*>::const_iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++){
		Log("Surface: %s", (*it)->getName().c_str());
	}
}

/// Get the current surface count
int UIDocument::getSurfaceCount()
{
	return static_cast<int>(m_surfaces.size());
}

/// Get a surface by its name. It will be created if it does not yet exist
UISurface* UIDocument::operator[](const String& name)
{
	UISurface* surface = getSurfaceByName(name);
	if(!surface)
	{
		surface = addSurface(name);
	}

	return surface;
}

/// Get a surface directly by its index
/// You must ensure that index is valid
UISurface* UIDocument::operator[](unsigned int index)
{
	return m_surfaces[index];
}

/// Get the surface closer to the user
UISurface* UIDocument::getTopSurface(){
	if(m_surfaces.empty()) return NULL;
	else{
		return m_surfaces[0];
	}
};

void UIDocument::showMessageBox(const String& message)
{
	UIControl* modalBackground = new UIControl();
	
	modalBackground->setProperty<Color>("background-color", Color(0,0,0, 80));
	
	UISurface* surface = addSurface("modal");
	surface->attach(modalBackground);
	modalBackground->setRect(surface->getRect());

	UILabel* label = new UILabel(message);
	label->setSize(700,50);
	label->setCenter(modalBackground->getMiddlePosition());
	label->setProperty<Color>("background-color", Color(0,0,0));
	label->setProperty<Color>("color", Color::White);
	modalBackground->attach(label);
};


/// Returns a control in the hierarchy with the name, or NULL if not found
UIControl* UIDocument::getControlByName(const String& name){
	UIControl* control = NULL;
	for(std::vector<UISurface*>::const_iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++){
		control = (*it)->findByName(name);
		if(control != NULL) return control; // the surface returned something
	}

	return NULL; // Nothing found.
};

UISurface* UIDocument::addSurface(const String& name)
{
	UISurface* surface = new UISurface();
	surface->setName(name);
	surface->setPosition(m_bounds.left, m_bounds.top);
	surface->setSize(m_bounds.width, m_bounds.height);
	surface->setContext(&m_state);
	surface->m_parentDocument = this;

	if(!m_surfaceContainerLock)
		m_surfaces.push_back(surface);
	else
	{
		// Schedule add
		PendingChange change;
		change.type = Add;
		change.surface = surface;
		m_pendingChanges.push_back(change);
	}

	return surface;
};


/// Draw the UI
void UIDocument::draw(Renderer* renderer){
	renderer->drawDebugQuad(m_bounds.left + m_bounds.width/2, m_bounds.top + m_bounds.height/2, 0, m_bounds.width, m_bounds.height, m_backgroundColor);
	renderer->drawDebugLine(Vec2f(m_bounds.left, m_bounds.top), Vec2f(m_bounds.left + m_bounds.width, m_bounds.top), m_topBorderColor);
	renderer->drawDebugLine(Vec2f(m_bounds.left, m_bounds.top + m_bounds.height), Vec2f(m_bounds.left + m_bounds.width, m_bounds.top + m_bounds.height), m_bottomBorderColor);
	renderer->drawDebugLine(Vec2f(m_bounds.left, m_bounds.top), Vec2f(m_bounds.left, m_bounds.top + m_bounds.height), m_leftBorderColor);
	renderer->drawDebugLine(Vec2f(m_bounds.left + m_bounds.width, m_bounds.top), Vec2f(m_bounds.left + m_bounds.width, m_bounds.top + m_bounds.height), m_rightBorderColor);

	/// Draw surfaces bottom to top
	for(std::vector<UISurface*>::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++){
		(*it)->draw(renderer);
	}
};

/// Destroys a surface from its children
void UIDocument::destroySurface(UISurface* surface)
{
	if(m_surfaceContainerLock > 0)
	{
		PendingChange op;
		op.surface = surface;
		op.type = Remove;
		m_pendingChanges.push_back(op);
	}
	else
	{
		m_surfaces.erase(std::find(m_surfaces.begin(), m_surfaces.end(), surface));
	}
}

/// Update the state of the ui
void UIDocument::update(float elapsedTime)
{	
	m_surfaceContainerLock++;
	for(std::vector<UISurface*>::reverse_iterator it = m_surfaces.rbegin(); it != m_surfaces.rend(); it++){
		(*it)->update(elapsedTime);
	}
	m_surfaceContainerLock--;
}

UIEventResult UIDocument::pushEvent(const Event& event)
{
	UIEventResult eventUsage;

	// -- Raw event delivery system
	m_surfaceContainerLock++;
	for(std::vector<UISurface*>::reverse_iterator it = m_surfaces.rbegin(); it != m_surfaces.rend(); it++)
	{
		if((*it)->getChildCount() > 0)
		{
			// deliver the event
			(*it)->dispatchEvent(event);


			if((*it)->isModal())
				break;
		}

	}
	m_surfaceContainerLock--;

	// -- Built-in event handling
	switch(event.type)
	{
		case Event::MouseMoved:
			{
				processMouseMove(event.mouseMove.x, event.mouseMove.y);
			}
		break;

		case Event::MouseButtonPressed:
			{
				// if the click is outside the focus control, unfocus it
				if(m_state.m_focusControl)
				{
					if(!m_state.m_focusControl->getBounds().contains(event.mouseButton.x, event.mouseButton.y))
					{
						m_state.m_focusControl->blur();
						m_state.m_focusControl->onLostFocus();
						m_state.m_focusControl = NULL;
					}
				}
				bool result = processMouseButtonPressed(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
				eventUsage.hitControls = false;
			}
		break;

		case Event::MouseButtonReleased:
			{				
				processMouseButtonReleased(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button, eventUsage);
			}
			break;

		case Event::TextEntered:
			{
				// Deliver input to the focused control
				if(m_state.m_focusControl)
				{
					m_state.m_focusControl->onTextEvent(event.text.unicode);
				}
			}
			break;
	}

	// now that surfaces were processed, apply changes to surfaces container
	// the updates in the container are only considered in the next event, so it is not wrongly consumed
	applyPendingChanges();

	return eventUsage;
};

/// Set the current language of the ui system
void UIDocument::setLanguage(const String& shortLanguageName)
{
	m_state.m_activeLanguage = shortLanguageName;
	m_state.m_localization.m_currentLanguage = shortLanguageName;
	for(std::vector<UISurface*>::const_iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++){
		(*it)->switchLanguage();
	}
};

/// Process a mouse press event
bool UIDocument::processMouseButtonPressed(int x, int y, Mouse::Button button)
{
	m_surfaceContainerLock = true;
	for(std::vector<UISurface*>::const_iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++)
	{
		(*it)->processMouseButtonPressed(x,y,button);
	}
	m_surfaceContainerLock = false;

	return false;
}

void UIDocument::processMouseButtonReleased(int x, int y, Mouse::Button button, UIEventResult& info)
{
	m_surfaceContainerLock = true;
	for(std::vector<UISurface*>::const_iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++)
	{
		(*it)->processMouseButtonReleased(x, y, button, info);
	}
	m_surfaceContainerLock = false;
}


/// Process a mouve movement event
bool UIDocument::processMouseMove(int x, int y)
{
	for(std::vector<UISurface*>::const_iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++){
		(*it)->processMouseMove(x,y);
	}
	return false;
}

void UIDocument::applyPendingChanges()
{
	for(unsigned int i = 0; i < m_pendingChanges.size(); i++)
	{
		switch(m_pendingChanges[i].type)
		{
			case Add:
				m_surfaces.push_back(m_pendingChanges[i].surface);
			break;

			case Remove:
				m_surfaces.erase(std::find(m_surfaces.begin(), m_surfaces.end(), m_pendingChanges[i].surface));
			break;
		}
	}
	m_pendingChanges.clear();
};



NEPHILIM_NS_END