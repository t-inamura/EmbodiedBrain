Name
====
MirrorTherapy  

## Overview  
 Rehabilitation project of phantom limb pain with Kinect V2, as Visual Studio 2013 C++ project.

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
 ../include: SIGVerse and OpenNI header files (.h) needed  
 library  
 ../lib: SIGVerse and OpenNI library file (.lib) needed  
 rutime  
 ../Release: OpenCV and OpenNI dynamic link library file(.dll) , SIGVerse config file(.ini) needed  

 you can download from below:  
 SIGViewer-2.2.0 of SIGVerse-2.2.0   
 <http://sigverse.org/sigverse/main/download/>
 Please install it as below:  
 <http://www.sigverse.com/wiki/en/index.php?Tutorial>   
 OpenCV2.4.9  
 <http://opencv.org/downloads.html>  
 OpenNI  
 <**********>  

 Please merge to the project directory.  

## Usage  
 1. boot up Ubuntu12.04 on VMware.
 2. start SIGServer on Ubuntu. SIGVerse must be sigverse-2.2.0.  
    $ cd /EmbodiedBrain/MirrorTherapy/Server/Controller  
    $ sigserver.sh -w ./MirrorTherapy.xml(filename) -p 9000(port number)  
 3. start SIGViewer on Windows8.
 4. push start button on SIGViewer to start simulation.
 5. edit MirrorTherapy.bat, set your IPadress and port number of Ubuntu.
 6. execute MirrorTherapy.bat.
 7. move your body in front fo Kinect sensor.

## Install
 set any place  

## Contribution

## Licence

## Author
 National Institute of Informatics  
 