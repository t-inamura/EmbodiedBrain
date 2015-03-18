#!/bin/bash
# For Right Hand Grasping
g++ Linkage_Grasp.cpp -shared  -fPIC -o Linkage_Grasp.so   -DUSE_ODE -DCONTROLLER -DdDOUBLE  -I /home/sigverse/sigverse-2.2.0/include 
# For Grasped Object
g++ Linkage_Object.cpp -shared -fPIC -o Linkage_Obon.so    -DUSE_ODE -DCONTROLLER -DdDOUBLE  -I /home/sigverse/sigverse-2.2.0/include
