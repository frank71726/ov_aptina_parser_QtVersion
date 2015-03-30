#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

enum SensorType
{
    SENSOR_NONE,
    APTINA_TYPE,
    OV_TYPE
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_released();

    void on_CloseButton_released();

    void on_AptinaButton_clicked(bool checked);

    void on_OvButton_clicked(bool checked);

    void on_openButton_released();

private:
    Ui::Widget *ui;
    QString fn;
    SensorType sensor_sel;
    QString rsrcPath;
};

#endif // WIDGET_H
