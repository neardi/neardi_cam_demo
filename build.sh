# please below command to install gcc first, and then install opencv
# sudo apt install build-essential libopencv-dev
#aarch64-linux-gnu-g++ -std=c++11  -O3  -I/usr/include/opencv4  -L/usr/lib/aarch64-linux-gnu -lX11 main.cpp -o main
aarch64-linux-gnu-g++ -std=c++11 -O3  -o n4camera main.cpp -lpthread -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -I/usr/include/opencv4  -L/usr/lib/aarch64-linux-gnu -L/usr/local/lib/aarch64-linux-gnu/gstreamer-1.0
