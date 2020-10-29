#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "Inversion_CUDA.h"

using namespace std;
using namespace cv;

Mat resizeImage(Mat input) {
    Mat output;
    resize(input, output, Size(), 0.5, 0.5);
    return output;
}

int invertImage(Mat input) {
    Mat Input_Image = input;
    cout << "Height: " << Input_Image.rows << ", Width: " << Input_Image.rows << ", Channels: " << Input_Image.channels() << endl;
    Image_Inversion_CUDA(Input_Image.data, Input_Image.rows, Input_Image.rows, Input_Image.channels());
    Mat output = resizeImage(Input_Image);
    imwrite("Inverted_Image.png", output);
    imshow("Your Image", output);
    int k = waitKey(0);
    destroyAllWindows();
    return 0;
}

Mat readImage(string location) {
    Mat InputImage = imread(location);
    return InputImage;
}

int menu() {
    int x;
    cout << "\n\t\tMenu";
    cout << "\n\tBasic Operations";
    cout << "\n\t  1.Invert Image";
    cout << "\n\t  2.Get Histogram Values:Grayscale";
    cout << "\n\t  3.Get Histogram Value:Color";
    cout << "\n\tOther Operations:";
    cout << "\n\t  4.Box Filter";
    cout << "\n\t  5.Sobel Filter";
    cout << "\n\t  6.Histogram Equalization";
    cout << "\n\t  7.Canny Edge Detection";
    cout << "\n\t  8.Underwater Image Enhancement";
    cout << "\n\tEnter your options:";
    cin >> x;
    return x;
}

int main()
{
    string location;
   
    while (1) {
        cout << "\tInput Image:";
        cin >> location;
        Mat InputImage = readImage(location);
        if (InputImage.empty())
        {
            cout << "Failed imread(): Image not found" << std::endl;
            continue;
        }
        int x = menu();
        if (x == 0) {
            cout << "\n\tEnding program";
            break;
        }
       
        switch (x) {
        case 1: invertImage(InputImage);
                break;
        }
    }
    return 0;
}