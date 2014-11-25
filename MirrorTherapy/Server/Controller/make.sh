#!/bin/bash
# For Right Hand Grasping
g++ Linkage_Grasp.cpp -shared  -fPIC -o Linkage_Grasp.so          -DUSE_ODE -DCONTROLLER -DdDOUBLE -D_REVERSE_GRASP -D_VERBOSE             -I /home/sigverse/sigverse-2.2.0/include 
g++ Linkage_Grasp.cpp -shared  -fPIC -o Linkage_RightHandGrasp.so -DUSE_ODE -DCONTROLLER -DdDOUBLE -D_REVERSE_GRASP -D_VERBOSE             -I /home/sigverse/sigverse-2.2.0/include
# For Right Foot Grasping
g++ Linkage_Grasp.cpp -shared  -fPIC -o Linkage_RightFootGrasp.so -DUSE_ODE -DCONTROLLER -DdDOUBLE -D_REVERSE_GRASP -D_VERBOSE -D_USE_FOOT -I /home/sigverse/sigverse-2.2.0/include
# For Left Hand Grasping
g++ Linkage_Grasp.cpp -shared  -fPIC -o Linkage_LeftHandGrasp.so  -DUSE_ODE -DCONTROLLER -DdDOUBLE                  -D_VERBOSE             -I /home/sigverse/sigverse-2.2.0/include
# For Left Foot Grasping
g++ Linkage_Grasp.cpp -shared  -fPIC -o Linkage_LeftFootGrasp.so  -DUSE_ODE -DCONTROLLER -DdDOUBLE                  -D_VERBOSE -D_USE_FOOT -I /home/sigverse/sigverse-2.2.0/include
# For Grasped Object
g++ Linkage_Object.cpp -shared -fPIC -o Linkage_Obon.so           -DUSE_ODE -DCONTROLLER -DdDOUBLE                  -D_VERBOSE             -I /home/sigverse/sigverse-2.2.0/include
g++ Linkage_Object.cpp -shared -fPIC -o Linkage_Object.so         -DUSE_ODE -DCONTROLLER -DdDOUBLE                  -D_VERBOSE             -I /home/sigverse/sigverse-2.2.0/include
