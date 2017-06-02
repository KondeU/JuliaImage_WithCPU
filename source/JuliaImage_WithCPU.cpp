// JuliaImage_WithCPU.cpp
// Writed by kondeu, 20,Oct,2016

// Standard header
#include<iostream>
using namespace std;

// OpenCV header
#include "cv.h"
#include "highgui.h"

#define JI_CHANNEL 4
#define JI_ITERACOUNT 200
#define JI_THRESHOLD 1000

class CComplex
{
public:

    CComplex(float fR = 0, float fI = 0)
    {
        m_fR = fR;
        m_fI = fI;
    }

    inline float CulcLength2()
    {
        return m_fR * m_fR + m_fI * m_fI;
    }

    inline CComplex operator *(const CComplex & c)
    {
        return CComplex(
            (this->m_fR * c.m_fR - this->m_fI * c.m_fI),
            (this->m_fI * c.m_fR + this->m_fR * c.m_fI));
    }
    inline CComplex operator +(const CComplex & c)
    {
        return CComplex(this->m_fR + c.m_fR, this->m_fI + c.m_fI);
    }

    inline void SetR(float fR)
    {
        m_fR = fR;
    }
    inline void SetI(float fI)
    {
        m_fI = fI;
    }
    inline float GetR() const
    {
        return m_fR;
    }
    inline float GetI() const
    {
        return m_fI;
    }

private:

    float m_fR; // Real number
    float m_fI; // Imaginary number
};

void BmpOrg2CrtOrg(float & fCrtX, float & fCrtY, const int x, const int y,
    const float fScale, const int iBmpWidth, const int iBmpHeigth)
{
    fCrtX = fScale * (float)(x - iBmpWidth  / 2) / (iBmpWidth  / 2);
    fCrtY = fScale * (float)(y - iBmpHeigth / 2) / (iBmpHeigth / 2);

    //fCrtX = fScale * (float)(x / 2) / (iBmpWidth / 2);
    //fCrtY = fScale * (float)(y / 2) / (iBmpHeigth / 2);

    //fCrtX = (float)iBmpWidth / 2 - x;
    //fCrtY = (float)iBmpHeigth / 2 - y;
}

int IsJulia(int & iIteraCount, const float fCrtX, const float fCrtY, const CComplex cJuliaConstant)
{
    CComplex cCrtPix(fCrtX, fCrtY);

    for (int i = 0; i <= JI_ITERACOUNT; i++)
    {
        iIteraCount = i;

        cCrtPix = cCrtPix * cCrtPix + cJuliaConstant;

        if (cCrtPix.CulcLength2() > JI_THRESHOLD)
        {
            return 0;
        }
    }

    return 1;
}

void GenerateJuliaImage(IplImage * pImage, const float fScale, const CComplex cJuliaConstant)
{
    for (int y = 0; y < pImage->height; y++)
    {
        for (int x = 0; x < pImage->width; x++)
        {
            int iIteraCount;
            float fCrtX, fCrtY;
            BmpOrg2CrtOrg(fCrtX, fCrtY, x, y, fScale, pImage->width, pImage->height);
            int iIsJulia = IsJulia(iIteraCount, fCrtX, fCrtY, cJuliaConstant);

            int iOffset = y * pImage->widthStep + JI_CHANNEL * x;
            pImage->imageData[iOffset + 0] = ((iIteraCount >= JI_ITERACOUNT) ? 0 : 255 - iIteraCount);
            pImage->imageData[iOffset + 1] = ((iIteraCount >= JI_ITERACOUNT) ? 0 : 255 - iIteraCount);
            pImage->imageData[iOffset + 2] = ((iIteraCount >= JI_ITERACOUNT) ? 0 : ((iIteraCount * 255) / JI_ITERACOUNT));
            pImage->imageData[iOffset + 3] = 255;
        }
    }
}

int main(void)
{
    char cCmd = '/0';

    while(true)
    {
        cout << "------------------------ Julia Image Generater ------------------------" << endl;
        cout << "Julia Image with CPU Core, coded by kondeu, 20,Oct,2016." << endl;
        cout << "Press any key with enter to continue, or press 'q' with enter to exit." << endl;

        cin >> cCmd;

        if (cCmd == 'q')
        {
            break;
        }

        cout << "Input param to create julia image." << endl;

        int iBmpWidth, iBmpHeigth;
        float fScale;
        CComplex cJuliaConstant;

        cout << "Image size - width:" << endl;
        cin >> iBmpWidth;

        cout << "Image size - heigth:" << endl;
        cin >> iBmpHeigth;
        
        cout << "Julia constant - real number:" << endl;
        cin >> fScale;
        cJuliaConstant.SetR(fScale);

        cout << "Julia constant - imaginary number:" << endl;
        cin >> fScale;
        cJuliaConstant.SetI(fScale);

        cout << "Scale:" << endl;
        cin >> fScale;

        cout << "Alloc memory to store image." << endl;

        IplImage * pImage = cvCreateImageHeader(cvSize(iBmpWidth, iBmpHeigth), IPL_DEPTH_8U, JI_CHANNEL);
        pImage->origin = IPL_ORIGIN_TL;
        cvCreateData(pImage);

        cout << "Generating julia iamge..." << endl;

        GenerateJuliaImage(pImage, fScale, cJuliaConstant);

        cout << "Generated julia iamge successfully." << endl;

        cvNamedWindow("Julia image generate", CV_WINDOW_AUTOSIZE);
        cvShowImage("Julia image generate", pImage);
        cvWaitKey(0);
        cvDestroyWindow("Julia image generate");

        cvReleaseImage(&pImage);

        cout << "-----------------------------------------------------------------------" << "\n\n" << endl;


    }

    return 0;
}