# README #

The project is having default setting considering OpencCV and Dlib is installed on the system with specific path else 
you need to edit the Opencv and dlib Header/ library path in build setting for the project. Which you definately need it to run the project .

### OpenCv and Dlib header search path in project setting ###
The header search path for dlib and opencv should be like this 

First you need to install the Opencv on your sytem for that. Also you need build dlib file .

/usr/local/Cellar/opencv3/3.2.0 

Follow these links to setup Opencv path 

http://charliegerard.github.io/blog/Installing-OpenCV/ 

https://blogs.wcode.org/2014/11/howto-setup-xcode-6-1-to-work-with-opencv-libraries/ 

You should have the dlib-19.4 header in a directory and give its path  like below 

/Users/durgeshtrivedi/durgesh/OpenCV/dlib-19.4

### OpenCv and Dlib Library search path in project setting ###
The library search path for Opencv is 

/usr/local/Cellar/opencv3/3.2.0

The library search path for Dlib 

You need to keep the libdlib.a file in lib folder inside the project dir. Which is actually part of project you no need to do any thing her actually
$(PROJECT_DIR)/ImageCluster/lib


### Dlib in build phase ##

 You need to add the libdlib.a  and Accelerate.framwork in link binary
 
 YOu again no need to do any thing over here 
 
 ### OpenCv Other linker flags ##
 
 add this flags 
 
 -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab
 
 again you no need to add it is already there
 
 ### dlib preprocess macros 
 
 To build the  project properly you need to add some preprocess macro for dlib else it will not build 

These 2 links will help to understand it. again you no need to do it now it is part of project .

https://github.com/zweigraf/face-landmarking-ios/issues/7 

### notes ## 
So to make this project build properly you only need to properly give path for headers for opencv and dlib .
rest of the setting are already part of the project and they should work all the info in just for understanding if any things not work. 