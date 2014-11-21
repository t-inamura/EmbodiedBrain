Name
====
MirrorTherapy  

## Overview  
 Rehabilitation project of phantom limb pain with Kinect V2, \as Visual Studio 2013 C++ project.

## Description
 If you lost your limbs or arms, you might suffer from pain of them even they doesn't exist. 
 That is called "phantom limb pain". You might relieve your pain with "MirrorTherapy". 
 "MirrorTherapy" is that you move existing limb or arm in the  mirror, recognizing it as a lost limb or arm. 
 MirrorTherapy project make the therapy efficient and easy with Kinect V2 and Oculus Rift. 

## Demo

## VS. 

## Requirement
 Please install SIGServer-2.2.0 and SIGViewer-2.2.0 as below:  
 <http://www.sigverse.com/wiki/en/index.php?Tutorial>  
 Please install Visual Studio 2013  
   
 Project settings below:  
 include  
 ../include: SIGVerse header files and OpenNI header files needed  
 library  
 ../lib: SIGVerse library file and OpenNI library file needed  
 rutime  
 ../Release: OpenCV dynamic link library files and a config file needed  

 you can download from below:  
 <**********>  
 Please merge to the project directory.  

## Usage
 1. boot up Ubuntu12.04 on VMware.
 2. start SIGServer on Ubuntu. SIGVerse must be sigverse-2.2.0.  
    $ cd /home/sigverse/sigverse-2.2.0/share/sigverse/data  
    $ sigserver.sh -w ./MyWorld.xml(filename) -p 9000(port number)  
 3. start SIGViewer on Windows8.
 4. push start button on SIGViewer to start simulation.
 5. edit MirrorTherapy.bat, set your IPadress and port number of Ubuntu.
 6. execute MirrorTherapy.bat.
 7. move your body infront fo Kinect sensor.

## Install
 set any place  

## Contribution

## Licence

## Author
 National Institute of Informatics  
 