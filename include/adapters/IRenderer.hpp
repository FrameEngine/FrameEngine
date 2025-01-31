#ifndef IRENDERER_HPP
#define IRENDERER_HPP

class IRenderer {
public:
  virtual ~IRenderer() = default;

  virtual void init() = 0;
  virtual void clear() = 0;
  virtual void draw_line(int x1, int y1, int x2, int y2) = 0;
  virtual void present() = 0;
};

#endif // IRENDERER_HPP
