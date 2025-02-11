#include "animation_system.h"

#include "framework/ecs_factory.h"
#include "components/visual/model/animated_model_component.h"

AnimationSystem* AnimationSystem::GetSingleton()
{
	static AnimationSystem singleton;
	return &singleton;
}

AnimationSystem::AnimationSystem()
    : System("AnimationSystem", UpdateOrder::Editor, true)
{
}

void AnimationSystem::update(float deltaMilliseconds)
{
	for (auto& amc : ECSFactory::GetAllAnimatedModelComponent())
	{
		if (!(amc.getOwner().getScene()->getIsScenePaused() && m_IsSystemPaused))
		{
			if (amc.isPlaying() && !amc.hasEnded())
			{
				amc.update(deltaMilliseconds);
			}
		}
	}
}
