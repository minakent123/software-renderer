#pragma once

namespace core {

class Surface;

class SoftwareRenderer
{
public:
    void Render(Surface& target, double timeSeconds) const;
};

}  // namespace core
