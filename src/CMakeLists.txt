
file(GLOB_RECURSE USER_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.c)

target_sources(${PROJECT_NAME} PRIVATE
        ${USER_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/tasks
        ${CMAKE_CURRENT_LIST_DIR}/usbd_midi_class
        ${CMAKE_CURRENT_LIST_DIR}/mpu6050
)