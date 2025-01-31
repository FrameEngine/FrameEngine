#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "components/MeshComponent.hpp"
#include "core/Registry.hpp"
#include "rendering/RendererAPI.hpp"

class RenderSystem {
public:
  static void render(Registry &registry) {
    RendererAPI::clear();

    for (Entity entity :
         registry.get_entities_with_component<MeshComponent>()) {
      auto &mesh = registry.get_component<MeshComponent>(entity);

      RendererAPI::draw_mesh(mesh.meshID);
    }

    RendererAPI::present();
  }
};

#endif // RENDER_SYSTEM_HPP
