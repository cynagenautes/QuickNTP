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

class CustomOverlayFrame : public OverlayFrame {
public:
    CustomOverlayFrame(const std::string& title, const std::string& subtitle) : OverlayFrame(title, subtitle) {}

    virtual void layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight) override {
        this->setBoundaries(parentX, parentY, parentWidth, parentHeight);

        if (this->m_contentElement != nullptr) {
            this->m_contentElement->setBoundaries(parentX + 35, parentY + 95, parentWidth - 85, parentHeight - 73 - 95);
            this->m_contentElement->invalidate();
        }
    }
};
} // namespace tsl::elm
