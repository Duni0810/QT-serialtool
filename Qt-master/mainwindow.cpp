/*****************************************************************************************
 * brief  : 该程序为简单的串口数据实时打点绘图，可以用来做PID 调试用的工具 蓝牙小车，电机闭环控制等；
 *
 * note   : 0.这里使用的QT是5.7，建议使用5.7以上版本打开工程。
 *          1.该程序没有用到多线程知识，比较容易看懂，但是带来负面影响就是实时性不够，可能有延时2~1000ms。
 *          2.串口能接受所有数据，但是并不是所有数据都可以绘图，例如字母等：该程序的串口发送的数据为
 *          ：“data:***”, *** 就是要绘制的点，这里要注意绘图区间问题。
 *          3.下面有写一个随机生成数据的函数，用定时器调用，每100ms 调用一次定时器服务函数，
 *          timerId = startTimer(100);
 *          这里默认屏蔽这个函数内容，拿到函数可以关闭屏蔽先测试一下效果。
 *          4.目前这个函数的串口检测只执行一次，所以测试时候先插上串口线，然后测试。
 *          5.当你有看不懂的地方，想想：百度是个好东西。
 *
 *          这里留了一些待升级的地方，使应用更人性化，怎么升级？3分看水平，7分天注定咯
 *
 * updata:  1.如何做到串口插上后就能自动检测到？
 *          写一个定时器，将参数初始化表中的检测串口函数放入定时器服务函数中。
 *          2.如何提高实时性？
 *          有钱加固态，没钱改线程。
 *          3.如何让这么丑的界面变的人性化？
 *          修改ui属性和控件属性。
 *          4.如何让界面自适应窗体大小？
 *          在ui界面中学会用layout控件。
 *          5.如何在同一个区域绘多通道曲线图？
 *          用多线程人多几个绘图点，记得添加不同颜色区分即可；如何你电脑很牛逼，可以用单线程。
 * **************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QHBoxLayout>
#include <QDebug>
#include <stdio.h>
#include <string.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //查找可用的串口
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite)) {
            ui->PortBox->addItem(serial.portName());
            ui->SendButton->setEnabled(false);
            serial.close();
        }
    }

    //设置波特率下拉菜单默认显示第0项
    ui->BaudBox->setCurrentIndex(0);

       maxSize = 60;                           /* 只存储最新的 60 个数据 */
       maxX    = 500;                          /* 设置x最大打点位置 */
       maxY    = 20;                           /* 设置y最大打点位置 */
       splineSeries  = new QSplineSeries();    /* 新建曲线图句柄 */
       scatterSeries = new QScatterSeries();   /* 新建点图句柄 */
       scatterSeries->setMarkerSize(8);        /* 打点大小 */
       chart = new QChart();                   /* 新建图标句柄 */
       chart->addSeries(splineSeries);
       chart->addSeries(scatterSeries);
       chart->legend()->hide();
       chart->setTitle("实时动态曲线");
       chart->createDefaultAxes();
       chart->axisX()->setRange(0, 500);        /* 设置X Y 坐标 范围 */
       chart->axisY()->setRange(0, 20);
       chart->axisY()->setTitleText("r/s");     /* Y名字 */
       chartView = new QChartView(chart);       /* 图表显示 */
       chartView->setRenderHint(QPainter::Antialiasing);
       QHBoxLayout *layout = new QHBoxLayout();
       layout->setContentsMargins(0, 0, 0, 0);  /* 设置边缘 */
       layout->addWidget(chartView);            /* 在widget 中显示图表 */
       ui->widget->setLayout(layout);
       timerId = startTimer(100);
      // qsrand(QDateTime::currentDateTime().toTime_t());
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 *  \brief 定时器服务函数
 */
void MainWindow::timerEvent(QTimerEvent *event) {
    if (event->timerId() == timerId) {
        /* 下面两个函数是随机生成数据  画图  测试函数 */
       //   int newData = qrand() % (maxY + 1);
       //    dataReceived(newData);
       }
}

/**
 * \brief 画图函数
 * \param value
 */
void MainWindow::dataReceived(int value) {
    data << value;

    /* 数据个数超过了最大数量，则删除最先接收到的数据，实现曲线向前移动 */
    while (data.size() > maxSize) {
        data.removeFirst();
    }

    /* 界面被隐藏后就没有必要绘制数据的曲线了 */
    if (isVisible()) {
        splineSeries->clear();
        scatterSeries->clear();

        int dx = maxX / (maxSize-1);
        int less = maxSize - data.size();
        for (int i = 0; i < data.size(); ++i) {
            splineSeries->append( less * dx + i * dx, data.at(i)); /* 曲线 */
            scatterSeries->append(less * dx + i * dx, data.at(i)); /* 绘点 */
        }
    }
}

/**
 * \brief 串口连接控件
 */
void MainWindow::on_OpenSerialButton_clicked()
{
    if(ui->OpenSerialButton->text() == tr("打开串口")) {
        serial = new QSerialPort;

        if(ui->PortBox->currentText() == "") {
            return;
        }

        /* 设置串口名 */
        serial->setPortName(ui->PortBox->currentText());

        /* 打开串口 */
        serial->open(QIODevice::ReadWrite);

        /* 设置波特率 */
        serial->setBaudRate(ui->BaudBox->currentText().toInt());

        /* 设置数据位数 */
        switch (ui->BitBox->currentIndex()) {
        case 8:
            serial->setDataBits(QSerialPort::Data8);
            break;
        default:
            break;
        }

        /* 设置校验位 */
        switch (ui->ParityBox->currentIndex()) {
        case 0:
            serial->setParity(QSerialPort::NoParity);
            break;
        default:
            break;
        }

        /* 设置停止位 */
        switch (ui->BitBox->currentIndex())
        {
        case 1:
            serial->setStopBits(QSerialPort::OneStop);
            break;
        case 2:
            serial->setStopBits(QSerialPort::TwoStop);
        default:
            break;
        }

        /* 设置流控制 */
        serial->setFlowControl(QSerialPort::NoFlowControl);

        /* 关闭设置菜单使能 */
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->BitBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->StopBox->setEnabled(false);
        ui->SendButton->setEnabled(true);
        ui->OpenSerialButton->setText(tr("关闭串口"));

        /* 连接信号槽 */
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::ReadData);

    } else {
        /* 关闭串口 */
        serial->clear();
        serial->close();
        serial->deleteLater();

        /* 恢复设置使能 */
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->SendButton->setEnabled(false);
        ui->OpenSerialButton->setText(tr("打开串口"));
    }
}
/**
 *  \brief 读取串口接收到的信息
 */
void MainWindow::ReadData()
{
    QByteArray   buf;
    char        *p_buff       = NULL;
    static int   clock        = 0;
    static int   receive_data = 0;

    buf = serial->readAll();
    if(!buf.isEmpty()) {
        QString str = ui->textEdit->toPlainText();
        str += tr(buf);
        p_buff = buf.data();

        static char head[32] = {0};
        memset(head, 0, sizeof(head));
        strcpy(head, strtok(p_buff, ":"));

        if(strcmp("data",head) != 0) {
            return ;
        }
        receive_data = atoi(strtok(NULL, ":"));
#if 0
        qDebug() << p_buff << '\n';
        dataReceived(receive_data);
#endif
        dataReceived(receive_data);
        ui->textEdit->clear();
        ui->textEdit->append(str);
        clock++;
        if (clock >= 255) {
            clock = 0;
            str.clear();
            qDebug() << str;
            ui->textEdit->clear();
        }
    }
    buf.clear();
}

/**
 * \brief  发送按钮槽函数
 */
void MainWindow::on_SendButton_clicked()
{
     QByteArray   buf;

     buf = ui->textEdit_2->toPlainText().toLatin1();
     buf.append("\r\n");
    serial->write(buf);
    qDebug() << buf;
}

/**
 * \brief 清文本框buf
 */
void MainWindow::on_pushButton_clicked()
{
    ui->textEdit->clear();
}

/**
 * \brief 清文本框buf
 */
void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit_2->clear();
}

