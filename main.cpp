/*
 * main.cpp
 *
 *  Created on: 2023/3/15
 *      Author: Linx
 *       Email: linx.wu@neardi.com
 */
#include <cstdio>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/videoio/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "queue.hpp"

using namespace std;
using namespace cv;

const int max_img_count = 16;

/**
 * create memory pool for images gotten from camera.
 */
Queue<cv::Mat*> _idleimgbuf;
Queue<cv::Mat*> _imgdata;

static bool g_exit = false;
int width          = 640;
int height         = 480;


/**
 * camera thread, got the camera stream over gstreamer.
 */
static int camera_thread(int cam_id) {
    //std::cout << __func__ << ", channel = " << channel << ", id = " << cam_id << std::endl;
    /**
     * load image from camera
     */
    std::string capfmt = "v4l2src device=/dev/video" 
                          + std::to_string(cam_id)
                          + " ! video/x-raw,format=NV12,width="
                          + std::to_string(width) + ",height="
                          + std::to_string(height)
                          + ",framerate=30/1 ! videoconvert ! video/x-raw,format=BGR ! appsink";

    /**
     * load hdmiin
     * RK3588: gst-launch-1.0 v4l2src device=/dev/video40 ! kmssink plane-id=54 -v
     */
    /** std::string capfmt = "v4l2src device=/dev/video"
                          + std::to_string(cam_id)
                          + " ! videoconvert ! video/x-raw,format=BGR ! appsink";
    */

    /**
     * USB camera
     */
    /** std::string usbcap = "v4l2src device=/dev/video"
                          + std::to_string(cam_id)
                          + " ! videoconvert ! appsink";
    */

    cv::VideoCapture cap(capfmt, cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        std::cout << "please open camera first!" << std::endl;
        return -1;
    }

    cv::Mat first;
    cap >> first;
    for (int i = 0; i < max_img_count; i++) {
        _idleimgbuf.push(new cv::Mat(first));
    }

    while (!g_exit) {
        cv::Mat *pimg = NULL;
        _idleimgbuf.pop(pimg);
        if (NULL != pimg) {
            cap >> *pimg;
            if (pimg->empty()) {
                _idleimgbuf.push(pimg);
            } else {
                _imgdata.push(pimg);
            }
        } else {
            /**
             * wait a moment to avoid consume high CPU performance.
             */
            std::cout << "no buffer" << std::endl;
            usleep(100);
            continue;
        }
    } // end while loop
      //std::cout << "it's going to out" << std::endl;
    cap.release();

    //std::cout << __func__ << ", <==Exit" << std::endl;
    return 0;
}

int main(int argc, char **argv) {

    //const char* model_path = argv[1];
    if (argc != 2) {
        printf("Usage:%s n4camera cam_id\n", argv[0]);
        return -1;
    }

    int index = std::stoi(argv[1]);

    /**
     * create the dedicated thread to capture image from camera.
     */
    std::thread thread = std::thread(&camera_thread, index);


    std::string n4window = "n4 cameras";
    namedWindow(n4window);

    //std::cout << "Enter the main loop!" << endl;
    while (true) {

        /**
         * query the image data gotten from camera.
         */
        cv::Mat *pimg = nullptr;
        _imgdata.pop(pimg);
        if (nullptr == pimg) {
            usleep(100);
            continue;
        }

		/**
		 * Fixed me: here, please try to show this pointer data directory.
		 */
        cv::Mat orig_img = pimg->clone();
        _idleimgbuf.push(pimg);	


        /**
         * show this image data.
         */
        imshow(n4window, orig_img);
		

        /**
         * input any key to exit.
         */
        if (waitKey(30) >= 0) {
            //std::cout << "got exit key" << std::endl;
            g_exit = true;
            break;
        }
    }

    /**
     * wait a moment, so the camera threads was completed.
     */
    usleep(3000);
	thread.join();

    /**
     * free the queue buffer.
     */
	while (_idleimgbuf.size() > 0) {
		cv::Mat *pmat = NULL;
		_idleimgbuf.pop(pmat);
		if (NULL != pmat) {
			delete pmat;
		}
	}
	
	while (_imgdata.size() > 0) {
		cv::Mat *pmat = NULL;
		_imgdata.pop(pmat);
		if (NULL != pmat) {
			delete pmat;
		}
	}


    std::cout << "Bye!" << std::endl;
    return 0;
}

