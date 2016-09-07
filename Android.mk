ifneq (,$(filter $(TARGET_ARCH), arm arm64))

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

#[FEATURE]Add-BEGIN by TCTSH.chunlei.zhang 2015/12/31 for camera tsHDR feature support
ifeq ($(filter true, $(TARGET_IDOL4_SUPPORT_TSHDR) $(TARGET_IDOL455_SUPPORT_TSHDR) $(TARGET_IDOL4S_SUPPORT_TSHDR) $(TARGET_IDOL4S_VDF_SUPPORT_TSHDR) $(TARGET_IDOL4_BELL_SUPPORT_TSHDR)),true)
TS_PATH := HAL/ts

include $(CLEAR_VARS)
LOCAL_MODULE        := libtsvideoprocess
LOCAL_MODULE_CLASS  := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
LOCAL_MULTILIB      := 32
LOCAL_MODULE_TAGS   := optional
LOCAL_SRC_FILES     := $(TS_PATH)/libtsvideoprocess.so
LOCAL_MODULE_PATH   := $(PRODUCT_OUT)/system/lib
LOCAL_PROPRIETARY_MODULE := true
include $(BUILD_PREBUILT)
endif
#[FEATURE]Add-END by TCTSH.chunlei.zhang

ifeq ($(strip $(TCT_TARGET_EIS_DXO_ENABLE)),true)
$(warning TCT_TARGET_EIS_DXO_ENABLE = $(TCT_TARGET_EIS_DXO_ENABLE))
ifeq ($(strip $(TARGET_IDOL4_CAMERA_USE_ONLY)),true)
$(warning DXO IDOL4)
EIS_PATH := HAL/DxOEIS_idol4
else ifeq ($(strip $(TARGET_IDOL4S_CAMERA_USE_ONLY)),true)
$(warning DXO IDOL4S)
EIS_PATH := HAL/DxOEIS_idol4s
else ifeq ($(strip $(TARGET_IDOL4S_VDF_CAMERA_USE_ONLY)),true)
$(warning DXO IDOL4S_VDF)
EIS_PATH := HAL/DxOEIS_idol4s_vdf
# MODIFIED-BEGIN by yinsheng.fu, 2016-06-22,BUG-2216601
else ifeq ($(strip $(TARGET_IDOL4_BELL_CAMERA_USE_ONLY)),true)
$(warning DXO IDOL4_BELL)
EIS_PATH := HAL/DxOEIS_idol4_bell
# MODIFIED-END by yinsheng.fu,BUG-2216601
endif
include $(CLEAR_VARS)
LOCAL_MODULE	    := DxOEIS
LOCAL_MODULE_CLASS  := STATIC_LIBRARIES
LOCAL_SRC_FILES     := $(EIS_PATH)/DxOEIS.a
LOCAL_MULTILIB      := 32
LOCAL_MODULE_SUFFIX := .a
LOCAL_MODULE_TAGS   := optional
LOCAL_PROPRIETARY_MODULE := true
LOCAL_2ND_ARCH_VAR_PREFIX := arm
include $(BUILD_PREBUILT)
endif

#[FEATURE]Add-BEGIN by TCTSH.zhenhuan.fan 2015/11/18 for camera visidon feature support
# MODIFIED-BEGIN by yinsheng.fu, 2016-06-22,BUG-2216601
ifeq ($(filter true, $(TARGET_IDOL4_SUPPORT_VISIDON) $(TARGET_IDOL455_SUPPORT_VISIDON) $(TARGET_IDOL4S_SUPPORT_VISIDON) $(TARGET_IDOL4S_VDF_SUPPORT_VISIDON) $(TARGET_IDOL4_BELL_SUPPORT_VISIDON)),true)
VISIDON_PATH := Visidon

include $(CLEAR_VARS)
# MODIFIED-END by yinsheng.fu,BUG-2216601
LOCAL_MODULE        := libVDSuperPhotoAPI
LOCAL_MODULE_CLASS  := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
LOCAL_MULTILIB      := 32
LOCAL_MODULE_TAGS   := optional
LOCAL_SRC_FILES     := $(VISIDON_PATH)/libVDSuperPhotoAPI.so
LOCAL_MODULE_PATH   := $(PRODUCT_OUT)/system/lib
LOCAL_PROPRIETARY_MODULE := true
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE        := libVDLowLightAPI
LOCAL_MODULE_CLASS  := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
LOCAL_MULTILIB      := 32
LOCAL_MODULE_TAGS   := optional
LOCAL_SRC_FILES     := $(VISIDON_PATH)/libVDLowLightAPI.so
LOCAL_MODULE_PATH   := $(PRODUCT_OUT)/system/lib
LOCAL_PROPRIETARY_MODULE := true
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE        := libVDHDRAPI
LOCAL_MODULE_CLASS  := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
LOCAL_MULTILIB      := 32
LOCAL_MODULE_TAGS   := optional
LOCAL_SRC_FILES     := $(VISIDON_PATH)/libVDHDRAPI.so
LOCAL_MODULE_PATH   := $(PRODUCT_OUT)/system/lib
LOCAL_PROPRIETARY_MODULE := true
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE        := libVDBeautyShotAPI
LOCAL_MODULE_CLASS  := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
LOCAL_MULTILIB      := 32
LOCAL_MODULE_TAGS   := optional
LOCAL_SRC_FILES     := $(VISIDON_PATH)/libVDBeautyShotAPI.so
LOCAL_MODULE_PATH   := $(PRODUCT_OUT)/system/lib
LOCAL_PROPRIETARY_MODULE := true
include $(BUILD_PREBUILT)
endif
#[FEATURE]Add-END by TCTSH.zhenhuan.fan

LOCAL_SRC_FILES := \
        util/QCameraCmdThread.cpp \
        util/QCameraQueue.cpp \
        util/QCameraBufferMaps.cpp \
        QCamera2Hal.cpp \
        QCamera2Factory.cpp

#HAL 3.0 source
LOCAL_SRC_FILES += \
        HAL3/QCamera3HWI.cpp \
        HAL3/QCamera3Mem.cpp \
        HAL3/QCamera3Stream.cpp \
        HAL3/QCamera3Channel.cpp \
        HAL3/QCamera3VendorTags.cpp \
        HAL3/QCamera3PostProc.cpp \
        HAL3/QCamera3CropRegionMapper.cpp

#HAL 1.0 source
LOCAL_SRC_FILES += \
        HAL/QCamera2HWI.cpp \
        HAL/QCameraMuxer.cpp \
        HAL/QCameraMem.cpp \
        HAL/QCameraStateMachine.cpp \
        HAL/QCameraChannel.cpp \
        HAL/QCameraStream.cpp \
        HAL/QCameraPostProc.cpp \
        HAL/QCamera2HWICallbacks.cpp \
        HAL/QCameraParameters.cpp \
        HAL/QCameraThermalAdapter.cpp

LOCAL_CFLAGS := -Wall -Wextra -Werror
LOCAL_CFLAGS += -DHAS_MULTIMEDIA_HINTS

#HAL 1.0 Flags
LOCAL_CFLAGS += -DDEFAULT_DENOISE_MODE_ON -DHAL3

LOCAL_C_INCLUDES := \
        $(LOCAL_PATH)/stack/common \
        frameworks/native/include/media/hardware \
        frameworks/native/include/media/openmax \
        hardware/qcom/media/libstagefrighthw \
        system/media/camera/include \
        $(LOCAL_PATH)/../mm-image-codec/qexif \
        $(LOCAL_PATH)/../mm-image-codec/qomx_core \
        $(LOCAL_PATH)/util \
        hardware/qcom/media/mm-core/inc \

#HAL 1.0 Include paths
LOCAL_C_INCLUDES += \
        frameworks/native/include/media/hardware \
        hardware/qcom/camera/QCamera2/HAL

ifeq ($(TARGET_COMPILE_WITH_MSM_KERNEL),true)
LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
endif
ifeq ($(TARGET_TS_MAKEUP),true)
LOCAL_CFLAGS += -DTARGET_TS_MAKEUP
LOCAL_C_INCLUDES += $(LOCAL_PATH)/HAL/tsMakeuplib/include
endif
ifneq (,$(filter msm8974 msm8916 msm8226 msm8610 msm8916 apq8084 msm8084 msm8994 msm8992 msm8952 msm8996,$(TARGET_BOARD_PLATFORM)))
    LOCAL_CFLAGS += -DVENUS_PRESENT
endif

ifneq (,$(filter msm8996,$(TARGET_BOARD_PLATFORM)))
    LOCAL_CFLAGS += -DUBWC_PRESENT
endif

ifeq (1,$(filter 1,$(shell echo "$$(( $(PLATFORM_SDK_VERSION) <= 22 ))" )))
LOCAL_CFLAGS += -DUSE_L_MR1
endif

#LOCAL_STATIC_LIBRARIES := libqcamera2_util
LOCAL_C_INCLUDES += \
        $(TARGET_OUT_HEADERS)/qcom/display
LOCAL_C_INCLUDES += \
        hardware/qcom/display/libqservice
# MODIFIED-BEGIN by yinsheng.fu, 2016-06-22,BUG-2216601
ifeq ($(strip $(TCT_TARGET_EIS_DXO_ENABLE)),true)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(EIS_PATH)
endif
LOCAL_SHARED_LIBRARIES := libcamera_client liblog libhardware libutils libcutils libdl
LOCAL_SHARED_LIBRARIES += libmmcamera_interface libmmjpeg_interface libui libcamera_metadata

#[FEATURE]Add-BEGIN by TCTSH.chunlei.zhang 2015/12/31 for camera tsHDR feature support
ifeq ($(filter true, $(TARGET_IDOL4_SUPPORT_TSHDR) $(TARGET_IDOL455_SUPPORT_TSHDR) $(TARGET_IDOL4S_SUPPORT_TSHDR) $(TARGET_IDOL4S_VDF_SUPPORT_TSHDR) $(TARGET_IDOL4_BELL_SUPPORT_TSHDR)),true)
# MODIFIED-END by yinsheng.fu,BUG-2216601
LOCAL_C_INCLUDES += $(LOCAL_PATH)/HAL/ts
LOCAL_SHARED_LIBRARIES += libtsvideoprocess
LOCAL_CFLAGS += -DTCT_TSHDR_FEATURE
endif
#[FEATURE]Add-END by TCTSH.chunlei.zhang

#[FEATURE]Add-BEGIN by TCTSH.zhenhuan.fan 2015/11/18 for camera visidon feature support
ifeq ($(filter true, $(TARGET_IDOL4_SUPPORT_VISIDON) $(TARGET_IDOL455_SUPPORT_VISIDON) $(TARGET_IDOL4S_SUPPORT_VISIDON) $(TARGET_IDOL4S_VDF_SUPPORT_VISIDON) $(TARGET_IDOL4_BELL_SUPPORT_VISIDON)),true)
LOCAL_SHARED_LIBRARIES += libVDSuperPhotoAPI libVDHDRAPI libVDLowLightAPI libVDBeautyShotAPI
LOCAL_CFLAGS += -DTCT_VISIDON_FEATURE
endif
#[FEATURE]Add-END by TCTSH.zhenhuan.fan

LOCAL_SHARED_LIBRARIES += libqdMetaData libqservice libbinder
ifeq ($(TARGET_TS_MAKEUP),true)
LOCAL_SHARED_LIBRARIES += libts_face_beautify_hal libts_detected_face_hal
endif
#modified by xiaoming.hu@tcl.com add project macro
ifeq ($(strip $(TCT_TARGET_EIS_DXO_ENABLE)),true)
LOCAL_SHARED_LIBRARIES += libandroid
LOCAL_STATIC_LIBRARIES += DxOEIS
endif
ifeq ($(strip $(TCT_TARGET_EIS_DXO_ENABLE)),true)
ifeq ($(strip $(TARGET_IDOL4_CAMERA_USE_ONLY)),true)
LOCAL_CFLAGS += -DTCT_TARGET_HW_CAMERA_IDOL4_USE_ONLY
else ifeq ($(strip $(TARGET_IDOL4S_CAMERA_USE_ONLY)),true)
LOCAL_CFLAGS += -DTCT_TARGET_HW_CAMERA_IDOL4S_USE_ONLY
else ifeq ($(strip $(TARGET_IDOL4S_VDF_CAMERA_USE_ONLY)),true)
LOCAL_CFLAGS += -DTCT_TARGET_HW_CAMERA_IDOL4S_VDF_USE_ONLY
# MODIFIED-BEGIN by yinsheng.fu, 2016-06-22,BUG-2216601
else ifeq ($(strip $(TARGET_IDOL4_BELL_CAMERA_USE_ONLY)),true)
LOCAL_CFLAGS += -DTCT_TARGET_HW_CAMERA_IDOL4_BELL_USE_ONLY
# MODIFIED-END by yinsheng.fu,BUG-2216601
endif
endif
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE := camera.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_TAGS := optional

LOCAL_32_BIT_ONLY := $(BOARD_QTI_CAMERA_32BIT_ONLY)
include $(BUILD_SHARED_LIBRARY)

include $(call first-makefiles-under,$(LOCAL_PATH))

endif
