#include <Nephilim/World/Level.h>

#include <Nephilim/Foundation/File.h>
#include <Nephilim/Foundation/DataStream.h>

#include <Nephilim/World/Prefab.h>
#include <Nephilim/World/ComponentManager.h>
#include <Nephilim/World/ComponentArray.h>
#include <Nephilim/World/ASpriteComponent.h>
#include <Nephilim/World/APointLightComponent.h>

NEPHILIM_NS_BEGIN

Level::Level()
: world(nullptr)
{
	
}

/// Ensure all stuff goes down when the level is destroyed
Level::~Level()
{

}

/// Get total number of GameObject and its subclasses spawned in this Level
int32_t Level::getGameObjectCount()
{
	return static_cast<int32_t>(actors.size());
}

/// Write this level to a binary file
bool Level::write(const String& filename)
{
	File file(filename, File::BinaryWrite);
	if (file)
	{
		DataStream stream(file);
		stream << name;



		return true;
	}
	else
		return false;
}

/// Read data from a binary file into this Level
bool Level::read(const String& filename)
{
	File file(filename, File::BinaryRead);
	if (file)
	{

		return true;
	}
	else
		return false;
}

/// Utility to quickly spawn a point light into our world
void Level::createPointLight(Vector3D position, Vector3D lightColor)
{
	
}

/// Instance a new game object from a prefab
GameObject* Level::instance(const Prefab& prefab)
{
	GameObject* go = nullptr;

	// Instance every GO in the prefab
	for (std::size_t i = 0; i < prefab.GameObjectPrefabs.size(); ++i)
	{
		const Prefab::GameObjectPrefab& goPrefab = prefab.GameObjectPrefabs[i];

		go = (GameObject*)Factory::Create(Factory::GetClass(prefab.GameObjectPrefabs[i].Class));
		go->_Class = Factory::GetClass(prefab.GameObjectPrefabs[i].Class);

		gameObjects.push_back(go);


		for (std::size_t j = 0; j < goPrefab.Components.size(); ++j)
		{
			String ComponentClass = goPrefab.Components[j]->ComponentClass;
			Component* Component_ = (Component*)Factory::Create(Factory::GetClass(ComponentClass));
			
			go->components.push_back(Component_);
		}
	}

	return go;
}


NEPHILIM_NS_END