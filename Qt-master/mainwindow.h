#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDateTime>
#include <QChartView>
#include <QList>
#include <QLineSeries>      /* 折现图 */
#include <QSplineSeries>    /* 曲线图 */
#include <QScatterSeries>   /* 点图 */
#include <QValueAxis>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    /**<  当定时器触发时，应用程序会发送一个QTimerEvent  */
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private slots:
    void on_OpenSerialButton_clicked();

    void ReadData();

    void on_SendButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

    /**
     * 接收到数据源发送来的数据，数据源可以下位机，采集卡，传感器等。
     */
    void dataReceived(int value);


    int maxX;                          /**< 设置x,y 的坐标最大打印点 */
    int maxY;
    int timerId;                       /**< 定时器ID 句柄 */
    int maxSize;                       /**< data 最多存储 maxSize 个元素 */

    QList<double>     data;            /**< 存储业务数据的 list */
    QChart           *chart;
    QChartView       *chartView;
    QSplineSeries    *splineSeries;
    QScatterSeries   *scatterSeries;

};

#endif // MAINWINDOW_H
