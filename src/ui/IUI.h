#pragma once

namespace UI {

class IUI {
public:
    virtual ~IUI() {}
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual void Draw() = 0;
};

} // namespace UI
