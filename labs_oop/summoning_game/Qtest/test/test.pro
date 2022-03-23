QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=                                                                      \
    main.cpp                                                                    \
    /Users/pavelshykov/Desktop/labs/summoning_game/summoning_game/Schools.cpp   \
    /Users/pavelshykov/Desktop/labs/summoning_game/summoning_game/Summoner.cpp  \
    /Users/pavelshykov/Desktop/labs/summoning_game/summoning_game/game.cpp      \
    /Users/pavelshykov/Desktop/labs/summoning_game/summoning_game/landscape.cpp \
    /Users/pavelshykov/Desktop/labs/summoning_game/summoning_game/menu.cpp      \
    /Users/pavelshykov/programming/MySpace/MySpace/MyLib.cpp                    \
    /Users/pavelshykov/Desktop/Qtest/test/button.cpp                            \
    /Users/pavelshykov/Desktop/Qtest/test/MyPushButton.cpp                      \
    /Users/pavelshykov/Desktop/Qtest/test/Warea.cpp                             \
    /Users/pavelshykov/Desktop/Qtest/test/fight.cpp                             \
    /Users/pavelshykov/Desktop/Qtest/test/fight_button.cpp

    

HEADERS += \
    /Users/pavelshykov/Desktop/labs/summoning_game/summoning_game/Schools.hpp   \
    /Users/pavelshykov/Desktop/labs/summoning_game/summoning_game/Summoner.hpp  \
    /Users/pavelshykov/Desktop/labs/summoning_game/summoning_game/game.hpp      \
    /Users/pavelshykov/Desktop/labs/summoning_game/summoning_game/landscape.hpp \
    /Users/pavelshykov/Desktop/labs/summoning_game/summoning_game/menu.hpp      \
    /Users/pavelshykov/programming/MySpace/MySpace/MyLib.hpp                    \
    /Users/pavelshykov/Desktop/Qtest/test/button.hpp                            \
    /Users/pavelshykov/Desktop/Qtest/test/MyPushButton.hpp                      \
    /Users/pavelshykov/Desktop/Qtest/test/Warea.hpp                             \
    /Users/pavelshykov/Desktop/Qtest/test/fight.hpp                             \
    /Users/pavelshykov/Desktop/Qtest/test/fight_button.hpp


FORMS += \
    menu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

