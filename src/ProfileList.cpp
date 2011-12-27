/*
    Copyright 2006-2008 by Robert Knight <robertknight@gmail.com>

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
#include "ProfileList.h"

// Qt
#include <QtGui/QAction>
#include <QtGui/QActionGroup>

// KDE
#include <KIcon>
#include <KLocalizedString>
#include <KDebug>

// Konsole
#include "SessionManager.h"

using Konsole::ProfileList;

ProfileList::ProfileList(bool addShortcuts , QObject* parent)
    : QObject(parent)
    , _addShortcuts(addShortcuts)
    , _emptyListAction(0)
{
    // construct the list of favorite profiles
    _group = new QActionGroup(this);

    // Even when there are no favorite profiles, allow user to
    // create new tabs using the default profile from the menu
    _emptyListAction = new QAction(i18n("Default profile"), _group);

    // TODO - Handle re-sorts when user changes profile names
    SessionManager* manager = SessionManager::instance();
    QList<Profile::Ptr> list = manager->sortedFavorites();

    QListIterator<Profile::Ptr> iter(list);

    while (iter.hasNext()) {
        favoriteChanged(iter.next(), true);
    }

    connect(_group, SIGNAL(triggered(QAction*)), this, SLOT(triggered(QAction*)));


    // listen for future changes to the profiles
    connect(manager, SIGNAL(favoriteStatusChanged(Profile::Ptr,bool)), this,
            SLOT(favoriteChanged(Profile::Ptr,bool)));
    connect(manager, SIGNAL(shortcutChanged(Profile::Ptr,QKeySequence)), this,
            SLOT(shortcutChanged(Profile::Ptr,QKeySequence)));
    connect(manager, SIGNAL(profileChanged(Profile::Ptr)), this,
            SLOT(profileChanged(Profile::Ptr)));
}
void ProfileList::updateEmptyAction()
{
    Q_ASSERT(_group);
    Q_ASSERT(_emptyListAction);

    // show this action only when it is the only action in the group
    const bool showEmptyAction = (_group->actions().count() == 1);

    if (showEmptyAction != _emptyListAction->isVisible())
        _emptyListAction->setVisible(showEmptyAction);
}
QAction* ProfileList::actionForKey(Profile::Ptr key) const
{
    QListIterator<QAction*> iter(_group->actions());
    while (iter.hasNext()) {
        QAction* next = iter.next();
        if (next->data().value<Profile::Ptr>() == key)
            return next;
    }
    return 0; // not found
}

void ProfileList::profileChanged(Profile::Ptr key)
{
    QAction* action = actionForKey(key);
    if (action)
        updateAction(action, key);
}

void ProfileList::updateAction(QAction* action , Profile::Ptr info)
{
    Q_ASSERT(action);
    Q_ASSERT(info);

    action->setText(info->name());
    action->setIcon(KIcon(info->icon()));
}
void ProfileList::shortcutChanged(Profile::Ptr info, const QKeySequence& sequence)
{
    if (!_addShortcuts)
        return;

    QAction* action = actionForKey(info);

    if (action) {
        action->setShortcut(sequence);
    }
}
void ProfileList::syncWidgetActions(QWidget* widget, bool sync)
{
    if (!sync) {
        _registeredWidgets.remove(widget);
        return;
    }

    _registeredWidgets.insert(widget);

    const QList<QAction*> currentActions = widget->actions();
    foreach(QAction * currentAction, currentActions)
    widget->removeAction(currentAction);

    widget->addActions(_group->actions());
}
void ProfileList::favoriteChanged(Profile::Ptr info, bool isFavorite)
{
    SessionManager* manager = SessionManager::instance();

    if (isFavorite) {
        QAction* action = new QAction(_group);
        action->setData(QVariant::fromValue(info));

        if (_addShortcuts) {
            action->setShortcut(manager->shortcut(info));
        }

        updateAction(action, info);

        foreach(QWidget * widget, _registeredWidgets)
        widget->addAction(action);
        emit actionsChanged(_group->actions());
    } else {
        QAction* action = actionForKey(info);

        if (action) {
            _group->removeAction(action);
            foreach(QWidget * widget, _registeredWidgets)
            widget->removeAction(action);
            emit actionsChanged(_group->actions());
        }
    }

    updateEmptyAction();
}
void ProfileList::triggered(QAction* action)
{
    emit profileSelected(action->data().value<Profile::Ptr>());
}

QList<QAction*> ProfileList::actions()
{
    return _group->actions();
}

#include "ProfileList.moc"
