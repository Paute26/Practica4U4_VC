#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include "YOLO11.hpp" // Ensure YOLO11.hpp or other version is in your include path

int main()
{
    // Configuration parameters
    const std::string labelsPath = "models/emergenci.names";       // Path to class labels
    const std::string modelPath  = "models/best.onnx";     // Path to YOLO11 model
    //const std::string imagePath  = "data/cuenca.jpg";           // Path to input image
    bool isGPU = true;                                           // Set to false for CPU processing

    // Initialize the YOLO11 detector
    YOLO11Detector detector(modelPath, labelsPath, isGPU);

    // Load an image
    //cv::Mat image = cv::imread(imagePath);

    // Perform object detection to get bboxs
    std::vector<Detection> detections ;
    
    cv::Mat frame;

    //CAMARA ACTUALMENTE ASIGNADA 
    //Cambiar segun sea necesario
    cv::VideoCapture video("/dev/video1");

    if (video.isOpened()){
        cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);
        cv::TickMeter tickMeter; // 

        while(3==3){
            tickMeter.start();  
            video >> frame;

            if(frame.empty()){
                break;
            }

            detections = detector.detect(frame);
            detector.drawBoundingBoxMask(frame, detections);            

            cv::imshow("Video", frame);

            // Stop measuring time and calculate FPS
            tickMeter.stop();
            double fps = tickMeter.getFPS();
            tickMeter.reset(); // Reset for the next iteration

            // Display FPS on the frame
            cv::putText(frame, "FPS: " + std::to_string(static_cast<int>(fps)),
                        cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0,
                        cv::Scalar(0, 255, 0), 2);

            // Display the updated frame with FPS
            cv::imshow("Video", frame);

            if(cv::waitKey(23)==27){
                break;
            }
        }

        video.release();
        cv::destroyAllWindows();

    }

    return 0;
}