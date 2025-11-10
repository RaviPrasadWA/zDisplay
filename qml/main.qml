
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0

import zDisplay 1.0

import "./ui/elements"
import "./video"


ApplicationWindow {
    id: applicationWindow
    visible: true


    //property int m_window_width: 1280
    //property int m_window_height: 720
    property int m_window_width: 800 // This is 480p 16:9
    property int m_window_height: 480

    //width: 850
    //height: 480
    width: (settings.general_screen_rotation == 90 || settings.general_screen_rotation == 270) ? m_window_height : m_window_width
    height: (settings.general_screen_rotation == 90 || settings.general_screen_rotation == 270) ? m_window_width : m_window_height

    onWidthChanged: {
        _qrenderstats.set_window_width(width)
    }
    onHeightChanged: {
        _qrenderstats.set_window_height(height)
    }

    contentOrientation: settings.general_screen_rotation===0 ? Qt.PortraitOrientation : Qt.LandscapeOrientation
    contentItem.rotation: settings.general_screen_rotation

    //minimumWidth: 850
    //minimumHeight: 480
    title: qsTr("zDisplay")
    // Transparent background is needed when the video is not rendered via (OpenGL) inside QT,
    // but rather done independently by using a pipeline that directly goes to the HW composer (e.g. mmal on pi).
    //color: "transparent" //Consti10 transparent background
    //color : "#2C3E50" // reduce KREBS
    color: settings.app_background_transparent ? "transparent" : "#2C3E50"
    //flags: Qt.WindowStaysOnTopHint| Qt.FramelessWindowHint| Qt.X11BypassWindowManagerHint;
    //flags: Qt.WindowStaysOnTopHint| Qt.X11BypassWindowManagerHint;
    //visibility: "FullScreen"
    // android / ios - specifc: We need to explicitly say full screen, otherwise things might be "cut off"
    // visibility: (settings.dev_force_show_full_screen || QOPENHD_IS_MOBILE) ? "FullScreen" : "AutomaticVisibility"

    // Local app settings. Uses the "user defaults" system on Mac/iOS, the Registry on Windows,
    // and equivalent settings systems on Linux and Android
    // On linux, they generally are stored under /home/username/.config/Open.HD
    // See https://doc.qt.io/qt-5/qsettings.html#platform-specific-notes for more info
    AppSettings {
        id: settings
    }
    // This only exists to be able to fully rotate "everything" for users that have their screen upside down for some reason.
    // Won't affect the video, but heck, then just mount your camera upside down.
    // TODO: the better fix really would be to somehow the the RPI HDMI config to rotate the screen in HW - but r.n there seems to be
    // no way, at least on MMAL
    // NOTE: If this creates issues, just comment it out - I'd love to get rid of it as soon as we can.
    Item{
        // rotation: settings.general_screen_rotation
        anchors.centerIn: parent
        width: (settings.general_screen_rotation == 90 || settings.general_screen_rotation == 270) ? parent.height : parent.width
        height: (settings.general_screen_rotation == 90 || settings.general_screen_rotation == 270) ? parent.width : parent.height

        // Loads the proper (platform-dependent) video widget for the main (primary) video
        // primary video is always full-screen and behind the HUD OSD Elements
        Loader {
            anchors.fill: parent
            z: 1.0
            source: {
                return "../video/MainVideoQSG.qml";
            }
        }
    }
}
