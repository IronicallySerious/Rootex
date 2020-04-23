#include "interpreter.h"
#include "core/resource_loader.h"

#include "common/common.h"
#include "components/hierarchy_component.h"
#include "components/transform_component.h"
#include "components/visual/text_visual_2d_component.h"
#include "components/visual/visual_2d_component.h"
#include "components/visual/visual_component.h"
#include "entity_factory.h"
#include "event_manager.h"
#include "script/interpreter.h"

void SolPanic(std::optional<String> maybeMsg)
{
	ERR("Lua is in a panic state and will now abort() the application");
	if (maybeMsg)
	{
		const String& msg = maybeMsg.value();
		ERR("\tError: " + msg);
	}
}

LuaInterpreter::LuaInterpreter()
    : m_Lua(sol::c_call<decltype(&SolPanic), &SolPanic>)
{
	m_Lua.open_libraries(sol::lib::base);
	m_Lua.open_libraries(sol::lib::io);
	m_Lua.open_libraries(sol::lib::math);
	m_Lua.open_libraries(sol::lib::os);
	m_Lua.open_libraries(sol::lib::string);
	m_Lua.open_libraries(sol::lib::table);

	registerTypes();
}

LuaInterpreter* LuaInterpreter::GetSingleton()
{
	static LuaInterpreter singleton;
	return &singleton;
}

void LuaInterpreter::registerTypes()
{
	sol::table rootex = m_Lua.create_named_table("Rootex");
	{
		sol::usertype<Vector2> vector2 = rootex.new_usertype<Vector2>(
		    "Vector2",
		    sol::constructors<Vector2(), Vector2(float, float)>(),
		    sol::meta_function::addition, [](Vector2& l, Vector2& r) { return l + r; },
		    sol::meta_function::subtraction, [](Vector2& l, Vector2& r) { return l - r; });
		vector2["dot"] = &Vector2::Dot;
		vector2["cross"] = [](const Vector2& l, const Vector2& r) { return l.Cross(r); };
		vector2["x"] = &Vector2::x;
		vector2["y"] = &Vector2::y;
	}
	{
		sol::usertype<Vector3> vector3 = rootex.new_usertype<Vector3>(
		    "Vector3",
		    sol::constructors<Vector3(), Vector3(float, float, float)>(),
		    sol::meta_function::addition, [](Vector3& l, Vector3& r) { return l + r; },
		    sol::meta_function::subtraction, [](Vector3& l, Vector3& r) { return l - r; });
		vector3["dot"] = &Vector3::Dot;
		vector3["cross"] = [](const Vector3& l, const Vector3& r) { return l.Cross(r); };
		vector3["x"] = &Vector3::x;
		vector3["y"] = &Vector3::y;
		vector3["z"] = &Vector3::z;
	}
	{
		sol::usertype<Vector4> vector4 = rootex.new_usertype<Vector4>(
		    "Vector4", sol::constructors<Vector4(), Vector4(float, float, float, float)>(),
		    sol::meta_function::addition, [](Vector4& l, Vector4& r) { return l + r; },
		    sol::meta_function::subtraction, [](Vector4& l, Vector4& r) { return l - r; });
		vector4["dot"] = &Vector4::Dot;
		vector4["cross"] = [](const Vector4& l, const Vector4& r) { return l.Cross(l, r); };
		vector4["x"] = &Vector4::x;
		vector4["y"] = &Vector4::y;
		vector4["z"] = &Vector4::z;
		vector4["w"] = &Vector4::w;
	}
	{
		sol::usertype<Color> color = rootex.new_usertype<Color>("Color", sol::constructors<Color(), Color(float, float, float, float)>());
		color["x"] = &Vector4::x;
		color["y"] = &Vector4::y;
		color["z"] = &Vector4::z;
		color["w"] = &Vector4::w;
	}
	{
		sol::usertype<Quaternion> quaternion = rootex.new_usertype<Quaternion>("Quaternion", sol::constructors<Quaternion(), Quaternion(float, float, float, float)>());
		quaternion["x"] = &Quaternion::x;
		quaternion["y"] = &Quaternion::y;
		quaternion["z"] = &Quaternion::z;
		quaternion["w"] = &Quaternion::w;
	}
	{
		sol::usertype<Matrix> matrix = rootex.new_usertype<Matrix>(
		    "Matrix",
		    sol::constructors<Matrix()>(),
		    sol::meta_function::addition, [](Matrix& l, Matrix& r) { return l + r; },
		    sol::meta_function::subtraction, [](Matrix& l, Matrix& r) { return l - r; },
		    sol::meta_function::multiplication, [](Matrix& l, Matrix& r) { return l * r; });
		matrix["Identity"] = sol::var(Matrix::Identity);
	}
	{
		sol::usertype<Event> event = rootex.new_usertype<Event>("Event", sol::constructors<Event(const String&, const Event::Type, const Variant)>());
		event["getName"] = &Event::getName;
		event["getType"] = &Event::getType;
		event["getData"] = &Event::getData;
	}
	{
		rootex["AddEvent"] = [](const String& eventType) { EventManager::GetSingleton()->addEvent(eventType); };
		rootex["RemoveEvent"] = [](const String& eventType) { EventManager::GetSingleton()->removeEvent(eventType); };
		rootex["CallEvent"] = [](const Event& event) { EventManager::GetSingleton()->call(event); };
		rootex["DeferredCallEvent"] = [](const Ref<Event>& event) { EventManager::GetSingleton()->deferredCall(event); };
		rootex["ReturnCallEvent"] = [](const Event& event) { return EventManager::GetSingleton()->returnCall(event); };
	}
	{
		sol::usertype<ResourceLoader> resourceLoader = rootex.new_usertype<ResourceLoader>("ResourceLoader");
		resourceLoader["CreateAudio"] = &ResourceLoader::CreateAudioResourceFile;
		resourceLoader["CreateFont"] = &ResourceLoader::CreateFontResourceFile;
		resourceLoader["CreateImage"] = &ResourceLoader::CreateImageResourceFile;
		resourceLoader["CreateLua"] = &ResourceLoader::CreateLuaTextResourceFile;
		resourceLoader["CreateText"] = &ResourceLoader::CreateTextResourceFile;
		resourceLoader["CreateNewText"] = &ResourceLoader::CreateNewTextResourceFile;
		resourceLoader["CreateVisualModel"] = &ResourceLoader::CreateVisualModelResourceFile;
	}
	{
		sol::usertype<ResourceFile> resourceFile = rootex.new_usertype<ResourceFile>("ResourceFile");
		resourceFile["isValid"] = &ResourceFile::isValid;
		resourceFile["isDirty"] = &ResourceFile::isDirty;
		resourceFile["isOpen"] = &ResourceFile::isOpen;
		resourceFile["reload"] = &ResourceFile::reload;
		resourceFile["getPath"] = [](ResourceFile& f) { return f.getPath().string(); };
		resourceFile["getType"] = &ResourceFile::getType;
	}
	{
		sol::usertype<TextResourceFile> textResourceFile = rootex.new_usertype<TextResourceFile>(
		    "TextResourceFile",
		    sol::base_classes, sol::bases<ResourceFile>());
		textResourceFile["getString"] = &TextResourceFile::getString;
	}
	{
		sol::usertype<LuaTextResourceFile> luaTextResourceFile = rootex.new_usertype<LuaTextResourceFile>(
		    "LuaTextResourceFile",
		    sol::base_classes, sol::bases<ResourceFile, TextResourceFile>());
	}
	{
		sol::usertype<AudioResourceFile> audioResourceFile = rootex.new_usertype<AudioResourceFile>(
		    "AudioResourceFile",
		    sol::base_classes, sol::bases<ResourceFile>());
		audioResourceFile["getAudioDataSize"] = &AudioResourceFile::getAudioDataSize;
		audioResourceFile["getFormat"] = &AudioResourceFile::getFormat;
		audioResourceFile["getFrequency"] = &AudioResourceFile::getFrequency;
		audioResourceFile["getBitDepth"] = &AudioResourceFile::getBitDepth;
		audioResourceFile["getChannels"] = &AudioResourceFile::getChannels;
		audioResourceFile["getDuration"] = &AudioResourceFile::getDuration;
	}
	{
		sol::usertype<VisualModelResourceFile> visualModelResourceFile = rootex.new_usertype<VisualModelResourceFile>(
		    "VisualModelResourceFile",
		    sol::base_classes, sol::bases<ResourceFile>());
	}
	{
		sol::usertype<ImageResourceFile> imageResourceFile = rootex.new_usertype<ImageResourceFile>(
		    "ImageResourceFile",
		    sol::base_classes, sol::bases<ResourceFile>());
	}
	{
		sol::usertype<FontResourceFile> fontResourceFile = rootex.new_usertype<FontResourceFile>(
		    "FontResourceFile",
		    sol::base_classes, sol::bases<ResourceFile>());
	}
	{
		sol::usertype<EntityFactory> entityFactory = rootex.new_usertype<EntityFactory>("EntityFactory");
		entityFactory["Create"] = [](TextResourceFile* t) { return EntityFactory::GetSingleton()->createEntity(t); };
		entityFactory["Find"] = [](EntityID e) { return EntityFactory::GetSingleton()->findEntity(e); };
	}
	{
		sol::usertype<Entity> entity = rootex.new_usertype<Entity>("Entity");
		entity["removeComponent"] = &Entity::removeComponent;
		entity["destroy"] = &Entity::destroy;
		entity["hasComponent"] = &Entity::hasComponent;
		entity["getID"] = &Entity::getID;
		entity["getName"] = &Entity::getName;
		entity["setName"] = &Entity::setName;

		sol::usertype<Component> component = rootex.new_usertype<Component>("Component");
		component["getOwner"] = &Component::getOwner;
		component["getComponentID"] = &Component::getComponentID;
		component["getName"] = &Component::getName;

		{
			sol::usertype<TransformComponent> transformComponent = rootex.new_usertype<TransformComponent>(
			    "TransformComponent",
			    sol::base_classes, sol::bases<Component>());
			entity["getTransform"] = [](Entity* e) { return e->getComponent<TransformComponent>(); };
			transformComponent["setPosition"] = &TransformComponent::setPosition;
			transformComponent["setRotation"] = &TransformComponent::setRotation;
			transformComponent["setScale"] = &TransformComponent::setScale;
			transformComponent["setTransform"] = &TransformComponent::setTransform;
			transformComponent["addTransform"] = &TransformComponent::addTransform;
			transformComponent["getPosition"] = &TransformComponent::getPosition;
			transformComponent["getRotation"] = &TransformComponent::getRotation;
			transformComponent["getScale"] = &TransformComponent::getScale;
			transformComponent["getLocalTransform"] = &TransformComponent::getLocalTransform;
			transformComponent["getParentAbsoluteTransform"] = &TransformComponent::getParentAbsoluteTransform;
			transformComponent["getComponentID"] = &TransformComponent::getComponentID;
			transformComponent["getName"] = &TransformComponent::getName;
		}
		{
			sol::usertype<HierarchyComponent> hierarchyComponent = rootex.new_usertype<HierarchyComponent>(
			    "HierarchyComponent",
			    sol::base_classes, sol::bases<Component>());
			entity["getHierarchy"] = [](Entity* e) { return e->getComponent<HierarchyComponent>(); };
			hierarchyComponent["addChild"] = &HierarchyComponent::addChild;
			hierarchyComponent["removeChild"] = &HierarchyComponent::removeChild;
			hierarchyComponent["clear"] = &HierarchyComponent::clear;
			hierarchyComponent["getParent"] = &HierarchyComponent::getParent;
			hierarchyComponent["getChildren"] = &HierarchyComponent::getChildren;
		}
		{
			sol::usertype<VisualComponent> visualComponent = rootex.new_usertype<VisualComponent>(
			    "VisualComponent",
			    sol::base_classes, sol::bases<Component>());
			entity["getVisual"] = [](Entity* e) { return e->getComponent<VisualComponent>(); };
			visualComponent["isVisible"] = &VisualComponent::isVisible;
			visualComponent["setVisibility"] = &VisualComponent::setVisibility;
		}
		{
			sol::usertype<Visual2DComponent> visual2DComponent = rootex.new_usertype<Visual2DComponent>(
			    "Visual2DComponent",
			    sol::base_classes, sol::bases<Component, VisualComponent>());
			entity["getVisual2D"] = [](Entity* e) { return e->getComponent<Visual2DComponent>(); };
		}
		{
			sol::usertype<TextVisual2DComponent> textVisual2DComponent = rootex.new_usertype<TextVisual2DComponent>(
			    "TextVisual2DComponent",
			    sol::base_classes, sol::bases<Component, VisualComponent, Visual2DComponent>());
			entity["getTextVisual2D"] = [](Entity* e) { return e->getComponent<TextVisual2DComponent>(); };
			textVisual2DComponent["setFont"] = &TextVisual2DComponent::setFont;
			textVisual2DComponent["setText"] = &TextVisual2DComponent::setText;
		}
	}
}
