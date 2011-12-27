/*
    Copyright 2007-2008 by Robert Knight <robertknight@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

// Own
#include "ShellCommand.h"

// KDE
#include <KShell>

using Konsole::ShellCommand;

// expands environment variables in 'text'
// function copied from kdelibs/kio/kio/kurlcompletion.cpp
static bool expandEnv(QString& text);

ShellCommand::ShellCommand(const QString& fullCommand)
{
    _arguments = KShell::splitArgs(fullCommand);
}
ShellCommand::ShellCommand(const QString& command , const QStringList& arguments)
{
    _arguments = arguments;

    if (!_arguments.isEmpty())
        _arguments[0] == command;
}
QString ShellCommand::fullCommand() const
{
    QStringList quotedArgs(_arguments);
    for (int i = 0; i < quotedArgs.count(); i++) {
        QString arg = quotedArgs.at(i);
        bool hasSpace = false;
        for (int j = 0; j < arg.count(); j++)
            if (arg[j].isSpace())
                hasSpace = true;
        if (hasSpace)
            quotedArgs[i] = '\"' + arg + '\"';
    }
    return quotedArgs.join(QChar(' '));
}
QString ShellCommand::command() const
{
    if (!_arguments.isEmpty())
        return _arguments[0];
    else
        return QString();
}
QStringList ShellCommand::arguments() const
{
    return _arguments;
}
bool ShellCommand::isRootCommand() const
{
    Q_ASSERT(0); // not implemented yet
    return false;
}
bool ShellCommand::isAvailable() const
{
    Q_ASSERT(0); // not implemented yet
    return false;
}
QStringList ShellCommand::expand(const QStringList& items)
{
    QStringList result;

    foreach(const QString & item , items)
    result << expand(item);

    return result;
}
QString ShellCommand::expand(const QString& text)
{
    QString result = text;
    expandEnv(result);
    return result;
}

/*
 * expandEnv
 *
 * Expand environment variables in text. Escaped '$' characters are ignored.
 * Return true if any variables were expanded
 */
static bool expandEnv(QString& text)
{
    // Find all environment variables beginning with '$'
    //
    int pos = 0;

    bool expanded = false;

    while ((pos = text.indexOf(QLatin1Char('$'), pos)) != -1) {

        // Skip escaped '$'
        //
        if (pos > 0 && text.at(pos - 1) == QLatin1Char('\\')) {
            pos++;
        }
        // Variable found => expand
        //
        else {
            // Find the end of the variable = next '/' or ' '
            //
            int pos2 = text.indexOf(QLatin1Char(' '), pos + 1);
            int pos_tmp = text.indexOf(QLatin1Char('/'), pos + 1);

            if (pos2 == -1 || (pos_tmp != -1 && pos_tmp < pos2))
                pos2 = pos_tmp;

            if (pos2 == -1)
                pos2 = text.length();

            // Replace if the variable is terminated by '/' or ' '
            // and defined
            //
            if (pos2 >= 0) {
                int len    = pos2 - pos;
                QString key    = text.mid(pos + 1, len - 1);
                QString value =
                    QString::fromLocal8Bit(qgetenv(key.toLocal8Bit()));

                if (!value.isEmpty()) {
                    expanded = true;
                    text.replace(pos, len, value);
                    pos = pos + value.length();
                } else {
                    pos = pos2;
                }
            }
        }
    }

    return expanded;
}
