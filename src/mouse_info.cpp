#include "mouse_info.hpp"
#include <format>

std::string MouseInfo::getEvdevInput() const
{
    return std::format("evdev:input:b{}v{}p{}e{}", busType, vendor, product, version);
}

void MouseInfo::addButton(ButtonInfo btn)
{
    // At start, current = default
    defaultButtons.push_back(btn);
    currentButtons.push_back(btn);

}
