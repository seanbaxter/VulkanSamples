file(GLOB UTILS_SOURCE *.cpp)

set(SAMPLES_DATA_DIR ${SAMPLES_DATA_DIR} "${PROJECT_SOURCE_DIR}/API-Samples/data")
if(SDK_INCLUDE_PATH)
    include_directories( ${SAMPLES_DATA_DIR} ${GLSLANG_SPIRV_INCLUDE_DIR} ${GLMINC_PREFIX} ${SDK_INCLUDE_PATH} )
else()
    include_directories( ${SAMPLES_DATA_DIR} ${GLSLANG_SPIRV_INCLUDE_DIR} ${GLMINC_PREFIX} )
endif()


add_library(${UTILS_NAME} STATIC ${UTILS_SOURCE})

if(ANDROID)
   add_library(native_app_glue STATIC
               ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
   target_include_directories(${UTILS_NAME} PRIVATE
                              ${ANDROID_NDK}/sources/android/native_app_glue
                              ${CMAKE_CURRENT_SOURCE_DIR}/../android/vulkan_wrapper
                              ${CMAKE_CURRENT_SOURCE_DIR}/../data)
   target_link_libraries(${UTILS_NAME}
                              android
                              log
                              native_app_glue)
endif()

