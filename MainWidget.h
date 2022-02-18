#ifndef J2534SHARK_MAINWIDGET_H
#define J2534SHARK_MAINWIDGET_H

#include <QWidget>

namespace jshark::forms {
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWidget; }
    QT_END_NAMESPACE

    class MainWidget : public QWidget {
    Q_OBJECT

    public:
        explicit MainWidget(QWidget *parent = nullptr);

        ~MainWidget() override;

    private:
        Ui::MainWidget *ui;

        QString filePath;
        int selectedDevice = -1;

    private slots:
        void selectedDeviceChanged(int idx);
        void selectOutputFile();
        void install();
    };
} // jshark::forms

#endif //J2534SHARK_MAINWIDGET_H
