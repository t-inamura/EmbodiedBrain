#アバター用パラメータ算出
#長い手の肘から先までの倍率
long_ratio=1.55
#短い手の肘から先までの倍率
short_ratio=0.75

ARM_LINK4_LEN=`echo       "scale=1; 1.6*1.0" | bc`
ARM_LINK4_LONG_LEN=`echo  "scale=1; 1.6*${long_ratio}" | bc`
ARM_LINK4_SHORT_LEN=`echo "scale=1; 1.6*${short_ratio}" | bc`

LARM_LINK4_x=`echo       "scale=1; ( 15)" | bc`
RARM_LINK4_x=`echo       "scale=1; (-15)" | bc`
LARM_LINK4_LONG_x=`echo  "scale=1; ( 15) * ${long_ratio}" | bc`
RARM_LINK4_LONG_x=`echo  "scale=1; (-15) * ${long_ratio}" | bc`
LARM_LINK4_SHORT_x=`echo "scale=1; ( 15) * ${short_ratio}" | bc`
RARM_LINK4_SHORT_x=`echo "scale=1; (-15) * ${short_ratio}" | bc`

LARM_JOINT5_x=`echo       "scale=1; (${forearm_len} * 0.7) / ${body_scale}" | bc`
RARM_JOINT5_x=`echo       "scale=1; (${forearm_len} *-0.7) / ${body_scale}" | bc`
LARM_JOINT5_LONG_x=`echo  "scale=1; (${forearm_len} * 0.7) / ${body_scale} * ${long_ratio}" | bc`
RARM_JOINT5_LONG_x=`echo  "scale=1; (${forearm_len} *-0.7) / ${body_scale} * ${long_ratio}" | bc`
LARM_JOINT5_SHORT_x=`echo "scale=1; (${forearm_len} * 0.7) / ${body_scale} * ${short_ratio}" | bc`
RARM_JOINT5_SHORT_x=`echo "scale=1; (${forearm_len} *-0.7) / ${body_scale} * ${short_ratio}" | bc`


# arm_robot_mid.xmlの生成
cp ./arm_robot_src.xml ./arm_robot_mid.xml
chmod 666 ./arm_robot_mid.xml

sed -i -e "s/SCALEX/$body_scale/g" ./arm_robot_mid.xml
sed -i -e "s/SCALEY/$body_scale/g" ./arm_robot_mid.xml
sed -i -e "s/SCALEZ/$body_scale/g" ./arm_robot_mid.xml
sed -i -e "s/X3DFILENAME/arm_robot_mid/g" ./arm_robot_mid.xml

# arm_robot_mid.x3dの生成
cp ./arm_robot_src.x3d ./arm_robot_mid.x3d
chmod 666 ./arm_robot_mid.x3d

sed -i -e "s/HEAD_JOINT0_y/$HEAD_JOINT0_y/g" ./arm_robot_mid.x3d
sed -i -e "s/LARM_JOINT0_x/$LARM_JOINT0_x/g" ./arm_robot_mid.x3d
sed -i -e "s/LARM_JOINT0_y/$LARM_JOINT0_y/g" ./arm_robot_mid.x3d
sed -i -e "s/RARM_JOINT0_x/$RARM_JOINT0_x/g" ./arm_robot_mid.x3d
sed -i -e "s/RARM_JOINT0_y/$RARM_JOINT0_y/g" ./arm_robot_mid.x3d

sed -i -e "s/LARM_JOINT3_x/$LARM_JOINT3_x/g" ./arm_robot_mid.x3d
sed -i -e "s/RARM_JOINT3_x/$RARM_JOINT3_x/g" ./arm_robot_mid.x3d
sed -i -e "s/LARM_JOINT5_x/$LARM_JOINT5_x/g" ./arm_robot_mid.x3d
sed -i -e "s/RARM_JOINT5_x/$RARM_JOINT5_x/g" ./arm_robot_mid.x3d

sed -i -e "s/LARM_LINK2_x/$LARM_LINK2_x/g"   ./arm_robot_mid.x3d
sed -i -e "s/RARM_LINK2_x/$RARM_LINK2_x/g"   ./arm_robot_mid.x3d
sed -i -e "s/ARM_LINK4_LEN/$ARM_LINK4_LEN/g" ./arm_robot_mid.x3d
sed -i -e "s/LARM_LINK4_x/$LARM_LINK4_x/g"   ./arm_robot_mid.x3d
sed -i -e "s/RARM_LINK4_x/$RARM_LINK4_x/g"   ./arm_robot_mid.x3d


# arm_robot_lengthen.xmlの生成
cp ./arm_robot_src.xml ./arm_robot_lengthen.xml
chmod 666 ./arm_robot_lengthen.xml

sed -i -e "s/SCALEX/$body_scale/g" ./arm_robot_lengthen.xml
sed -i -e "s/SCALEY/$body_scale/g" ./arm_robot_lengthen.xml
sed -i -e "s/SCALEZ/$body_scale/g" ./arm_robot_lengthen.xml
sed -i -e "s/X3DFILENAME/arm_robot_lengthen/g" ./arm_robot_lengthen.xml

# arm_robot_lengthen.x3dの生成
cp ./arm_robot_src.x3d ./arm_robot_lengthen.x3d
chmod 666 ./arm_robot_lengthen.x3d

sed -i -e "s/HEAD_JOINT0_y/$HEAD_JOINT0_y/g"      ./arm_robot_lengthen.x3d
sed -i -e "s/LARM_JOINT0_x/$LARM_JOINT0_x/g"      ./arm_robot_lengthen.x3d
sed -i -e "s/LARM_JOINT0_y/$LARM_JOINT0_y/g"      ./arm_robot_lengthen.x3d
sed -i -e "s/RARM_JOINT0_x/$RARM_JOINT0_x/g"      ./arm_robot_lengthen.x3d
sed -i -e "s/RARM_JOINT0_y/$RARM_JOINT0_y/g"      ./arm_robot_lengthen.x3d

sed -i -e "s/LARM_JOINT3_x/$LARM_JOINT3_LONG_x/g" ./arm_robot_lengthen.x3d
sed -i -e "s/RARM_JOINT3_x/$RARM_JOINT3_LONG_x/g" ./arm_robot_lengthen.x3d
sed -i -e "s/LARM_JOINT5_x/$LARM_JOINT5_LONG_x/g" ./arm_robot_lengthen.x3d
sed -i -e "s/RARM_JOINT5_x/$RARM_JOINT5_LONG_x/g" ./arm_robot_lengthen.x3d

sed -i -e "s/LARM_LINK2_x/$LARM_LINK2_LONG_x/g"   ./arm_robot_lengthen.x3d
sed -i -e "s/RARM_LINK2_x/$RARM_LINK2_LONG_x/g"   ./arm_robot_lengthen.x3d
sed -i -e "s/ARM_LINK4_LEN/$ARM_LINK4_LONG_LEN/g" ./arm_robot_lengthen.x3d
sed -i -e "s/LARM_LINK4_x/$LARM_LINK4_LONG_x/g"   ./arm_robot_lengthen.x3d
sed -i -e "s/RARM_LINK4_x/$RARM_LINK4_LONG_x/g"   ./arm_robot_lengthen.x3d


# arm_robot_cut.xmlの生成
cp ./arm_robot_src.xml ./arm_robot_cut.xml
chmod 666 ./arm_robot_cut.xml

sed -i -e "s/SCALEX/$body_scale/g" ./arm_robot_cut.xml
sed -i -e "s/SCALEY/$body_scale/g" ./arm_robot_cut.xml
sed -i -e "s/SCALEZ/$body_scale/g" ./arm_robot_cut.xml
sed -i -e "s/X3DFILENAME/arm_robot_cut/g" ./arm_robot_cut.xml

# arm_robot_cut.x3dの生成
cp ./arm_robot_src.x3d ./arm_robot_cut.x3d
chmod 666 ./arm_robot_cut.x3d

sed -i -e "s/HEAD_JOINT0_y/$HEAD_JOINT0_y/g"       ./arm_robot_cut.x3d
sed -i -e "s/LARM_JOINT0_x/$LARM_JOINT0_x/g"       ./arm_robot_cut.x3d
sed -i -e "s/LARM_JOINT0_y/$LARM_JOINT0_y/g"       ./arm_robot_cut.x3d
sed -i -e "s/RARM_JOINT0_x/$RARM_JOINT0_x/g"       ./arm_robot_cut.x3d
sed -i -e "s/RARM_JOINT0_y/$RARM_JOINT0_y/g"       ./arm_robot_cut.x3d

sed -i -e "s/LARM_JOINT3_x/$LARM_JOINT3_SHORT_x/g" ./arm_robot_cut.x3d
sed -i -e "s/RARM_JOINT3_x/$RARM_JOINT3_SHORT_x/g" ./arm_robot_cut.x3d
sed -i -e "s/LARM_JOINT5_x/$LARM_JOINT5_SHORT_x/g" ./arm_robot_cut.x3d
sed -i -e "s/RARM_JOINT5_x/$RARM_JOINT5_SHORT_x/g" ./arm_robot_cut.x3d

sed -i -e "s/LARM_LINK2_x/$LARM_LINK2_SHORT_x/g"   ./arm_robot_cut.x3d
sed -i -e "s/RARM_LINK2_x/$RARM_LINK2_SHORT_x/g"   ./arm_robot_cut.x3d
sed -i -e "s/ARM_LINK4_LEN/$ARM_LINK4_SHORT_LEN/g" ./arm_robot_cut.x3d
sed -i -e "s/LARM_LINK4_x/$LARM_LINK4_SHORT_x/g"   ./arm_robot_cut.x3d
sed -i -e "s/RARM_LINK4_x/$RARM_LINK4_SHORT_x/g"   ./arm_robot_cut.x3d

