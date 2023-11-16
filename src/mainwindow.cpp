#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "ini.hpp"
#include "buttons_tags.hpp"
#include "version.hpp"

#include <QShortcut>
#include <QProcess>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QDesktopServices>
#include <QUrl>

#include <iostream>

#include <fmt/core.h>

constexpr auto configRelativePath {".config/qmice-remap"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //TODO: How to do this in a loop?.
    m_controlItems.reserve(MAX_BUTTONS);
    m_controlItems.push_back(Control(ui->label_1, ui->labelDefault1, ui->radioButtonBtn1, ui->comboBoxBtn1));
    m_controlItems.push_back(Control(ui->label_2, ui->labelDefault2, ui->radioButtonBtn2, ui->comboBoxBtn2));
    m_controlItems.push_back(Control(ui->label_3, ui->labelDefault3, ui->radioButtonBtn3, ui->comboBoxBtn3));
    m_controlItems.push_back(Control(ui->label_4, ui->labelDefault4, ui->radioButtonBtn4, ui->comboBoxBtn4));
    m_controlItems.push_back(Control(ui->label_5, ui->labelDefault5, ui->radioButtonBtn5, ui->comboBoxBtn5));
    m_controlItems.push_back(Control(ui->label_6, ui->labelDefault6, ui->radioButtonBtn6, ui->comboBoxBtn6));
    m_controlItems.push_back(Control(ui->label_7, ui->labelDefault7, ui->radioButtonBtn7, ui->comboBoxBtn7));
    m_controlItems.push_back(Control(ui->label_8, ui->labelDefault8, ui->radioButtonBtn8, ui->comboBoxBtn8));
    m_controlItems.push_back(Control(ui->label_9, ui->labelDefault9, ui->radioButtonBtn9, ui->comboBoxBtn9));

    // Slots
    // comboBoxName Already done in design mode:
    //connect(ui->comboBoxName, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBoxName_currentIndexChanged(int)));

    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->pushButtonApply, &QPushButton::clicked, [&](){applyChanges();});
    connect(ui->pushButtonSetDefaults, &QPushButton::clicked,
            [&]() {
                setButtonsToDefault(ui->comboBoxName->currentIndex());
                updateControls(ui->comboBoxName->currentIndex());
    });
    for (auto& ci : m_controlItems) {
        // Select nearest radio button.
        connect(ci.comboCurrent, &QComboBox::highlighted,
                [&]() {on_comboBoxBtn_highlighted(ci.radioCurrent);});
        // Updates currentButton on MouseItem
        connect(ci.comboCurrent, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBoxBtn_currentIndexChanged(int)));
    }

    // Shortcuts
    connect(new QShortcut(QKeySequence(Qt::ALT | Qt::Key_M), ui->centralwidget), SIGNAL(activated()),
            ui->comboBoxName, SLOT(setFocus()));

    // Button tags availables for remaps
    setButtonTagsItems();

    // Fill comboboxes with those tags
    for (auto& ci : m_controlItems) {
        int index {};
        for (auto& tag : m_tagsItems) {
            ci.comboCurrent->addItem(tag, index);
        }
    }

    // Menu bar
    auto helpMenu = ui->menubar->addMenu(tr("&Help"));
    auto actGithubPage = new QAction(tr("&Proyect page..."), this);
    connect(actGithubPage, &QAction::triggered, [&]()
        {
            QDesktopServices::openUrl(QUrl("https://github.com/josepuga/qmice-remap"));
        });
    helpMenu->addAction(actGithubPage);

    auto actAbout = new QAction(tr("A&bout..."), this);
    connect(actAbout, &QAction::triggered, [&]()
            {
                QMessageBox::information(this, VERSION_STRING, tr("(c) José Puga 2023. Under GPL3 License."));
            });
    helpMenu->addAction(actAbout);
    //ui->menubar->addMenu(tr("..."));

    this->setWindowTitle(QString::fromStdString(fmt::format("{} - {}",APP_NAME, VERSION_STRING)));
    ui->statusbar->showMessage(
        tr("You can use this app without mouse. Move with TAB or press ALT + underline word: ALT+M to change mouse model."));
}

// Update MouseInfo.currentButtons
void MainWindow::on_comboBoxBtn_currentIndexChanged(int index)
{
    // on_comboBoxBtn_currentIndexChanged([[maybe_unused]] int index) works,
    // but the app shows an annoying warning "No such slot...
    std::ignore = index;

    auto mouseIndex {ui->comboBoxName->currentIndex()};
    if (mouseIndex < 0) return;

    QComboBox *objSender = qobject_cast<QComboBox*>(sender());

    //What "order" does the combobox has?. I dont know if there is a way to do that on Qt.
    //Meanwhile, I'm using my controlItems vector.
    size_t order {};
    for (auto& ci : m_controlItems)  {
        if (ci.comboCurrent == objSender) break;
        order++;
    }
    auto actionTag {objSender->currentText().toStdString()};
    m_miItems.at(mouseIndex).currentButtons.at(order).setActionTag(actionTag);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBoxBtn_highlighted(QAbstractButton* rb)
{
    rb->setAutoExclusive(false);
    rb->setChecked(true);
    rb->setAutoExclusive(true);
}

void MainWindow::updateControls(int index) {

    size_t buttonsCount {m_miItems.at(index).defaultButtons.size()};
    // Hide unused.
    for (size_t i{}; i < MAX_BUTTONS; i++) {
        bool active = i < buttonsCount;
        //TEST GUI: active = true;
        m_controlItems.at(i).radioCurrent->setVisible(active);
        m_controlItems.at(i).comboCurrent->setVisible(active);
        m_controlItems.at(i).labelDefault->setVisible(active);
        m_controlItems.at(i).labelNumber->setVisible(active);
    }

    const MouseInfo *mi = &m_miItems.at(index);
    ui->labelIdVal->setText(QString::fromStdString(fmt::format("{} - {} - {} - {}",
                                                mi->busType, mi->vendor, mi->product, mi->version)));
    ui->labelUploadedByVal->setText(QString::fromStdString(mi->uploader));
    ui->labelCommentsVal->setText(QString::fromStdString(mi->comments));

    //NO NECESARIO AL INICIO: setButtonsToDefault(index);
    updateButtonsWidgets(index);
    updateImage(index);
}

void MainWindow::updateButtonsWidgets(int index) {
    const MouseInfo *mi = &m_miItems.at(index);
    for (size_t i{}; i < mi->currentButtons.size(); i++) {
        auto textCurrent = QString::fromStdString(mi->currentButtons.at(i).getActionTag());
        auto textDefault = QString::fromStdString(mi->defaultButtons.at(i).getActionTag());
        m_controlItems.at(i).labelDefault->setText(textDefault);
        m_controlItems.at(i).comboCurrent->setCurrentIndex(getIndexByTag(textCurrent));
    }
}

void MainWindow::setButtonsToDefault(int index) {
    MouseInfo *mi = &m_miItems.at(index);
    for (size_t i{}; i < mi->defaultButtons.size(); i++) {
        mi->currentButtons.at(i) = mi->defaultButtons.at(i); //TODO: create operator=
    }
    //First combo checked
    m_controlItems.at(0).radioCurrent->setAutoExclusive(false);
    m_controlItems.at(0).radioCurrent->setChecked(true);
    m_controlItems.at(0).radioCurrent->setAutoExclusive(true);
}

// Not too fast. But enough for me.
int MainWindow::getIndexByTag(const QString& tag) {
    for (size_t i{}; i < m_tagsItems.size(); i++) {
        if (m_tagsItems.at(i) == tag) {
            return i;
        }
    }
    return 0;
}

void MainWindow::applyChanges() {

    std::stringstream hwdbFileContent = funcGetHwdbFileContent(m_miItems.at(ui->comboBoxName->currentIndex()));
    std::string hwdbFileName = funcGetHwdbFileName(m_miItems.at(ui->comboBoxName->currentIndex()));
    QTemporaryFile tmpFile;
    tmpFile.open();
    Q_ASSERT(tmpFile.isOpen());
    auto tmpFileName = tmpFile.fileName().toStdString();
    QTextStream stream(&tmpFile);
    stream << QString::fromStdString(hwdbFileContent.str());
    stream.flush(); //Important!

    QProcess process;
    process.setEnvironment(QProcess::systemEnvironment());
    auto commands {QString::fromStdString(
        fmt::format("cp {} {} && udevadm control --reload-rules && udevadm trigger", tmpFileName, hwdbFileName))};
    process.start("pkexec", QStringList() << "bash" << "-c" << commands);
    process.waitForFinished();
    if (process.exitCode()) {
        QMessageBox::critical(this, APP_NAME, tr("Error applying layout."));
    }
    process.close();
}

void MainWindow::updateImage(int index) {
    // Convert from std::string to QByteArray... Seriously?
    QPixmap image;
    QString raw {QString::fromStdString(m_miItems.at(index).userData)};
    QByteArray qba {raw.toUtf8()};
    image.loadFromData(QByteArray::fromBase64(qba));
    ui->labelImage->setPixmap(image);
}

void MainWindow::setButtonTagsItems() {
    const auto configFile {fmt::format("{}/{}/{}",
        getenv("HOME"), configRelativePath, "qmice-remap.ini")}; //TODO: Not hardcoded.
    Ini ini;
    ini.LoadFromFile(configFile);
    auto availableTags {ini.GetSectionKeys("buttons")};

    for (auto tag: availableTags) {
        if (buttons_tags::tags.contains(tag)) {
            m_tagsItems.push_back(QString::fromStdString(tag));
        } else {
            // If not in list. Value is NONE
            m_tagsItems.push_back(QString::fromStdString(availableTags.at(0)));
        }
    }
}


void MainWindow::setMouseItems(std::vector<MouseInfo>& miItems) {
    m_miItems = miItems;
    for (const auto& mi : m_miItems) {
        ui->comboBoxName->addItem(QString::fromStdString(mi.name));
    }
}
std::vector<MouseInfo>& MainWindow::getMouseItems() {
    return m_miItems;
}

void MainWindow::on_comboBoxName_currentIndexChanged(int index = 0)
{
    // Changing the combo of Profiles, updates all widgets
    updateControls(index);
}

