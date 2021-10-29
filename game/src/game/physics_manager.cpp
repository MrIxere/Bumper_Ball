#include <game/physics_manager.h>

namespace game
{

    PhysicsManager::PhysicsManager(core::EntityManager& entityManager) :
        bodyManager_(entityManager), boxManager_(entityManager), entityManager_(entityManager)
    {

    }

    bool Circle2Circle(float r1x, float r1y, float r1w, float r1h, float r2x, float r2y, float r2w, float r2h)
    {
        return r1x + r1w >= r2x &&    // r1 right edge past r2 left
            r1x <= r2x + r2w &&    // r1 left edge past r2 right
            r1y + r1h >= r2y &&    // r1 top edge past r2 bottom
            r1y <= r2y + r2h;
    }

    void PhysicsManager::FixedUpdate(sf::Time dt)
    {
        for (core::Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
        {
            if (!entityManager_.HasComponent(entity, static_cast<core::EntityMask>(core::ComponentType::BODY2D)))
                continue;
            auto body = bodyManager_.GetComponent(entity);
            body.position += body.velocity * dt.asSeconds();
            body.rotation += body.angularVelocity * dt.asSeconds();
            bodyManager_.SetComponent(entity, body);
        }
        for (core::Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
        {
            if (!entityManager_.HasComponent(entity,
                                                   static_cast<core::EntityMask>(core::ComponentType::BODY2D) |
                                                   static_cast<core::EntityMask>(core::ComponentType::CIRCLE_COLLIDER2D)) ||
                entityManager_.HasComponent(entity, static_cast<core::EntityMask>(ComponentType::DESTROYED)))
                continue;
            for (core::Entity otherEntity = entity; otherEntity < entityManager_.GetEntitiesSize(); otherEntity++)
            {
                if (entity == otherEntity)
                    continue;
                if (!entityManager_.HasComponent(otherEntity,
                                                 static_cast<core::EntityMask>(core::ComponentType::BODY2D) | static_cast<core::EntityMask>(core::ComponentType::CIRCLE_COLLIDER2D)) ||
                    entityManager_.HasComponent(entity, static_cast<core::EntityMask>(ComponentType::DESTROYED)))
                    continue;
                const Body& body1 = bodyManager_.GetComponent(entity);
                const Circle& circle1 = boxManager_.GetComponent(entity);

                const Body& body2 = bodyManager_.GetComponent(otherEntity);
                const Circle& circle2 = boxManager_.GetComponent(otherEntity);

                if (Circle2Circle(
                    body1.position.x - circle1.extends.x,
                    body1.position.y - circle1.extends.y,
                    circle1.extends.x * 2.0f,
                    circle1.extends.y * 2.0f,
                    body2.position.x - circle2.extends.x,
                    body2.position.y - circle2.extends.y,
                    circle2.extends.x * 2.0f,
                    circle2.extends.y * 2.0f))
                {
                    onTriggerAction_.Execute(entity, otherEntity);
                }

            }
        }
    }

    void PhysicsManager::SetBody(core::Entity entity, const Body& body)
    {
        bodyManager_.SetComponent(entity, body);
    }

    const Body& PhysicsManager::GetBody(core::Entity entity) const
    {
        return bodyManager_.GetComponent(entity);
    }

    void PhysicsManager::AddBody(core::Entity entity)
    {
        bodyManager_.AddComponent(entity);
    }

    void PhysicsManager::AddCircle(core::Entity entity)
    {
        boxManager_.AddComponent(entity);
    }

    void PhysicsManager::SetCircle(core::Entity entity, const Circle& circle)
    {
        boxManager_.SetComponent(entity, circle);
    }

    const Circle& PhysicsManager::GetCircle(core::Entity entity) const
    {
        return boxManager_.GetComponent(entity);
    }

    void PhysicsManager::RegisterTriggerListener(OnTriggerInterface& collisionInterface)
    {
        onTriggerAction_.RegisterCallback(
            [&collisionInterface](core::Entity entity1, core::Entity entity2) { collisionInterface.OnTrigger(entity1, entity2); });
    }

    void PhysicsManager::CopyAllComponents(const PhysicsManager& physicsManager)
    {
        bodyManager_.CopyAllComponents(physicsManager.bodyManager_.GetAllComponents());
        boxManager_.CopyAllComponents(physicsManager.boxManager_.GetAllComponents());
    }
}
