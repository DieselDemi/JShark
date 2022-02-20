#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <iostream>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

namespace jshark::forms {
    MainWidget::MainWidget(QWidget *parent) :
            QWidget(parent), ui(new Ui::MainWidget) {
        ui->setupUi(this);

        try {
            const QString PASSTHRU_REG_PATH = R"(HKEY_LOCAL_MACHINE\SOFTWARE\PassThruSupport.04.04)";

            QSettings settings(PASSTHRU_REG_PATH, QSettings::NativeFormat);

            if (settings.childGroups().empty())
                this->ui->outputTextEdit->setPlainText("No devices found");

            for (const auto &deviceLibrary: settings.childGroups()) {
                if (settings.value("Name").toString() == "JSHARK Shim")
                    continue;

                settings.beginGroup(deviceLibrary);
                this->ui->deviceComboBox->addItem(settings.value("Name", "").toString(),
                                                  settings.value("FunctionLibrary", ""));
                settings.endGroup();
            }

        } catch (std::exception &e) {
            std::cout << "An error has occurred" << std::endl << std::flush;
        }

        connect(ui->deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                &MainWidget::selectedDeviceChanged);
        connect(ui->selectOutputFileButton, &QPushButton::released, this, &MainWidget::selectOutputFile);
        connect(ui->installButton, &QPushButton::released, this, &MainWidget::install);
        connect(ui->removeJSharkButton, &QPushButton::released, this, &MainWidget::uninstall);
    }

    MainWidget::~MainWidget() {
        delete ui;
    }

    void MainWidget::selectedDeviceChanged(int idx) {
        selectedDevice = idx;
    }

    void MainWidget::selectOutputFile() {
        this->filePath = QFileDialog::getSaveFileName(this, "/home", tr("Log Files (*.log)"));
    }

    void MainWidget::install() {
        QMessageBox msgBox;

        if (this->filePath.isEmpty()) {
            msgBox.setText("Select a save file path first");
            msgBox.exec();
            return;
        }
        if (selectedDevice == -1) {
            msgBox.setText("Select a PassThru adapter first");
            msgBox.exec();
            return;
        }

        try {
            if(QFile::exists("C:\\JSHARK.dll"))
            {
                if(QFile::remove("C:\\JSHARK.dll")) {
                    if(!QFile::copy("./JSHARK.dll", "C:\\JSHARK.dll")) {
                        msgBox.setText("Could not copy dll to the C:\\ drive... aborting.");
                        msgBox.exec();
                        return;
                    }
                } else  {
                    msgBox.setText("Could not delete old JSHARK.dll file, please ensure it exists");
                    msgBox.exec();
                }
            } else {
                if(!QFile::copy("./JSHARK.dll", "C:\\JSHARK.dll")) {
                    msgBox.setText("Could not copy dll to the C:\\ drive... aborting.");
                    msgBox.exec();
                    return;
                }
            }


            const QString JSHARK_REG_PATH = R"(HKEY_LOCAL_MACHINE\SOFTWARE\PassThruSupport.04.04)";
            QSettings settings(JSHARK_REG_PATH, QSettings::NativeFormat);

            QString functionLibrary = this->ui->deviceComboBox->currentData().toString();

            settings.beginGroup("JSHARK");
            settings.setValue("CAN", 0x1);
            settings.setValue("ConfigApplication", "JSharkConfigurator.exe");
            settings.setValue("DeviceId", 0x0);
            settings.setValue("DllPath", functionLibrary);
            settings.setValue("FunctionLibrary", "C:\\JSHARK.dll"); //TODO
            settings.setValue("IOS14230", 0x1);
            settings.setValue("ISO14230_CH1", 0x1);
            settings.setValue("ISO14230_CH2", 0x1);
            settings.setValue("ISO14230_CH3", 0x1);
            settings.setValue("ISO15765", 0x1);
            settings.setValue("ISO9141", 0x1);
            settings.setValue("ISO9141_CH1", 0x1);
            settings.setValue("ISO9141_CH2", 0x1);
            settings.setValue("ISO9141_CH3", 0x1);
            settings.setValue("J1850PWM", 0x1);
            settings.setValue("J1850VPW", 0x1);
            settings.setValue("LogPath", filePath);
            settings.setValue("Name", "JSHARK Shim");
            settings.setValue("Vendor", "Diesel Demi");
            settings.endGroup();

        } catch (std::exception &e) {
            msgBox.setText("Something went wrong");
            msgBox.exec();
        }
    }

    void MainWidget::uninstall() {
        if(!QFile::remove("C:\\JSHARK.dll")) {
            QMessageBox msgBox;
            msgBox.setText("Could not remove dll from the C:\\ drive, will still uninstall registry values.");
            msgBox.exec();
        }

        const QString JSHARK_REG_PATH = R"(HKEY_LOCAL_MACHINE\SOFTWARE\PassThruSupport.04.04)";
        QSettings settings(JSHARK_REG_PATH, QSettings::NativeFormat);

        settings.remove("JSHARK");
    }

} // jshark::forms
