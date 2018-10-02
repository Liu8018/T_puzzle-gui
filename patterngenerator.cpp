#include "patterngenerator.h"

PatternGenerator::PatternGenerator()
{
    m_minRadius = -1;
    m_maxRadius = -1;
    m_n_sides = -1;
    m_n_units = -1;
}

void PatternGenerator::getDstPattern(std::vector<cv::Point> &dstPattern)
{
    randomizeParas();
    
    generateDstPattern();
    
    dstPattern.assign(m_dstPattern.begin(), m_dstPattern.end());
}

void PatternGenerator::getUnitsPattern(std::vector<std::vector<cv::Point>> &unitsPattern)
{
    if(m_dstPattern.empty())
    {
        std::cout<<"dstPattern is empty!"<<std::endl;
        return;
    }
    
    generateUnitsPattern();
    
    unitsPattern.assign(m_unitsPattern.begin(),m_unitsPattern.end());
}

void PatternGenerator::setRadiusLimit(const int minRadius, const int maxRadius)
{
    m_minRadius = minRadius;
    m_maxRadius = maxRadius;
}
void PatternGenerator::setN_sides(const int n_sides)
{
    m_n_sides = n_sides;
}
void PatternGenerator::setN_units(const int n_units)
{
    m_n_units = n_units;
}

void PatternGenerator::randomizeParas()
{
    srand((int)time(nullptr));
    
    m_minRadius = 500 + rand()%200;
    m_maxRadius = m_minRadius + 200 + rand()%600;
    
    m_n_sides = 3 + rand()%10;
    m_n_units = 3 + rand()%7;
}

void PatternGenerator::generateDstPattern()
{
    double angle_interval = 2 * CV_PI / (double)m_n_sides;
    
    for(double angle = 0; angle < 2*CV_PI-0.1; angle += angle_interval)
    {
        double ratio = (rand()%10)/(double)9;
        
        double radius = m_minRadius + (m_maxRadius-m_minRadius)*ratio;
        
        cv::Point pt;
        pt.x = radius*std::cos(angle) + 1500;
        pt.y = radius*std::sin(angle) + 1500;
        m_dstPattern.push_back(pt);
    }
}

void PatternGenerator::generateUnitsPattern()
{
    srand((int)time(nullptr));
    
    std::vector<cv::Point> pts;
    pts.assign(m_dstPattern.begin(),m_dstPattern.end());
    
    cv::Mat img(3000,3000,CV_8U,cv::Scalar(0));
    std::vector<std::vector<cv::Point>> contours;
    contours.push_back(pts);
    cv::drawContours(img, contours, 0,cv::Scalar(255));
    cv::findContours(img, contours, cv::RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    cv::drawContours(img, contours, 0,cv::Scalar(255), -1);
    pts.clear();
    for(int i=0;i<contours.size();i++)
        for(int j=0;j<contours[i].size();j++)
            pts.push_back(contours[i][j]);
    
    int unitsNum = 1;
    while(unitsNum < m_n_units)
    {
        //分割
        int n1 = rand()%pts.size();
        int n2 = rand()%pts.size();
        
        cv::Mat tmpImg;
        img.copyTo(tmpImg);
        cv::line(tmpImg, pts[n1], pts[n2], cv::Scalar(0), 3);
        
        //cv::namedWindow(".",0);
        //cv::imshow(".",tmpImg);
        //cv::waitKey();
        
        cv::findContours(tmpImg, contours, cv::RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        
        if(contours.size() <= unitsNum || contours.size() > m_n_units)
            continue;
        
        //出现面积过小的轮廓或过于细长的轮廓则重新分割
        bool smallArea = false, thinContour = false;
        for(int i=0;i<contours.size();i++)
        {
            if(cv::contourArea(contours[i]) < 30000)
            {
                smallArea = true;
                break;
            }
            
            cv::RotatedRect rect = cv::minAreaRect(contours[i]);
            cv::Point2f pts[4];
            rect.points(pts);
            int width = std::sqrt((pts[0].x-pts[1].x)*(pts[0].x-pts[1].x)+(pts[0].y-pts[1].y)*(pts[0].y-pts[1].y)) + 1;
            int height = std::sqrt((pts[2].x-pts[1].x)*(pts[2].x-pts[1].x)+(pts[2].y-pts[1].y)*(pts[2].y-pts[1].y)) + 1;
            
            if(width / height > 3 || height / width > 3)
            {
                thinContour = true;
                break;
            }
        }
        if(smallArea || thinContour)
            continue;
        
        //
        tmpImg.copyTo(img);
        
        pts.clear();
        for(int i=0;i<contours.size();i++)
            for(int j=0;j<contours[i].size();j++)
                pts.push_back(contours[i][j]);
        
        unitsNum = contours.size();
    }
    
    m_unitsPattern.assign(contours.begin(),contours.end());
}
