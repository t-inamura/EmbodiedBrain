#!/bin/sh

#被験者パラメータ設定
#頭の高さ
#head_hight=13
head_hight=10
#腕の付け根までの長さ
#chest_len=16
chest_len=13

#前腕の開始位置
#upper_arm_len=25
upper_arm_len=27
#手首の開始位置
#forearm_len=29
forearm_len=20

#body scale (Because this body is too big.)
body_scale=0.7

#-------------------------------------------------------------

#アバター用パラメータ算出
#shoulder_height=`echo "scale=1; 44 / ${body_scale}" | bc`
shoulder_height=`echo "scale=1; 47 / ${body_scale}" | bc`
LARM_JOINT0_y=${shoulder_height}
RARM_JOINT0_y=${shoulder_height}

HEAD_JOINT0_y=`echo "scale=1; (${head_hight} * 0.7) / ${body_scale} + ${shoulder_height}" | bc`
LARM_JOINT0_x=`echo "scale=1; (${chest_len}  * 0.7) / ${body_scale}" | bc`
RARM_JOINT0_x=`echo "scale=1; (${chest_len}  *-0.7) / ${body_scale}" | bc`

LARM_LINK2_x=`echo "scale=1; (${LARM_JOINT0_x})+2" | bc`
RARM_LINK2_x=`echo "scale=1; (${RARM_JOINT0_x})-2" | bc`

LARM_JOINT3_x=`echo "scale=1; (${upper_arm_len}* 0.7) / ${body_scale}" | bc`
RARM_JOINT3_x=`echo "scale=1; (${upper_arm_len}*-0.7) / ${body_scale}" | bc`

LARM_LINK2_LONG_x=`echo "scale=1; (${LARM_JOINT0_x})+2" | bc`
RARM_LINK2_LONG_x=`echo "scale=1; (${RARM_JOINT0_x})-2" | bc`

LARM_JOINT3_LONG_x=`echo "scale=1; (${upper_arm_len}* 0.7) / ${body_scale}" | bc`
RARM_JOINT3_LONG_x=`echo "scale=1; (${upper_arm_len}*-0.7) / ${body_scale}" | bc`

LARM_LINK2_SHORT_x=`echo "scale=1; (${LARM_JOINT0_x})+2" | bc`
RARM_LINK2_SHORT_x=`echo "scale=1; (${RARM_JOINT0_x})-2" | bc`

LARM_JOINT3_SHORT_x=`echo "scale=1; (${upper_arm_len}* 0.7) / ${body_scale}" | bc`
RARM_JOINT3_SHORT_x=`echo "scale=1; (${upper_arm_len}*-0.7) / ${body_scale}" | bc`


#echo ${HEAD_JOINT0_y}
#echo ${LARM_JOINT0_x}
#echo ${LARM_JOINT3_x}
#echo ${LARM_JOINT5_x}
#echo ${ARM_LINK4_LEN}
#echo ${RARM_JOINT0_x}
#echo ${RARM_JOINT3_x}
#echo ${RARM_JOINT5_x}

# RightArm_mid, RightArm_lengthen, RightArm_cut の生成
. ./adjust_avatar_standard.sh

# RightArm_robot_mid, RightArm_robot_lengthen, RightArm_robot_cut の生成
. ./adjust_avatar_robot.sh

echo "finished"

