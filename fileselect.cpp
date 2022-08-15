#include "fileselect.h"

QString selectedFileName;

#ifdef Q_OS_ANDROID
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_amin_QtAndroidGallery_QtAndroidGallery_fileSelected(JNIEnv */*env*/,
                                                             jobject /*obj*/,
                                                             jstring results)
{
    selectedFileName = QJniObject(results).toString();
}

#ifdef __cplusplus
}
#endif
#endif

FileSelect::FileSelect(QObject *parent)
    : QObject{parent}
{

}

QString FileSelect::selectFile()
{
    selectedFileName = "#";
    #ifdef Q_OS_ANDROID
    QJniObject::callStaticMethod<void>("com/amin/QtAndroidGallery/QtAndroidGallery",
                                              "openAnImage",
                                              "()V");

    while(selectedFileName == "#")
        qApp->processEvents();

    #endif

    return selectedFileName;
}


