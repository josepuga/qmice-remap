#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <iostream>
#include <format>
#include <vector>
#include <filesystem>
#include <sstream>
#include <cstdlib>
#include <ranges>
#include <algorithm>

#include <unistd.h>
#include <fmt/core.h>

#include "version.hpp"
#include "ini.hpp"
#include "mouse_info.hpp"

using namespace std;
namespace fs = std::filesystem;

constexpr auto profilesRelativePath {".config/qmice-remap/profiles"};
constexpr auto hwdbPath {"/etc/udev/hwdb.d"};
constexpr auto hwdbPrefix {"90-"};
constexpr auto hwdbExtension {".hwdb"};

vector<MouseInfo> loadAllMouseInfo(const string& path);
void setCurrentButtons(MouseInfo&);
string getHwdbFileName(const MouseInfo&);
stringstream getHwdbFileContent(const MouseInfo&);
void test(const vector<MouseInfo>&);

//void displayUI_OLD(vector<MouseInfo>&);
void displayUI(int argc, char* argv[], vector<MouseInfo>&);

int main(int argc, char* argv[]) {

    // Check Wayland session...
    //FIXME: With sudo XDG_SESSION_TYPE is NULL.

    const auto sessionType {getenv("XDG_SESSION_TYPE")};
    if (sessionType == NULL || strcmp(sessionType, "wayland") != 0) {
        cerr << "ERROR: This app requires a Wayland session.\n";
        return EXIT_FAILURE;
    }

    // Load all profiles data...
    const string profilesPath {fmt::format("{}/{}", getenv("HOME"), profilesRelativePath)};
    auto miItems = loadAllMouseInfo(profilesPath);
    if (miItems.empty()) {
        fmt::println(stderr, "ERROR: Unable to load profile files or not profile files found.");
        fmt::println(stderr, "Copy your profile files on '{}' and run again.", profilesPath);
        return EXIT_FAILURE;
    }

    // TEST!
    const vector<string> args(argv+1, argv+argc);
    if (!args.empty()) {
        if (args.at(0) == "--test") {
            test(miItems);
            return EXIT_SUCCESS;
        }
    }

    // Launch the GUI...
    displayUI(argc, argv, miItems);
    return EXIT_SUCCESS;
}

void test(const vector<MouseInfo>& miItems) {
    cout << APP_NAME << " " << VERSION_STRING << " - TEST REPORT.\n";
    fmt::println("Profiles path  : {}/{}", getenv("HOME"), profilesRelativePath);
    fmt::println("Profiles found : {}", miItems.size());
    const string sep(80, '=');
    for (const auto& mi: miItems) {
        fmt::println("{}\n", sep);
        fmt::println("Mouse Name  : {}", mi.name);
        fmt::println("Uploaded by : {}", mi.uploader);
        fmt::println("Comments    : {}", mi.comments);
        fmt::println("Bus-Vendor-Product-Version: {}-{}-{}-{}",
                     mi.busType, mi.vendor, mi.product, mi.version);
        fmt::println("Buttons count : {}. Keyboard Key - Action Tag - Action Number",
                     mi.defaultButtons.size());
        //TODO: currentButtons.
        for (const auto& btn : mi.defaultButtons) {
            fmt::println("  {}-{}-{}", btn.getKeyboardKey(), btn.getActionTag(), btn.getActionNumber());
        }
        // -------
        //FIXME: Esto no tiene mucha utilidad, ya que si se usa sudo, el $HOME
        //------ es el de root y no se aceden a los profiles.
        // --------
        // Check sudo. (write permissions of hwdb files)
        const string canWrite = (access(hwdbPath, F_OK | R_OK | W_OK) == -1) ?  "NO" : "YES";
        fmt::println("Output file name: {}\nWritable : {}\nFile content:", getHwdbFileName(mi), canWrite);
        cout << getHwdbFileContent(mi).str();
    }

}

vector<MouseInfo> loadAllMouseInfo(const string &path)
{
    vector<MouseInfo> result {};
    for (const auto& entry : fs::directory_iterator(path)) {
        Ini ini;
        if (! ini.LoadFromFile(entry.path())) {
            return result;
        }
        MouseInfo mi;
        mi.name = ini.GetValue<string>("info", "name", "");
        if (mi.name.empty()) {
            fmt::println(stderr, "ERROR: name not found for device.");
            continue;
        }
        mi.uploader = ini.GetValue<string>("info", "uploader", "");
        mi.comments = ini.GetValue<string>("info", "comments", "");
        mi.busType = ini.GetValue<string>("id", "bustype", "0000");
        mi.product= ini.GetValue<string>("id", "product", "0000");
        mi.vendor = ini.GetValue<string>("id", "vendor", "0000");
        mi.version = ini.GetValue<string>("id", "version", "0000");
        mi.userData = ini.GetValue<string>("gui", "image", "");
        for (const auto& key : ini.GetSectionKeys("default buttons")) {
            auto btnActionTag = ini.GetValue<string>("default buttons", key);
            mi.addButton(ButtonInfo(key, btnActionTag));
        }
        // from /etc/udev/hwdb.d/
        setCurrentButtons(mi);
        result.push_back(mi);
    }
    return result;
}

void setCurrentButtons(MouseInfo& mi) {
    Ini ini;
    if (!ini.LoadFromFile(getHwdbFileName(mi))) {
        return;
    }
    size_t index {};
    for (const auto& key : ini.GetSectionKeys("")) {
        if (key.rfind("KEYBOARD_KEY_", 0) != 0) {
            continue;
        }
        auto keyboardKey {key};
        keyboardKey.erase(0, size("KEYBOARD_KEY_")-1);
        auto actionNumber {ini.GetValue<int>("", key)};
        mi.currentButtons.at(index).setKeyboardKey(keyboardKey);
        mi.currentButtons.at(index).setActionNumber(actionNumber);
        index++;
    }
}

stringstream getHwdbFileContent(const MouseInfo& mi) {
    stringstream result {};
    result << fmt::format("# Autogenerated with {} {}.\n# Mouse: {}\n# Profile created by: {}\n# Comments: {}\n\n",
                          APP_NAME, VERSION_STRING, mi.name, mi.uploader, mi.comments);
    result << mi.getEvdevInput() << '\n';
    result << "ID_INPUT_KEY=1\n";
    for (const auto& btn: mi.currentButtons) {
        result << "KEYBOARD_KEY_" << btn.getKeyboardKey() << "=" << btn.getActionNumber() << '\n';
    }
    return result;
}
string getHwdbFileName(const MouseInfo& mi)
{
    return fmt::format("{}/{}{}-{}-{}-{}{}",
                       hwdbPath, hwdbPrefix, mi.busType, mi.vendor, mi.product, mi.version, hwdbExtension);
}




void displayUI(int argc, char* argv[], vector<MouseInfo>& miItems)
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_Use96Dpi);
    QApplication::setWindowIcon(QIcon(":/images/icon")); //TODO: Does *NOT* work
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qmice-remap_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.funcGetHwdbFileContent = getHwdbFileContent;
    w.funcGetHwdbFileName = getHwdbFileName;
    w.setMouseItems(miItems); //FIXME: Use pointer!
    w.show();
    a.exec();
}
