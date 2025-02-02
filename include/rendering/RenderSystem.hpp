/**
 * @file RenderSystem.hpp
 * @brief Handles rendering of ECS entities with meshes.
 *
 * RenderSystem updates the "rendering pipeline", applying transformations and
 * sending mesh data to the active Renderer to draw.
 */

#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "components/MeshComponent.hpp"
#include "components/TransformComponent.hpp"
#include "core/Registry.hpp"
#include "rendering/Renderer.hpp"

/**
 * @class RenderSystem
 * @brief Manages rendering of all entities in the ECS.
 *
 * The RenderSystem loops through entities containing both:
 * - MeshComponent (defines geometry).
 * - TransformComponent (defines position/rotation/scale).
 *
 * It then calculates the transformation matrix and sends the mesh
 * to the active Renderer to draw.
 */
class RenderSystem {
public:
  static void render(Registry &registry) {
    Renderer::clear();

    for (Entity entity :
         registry.get_entities_with_component<MeshComponent>()) {
      if (registry.has_component<TransformComponent>(entity)) {
        auto &mesh = registry.get_component<MeshComponent>(entity);
        auto &transform = registry.get_component<TransformComponent>(entity);

        // TODO probably make it as separate method
        Matrix4 transformMatrix = transform.get_transformation_matrix();

        Renderer::draw_mesh(mesh.mesh, transformMatrix);
      }
    }

    Renderer::present(); // Swap buffers
  }
};

#endif // RENDER_SYSTEM_HPP
