QT       += core gui network sql core-private multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fileselect.cpp \
    httpclass.cpp \
    main.cpp \
    homepage.cpp \
    petinfo.cpp \
    publish.cpp \
    publishreply.cpp \
    publishres.cpp \
    reply.cpp \
    resetpwd.cpp \
    resource.cpp \
    userdb.cpp \
    userlogin.cpp \
    userresource.cpp \
    usersignup.cpp

HEADERS += \
    fileselect.h \
    homepage.h \
    httpclass.h \
    petinfo.h \
    publish.h \
    publishreply.h \
    publishres.h \
    reply.h \
    resetpwd.h \
    resource.h \
    userdb.h \
    userlogin.h \
    userresource.h \
    usersignup.h

FORMS += \
    homepage.ui \
    petinfo.ui \
    publish.ui \
    publishreply.ui \
    publishres.ui \
    reply.ui \
    resetpwd.ui \
    resource.ui \
    userlogin.ui \
    userresource.ui \
    usersignup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/src/com/amin/QtAndroidGallery/QtAndroidGallery.java

RESOURCES += \
    res.qrc

android{
    data.files += database/pet_social.db
    data.path = /assets/database/
    INSTALLS += data
}

contains(ANDROID_TARGET_ARCH,x86) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
android: include(D:/Andriod/openssl/android_openssl-master/openssl.pri)
