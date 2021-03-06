//
//  FaceBlendCommon.cpp
//  Opencv
//
//  Created by Durgesh Trivedi on 19/07/17.
//  Copyright © 2017 durgesh. All rights reserved.
//

#include "FaceBlendCommon.hpp"
extern string rootDirPath;
extern string resultDirPath;
extern string facesDirPath;
Mat getCroppedFaceRegion(Mat image, std::vector<Point2f> landmarks, cv::Rect &selectedRegion)
{
    int x1Limit = landmarks[0].x - (landmarks[36].x - landmarks[0].x);
    int x2Limit = landmarks[16].x + (landmarks[16].x - landmarks[45].x);
    int y1Limit = landmarks[27].y - 3*(landmarks[30].y - landmarks[27].y);
    int y2Limit = landmarks[8].y + (landmarks[30].y - landmarks[29].y);
    
    int imWidth = image.cols;
    int imHeight = image.rows;
    int x1 = max(x1Limit,0);
    int x2 = min(x2Limit, imWidth);
    int y1 = max(y1Limit, 0);
    int y2 = min(y2Limit, imHeight);
    
    // Take a patch over the eye region
    Mat cropped;
    selectedRegion = cv::Rect( x1, y1, x2-x1, y2-y1 );
    cropped = image(selectedRegion);
    return cropped;
}


// read descriptors saved on disk
void readDescriptors(const string& filename, std::vector<string>& faceLabels, std::vector<matrix<float,0,1>>& faceDescriptors, char separator) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    // each line has:
    // 1st element = face label
    // rest 128 elements = descriptor elements
    string line;
    string faceLabel;
    // valueStr = one element of descriptor in string format
    // value = one element of descriptor in float
    string valueStr;
    
    std::vector<float> faceDescriptorVec;
    // read lines from file one by one
    while (getline(file, line)) {
        stringstream liness(line);
        // read face label
        // read first word on a line till separator
        getline(liness, faceLabel, separator);
        if(!faceLabel.empty()) {
            faceLabels.push_back(faceLabel);
        }
        
        faceDescriptorVec.clear();
        // read rest of the words one by one using separator
        while (getline(liness, valueStr, separator)) {
            if (!valueStr.empty()) {
                // convert descriptor element from string to float
                faceDescriptorVec.push_back(atof(valueStr.c_str()));
            }
        }
        
        // convert face descriptor from vector of float to Dlib's matrix format
        dlib::matrix<float, 0, 1> faceDescriptor = dlib::mat(faceDescriptorVec);
        faceDescriptors.push_back(faceDescriptor);
    }
}

// find nearest face descriptor from vector of enrolled faceDescriptor
// to a query face descriptor
void nearestNeighbor(dlib::matrix<float, 0, 1>& faceDescriptorQuery,
                     std::vector<dlib::matrix<float, 0, 1>>& faceDescriptors,
                     std::vector<string>& faceLabels, string &label) {
    int minDistIndex = 0;
    float minDistance = 1.0;
    label = NEW_FACE;
    // Calculate Euclidean distances between face descriptor calculated on face dectected
    // in current frame with all the face descriptors we calculated while enrolling faces
    // Calculate minimum distance and index of this face
    for (int i = 0; i < faceDescriptors.size(); i++) {
        double distance = length(faceDescriptors[i] - faceDescriptorQuery);
        if (distance < minDistance) {
            minDistance = distance;
            minDistIndex = i;
        }
    }
    // Dlib specifies that in general, if two face descriptor vectors have a Euclidean
    // distance between them less than 0.6 then they are from the same
    // person, otherwise they are from different people.
    
    // This threshold will vary depending upon number of images enrolled
    // and various variations (illuminaton, camera quality) between
    // enrolled images and query image
    // We are using a threshold of 0.5
    // if minimum distance is greater than a threshold
    // assign integer label -1 or NEW_FACE i.e. unknown face
    if (minDistance > THRESHOLD){
        label = NEW_FACE;
    } else {
        label = faceLabels[minDistIndex];
    }
}

// filter files having extension ext
void filterFiles(string dirPath, std::vector<string>& fileNames, std::vector<string>& filteredFilePaths, std:: vector<string> ext)
{
    for(int i = 0; i < fileNames.size(); i++)
    {
        string fname = fileNames[i];
        string value = fname.substr(fname.find_last_of(".") + 1);
        STRTOLOWER(value);
        std::vector<std::string>::iterator iter = find(ext.begin(), ext.end(), value);
        if (iter != ext.end())
        {
            filteredFilePaths.push_back(dirPath + "/" + fname);
        }
    }
}
// Reads files, folders and symbolic links in a directory
void listdir(string dirName, std::vector<string>& folderNames, std::vector<string>& fileNames, std::vector<string>& symlinkNames) {
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir(dirName.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) {
            // ignore . and .. and file name start with .
            string temp_name = ent->d_name;
            if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0 || (temp_name.find_first_of(".") == 0) ) {
                continue;
            }
            
            // Read more about file types identified by dirent.h here
            // https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html
            switch (ent->d_type) {
                case DT_REG:
                    fileNames.push_back(temp_name);
                    break;
                case DT_DIR:
                    folderNames.push_back(dirName + "/" + temp_name);
                    break;
                case DT_LNK:
                    symlinkNames.push_back(temp_name);
                    break;
                default:
                    break;
            }
            // cout << temp_name << endl;
        }
        // sort all the files
        std::sort(folderNames.begin(), folderNames.end());
        std::sort(fileNames.begin(), fileNames.end());
        std::sort(symlinkNames.begin(), symlinkNames.end());
        closedir(dir);
    }
}

bool is_file(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

bool is_dir(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}
