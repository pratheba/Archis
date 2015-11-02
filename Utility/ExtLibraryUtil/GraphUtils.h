//------------------------------------------------------------------------------
// Graphing functions for OpenCV.	Part of "ImageUtils.cpp", a set of handy utility functions for dealing with images in OpenCV.
// by Shervin Emami (http://www.shervinemami.co.cc/) on 20th May, 2010.
//------------------------------------------------------------------------------

#ifndef __Archis__GraphUtils__
#define __Archis__GraphUtils__

#include <stdio.h>
#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//------------------------------------------------------------------------------
// Graphing functions
//------------------------------------------------------------------------------
// Draw the graph of an array of doubles into imageDst or a new image, between minV & maxV if given.
// Remember to free the newly created image if imageDst is not given.

enum DATATYPE {
    UNCHAR = 0,
    INT = 1,
    FLOAT = 2,
    DOUBLE = 3
};

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

//------------------------------------------------------------------------------
// Graphing functions
//------------------------------------------------------------------------------
const cv::Scalar BLACK = CV_RGB(0,0,0);
const cv::Scalar WHITE = CV_RGB(255,255,255);
const cv::Scalar GREY = CV_RGB(150,150,150);

int countGraph = 0;	// Used by 'getGraphColor()'
cv::Scalar customGraphColor;
int usingCustomGraphColor = 0;

// Get a new color to draw graphs. Will use the latest custom color, or change between blue, green, red, dark-blue, dark-green and dark-red until a new image is created.
cv::Scalar getGraphColor(void)
{
    if (usingCustomGraphColor) {
        usingCustomGraphColor = 0;
        return customGraphColor;
    }
    
    countGraph++;
    switch (countGraph) {
        case 1:	return CV_RGB(60,60,255);	// light-blue
        case 2:	return CV_RGB(60,255,60);	// light-green
        case 3:	return CV_RGB(255,60,40);	// light-red
        case 4:	return CV_RGB(0,210,210);	// blue-green
        case 5:	return CV_RGB(180,210,0);	// red-green
        case 6:	return CV_RGB(210,0,180);	// red-blue
        case 7:	return CV_RGB(0,0,185);		// dark-blue
        case 8:	return CV_RGB(0,185,0);		// dark-green
        case 9:	return CV_RGB(185,0,0);		// dark-red
        default:
            countGraph = 0;	// start rotating through colors again.
            return CV_RGB(200,200,200);	// grey
    }
}
// Call 'setGraphColor()' to reset the colors that will be used for graphs.
void setGraphColor(int index)
{
    countGraph = index;
    usingCustomGraphColor = 0;	// dont use a custom color.
}
// Specify the exact color that the next graph should be drawn as.
void setCustomGraphColor(int R, int B, int G)
{
    customGraphColor = CV_RGB(R, G, B);
    usingCustomGraphColor = 1;	// show that it will be used.
}

template <class T>
cv::Mat drawGraph(const T *arraySrc, int nArrayLength, DATATYPE dataType=INT, cv::Mat imageDst=cv::Mat(0,0,CV_8UC3), T minV=0.0, T maxV=0.0, int width=0, int height=0, std::string graphLabel="", bool showScale=true) {
    int w = width;
    int h = height;
    int b = 10;		// border around graph within the image
    if (w <= 20)
        w = nArrayLength + b*2;	// width of the image
    if (h <= 20)
        h = 220;
    
    int s = h - b*2;// size of graph height
    T xscale = 1.0;
    if (nArrayLength > 1)
        xscale = (w - b*2) / (T)(nArrayLength-1);	// horizontal scale
    cv::Mat imageGraph;	// output image
    
    // Get the desired image to draw into.
    if (imageDst.empty()) {
        // Create an RGB image for graphing the data
        imageGraph = cv::Mat(w,h, CV_8UC3);
        
        // Clear the image
        imageGraph = WHITE;
    }
    else {
        // Draw onto the given image.
        imageGraph = imageDst;
    }
    if (imageGraph.empty()) {
        std::cerr << "ERROR in drawFloatGraph(): Couldn't create image of " << w << " x " << h << std::endl;
        exit(1);
    }
    cv::Scalar colorGraph = getGraphColor();	// use a different color each time.
    
    // If the user didnt supply min & mav values, find them from the data, so we can draw it at full scale.
    T diffV;
    if ((dataType == DATATYPE::FLOAT) || (dataType == DATATYPE::DOUBLE)){
        if (std::abs(minV) < (T)(0.0000001) && std::abs(maxV) < (T)(0.0000001)) {
            for (int i=0; i<nArrayLength; i++) {
                T v = (T)arraySrc[i];
                if (v < minV)
                    minV = v;
                if (v > maxV)
                    maxV = v;
            }
        }
        diffV = maxV - minV;
        if (diffV == 0)
            diffV = 0.00000001f;	// Stop a divide-by-zero error
        T fscale = (T)s / diffV;
    }
    else {
        if (minV == 0 && maxV == 0) {
            for (int i=0; i<nArrayLength; i++) {
                int v = arraySrc[i];
                if (v < minV)
                    minV = v;
                if (v > maxV)
                    maxV = v;
            }
        }
        diffV = maxV - minV;
        if (diffV == 0)
            diffV = 1;	// Stop a divide-by-zero error
        
    }
    
    double fscale = (double)s / (double)diffV;
    
    // Draw the horizontal & vertical axis
    int y0 = cvRound(minV*fscale);
    cv::line(imageGraph, cv::Point2d(b,h-(b-y0)), cv::Point2d(w-b, h-(b-y0)), BLACK);
    cv::line(imageGraph, cv::Point2d(b,h-(b)), cv::Point2d(b, h-(b+s)), BLACK);
    
    // Write the scale of the y axis
    
    if (showScale) {
        //cvInitFont(CV_FONT_HERSHEY_PLAIN, 0.55,CV_FONT_HERSHEY_PLAIN,0.5,0.6, 0,1, CV_AA);	// For OpenCV 2.0
        cv::Scalar clr = GREY;
        char text[16];
        snprintf(text, sizeof(text)-1, "%.1f", maxV);
        cv::putText(imageGraph, text, cv::Point2d(1, b+4), CV_FONT_HERSHEY_PLAIN, 0.55, clr);
        // Write the scale of the x axis
        snprintf(text, sizeof(text)-1, "%d", (nArrayLength-1) );
        cv::putText(imageGraph, text, cv::Point2d(w-b+4-5*strlen(text), (h/2)+10), CV_FONT_HERSHEY_PLAIN, 0.55, clr);
    }
    
    // Draw the values
    cv::Point2d ptPrev = cv::Point2d(b,h-(b-y0));	// Start the lines at the 1st point.
    for (int i=0; i<nArrayLength; i++) {
        int y = cvRound((arraySrc[i] - minV) * fscale);	// Get the values at a bigger scale
        int x = cvRound(i * xscale);
        cv::Point2d ptNew = cv::Point2d(b+x, h-(b+y));
        cv::line(imageGraph, ptPrev, ptNew, colorGraph, 1, CV_AA);	// Draw a line from the previous point to the new point
        ptPrev = ptNew;
    }
    
    // Write the graph label, if desired
    if (!graphLabel.empty()) {
        //cvInitFont(CV_FONT_HERSHEY_PLAIN, 0.55,CV_FONT_HERSHEY_PLAIN, 0.5,0.7, 0,1,CV_AA);
        cv::putText(imageGraph, graphLabel, cv::Point2d(30, 10), CV_FONT_HERSHEY_PLAIN, 0.55, cv::Scalar::all(0));	// black text
    }
    
    return imageGraph;
}

// Display a graph of the given array.
// If background is provided, it will be drawn into, for combining multiple graphs using drawGraph().
// Set delay_ms to 0 if you want to wait forever until a keypress, or set it to 1 if you want it to delay just 1 millisecond.
template <class T>
void showGraph(const std::string name, const float *arraySrc, int nArrayLength, DATATYPE dataType, int delay_ms=500, cv::Mat background=cv::Mat::zeros(0, 0, CV_8UC3)) {
    cv::Mat imageGraph = drawGraph(arraySrc, nArrayLength, dataType, background);
    
    // Display the graph into a window
    cv::namedWindow( name );
    cv::imshow( name, imageGraph );
    
    cv::waitKey( 10 );		// Note that cv::waitkey() is required for the OpenCV window to show!
    cv::waitKey( delay_ms );
}

// Simple helper function to easily view an image, with an optional pause.
void showImage(const cv::Mat img, int delay_ms=0, std::string name=0) {
    if (name.empty())
        name = "Image";
    cv::namedWindow(name, CV_WINDOW_AUTOSIZE);
    cv::imshow(name, img);
    cv::waitKey(delay_ms);
}


#endif //end GRAPH_UTILS