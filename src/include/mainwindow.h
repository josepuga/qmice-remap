#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mouse_info.hpp"

#include <vector>

#include <QMainWindow>
#include <QLabel>
#include <QAbstractButton>
#include <QComboBox>
#include <QRadioButton>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    
    void updateImage(int index);
    void setMouseItems(std::vector<MouseInfo>&);
    std::vector<MouseInfo>& getMouseItems();

    // Reusing code form main.cpp. This was done before the GUI.
    std::function<std::stringstream(const MouseInfo&)> funcGetHwdbFileContent;
    std::function<std::string(const MouseInfo&)> funcGetHwdbFileName;

private slots:
    void on_comboBoxName_currentIndexChanged(int index);
    void on_comboBoxBtn_highlighted(QAbstractButton*);
    void on_comboBoxBtn_currentIndexChanged(int index);


private:
    // To enable/disable/change some widgets at change selected mouse...
    // I know. Ugly name.
    struct Control {
        Control(QLabel* num, QLabel* lab, QAbstractButton* rad, QComboBox* com) :
            labelNumber(num), labelDefault(lab), radioCurrent(rad), comboCurrent(com) {}
        QLabel *labelNumber {};
        QLabel *labelDefault {};
        QAbstractButton *radioCurrent {};
        QComboBox *comboCurrent {};
    };

    Ui::MainWindow *ui;
    std::vector<MouseInfo> m_miItems {};
    const size_t MAX_BUTTONS = 9;
    std::vector<Control> m_controlItems {};
    std::vector<QString> m_tagsItems {};

    // Update all witgets (changing combo Mouse Name)
    void updateControls(int index);
    // set buttons current values to default values.
    void setButtonsToDefault(int index);
    // Called from updateControls (updates mouse buttons on GUI)
    void updateButtonsWidgets(int index);

    // Buttons tags used for remap. Some of the full list.
    void setButtonTagsItems();
    int getIndexByTag(const QString&);

    // Default value for Button Tag
    void setComboBoxBtnDefaultValue(int index);

    // Apply changes to system.
    void applyChanges();

};
#endif // MAINWINDOW_H
