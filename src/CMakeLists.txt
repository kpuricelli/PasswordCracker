# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
   SrcMain.h
   sha1.h
	PasswordCracker.h
)

set(SOURCE_FILES
   SrcMain.cpp
   sha1.cpp
	PasswordCracker.cpp
)

# Don't change this
add_library(src ${SOURCE_FILES} ${HEADER_FILES})
