
.. _program_listing_file_rootex_core_ui_flipbook_decorator.h:

Program Listing for File flipbook_decorator.h
=============================================

|exhale_lsh| :ref:`Return to documentation for file <file_rootex_core_ui_flipbook_decorator.h>` (``rootex\core\ui\flipbook_decorator.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "common/common.h"
   #include "core/renderer/texture.h"
   
   #include "rootex_decorator.h"
   
   #include "RmlUi/Core/DecoratorInstancer.h"
   #include "RmlUi/Core/PropertyDefinition.h"
   #include "RmlUi/Core/Geometry.h"
   #include "RmlUi/Core/GeometryUtilities.h"
   
   class FlipbookDecorator : public RootexDecorator
   {
       struct FlipbookElementData
       {
           static inline Vector<FlipbookElementData*> s_DataInstances;
   
           float currentFrame = 0;
           Rml::Geometry geometry;
   
           FlipbookElementData();
           ~FlipbookElementData();
       };
   
       Vector<Rml::Rectangle> m_SpriteRectangles;
       float m_FPS = 1.0f;
   
   public:
       FlipbookDecorator() = default;
       ~FlipbookDecorator() = default;
   
       bool addFrame(const Rml::Sprite* sprite);
       void setFPS(float fps) { m_FPS = fps; }
   
       void update(float deltaSeconds) override;
   
       Rml::DecoratorDataHandle GenerateElementData(Rml::Element* element) const override;
       void ReleaseElementData(Rml::DecoratorDataHandle elementData) const override;
       void RenderElement(Rml::Element* element, Rml::DecoratorDataHandle elementData) const override;
   };
   
   class FlipbookDecoratorInstancer : public Rml::DecoratorInstancer
   {
       Rml::PropertyId m_FPSID;
       Rml::PropertyId m_SpritesID;
   
   public:
       FlipbookDecoratorInstancer();
   
       Rml::SharedPtr<Rml::Decorator> InstanceDecorator(const Rml::String& name, const Rml::PropertyDictionary& properties, const Rml::DecoratorInstancerInterface& instancerInterface) override;
   };
