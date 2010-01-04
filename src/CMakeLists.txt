file(GLOB SRCS *.cpp)

file(GLOB RCS *.qrc)
qt4_add_resources ( RC_SRCS ${RCS} )

file(GLOB UIS *.ui)
qt4_wrap_ui ( UI_HDRS ${UIS} )

file(GLOB MOC_HDRS *.h)
qt4_wrap_cpp ( MOC_SRCS ${MOC_HDRS} )
 
add_executable(tesseract-gui ${SRCS} ${RC_SRCS} ${UI_HDRS} ${MOC_SRCS})
target_link_libraries(tesseract-gui ${QT_LIBRARIES})
