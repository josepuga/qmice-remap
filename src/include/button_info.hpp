#ifndef B475D76B_2132_44D3_AE48_155FEE6A9C16
#define B475D76B_2132_44D3_AE48_155FEE6A9C16

#include <string>

class ButtonInfo {
public:
    ButtonInfo(const std::string& keyboardKey, const std::string& actionTag);
    ButtonInfo(const std::string& keyboardKey, const int actionNumber);

    void setKeyboardKey(const std::string& keyboardKey);
    //setActionTag updates m_actionNumber, and setActionNumber up. m_actionTag
    void setActionTag(const std::string& actionTag);
    void setActionNumber(const int actionNumber);

    std::string getKeyboardKey() const;
    std::string getActionTag() const;
    int getActionNumber() const;

    /*
    keyboardKey: Keyboard Key showed with edev (90000x)
    actionTag: The action in string "BTN_LEFT", "BTN_CENTER", and so on...
    actionNumber: The number that Linux associated with actionTag
*/
private:
    std::string m_keyboardKey {};
    std::string m_actionTag {"BTN_NONE"};
    int m_actionNumber {};

};

#endif /* B475D76B_2132_44D3_AE48_155FEE6A9C16 */
