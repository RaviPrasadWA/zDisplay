#include <csignal>
#include <iostream>
#include <memory>
#include <cstdlib>

#include "qqmlcontext.h"
#include "qscreen.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QDebug>
#include <QFontDatabase>

#include "avcodec_helper.hpp"
#include "QOpenHDVideoHelper.hpp"
#include "rtpreceiver.h"
#include "qopenhd.h"
#include "qrenderstats.h"
#include "mousehelper.h"
#include "decodingstatistcs.h"
#include "TimeHelper.hpp"
#include "avcodec.h"
#include "QSGVideoTextureItem.h"


static constexpr std::chrono::milliseconds kDefaultFrameTimeout{33*2};

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("zDisplay");
    QCoreApplication::setOrganizationDomain("zdisplay.org");
    QCoreApplication::setApplicationName("zDisplay");
    {// Original screen resoluton before setting anything
        //QApplication a(argc, argv);
        const auto screen=QGuiApplication::primaryScreen();
        if(screen){
            const auto actual_size=screen->size();
            QRenderStats::instance().set_screen_width_height(actual_size.width(),actual_size.height());
        }
        // a is deleted again
    }
    
    QSettings settings;
    qDebug()<<"Storing settings at ["<<settings.fileName()<<"]";
    // RPI and ROCK - disable font dpi. The user has to scale manually when using displays
    // (Big screens) according to its preferences. Auto scale is just bugged,
    // nothing more to say.
    if(QOpenHD::instance().is_platform_rpi() || QOpenHD::instance().is_platform_rock()){
        static constexpr auto TAG_QOPENHD_INITIAL_FONT_DPI_HAS_BEEN_SET="qopenhd_initial_font_dpi_has_been_set";
        if(!settings.value(TAG_QOPENHD_INITIAL_FONT_DPI_HAS_BEEN_SET,false).toBool()){
            qDebug()<<"RPI/ROCK: Disable font dpi by default with setting it to 100%";
            settings.setValue(TAG_QOPENHD_INITIAL_FONT_DPI_HAS_BEEN_SET,true);
            settings.setValue("screen_custom_font_dpi",100);
        }
    }
    if(QOpenHD::instance().is_platform_rock()){
        if(!settings.value("dev_rpi_use_external_omx_decode_service",true).toBool()){
            qDebug()<<"Disabling RPI decode!";
            settings.setValue("dev_rpi_use_external_omx_decode_service",false);
        }
        if(!settings.value("dev_always_use_generic_external_decode_service",false).toBool()){
            qDebug()<<"Enable rockchip HW decoding!";
            settings.setValue("dev_always_use_generic_external_decode_service",true);
        }
    }
    const int screen_custom_font_dpi = settings.value("screen_custom_font_dpi").toInt();
    if(screen_custom_font_dpi<0){
        // Disabled
        QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    }else if(screen_custom_font_dpi==0){
        // Enabled (whatever qt thinks it wanna do on auto). Works on android, on other devices, meh
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }else{
        // Custom font dpi set by the user
        QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
        const std::string font_dpi_s = std::to_string(screen_custom_font_dpi);
        qputenv("QT_FONT_DPI", QByteArray(font_dpi_s.c_str(), font_dpi_s.length()));
    }
    //QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    //QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    const double global_scale = settings.value("global_scale", 1.0).toDouble();
    const std::string global_scale_s = std::to_string(global_scale);
    qputenv("QT_SCALE_FACTOR", QByteArray(global_scale_s.c_str(), global_scale_s.length()));

    QApplication app(argc, argv);
    QOpenHDVideoHelper::reset_qopenhd_switch_primary_secondary();

    QOpenHD::instance().keep_screen_on(true);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("_qopenhd", &QOpenHD::instance());
    engine.rootContext()->setContextProperty("_qrenderstats", &QRenderStats::instance());
    QOpenHD::instance().setEngine(&engine);

    // QT doesn't have the define(s) from c++
    engine.rootContext()->setContextProperty("QOPENHD_ENABLE_VIDEO_VIA_AVCODEC", QVariant(true));
    qmlRegisterType<QSGVideoTextureItem>("zDisplay", 1, 0, "QSGVideoTextureItem");

    //platform_start_audio_streaming_if_enabled();

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);

    qDebug() << "QML loaded";
    const int retval = app.exec();

    return retval;
}