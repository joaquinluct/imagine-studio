#pragma once

#include "IUI.h"

namespace UI {

class SimpleUI : public IUI {
public:
    SimpleUI() = default;
    ~SimpleUI() override = default;

    void Initialize(HWND hwnd) override;
    void Shutdown() override;
    void Draw() override;

private:
    HWND hwnd_ = nullptr;
};

} // namespace UI
