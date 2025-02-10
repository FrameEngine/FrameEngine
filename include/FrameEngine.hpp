/**
 * @file FrameEngine.hpp
 * @brief The main public API for FrameEngine.
 *
 * This header consolidates all necessary includes into a single file.
 * Users can include this to gain access to the core engine, ECS, physics, and
 * rendering.
 */

#ifndef FRAME_ENGINE_HPP
#define FRAME_ENGINE_HPP

// Core systems
#include "Engine.hpp"
#include "Entity.hpp"
#include "Object.hpp"
#include "Registry.hpp"

// Components
#include "components/PhysicsComponent.hpp"
#include "components/TransformComponent.hpp"

// Math utilities
#include "math/Matrix4.hpp"
#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"

// Physics
#include "physics/PhysicsSystem.hpp"

#endif // FRAME_ENGINE_HPP
