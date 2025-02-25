#define CATCH_CONFIG_MAIN

#include "Registry.hpp"
#include "catch2/catch_all.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/TransformComponent.hpp"
#include "physics/PhysicsSystem.hpp"

TEST_CASE(
    "Registry: Should create unique entity IDs and destroy entities correctly",
    "[ECS][Entity]") {
  Registry registry;

  EntityID e1 = registry.create_entity();
  EntityID e2 = registry.create_entity();

  REQUIRE(e1 != e2); // Each entity should have a unique ID

  registry.destroy_entity(e1);

  // Ensure that destroyed entity no longer has any components
  REQUIRE_FALSE(registry.has_component<TransformComponent>(e1));
}

TEST_CASE("Registry: Should add and retrieve components correctly",
          "[ECS][Component]") {
  Registry registry;
  EntityID entity = registry.create_entity();

  // Add a TransformComponent with specific position values
  TransformComponent transform = {{1.0f, 2.0f, 3.0f}};
  registry.add_component<TransformComponent>(entity, transform);

  // Verify the component was added
  REQUIRE(registry.has_component<TransformComponent>(entity));

  // Check the position data of the added component
  auto &retrievedTransform = registry.get_component<TransformComponent>(entity);
  REQUIRE(retrievedTransform.position.x == Catch::Approx(1.0f));
  REQUIRE(retrievedTransform.position.y == Catch::Approx(2.0f));
  REQUIRE(retrievedTransform.position.z == Catch::Approx(3.0f));
}

TEST_CASE("Registry: Should remove components correctly", "[ECS][Component]") {
  Registry registry;
  EntityID entity = registry.create_entity();

  // Add and then remove a component
  registry.add_component<TransformComponent>(entity, {{1.0f, 2.0f, 3.0f}});
  registry.remove_component<TransformComponent>(entity);

  // Verify that the component has been removed
  REQUIRE_FALSE(registry.has_component<TransformComponent>(entity));
}

TEST_CASE("Registry: Should correctly query entities with a specific component",
          "[ECS][Query]") {
  Registry registry;

  // Create two entities, only one gets the TransformComponent
  EntityID e1 = registry.create_entity();
  EntityID e2 = registry.create_entity();
  registry.add_component<TransformComponent>(e1, {{1.0f, 2.0f, 3.0f}});

  auto entities = registry.get_entities_with_component<TransformComponent>();

  REQUIRE(entities.size() == 1); // Only one entity should have the component
  REQUIRE(entities[0] == e1);    // The entity should be e1
}

TEST_CASE(
    "PhysicsSystem: Should update TransformComponent based on PhysicsComponent",
    "[ECS][Physics]") {
  Registry registry;
  EntityID entity = registry.create_entity();

  // Add Transform and Physics components
  registry.add_component<TransformComponent>(entity, {{0.0f, 0.0f, 0.0f}});
  registry.add_component<PhysicsComponent>(
      entity, {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 1.0f});

  float dt = 1.0f;
  PhysicsSystem::update(registry, dt);

  // Verify the TransformComponent position was updated by velocity
  auto &transform = registry.get_component<TransformComponent>(entity);
  REQUIRE(transform.position.x == Catch::Approx(1.0f)); // x += 1.0f * dt
  REQUIRE(transform.position.y == Catch::Approx(0.0f));
  REQUIRE(transform.position.z == Catch::Approx(0.0f));
}
