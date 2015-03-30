#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    rsrcPath = ":/picture";
    QPixmap pixmap(rsrcPath+"/fileopen.png");
    QIcon   openIcon(pixmap);
    ui->openButton->setIcon(openIcon);

    connect(ui->CloseButton,SIGNAL(released()),this,SLOT(close()));//close widget

    sensor_sel=SENSOR_NONE;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_released()
{
    QMessageBox msgbox1;
    QFile   sensor_data_file(fn);
    QFile   out_hi_file("out_hi.txt");
    QFile   out_lo_file("out_lo.txt");
    QFile   out_data_file("out_data.txt");
    QFile   out_num_file("out_num.txt");
    QFile   out_count_file("out_count.txt");

    int cnt=0,total_item=0,total_data=0;

    if(!sensor_data_file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    if(!out_hi_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    if(!out_lo_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    if(!out_data_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    if(!out_num_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    if(!out_count_file.open(QIODevice::WriteOnly |QIODevice::Text))
        return;

     QTextStream intext(&sensor_data_file);
     QTextStream hi_text(&out_hi_file);
     QTextStream lo_text(&out_lo_file);
     QTextStream data_text(&out_data_file);
     QTextStream num_text(&out_num_file);
     QTextStream count_text(&out_count_file);

    switch (sensor_sel) {
    case APTINA_TYPE:
        while(!intext.atEnd())
        {
            QString line = intext.readLine();

            total_item++;

            hi_text << "0x" << line[7] << line[8];
            hi_text << ", " ;
            lo_text << "0x" << line[9] << line[10];
            lo_text << ", " ;

            if(line[17] == 0x20)
            {
                total_data += 1;
                data_text << "0x" << line[15] << line[16];
                data_text << ", ";

                num_text << "0x01";
                num_text << ", ";
            }
            else
            {
                total_data += 2;
                data_text << "0x" << line[15] << line[16];
                data_text << ", ";
                data_text << "0x" << line[17] << line[18];
                data_text << ", ";

                num_text << "0x02";
                num_text << ", ";
            }

            if(total_data % 10 == 0)
                data_text << "\n";

            cnt++;
            if(cnt == 10)
            {
                hi_text << "\n" ;
                lo_text << "\n" ;
                num_text << "\n" ;
                cnt = 0;
            }
        }
        count_text << "hi_lo byte count total number are " << total_item << "\n";
        count_text << "data byte count total number are " << total_data;

        msgbox1.setText("aptina parsering finish");
        msgbox1.exec();
        break;
    case OV_TYPE:
        while(!intext.atEnd())
        {
            QString line = intext.readLine();

            total_item++;

            hi_text << "0x" << line[3] << line[4];
            hi_text << ", " ;
            lo_text << "0x" << line[5] << line[6];
            lo_text << ", " ;

            total_data += 1;
            data_text << "0x" << line[8] << line[9];
            data_text << ", ";

            num_text << "0x01";
            num_text << ", ";

            if(total_data % 10 == 0)
                data_text << "\n";

            cnt++;
            if(cnt == 10)
            {
                hi_text << "\n" ;
                lo_text << "\n" ;
                num_text << "\n" ;
                cnt = 0;
            }
        }
        count_text << "hi_lo byte count total number are " << total_item << "\n";
        count_text << "data byte count total number are " << total_data;

        msgbox1.setText("omnivision parsering finish");
        msgbox1.exec();
        break;
    default:
        msgbox1.setText("parsering fail !");
        msgbox1.exec();
        break;
    }
    ui->sensor_sel->setEnabled(false);
    ui->sensor_sel->setChecked(false);

    ui->AptinaButton->setChecked(false);
    ui->OvButton->setChecked(false);
    ui->pushButton->setEnabled(false);
}

void Widget::on_CloseButton_released()
{

}

void Widget::on_AptinaButton_clicked(bool checked)
{
    if(checked == true)
    {
        QFile   sensor_data_file(fn);
        if(!sensor_data_file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream intext(&sensor_data_file);
        QString line = intext.readLine();

        if((line[0]=='R') && (line[1]=='E') && (line[2]=='G'))
        {
             sensor_sel = APTINA_TYPE;
              ui->pushButton->setEnabled(true);
        }
        else
        {
             QMessageBox msgbox1;
             msgbox1.setText("select error file !");
             msgbox1.exec();
             ui->pushButton->setEnabled(false);
        }
    }
}

void Widget::on_OvButton_clicked(bool checked)
{
    if(checked == true)
    {
        QFile   sensor_data_file(fn);
        if(!sensor_data_file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream intext(&sensor_data_file);
        QString line = intext.readLine();

        if(((line[0]>='0' && line[0]<='9')|(line[0]>='a' && line[0]<='f')|(line[0]>='A' && line[0]<='F')) &&
                (((line[1]>='0' && line[1]<='9')|(line[1]>='a' && line[1]<='f')|(line[1]>='A' && line[1]<='F'))))
        {
             sensor_sel = OV_TYPE;
             ui->pushButton->setEnabled(true);
        }
        else
        {
             QMessageBox msgbox1;
             msgbox1.setText("select error file !");
             msgbox1.exec();
             ui->pushButton->setEnabled(false);
        }
    }
}

void Widget::on_openButton_released()
{
    QMessageBox msgbox1;

    fn = QFileDialog::getOpenFileName(this, "Open File...",
                                              QString(), tr("Parameter file(*.txt);;All Files(*)"));
    if(!fn.isEmpty())
    {
        ui->sensor_sel->setEnabled(true);
        ui->AptinaButton->setEnabled(true);
        ui->OvButton->setEnabled(true);
    }
}
/*
void Widget::on_sensor_sel_clicked(bool checked)
{
    if(checked == true)
    {
        ui->AptinaButton->setEnabled(true);
        ui->OvButton->setEnabled(true);
    }
}
*/
