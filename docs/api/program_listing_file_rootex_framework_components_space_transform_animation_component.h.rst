
.. _program_listing_file_rootex_framework_components_space_transform_animation_component.h:

Program Listing for File transform_animation_component.h
========================================================

|exhale_lsh| :ref:`Return to documentation for file <file_rootex_framework_components_space_transform_animation_component.h>` (``rootex\framework\components\space\transform_animation_component.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "component.h"
   #include "components/space/transform_component.h"
   
   class TransformAnimationComponent : public Component
   {
       COMPONENT(TransformAnimationComponent, Category::General);
       DEPENDS_ON(TransformComponent);
   
   public:
       struct Keyframe
       {
           float timePosition;
           Matrix transform;
       };
   
       enum class TransitionType
       {
           SmashSmash = 0,
           EaseEase = 1,
           SmashEase = 2,
           EaseSmash = 3
       };
   
       enum class AnimationMode
       {
           None = 0,
           Looping = 1,
           Alternating = 2
       };
   
   private:
       Vector<Keyframe> m_Keyframes;
       bool m_IsPlayOnStart;
       AnimationMode m_AnimationMode;
       TransitionType m_TransitionType;
   
       bool m_IsPlaying;
       float m_CurrentTimePosition;
       float m_TimeDirection;
   
       friend class TransformAnimationSystem;
   
   public:
       TransformAnimationComponent(Entity& owner, const JSON::json& data);
       ~TransformAnimationComponent() = default;
   
       void pushKeyframe(const Keyframe& keyFrame);
       void popKeyframe(int count);
       bool isPlaying() const { return m_IsPlaying; }
       bool isPlayOnStart() const { return m_IsPlayOnStart; }
       bool hasEnded() const;
       float getStartTime() const;
       float getEndTime() const;
       void reset();
   
       void interpolate(float t);
   
       void setPlaying(bool enabled);
   
       bool setupData() override;
       JSON::json getJSON() const override;
       void draw() override;
   };
   
   DECLARE_COMPONENT(TransformAnimationComponent);
