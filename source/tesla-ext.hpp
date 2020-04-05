#include <tesla.hpp>

namespace tsl::elm {
class CustomDrawerUnscissored : public Element {
public:
    CustomDrawerUnscissored(std::function<void(gfx::Renderer* r, s32 x, s32 y, s32 w, s32 h)> renderFunc) : Element(), m_renderFunc(renderFunc) {}
    virtual ~CustomDrawerUnscissored() {}

    virtual void draw(gfx::Renderer* renderer) override {
        this->m_renderFunc(renderer, ELEMENT_BOUNDS(this));
    }

    virtual void layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight) override {
    }

private:
    std::function<void(gfx::Renderer*, s32 x, s32 y, s32 w, s32 h)> m_renderFunc;
};
} // namespace tsl::elm
