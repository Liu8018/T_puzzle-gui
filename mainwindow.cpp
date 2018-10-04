#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    maxUnitSize = 10;
    
    unitsButtons[0] = ui->pushButton;
    unitsButtons[1] = ui->pushButton_2;
    unitsButtons[2] = ui->pushButton_3;
    unitsButtons[3] = ui->pushButton_4;
    unitsButtons[4] = ui->pushButton_5;
    unitsButtons[5] = ui->pushButton_6;
    unitsButtons[6] = ui->pushButton_7;
    unitsButtons[7] = ui->pushButton_8;
    unitsButtons[8] = ui->pushButton_9;
    unitsButtons[9] = ui->pushButton_10;
    
    //for(int i=0;i<10;i++)
    //    unitsButtons[i]->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_randomGenerate_action_triggered()
{
    //生成目标图案
    PatternGenerator generator;
    generator.getDstPattern(dstPattern);
    
    //重绘mainFrame
    mainFrame = cv::Mat(3000,3000,CV_8U,cv::Scalar(0));
    std::vector<std::vector<cv::Point>> tmpContours;
    tmpContours.push_back(dstPattern);
    cv::drawContours(mainFrame, tmpContours, 0, cv::Scalar(255), -1);
    
    //生成单元块
    generator.getUnitsPattern(unitPatterns);
    
    //test
    cv::Mat unitsImg(3000,3000,CV_8U,cv::Scalar(0));
    cv::drawContours(unitsImg,unitPatterns,-1,cv::Scalar(255),-1);
    cv::namedWindow("unitsImg",0);
    cv::resizeWindow("unitsImg",600,600);
    cv::imshow("unitsImg",unitsImg);
    
    //随机旋转
    rotateUnits();
    
    //显示单元块button
    showUnitsButtons();
    
    //显示mainFrame
    showMainFrame();
}

void mat2Qimg(const cv::Mat &image, QImage &qimg)
{
    if(image.channels() > 1)
    {
        std::cout<<"only gray image"<<std::endl;
        return;
    }
    
    qimg = QImage((const uchar*)image.data,image.cols,image.rows,image.cols*image.channels(),QImage::Format_Indexed8);
}

void MainWindow::showMainFrame()
{
    //改变图像大小使适应窗口
    mainFrame = 255 - mainFrame;
    cv::resize(mainFrame,mainFrame,cv::Size(ui->main_label->width()-2,ui->main_label->height()-2));

    //转换为QImage
    QImage qimg;
    mat2Qimg(mainFrame, qimg);
    
    //显示到label
    ui->main_label->setPixmap(QPixmap::fromImage(qimg));
}

void rotate(const cv::Mat &src, cv::Mat &result, int angle)
{
    int len=std::max(src.cols,src.rows);
    cv::Mat rot_mat=getRotationMatrix2D(cv::Point(len/2,len/2),angle,1.0);
    warpAffine(src,result,rot_mat,cv::Size(len,len));
}
void MainWindow::rotateUnits()
{
    srand((int)time(nullptr));
    
    for(int i=0;i<unitPatterns.size();i++)
    {
        cv::Mat tmpImg(3000,3000,CV_8U,cv::Scalar(0));
        cv::drawContours(tmpImg,unitPatterns,i,cv::Scalar(255),-1);
        
        rotate(tmpImg,tmpImg,rand()%360);
        
        std::vector<std::vector<cv::Point>> tmpContours;
        cv::findContours(tmpImg,tmpContours,cv::RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
        unitPatterns[i].assign(tmpContours[0].begin(),tmpContours[0].end()) ;
    }
}

void MainWindow::showUnitsButtons()
{
    for(int i=0;i<maxUnitSize;i++)
    {
        if(i < unitPatterns.size())
        {
            cv::Rect bdRect = cv::boundingRect(unitPatterns[i]);
            
            cv::Mat unitImg(bdRect.size(),CV_8U,cv::Scalar(0));
            
            for(int p=0;p<unitPatterns[i].size();p++)
            {
                unitPatterns[i][p].x -= bdRect.x;
                unitPatterns[i][p].y -= bdRect.y;
            }
            cv::drawContours(unitImg, unitPatterns, i, cv::Scalar(255), -1);
            
            unitImg = 255 - unitImg;
            QImage qimg;
            mat2Qimg(unitImg,qimg);
            
            QIcon icon;
            icon.addPixmap(QPixmap::fromImage(qimg));
            unitsButtons[i]->setIcon(icon);
            unitsButtons[i]->setIconSize(QSize(ui->pushButton->size().width()*0.9,ui->pushButton->size().height()*0.9));
        }
        else
        {
            QIcon icon;
            unitsButtons[i]->setIcon(icon);
        }
    }
}




