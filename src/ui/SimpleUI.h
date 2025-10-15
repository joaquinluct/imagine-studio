#pragma once

#include "IUI.h"

namespace UI {

class SimpleUI : public IUI {
public:
    SimpleUI() = default;
    ~SimpleUI() override = default;

    void Initialize() override;
    void Shutdown() override;
    void Draw() override;
};

} // namespace UI
