#pragma once
#include "game_globals.h"
#include "engine/component.h"
#include "engine/entity.h"
#include "maths/angle.h"
#include "maths/vec2.h"

#include <SFML/System/Time.hpp>

#include "utils/action_utility.h"

namespace game
{
    enum class BodyType
    {
        DYNAMIC,
        STATIC
    };
    struct Body
    {
        core::Vec2f position = core::Vec2f::zero();
        core::Vec2f velocity = core::Vec2f::zero();
        core::degree_t angularVelocity = core::degree_t(0.0f);
        BodyType bodyType = BodyType::DYNAMIC;
    	static float CalculateDistance(Body body1, Body body2);
        static constexpr float Rebound = 0.99f;
        static constexpr float radius = 0.17f;
    };

    struct Circle
    {
        core::Vec2f extends = core::Vec2f::one();
        bool isTrigger = false;
    };

    class OnTriggerInterface
    {
    public:
        virtual ~OnTriggerInterface() = default;
        virtual void OnTrigger(core::Entity entity1, core::Entity entity2) = 0;
    };

    class BodyManager : public core::ComponentManager<Body, static_cast<core::EntityMask>(core::ComponentType::BODY2D)>
    {
    public:
        using ComponentManager::ComponentManager;
    };
    class CircleManager : public core::ComponentManager<Circle, static_cast<core::EntityMask>(core::ComponentType::CIRCLE_COLLIDER2D)>
    {
    public:
        using ComponentManager::ComponentManager;
    };

    class PhysicsManager
    {
    public:
        explicit PhysicsManager(core::EntityManager& entityManager);
        void FixedUpdate(sf::Time dt);
        [[nodiscard]] const Body& GetBody(core::Entity entity) const;
        void SetBody(core::Entity entity, const Body& body);
        void AddBody(core::Entity entity);

        void AddCircle(core::Entity entity);
        void SetCircle(core::Entity entity, const Circle& circle);
        [[nodiscard]] const Circle& GetCircle(core::Entity entity) const;

        void RegisterTriggerListener(OnTriggerInterface& collisionInterface);
        void CopyAllComponents(const PhysicsManager& physicsManager);
    private:
        core::EntityManager& entityManager_;
        BodyManager bodyManager_;
        CircleManager circleManager_;
        core::Action<core::Entity, core::Entity> onTriggerAction_;
        bool BodyContact(Body body1, Body body2);
        void ResolveBodyContact(Body& body1, Body& body2);
        core::Vec2f ContactPoint(const Body& rb1, const Body& rb2) const;
        core::Vec2f RelocatedCenter(const Body& body, const core::Vec2f& v);
    };

}
