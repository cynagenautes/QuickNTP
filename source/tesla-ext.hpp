#include <tesla.hpp>

#include <string>
#include <vector>

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

class NamedStepTrackBarVector : public StepTrackBar {
public:
    /**
     * @brief Constructor
     *
     * @param icon Icon shown next to the track bar
     * @param stepDescriptions Step names displayed above the track bar
     */
    NamedStepTrackBarVector(const char icon[3], std::vector<std::string> stepDescriptions)
        : StepTrackBar(icon, stepDescriptions.size()), m_stepDescriptions(stepDescriptions.begin(), stepDescriptions.end()) {}

    virtual ~NamedStepTrackBarVector() {}

    virtual void draw(gfx::Renderer* renderer) override {

        u16 trackBarWidth = this->getWidth() - 95;
        u16 stepWidth = trackBarWidth / (this->m_numSteps - 1);

        for (u8 i = 0; i < this->m_numSteps; i++) {
            renderer->drawRect(this->getX() + 60 + stepWidth * i, this->getY() + 50, 1, 10, a(tsl::style::color::ColorFrame));
        }

        u8 currentDescIndex = std::clamp(this->m_value / (100 / (this->m_numSteps - 1)), 0, this->m_numSteps - 1);

        auto [descWidth, descHeight] = renderer->drawString(this->m_stepDescriptions[currentDescIndex].c_str(), false, 0, 0, 15, tsl::style::color::ColorTransparent);
        renderer->drawString(this->m_stepDescriptions[currentDescIndex].c_str(), false, ((this->getX() + 60) + (this->getWidth() - 95) / 2) - (descWidth / 2), this->getY() + 20, 15, a(tsl::style::color::ColorDescription));

        StepTrackBar::draw(renderer);
    }

protected:
    std::vector<std::string> m_stepDescriptions;
};
} // namespace tsl::elm
