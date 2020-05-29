#ifndef UI_H
#define UI_H
class Ui {
public:
  Ui() {
    struct NVGcontext* vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
  }
private:
};


#endif
