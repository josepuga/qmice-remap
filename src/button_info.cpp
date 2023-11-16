#include <algorithm>
#include <ranges>
#include <map>

#include "button_info.hpp"
#include "buttons_tags.hpp"

ButtonInfo::ButtonInfo(const std::string& keyboardKey, const std::string& actionTag) 
{
    setKeyboardKey(keyboardKey);
    setActionTag(actionTag);
}

ButtonInfo::ButtonInfo(const std::string& keyboardKey, const int actionNumber) 
{
    setKeyboardKey(keyboardKey);
    setActionNumber(actionNumber);
}

void ButtonInfo::setKeyboardKey(const std::string &keyboardKey)
{
    m_keyboardKey = keyboardKey;
}

void ButtonInfo::setActionTag(const std::string &actionTag)
{
    using buttons_tags::tags;
    m_actionTag = actionTag;
    m_actionNumber = 0x0;
    if (tags.contains(actionTag)) {
        m_actionNumber = tags.at(actionTag);
    }
}

void ButtonInfo::setActionNumber(const int actionNumber)
{
    using buttons_tags::tags;
    m_actionNumber = actionNumber;
    m_actionTag = "BTN_NONE";
    auto finded = std::ranges::find_if(tags, [actionNumber](const auto& t) {
        return t.second == actionNumber;});
    if (finded != std::end(tags)) {
        m_actionTag = finded->first;
    }
}

std::string ButtonInfo::getKeyboardKey() const
{
    return m_keyboardKey;
}

std::string ButtonInfo::getActionTag() const
{
    return m_actionTag;
}

int ButtonInfo::getActionNumber() const
{
    return m_actionNumber;
}
