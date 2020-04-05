#define TESLA_INIT_IMPL
#include <tesla.hpp>

#include <stdarg.h>
#include <string>
#include <vector>

#include "ntp-client.hpp"
#include "servers.hpp"
#include "tesla-ext.hpp"

char* strdup(const char* s) {
    size_t slen = strlen(s);
    char* result = (char*)malloc(slen + 1);
    if (result == NULL) {
        return NULL;
    }

    memcpy(result, s, slen + 1);
    return result;
}

TimeServiceType __nx_time_service_type = TimeServiceType_System;

class NtpGui : public tsl::Gui {
private:
    char* Message;
    int currentServer = 0;
    bool blockFlag = false;
    std::vector<std::string> serverAddresses;

    char* getCurrentServer() {
        return &serverAddresses[currentServer][0];
    }

    void setMessage(const char* fmt, ...) {
        char buff[200];
        va_list va;
        va_start(va, fmt);
        vsnprintf(buff, sizeof(buff), fmt, va);
        va_end(va);

        Message = strdup(buff);
    }

    bool setNetworkSystemClock(time_t time) {
        Result rs = timeSetCurrentTime(TimeType_NetworkSystemClock, (uint64_t)time);
        if (R_FAILED(rs)) {
            return false;
        }
        return true;
    }

    void setTime() {
        char* srv = getCurrentServer();
        NTPClient* client = new NTPClient(srv);
        time_t ntpTime = client->getTime();

        if (!client->hasFailed()) {
            if (setNetworkSystemClock(ntpTime)) {
                setMessage("Synced with %s", srv);
            } else {
                setMessage("Unable to set system clock.");
            }
        } else {
            setMessage("Error: %s", client->getErrorMessage().c_str());
        }

        delete client;
    }

    void getOffset() {
        time_t currentTime;
        Result rs = timeGetCurrentTime(TimeType_UserSystemClock, (u64*)&currentTime);
        if (R_FAILED(rs)) {
            setMessage("timeGetCurrentTime %x", rs);
            return;
        }

        char* srv = getCurrentServer();
        NTPClient* client = new NTPClient(srv);
        time_t ntpTimeOffset = client->getTimeOffset(currentTime);

        if (!client->hasFailed()) {
            setMessage("Offset: %+ds", ntpTimeOffset);
        } else {
            setMessage("Error: %s", client->getErrorMessage().c_str());
        }

        delete client;
    }

    void serverChanged() {
        setMessage("");
    }

    inline void operationBlock(std::function<void()> f) {
        setMessage("");
        blockFlag = true;
        f();
        blockFlag = false;
    }

    std::function<bool(u64 keys)> syncListener = [=](u64 keys) {
        if (!blockFlag && keys & KEY_A) {
            operationBlock([=]() {
                setTime();
            });
            return true;
        }
        return false;
    };

    std::function<bool(u64 keys)> offsetListener = [=](u64 keys) {
        if (!blockFlag && keys & KEY_Y) {
            operationBlock([=]() {
                getOffset();
            });
            return true;
        }
        return false;
    };

public:
    NtpGui() : serverAddresses(NTPSERVERS[1].begin(), NTPSERVERS[1].end()) {
        setMessage("");
    }

    virtual tsl::elm::Element* createUI() override {
        auto frame = new tsl::elm::OverlayFrame("QuickNTP", "by NedEX - v1.0.0");

        auto list = new tsl::elm::List();

        list->addItem(new tsl::elm::CategoryHeader("Pick server   |   \uE0E0  Sync   |   \uE0E3  Offset"));

        auto* trackbar = new tsl::elm::NamedStepTrackBar("\uE017", NTPSERVERS[0]);
        trackbar->setValueChangedListener([=](u8 val) {
            currentServer = val;
            serverChanged();
        });
        trackbar->setClickListener([=](u8 val) {
            return syncListener(val) || offsetListener(val);
        });
        list->addItem(trackbar);

        auto* syncTimeItem = new tsl::elm::ListItem("Sync time");
        syncTimeItem->setClickListener(syncListener);
        list->addItem(syncTimeItem);

        list->addItem(new tsl::elm::CustomDrawer([](tsl::gfx::Renderer* renderer, s32 x, s32 y, s32 w, s32 h) {
                          renderer->drawString("Syncs the time with the selected server.", false, x + 20, y + 20, 15, renderer->a(tsl::style::color::ColorDescription));
                      }),
                      50);

        auto* getOffsetItem = new tsl::elm::ListItem("Get offset");
        getOffsetItem->setClickListener([=](u64 keys) {
            if (!blockFlag && keys & KEY_A) {
                operationBlock([=]() {
                    getOffset();
                });
                return true;
            }
            return false;
        });
        list->addItem(getOffsetItem);

        list->addItem(new tsl::elm::CustomDrawer([](tsl::gfx::Renderer* renderer, s32 x, s32 y, s32 w, s32 h) {
                          renderer->drawString("Gets the seconds offset with the selected server.\n\n\uE016  A value of ± 3 seconds is acceptable.", false, x + 20, y + 20, 15, renderer->a(tsl::style::color::ColorDescription));
                      }),
                      50);

        list->addItem(new tsl::elm::CustomDrawerUnscissored([=](tsl::gfx::Renderer* renderer, s32 x, s32 y, s32 w, s32 h) {
            renderer->drawString(Message, false, x + 5, tsl::cfg::FramebufferHeight - 100, 20, renderer->a(tsl::style::color::ColorText));
        }));

        frame->setContent(list);
        return frame;
    }

    virtual void update() override {}

    virtual bool handleInput(u64 keysDown, u64 keysHeld, touchPosition touchInput,
                             JoystickPosition leftJoyStick,
                             JoystickPosition rightJoyStick) override {
        return false;
    }
};

class NtpOverlay : public tsl::Overlay {
public:
    virtual void initServices() override {
        ASSERT_FATAL(socketInitializeDefault());
        ASSERT_FATAL(nifmInitialize(NifmServiceType_User));
        ASSERT_FATAL(timeInitialize());
        ASSERT_FATAL(smInitialize());
    }

    virtual void exitServices() override {
        socketExit();
        nifmExit();
        timeExit();
        smExit();
    }

    virtual void onShow() override {}
    virtual void onHide() override {}

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<NtpGui>();
    }
};

int main(int argc, char** argv) {
    return tsl::loop<NtpOverlay>(argc, argv);
}
