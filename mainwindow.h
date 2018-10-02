#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "patterngenerator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void on_randomGenerate_action_triggered();
    
private:
    Ui::MainWindow *ui;
    
    int maxUnitSize;
    
    QPushButton *unitsButtons[10];
    
    void showMainFrame();
    void showUnitsButtons();
    void rotateUnits();
    
    cv::Mat mainFrame;
    std::vector<cv::Point> dstPattern;
    std::vector<std::vector<cv::Point>> unitPatterns;
    
};

#endif // MAINWINDOW_H
