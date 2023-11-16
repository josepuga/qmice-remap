#ifndef AC2D47A8_13DC_4637_8705_293C4FD41E34
#define AC2D47A8_13DC_4637_8705_293C4FD41E34

#include <vector>
#include <string>
#include <map>

#include "button_info.hpp"

struct MouseInfo {
public:
    std::string name {};
    std::string uploader {};
    std::string comments {};
    std::string busType {"0000"};
    std::string product {"0000"};
    std::string vendor {"0000"};
    std::string version {"0000"};
    // For any data. In this case will be use for a png encode64
    std::string userData {};
    std::vector<ButtonInfo> defaultButtons{};
    std::vector<ButtonInfo> currentButtons{};

    std::string getEvdevInput() const;
    void addButton(ButtonInfo btn);
};


#endif /* AC2D47A8_13DC_4637_8705_293C4FD41E34 */
