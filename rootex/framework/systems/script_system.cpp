#include "script_system.h"

#include "components/script_component.h"

ScriptSystem* ScriptSystem::GetSingleton()
{
	static ScriptSystem singleton;
	return &singleton;
}

void ScriptSystem::begin()
{
	ScriptComponent* scriptComponent = nullptr;
	for (auto&& component : s_Components[ScriptComponent::s_ID])
	{
		scriptComponent = (ScriptComponent*)component;
		scriptComponent->begin();
	}
}

void ScriptSystem::update(float deltaMilliseconds)
{
	ScriptComponent* scriptComponent = nullptr;
	for (auto&& component : s_Components[ScriptComponent::s_ID])
	{
		scriptComponent = (ScriptComponent*)component;
		scriptComponent->update(deltaMilliseconds);
	}
}

void ScriptSystem::end()
{
	ScriptComponent* scriptComponent = nullptr;
	for (auto&& component : s_Components[ScriptComponent::s_ID])
	{
		scriptComponent = (ScriptComponent*)component;
		scriptComponent->end();
	}
}
