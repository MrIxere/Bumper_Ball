#include <game/physics_manager.h>

namespace game
{
    
    PhysicsManager::PhysicsManager(core::EntityManager& entityManager) :
        bodyManager_(entityManager), circleManager_(entityManager), entityManager_(entityManager)
    {

    }

    void PhysicsManager::FixedUpdate(sf::Time dt)
    {
        for (core::Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
        {
            if (!entityManager_.HasComponent(entity, static_cast<core::EntityMask>(core::ComponentType::BODY2D)))
                continue;
            auto body = bodyManager_.GetComponent(entity);
            core::Vec2f max_pos = { (core::windowSize.x / core::pixelPerMeter / 2),
                (core::windowSize.y / core::pixelPerMeter / 2) };
            core::Vec2f min_pos = { -(core::windowSize.x / core::pixelPerMeter / 2),
            -(core::windowSize.y / core::pixelPerMeter / 2) };
            
            body.position += body.velocity * dt.asSeconds();

            if (body.position.x <= min_pos.x + Body::radius)
            {
                body.position.x = min_pos.x + Body::radius;
                body.velocity.x = -Body::Rebound * body.velocity.x;
            }
            if (body.position.y <= min_pos.y + Body::radius)           
            {
                body.position.y = min_pos.y + Body::radius;
                body.velocity.y = -Body::Rebound * body.velocity.y;
            }
            if (body.position.x >= max_pos.x - Body::radius)
            {
                body.position.x = max_pos.x - Body::radius;
                body.velocity.x = -Body::Rebound * body.velocity.x;
            }
            if (body.position.y >= max_pos.y - Body::radius)
            {
                body.position.y = max_pos.y - Body::radius;
                body.velocity.y = -Body::Rebound * body.velocity.y;
            }

            bodyManager_.SetComponent(entity, body);

            auto body1 = bodyManager_.GetComponent(0);
            auto body2 = bodyManager_.GetComponent(1);

            if(BodyContact(body1, body2))
            {
                ResolveBodyContact(body1, body2);
                bodyManager_.SetComponent(0, body1);
                bodyManager_.SetComponent(1, body2);
            }

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
                const Circle& circle1 = circleManager_.GetComponent(entity);

                const Body& body2 = bodyManager_.GetComponent(otherEntity);
                const Circle& circle2 = circleManager_.GetComponent(otherEntity);

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
        circleManager_.AddComponent(entity);
    }

    void PhysicsManager::SetCircle(core::Entity entity, const Circle& circle)
    {
        circleManager_.SetComponent(entity, circle);
    }

    const Circle& PhysicsManager::GetCircle(core::Entity entity) const
    {
        return circleManager_.GetComponent(entity);
    }

    void PhysicsManager::RegisterTriggerListener(OnTriggerInterface& collisionInterface)
    {
        onTriggerAction_.RegisterCallback(
            [&collisionInterface](core::Entity entity1, core::Entity entity2) { collisionInterface.OnTrigger(entity1, entity2); });
    }

    void PhysicsManager::CopyAllComponents(const PhysicsManager& physicsManager)
    {
        bodyManager_.CopyAllComponents(physicsManager.bodyManager_.GetAllComponents());
        circleManager_.CopyAllComponents(physicsManager.circleManager_.GetAllComponents());
    }

    bool PhysicsManager::BodyContact(Body body1, Body body2)
    {
        return Body::CalculateDistance(body1, body2) < (Body::radius  + Body::radius);
    }

    void PhysicsManager::ResolveBodyContact(Body& body1, Body& body2)
    {
        float v1n = ComputeNormal(body1.position, ContactPoint(body1, body2)).x * body1.velocity.x +
            ComputeNormal(body1.position, ContactPoint(body1, body2)).y * body1.velocity.y;
        float v1t = ComputeTangent(body1.position, ContactPoint(body1, body2)).x * body1.velocity.x +
            ComputeTangent(body1.position, ContactPoint(body1, body2)).y * body1.velocity.y;
        float v2n = ComputeNormal(body2.position, ContactPoint(body1, body2)).x * body2.velocity.x +
            ComputeNormal(body2.position, ContactPoint(body1, body2)).y * body2.velocity.y;
        float v2t = ComputeTangent(body2.position, ContactPoint(body1, body2)).x * body2.velocity.x +
            ComputeTangent(body2.position, ContactPoint(body1, body2)).y * body2.velocity.y;

        body1.velocity.x = ComputeNormal(body1.position, ContactPoint(body1, body2)).x * v2n + ComputeTangent(
            body1.position, ContactPoint(body1, body2)).x * v1t * -Body::Rebound;
        body1.velocity.y = ComputeNormal(body1.position, ContactPoint(body1, body2)).y * v2n + ComputeTangent(
            body1.position, ContactPoint(body1, body2)).y * v1t * -Body::Rebound;
        body2.velocity.x = ComputeNormal(body2.position, ContactPoint(body1, body2)).x * v1n + ComputeTangent(
            body2.position, ContactPoint(body1, body2)).x * v2t * -Body::Rebound;
        body2.velocity.y = ComputeNormal(body2.position, ContactPoint(body1, body2)).y * v1n + ComputeTangent(
            body2.position, ContactPoint(body1, body2)).y * v2t * -Body::Rebound;


        body1.position = RelocatedCenter(body1, ContactPoint(body1, body2));
        body2.position = RelocatedCenter(body2, ContactPoint(body1, body2));
        body1.velocity = body1.velocity * -Body::Rebound;
        body2.velocity = body2.velocity * -Body::Rebound;
    }

    core::Vec2f PhysicsManager::ContactPoint(const Body& body1, const Body& body2) const
    {
        double ratio = (Body::radius) / ((Body::radius)+(Body::radius));
        return (body2.position - body1.position) * ratio + body1.position;
    }

    core::Vec2f PhysicsManager::RelocatedCenter(const Body& body, const core::Vec2f& v)
    {
        double ratio = (Body::radius) / (body.position - v).GetMagnitude();
        return (body.position - v) * ratio + v;
    }


    float Body::CalculateDistance(Body body1, Body body2)
    {
        const float dx = body2.position.x - body1.position.x;
        const float dy = body2.position.y - body1.position.y;
        return std::sqrt(dx * dx + dy * dy);
    }
}
