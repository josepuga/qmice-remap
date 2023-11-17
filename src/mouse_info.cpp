#include "mouse_info.hpp"
#include <format>
#include <algorithm>
#include <ranges>

std::string MouseInfo::getEvdevInput() const
{
    //FIXME: Bad code
    auto bus {busType};
    auto ven {vendor};
    auto pro {product};
    auto ver {version};
    //WTF: Numbers sould be in uppercase...
    std::transform(bus.begin(), bus.end(), bus.begin(), ::toupper);
    std::transform(ven.begin(), ven.end(), ven.begin(), ::toupper);
    std::transform(pro.begin(), pro.end(), pro.begin(), ::toupper);
    std::transform(ver.begin(), ver.end(), ver.begin(), ::toupper);

    return std::format("evdev:input:b{}v{}p{}e{}*", bus, ven, pro, ver);
}

void MouseInfo::addButton(ButtonInfo btn)
{
    // At start, current = default
    defaultButtons.push_back(btn);
    currentButtons.push_back(btn);

}
