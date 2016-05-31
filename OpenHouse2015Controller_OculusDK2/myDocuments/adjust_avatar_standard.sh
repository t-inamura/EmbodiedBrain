#アバター用パラメータ算出
#長い手の前腕の倍率
long_ratio=2.0
#短い手の前腕の倍率
short_ratio=0.5

ARM_LINK4_LEN=`echo       "scale=1; 60.0*1.0" | bc`
ARM_LINK4_LONG_LEN=`echo  "scale=1; 60.0*${long_ratio}" | bc`
ARM_LINK4_SHORT_LEN=`echo "scale=1; 60.0*${short_ratio}" | bc`

LARM_LINK4_x=`echo       "scale=1; ( 7)" | bc`
RARM_LINK4_x=`echo       "scale=1; (-7)" | bc`
LARM_LINK4_LONG_x=`echo  "scale=1; ( 7) * ${long_ratio}" | bc`
RARM_LINK4_LONG_x=`echo  "scale=1; (-7) * ${long_ratio}" | bc`
LARM_LINK4_SHORT_x=`echo "scale=1; ( 7) * ${short_ratio}" | bc`
RARM_LINK4_SHORT_x=`echo "scale=1; (-7) * ${short_ratio}" | bc`

LARM_JOINT5_x=`echo       "scale=1; (${forearm_len} * 0.7) / ${body_scale}" | bc`
RARM_JOINT5_x=`echo       "scale=1; (${forearm_len} *-0.7) / ${body_scale}" | bc`
LARM_JOINT5_LONG_x=`echo  "scale=1; (${forearm_len} * 0.7) / ${body_scale} * ${long_ratio}" | bc`
RARM_JOINT5_LONG_x=`echo  "scale=1; (${forearm_len} *-0.7) / ${body_scale} * ${long_ratio}" | bc`
LARM_JOINT5_SHORT_x=`echo "scale=1; (${forearm_len} * 0.7) / ${body_scale} * ${short_ratio}" | bc`
RARM_JOINT5_SHORT_x=`echo "scale=1; (${forearm_len} *-0.7) / ${body_scale} * ${short_ratio}" | bc`


# Man-nii_head.xmlの生成
cp ./Man-nii_head_src.xml ./Man-nii_head.xml
chmod 666 ./Man-nii_head.xml

sed -i -e "s/SCALEX/$body_scale/g" ./Man-nii_head.xml
sed -i -e "s/SCALEY/$body_scale/g" ./Man-nii_head.xml
sed -i -e "s/SCALEZ/$body_scale/g" ./Man-nii_head.xml
sed -i -e "s/X3DFILENAME/man_nii_v2_head/g" ./Man-nii_head.xml

# man_nii_v2_head.x3dの生成
cp ./man_nii_v2_head_src.x3d ./man_nii_v2_head.x3d
chmod 666 ./man_nii_v2_head.x3d

sed -i -e "s/HEAD_JOINT0_y/$HEAD_JOINT0_y/g" ./man_nii_v2_head.x3d
sed -i -e "s/LARM_JOINT0_x/$LARM_JOINT0_x/g" ./man_nii_v2_head.x3d
sed -i -e "s/LARM_JOINT0_y/$LARM_JOINT0_y/g" ./man_nii_v2_head.x3d
sed -i -e "s/LARM_JOINT3_x/$LARM_JOINT3_x/g" ./man_nii_v2_head.x3d
sed -i -e "s/LARM_JOINT5_x/$LARM_JOINT5_x/g" ./man_nii_v2_head.x3d
sed -i -e "s/RARM_JOINT0_x/$RARM_JOINT0_x/g" ./man_nii_v2_head.x3d
sed -i -e "s/RARM_JOINT0_y/$RARM_JOINT0_y/g" ./man_nii_v2_head.x3d
sed -i -e "s/RARM_JOINT3_x/$RARM_JOINT3_x/g" ./man_nii_v2_head.x3d
sed -i -e "s/RARM_JOINT5_x/$RARM_JOINT5_x/g" ./man_nii_v2_head.x3d



# arm_mid.xmlの生成
cp ./arm_standard_src.xml ./arm_mid.xml
chmod 666 ./arm_mid.xml

sed -i -e "s/SCALEX/$body_scale/g" ./arm_mid.xml
sed -i -e "s/SCALEY/$body_scale/g" ./arm_mid.xml
sed -i -e "s/SCALEZ/$body_scale/g" ./arm_mid.xml
sed -i -e "s/X3DFILENAME/arm_mid/g" ./arm_mid.xml

# arm_mid.x3dの生成
cp ./arm_standard_src.x3d ./arm_mid.x3d
chmod 666 ./arm_mid.x3d

sed -i -e "s/HEAD_JOINT0_y/$HEAD_JOINT0_y/g" ./arm_mid.x3d
sed -i -e "s/LARM_JOINT0_x/$LARM_JOINT0_x/g" ./arm_mid.x3d
sed -i -e "s/LARM_JOINT0_y/$LARM_JOINT0_y/g" ./arm_mid.x3d
sed -i -e "s/RARM_JOINT0_x/$RARM_JOINT0_x/g" ./arm_mid.x3d
sed -i -e "s/RARM_JOINT0_y/$RARM_JOINT0_y/g" ./arm_mid.x3d

sed -i -e "s/LARM_JOINT3_x/$LARM_JOINT3_x/g" ./arm_mid.x3d
sed -i -e "s/RARM_JOINT3_x/$RARM_JOINT3_x/g" ./arm_mid.x3d
sed -i -e "s/LARM_JOINT5_x/$LARM_JOINT5_x/g" ./arm_mid.x3d
sed -i -e "s/RARM_JOINT5_x/$RARM_JOINT5_x/g" ./arm_mid.x3d

sed -i -e "s/LARM_LINK2_x/$LARM_LINK2_x/g"   ./arm_mid.x3d
sed -i -e "s/RARM_LINK2_x/$RARM_LINK2_x/g"   ./arm_mid.x3d
sed -i -e "s/ARM_LINK4_LEN/$ARM_LINK4_LEN/g" ./arm_mid.x3d
sed -i -e "s/LARM_LINK4_x/$LARM_LINK4_x/g"   ./arm_mid.x3d
sed -i -e "s/RARM_LINK4_x/$RARM_LINK4_x/g"   ./arm_mid.x3d


# arm_long.xmlの生成
cp ./arm_standard_src.xml ./arm_long.xml
chmod 666 ./arm_long.xml

sed -i -e "s/SCALEX/$body_scale/g" ./arm_long.xml
sed -i -e "s/SCALEY/$body_scale/g" ./arm_long.xml
sed -i -e "s/SCALEZ/$body_scale/g" ./arm_long.xml
sed -i -e "s/X3DFILENAME/arm_long/g" ./arm_long.xml

# arm_long.x3dの生成
cp ./arm_standard_src.x3d ./arm_long.x3d
chmod 666 ./arm_long.x3d

sed -i -e "s/HEAD_JOINT0_y/$HEAD_JOINT0_y/g"      ./arm_long.x3d
sed -i -e "s/LARM_JOINT0_x/$LARM_JOINT0_x/g"      ./arm_long.x3d
sed -i -e "s/LARM_JOINT0_y/$LARM_JOINT0_y/g"      ./arm_long.x3d
sed -i -e "s/RARM_JOINT0_x/$RARM_JOINT0_x/g"      ./arm_long.x3d
sed -i -e "s/RARM_JOINT0_y/$RARM_JOINT0_y/g"      ./arm_long.x3d

sed -i -e "s/LARM_JOINT3_x/$LARM_JOINT3_LONG_x/g" ./arm_long.x3d
sed -i -e "s/RARM_JOINT3_x/$RARM_JOINT3_LONG_x/g" ./arm_long.x3d
sed -i -e "s/LARM_JOINT5_x/$LARM_JOINT5_LONG_x/g" ./arm_long.x3d
sed -i -e "s/RARM_JOINT5_x/$RARM_JOINT5_LONG_x/g" ./arm_long.x3d

sed -i -e "s/LARM_LINK2_x/$LARM_LINK2_LONG_x/g"   ./arm_long.x3d
sed -i -e "s/RARM_LINK2_x/$RARM_LINK2_LONG_x/g"   ./arm_long.x3d
sed -i -e "s/ARM_LINK4_LEN/$ARM_LINK4_LONG_LEN/g" ./arm_long.x3d
sed -i -e "s/LARM_LINK4_x/$LARM_LINK4_LONG_x/g"   ./arm_long.x3d
sed -i -e "s/RARM_LINK4_x/$RARM_LINK4_LONG_x/g"   ./arm_long.x3d


# arm_short.xmlの生成
cp ./arm_standard_src.xml ./arm_short.xml
chmod 666 ./arm_short.xml

sed -i -e "s/SCALEX/$body_scale/g" ./arm_short.xml
sed -i -e "s/SCALEY/$body_scale/g" ./arm_short.xml
sed -i -e "s/SCALEZ/$body_scale/g" ./arm_short.xml
sed -i -e "s/X3DFILENAME/arm_short/g" ./arm_short.xml

# arm_short.x3dの生成
cp ./arm_standard_src.x3d ./arm_short.x3d
chmod 666 ./arm_short.x3d

sed -i -e "s/HEAD_JOINT0_y/$HEAD_JOINT0_y/g"       ./arm_short.x3d
sed -i -e "s/LARM_JOINT0_x/$LARM_JOINT0_x/g"       ./arm_short.x3d
sed -i -e "s/LARM_JOINT0_y/$LARM_JOINT0_y/g"       ./arm_short.x3d
sed -i -e "s/RARM_JOINT0_x/$RARM_JOINT0_x/g"       ./arm_short.x3d
sed -i -e "s/RARM_JOINT0_y/$RARM_JOINT0_y/g"       ./arm_short.x3d

sed -i -e "s/LARM_JOINT3_x/$LARM_JOINT3_SHORT_x/g" ./arm_short.x3d
sed -i -e "s/RARM_JOINT3_x/$RARM_JOINT3_SHORT_x/g" ./arm_short.x3d
sed -i -e "s/LARM_JOINT5_x/$LARM_JOINT5_SHORT_x/g" ./arm_short.x3d
sed -i -e "s/RARM_JOINT5_x/$RARM_JOINT5_SHORT_x/g" ./arm_short.x3d

sed -i -e "s/LARM_LINK2_x/$LARM_LINK2_SHORT_x/g"   ./arm_short.x3d
sed -i -e "s/RARM_LINK2_x/$RARM_LINK2_SHORT_x/g"   ./arm_short.x3d
sed -i -e "s/ARM_LINK4_LEN/$ARM_LINK4_SHORT_LEN/g" ./arm_short.x3d
sed -i -e "s/LARM_LINK4_x/$LARM_LINK4_SHORT_x/g"   ./arm_short.x3d
sed -i -e "s/RARM_LINK4_x/$RARM_LINK4_SHORT_x/g"   ./arm_short.x3d

