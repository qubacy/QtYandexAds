import QtQuick 2.15
import QtQuick.Window 2.15

import com.test.QmlYandexAdsBanner 1.0

Window {
    width: 640
    height: 480
    
    visible: true
    title: qsTr("Hello World")
    
    YandexAdsBanner {
        Component.onCompleted: {
            console.log("Yandex banner has been created!");
        }
    }
}
