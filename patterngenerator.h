#ifndef PATTERNGENERATOR_H
#define PATTERNGENERATOR_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <time.h>
#include <math.h>

class PatternGenerator
{
public:
    PatternGenerator();
    
    void getDstPattern(std::vector<cv::Point> &dstPattern);
    void getUnitsPattern(std::vector<std::vector<cv::Point>> &unitsPattern);
    
    void setRadiusLimit(const int minRadius, const int maxRadius);
    void setN_sides(const int n_sides);
    void setN_units(const int n_units);
    
private:
    void randomizeParas();
    void generateDstPattern();
    void generateUnitsPattern();
    
    std::vector<cv::Point> m_dstPattern;
    std::vector<std::vector<cv::Point>> m_unitsPattern;
    
    int m_minRadius;
    int m_maxRadius;
    int m_n_sides;
    int m_n_units;
};

#endif // PATTERNGENERATOR_H
