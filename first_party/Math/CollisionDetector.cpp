#include "CollisionDetector.h"

#include "Collision.h"
#include "Rigidbody.h"

void CollisionDetector::BeginContact(b2Contact* contact)
{
    /*b2Fixture* fixture_a = contact->GetFixtureA();
    b2Fixture* fixture_b = contact->GetFixtureB();

    b2WorldManifold world_manifold;
    contact->GetWorldManifold(&world_manifold);

    Collision collision;
    collision.relative_velocity = fixture_a->GetBody()->GetLinearVelocity() - fixture_b->GetBody()->GetLinearVelocity();


    if (fixture_a->IsSensor() && fixture_b->IsSensor() && fixture_a->GetFilterData().categoryBits != 0xBEEF && fixture_b->GetFilterData().categoryBits != 0xBEEF) {
        Actor* actor_a = reinterpret_cast<Rigidbody*>((fixture_a->GetUserData()).pointer)->actor;
        Actor* actor_b = reinterpret_cast<Rigidbody*>((fixture_b->GetUserData()).pointer)->actor;

        collision.other = actor_b;
        actor_a->OnTriggerEnter(&collision);
        collision.other = actor_a;
        actor_b->OnTriggerEnter(&collision);
    }
    else if (!fixture_a->IsSensor() && !fixture_b->IsSensor()) {
        b2WorldManifold world_manifold;
        contact->GetWorldManifold(&world_manifold);
        collision.point = world_manifold.points[0];
        collision.normal = world_manifold.normal;
        Actor* actor_a = reinterpret_cast<Rigidbody*>((fixture_a->GetUserData()).pointer)->actor;
        Actor* actor_b = reinterpret_cast<Rigidbody*>((fixture_b->GetUserData()).pointer)->actor;

        collision.other = actor_b;
        actor_a->OnCollisionEnter(&collision);
        collision.other = actor_a;
        actor_b->OnCollisionEnter(&collision);
    }*/
}

void CollisionDetector::EndContact(b2Contact* contact)
{
    /*b2Fixture* fixture_a = contact->GetFixtureA();
    b2Fixture* fixture_b = contact->GetFixtureB();

    Collision collision;
    collision.relative_velocity = fixture_a->GetBody()->GetLinearVelocity() - fixture_b->GetBody()->GetLinearVelocity();

    if (fixture_a->IsSensor() && fixture_b->IsSensor() && fixture_a->GetFilterData().categoryBits != 0xBEEF && fixture_b->GetFilterData().categoryBits != 0xBEEF) {
        Actor* actor_a = reinterpret_cast<Rigidbody*>((fixture_a->GetUserData()).pointer)->actor;
        Actor* actor_b = reinterpret_cast<Rigidbody*>((fixture_b->GetUserData()).pointer)->actor;

        collision.other = actor_b;
        actor_a->OnTriggerExit(&collision);
        collision.other = actor_a;
        actor_b->OnTriggerExit(&collision);
    }
    else if (!fixture_a->IsSensor() && !fixture_b->IsSensor()) {
        Actor* actor_a = reinterpret_cast<Rigidbody*>((fixture_a->GetUserData()).pointer)->actor;
        Actor* actor_b = reinterpret_cast<Rigidbody*>((fixture_b->GetUserData()).pointer)->actor;

        collision.other = actor_b;
        actor_a->OnCollisionExit(&collision);
        collision.other = actor_a;
        actor_b->OnCollisionExit(&collision);
    }*/
}