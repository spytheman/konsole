/*
 * This file was generated by dbusidl2cpp version 0.5
 * when processing input file org.kde.konsole.Konsole.Scripting.xml
 *
 * dbusidl2cpp is Copyright (C) 2006 Trolltech AS. All rights reserved.
 *
 * This is an auto-generated file.
 */

#include "konsolescriptingadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class KonsoleScriptingAdaptor
 */

KonsoleScriptingAdaptor::KonsoleScriptingAdaptor(QObject *parent)
   : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

KonsoleScriptingAdaptor::~KonsoleScriptingAdaptor()
{
    // destructor
}

void KonsoleScriptingAdaptor::feedSession(const QString &text)
{
    // handle method call org.kde.konsole.Konsole.Scripting.feedSession
    QMetaObject::invokeMethod(parent(), "feedSession", Q_ARG(QString, text));

    // Alternative:
    //static_cast<YourObjectType *>(parent())->feedSession(text);
}

void KonsoleScriptingAdaptor::sendSession(const QString &text)
{
    // handle method call org.kde.konsole.Konsole.Scripting.sendSession
    QMetaObject::invokeMethod(parent(), "sendSession", Q_ARG(QString, text));

    // Alternative:
    //static_cast<YourObjectType *>(parent())->sendSession(text);
}


#include "konsolescriptingadaptor.moc"