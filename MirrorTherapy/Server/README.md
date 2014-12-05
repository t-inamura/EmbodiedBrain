Name  
====  
MirrorTherapy  

## Overview  
 Rehabilitation project of phantom limb pain with Kinect V2, as C++ project.

## Description  
 If you lost your limbs or arms, you might suffer from pain of them even they doesn't exist. 
 That is called "phantom limb pain". You might relieve your pain with "MirrorTherapy". 
 "MirrorTherapy" is that you move existing limb or arm in the  mirror, recognizing it as a lost limb or arm. 
 MirrorTherapy project make the therapy efficient and easy with Kinect V2 and Oculus Rift. 

## Demo  

## VS. 

## Requirement  

 You can download from below:  
 SIGViewer-2.2.0 of SIGVerse-2.2.0   
 <http://sigverse.org/sigverse/main/download/>  
 Please install it as below:  
 <http://www.sigverse.com/wiki/en/index.php?Tutorial>   
 
 Ubuntu 12.04  
 <*****>  
   
## Usage  
 1. boot up Ubuntu12.04.  
 2. start SIGServer on Ubuntu. SIGVerse must be sigverse-2.2.0.  
    $ cd /EmbodiedBrain/MirrorTherapy/Server/Controller  
    $ sigserver.sh -w ./MirrorTherapyHand.xml(filename) -p 9000(port number)  
 3. start SIGViewer on Windows8.  
 4. push start button on SIGViewer to start simulation.  
 5. edit MirrorTherapy2013.bat, set your IPadress and port number of Ubuntu.  
 6. execute MirrorTherapy2013.bat.  
 7. move your body in front fo Kinect sensor.  

You can send text message with SIGViewer to change the movement of avator. Text messages are below:
* HAND_FIXED            : fix both left and right hands.
* REVERSE_NONE_HAND     : able to move both hands as usual.
* REVERSE_RIGHT_HAND    : reverse right hand movement to aply left hand.
* REVERSE_LEFT_HAND     : reverse left hand movement to aply right hand.
* FOOT_FIXED            : fix both left and right legs.
* REVERSE_NONE_FOOT     : able to move both legss as usual.
* REVERSE_RIGHT_FOOT    : reverse right leg movement to aply left leg.
* REVERSE_LEFT_FOOT     : reverse left leg movement to aply right leg.


## Install  
 set any place  

## Contribution  

## Licence  

## Author  
 National Institute of Informatics  
 