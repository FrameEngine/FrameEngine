#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <algorithm>
#include <bitset>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

// Yeah, could use vector, but MAX_COMPONENTS allows to use bitset for
// fast lookup, might be handy in physics engine. And I doubt that I'll
// need more billions of objects, if so, I'll make particle system, that
// will work as stand alone component
constexpr int MAX_COMPONENTS = 32;

// Each entity must have unique ID; Why not store objects? Well, it looks like
// storing only int IDs is way more efficient
using Entity = int;

class IComponentStorage {
public:
  virtual ~IComponentStorage() = default;
};

// Each component type will have their own storage container
template <typename T> class ComponentStorage : public IComponentStorage {
private:
  // Dynamic storage with O(1) access time
  std::unordered_map<Entity, T> components;

public:
  void add(Entity entity, const T &component) {
    components[entity] = component;
  }

  void remove(Entity entity) { components.erase(entity); }

  bool has(Entity entity) const {
    return components.find(entity) != components.end();
  }

  T &get(Entity entity) { return components.at(entity); }
};

// Core of system, manages entities and components
class Registry {
private:
  std::vector<Entity> entities;

  // I needed a generic way to store multiple types of components as different
  // components have completley different data structers
  std::unordered_map<size_t, std::unique_ptr<IComponentStorage>> componentPools;

  // Each entity gets a bitset that tracks which components it has.
  //  This allows for fast filtering when querying for entities with
  //  specific components.
  std::unordered_map<Entity, std::bitset<MAX_COMPONENTS>> entityComponentMasks;

  // As entities are stored as integers, this is fast way to make them "unique"
  int nextEntityID = 0;

public:
  Entity create_entity() {
    Entity entity = nextEntityID++;
    entities.push_back(entity);
    return entity;
  }

  void destroy_entity(Entity entity) {
    entities.erase(std::remove(entities.begin(), entities.end(), entity),
                   entities.end());
    entityComponentMasks.erase(entity);
  }

  template <typename T> void add_component(Entity entity, const T &component) {
    size_t typeID = typeid(T).hash_code();
    // If this is the first time this component type is used, create a storage
    // bucket
    if (componentPools.find(typeID) == componentPools.end()) {
      componentPools[typeID] = std::make_unique<ComponentStorage<T>>();
    }

    // Store component in the bucket
    static_cast<ComponentStorage<T> *>(componentPools[typeID].get())
        ->add(entity, component);

    // Update entity's bitmask
    entityComponentMasks[entity].set(typeID % MAX_COMPONENTS);
  }

  template <typename T> bool has_component(Entity entity) {
    size_t typeID = typeid(T).hash_code();
    return entityComponentMasks[entity].test(typeID % MAX_COMPONENTS);
  }

  template <typename T> T &get_component(Entity entity) {
    size_t typeID = typeid(T).hash_code();
    return static_cast<ComponentStorage<T> *>(componentPools[typeID].get())
        ->get(entity);
  }

  template <typename T> void remove_component(Entity entity) {
    size_t typeID = typeid(T).hash_code();

    if (componentPools.find(typeID) != componentPools.end()) {
      static_cast<ComponentStorage<T> *>(componentPools[typeID].get())
          ->remove(entity);
      entityComponentMasks[entity].reset(typeID % MAX_COMPONENTS);
    }
  }

  // The key optimization here is that I can filter entities in O(1) using
  // ComponentMask, saves a lot of time on cycles, but amount of entities must
  // be specified before simulation starts (before compilation? not sure)
  template <typename T> std::vector<Entity> get_entities_with_component() {
    std::vector<Entity> result;
    size_t typeID = typeid(T).hash_code();
    for (auto &[entity, mask] : entityComponentMasks) {
      if (mask.test(typeID % MAX_COMPONENTS)) {
        result.push_back(entity);
      }
    }

    return result;
  }
};

#endif // !REGISTRY_HPP
